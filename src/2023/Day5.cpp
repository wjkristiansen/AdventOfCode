#include "pch.h"
#include "SolutionFactory.h"

class CSolution5 : public CSolutionBase
{
public:
    CSolution5() : CSolutionBase()
    {
    }

    virtual void Execute(int part)
    {
        class MapRange
        {
            std::int64_t m_DestStart = 0;
            std::int64_t m_SourceStart = 0;
            std::int64_t m_Size;

        public:
            MapRange(std::int64_t destStart, std::int64_t sourceStart, std::int64_t size) :
                m_DestStart(destStart),
                m_SourceStart(sourceStart),
                m_Size(size)
            {
            }

            static MapRange FromString(const std::string& s)
            {
                std::istringstream ss(s);
                std::int64_t sourceStart;
                std::int64_t destStart;
                std::int64_t size;
                ss >> destStart >> sourceStart >> size;
                return MapRange(destStart, sourceStart, size);
            }

            std::pair<std::int64_t, std::int64_t> GetDest(std::int64_t source)
            {
                std::int64_t offset = source - m_SourceStart;
                if (offset < 0 || offset > m_Size)
                    return std::make_pair(source, std::int64_t(0));

                std::int64_t location = m_DestStart + offset;
                std::int64_t toEnd = m_Size - offset;

                return std::make_pair(location, toEnd);
            }

            std::int64_t SourceStart() const { return m_SourceStart; }
            std::int64_t SourceEnd() const { return m_SourceStart + m_Size; }
            std::int64_t DestStart() const { return m_DestStart; }
            std::int64_t DestEnd() const { return m_DestStart + m_Size; }
        };

        class Map
        {
            std::vector<MapRange> m_MapRanges;
        public:
            Map() = default;

            void AddRange(const MapRange& range)
            {
                m_MapRanges.push_back(range);
            }

            std::pair<std::int64_t, std::int64_t> GetDest(std::int64_t source)
            {
                std::pair<std::int64_t, std::int64_t> dest = std::make_pair(source, std::int64_t(1));

                std::int64_t distToNextRange = std::numeric_limits<int64_t>::max();
                for (auto& r : m_MapRanges)
                {
                    dest = r.GetDest(source);
                    if (dest.second > 0)
                        break;
                    if (r.SourceStart() > source)
                    {
                        distToNextRange = std::min(r.SourceStart() - source, distToNextRange);
                    }
                }

                // If not in a range...
                if (dest.second == 0)
                    dest.second = distToNextRange; // ...set dest.second to next nearest range distance

                return dest;
            }
        };

        Map seedToSoilMap;
        Map soilToFertilizerMap;
        Map fertilizerToWaterMap;
        Map waterToLightMap;
        Map lightToTemperatureMap;
        Map temperatureToHumidityMap;
        Map humidityToLocationMap;

        struct SeedRange
        {
            std::int64_t Start;
            std::int64_t Size;
        };
        std::vector<SeedRange> seeds;

        std::ifstream fstream("2023/Day5.txt");
        std::string line;

        for (; !fstream.eof();)
        {
            std::getline(fstream, line);

            // Ignore empty lines
            if (line.empty())
                continue;

            auto readMap = [&fstream, &line](Map& map)
                {
                    for (; !fstream.eof();)
                    {
                        std::getline(fstream, line);
                        if (line.empty())
                            break;

                        map.AddRange(MapRange::FromString(line));
                    }
                };

            if (line.rfind("seeds:", 0) == 0)
            {
                // Read the list of seeds
                std::istringstream seedss(line);
                seedss.seekg(6);
                for (; !seedss.eof();)
                {
                    std::int64_t seedRangeStart;
                    std::int64_t seedRangeSize;
                    seedss >> seedRangeStart >> seedRangeSize;
                    seeds.push_back(SeedRange{ .Start = seedRangeStart, .Size = seedRangeSize });
                }
            }
            else if (line == "seed-to-soil map:")
            {
                readMap(seedToSoilMap);
            }
            else if (line == "soil-to-fertilizer map:")
            {
                readMap(soilToFertilizerMap);
            }
            else if (line == "fertilizer-to-water map:")
            {
                readMap(fertilizerToWaterMap);
            }
            else if (line == "water-to-light map:")
            {
                readMap(waterToLightMap);
            }
            else if (line == "light-to-temperature map:")
            {
                readMap(lightToTemperatureMap);
            }
            else if (line == "temperature-to-humidity map:")
            {
                readMap(temperatureToHumidityMap);
            }
            else if (line == "humidity-to-location map:")
            {
                readMap(humidityToLocationMap);
            }
        }

        // Iterate through the seeds and find nearest location
        std::int64_t nearest = std::numeric_limits<std::int64_t>::max();

        std::unordered_map<std::int64_t, std::int64_t> locationCache;
        for (auto& seedRange : seeds)
        {
            std::int64_t increment = seedRange.Size;
            for (std::int64_t seed = seedRange.Start; seed < seedRange.Start + seedRange.Size; )
            {
                std::pair<std::int64_t, std::int64_t> soil = seedToSoilMap.GetDest(seed);
                increment = std::min(increment, soil.second);
                std::pair<std::int64_t, std::int64_t> fertilizer = soilToFertilizerMap.GetDest(soil.first);
                increment = std::min(increment, fertilizer.second);
                std::pair<std::int64_t, std::int64_t> water = fertilizerToWaterMap.GetDest(fertilizer.first);
                increment = std::min(increment, water.second);
                std::pair<std::int64_t, std::int64_t> light = waterToLightMap.GetDest(water.first);
                increment = std::min(increment, light.second);
                std::pair<std::int64_t, std::int64_t> temperature = lightToTemperatureMap.GetDest(light.first);
                increment = std::min(increment, temperature.second);
                std::pair<std::int64_t, std::int64_t> humidity = temperatureToHumidityMap.GetDest(temperature.first);
                increment = std::min(increment, humidity.second);
                std::pair<std::int64_t, std::int64_t> location = humidityToLocationMap.GetDest(humidity.first);
                increment = std::min(increment, location.second);

                nearest = std::min(location.first, nearest);

                // Increment seed number
                seed += increment;

                // Reset max increment for next iteration
                increment = seedRange.Size - increment;
            }
        }

        std::cout << "Nearest: " << nearest << std::endl;
    }
};

static CSolutionFactory::CSolutionAllocator<CSolution5> SolutionAllocator(5, "If You Give A Seed A Fertilizer");
