#include "pch.h"
#include "SolutionFactory.h"

void CSolution<5>::Execute(int part)
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

        static MapRange MapRange::FromString(const std::string s)
        {
            std::istringstream ss(s);
            std::int64_t sourceStart;
            std::int64_t destStart;
            std::int64_t size;
            ss >> destStart >> sourceStart >> size;
            return MapRange(destStart, sourceStart, size);
        }

        std::int64_t GetDest(std::int64_t source)
        {
            std::int64_t offset = source - m_SourceStart;
            if(offset < 0 || offset > m_Size)
                return source;

            return m_DestStart + offset;
        }
    };

    class Map
    {
        std::vector<MapRange> m_MapRanges;
    public:
        Map() = default;

        void AddRange(const MapRange &range)
        {
            m_MapRanges.push_back(range);
        }

        std::int64_t GetDest(std::int64_t source)
        {
            std::int64_t dest = source;

            for( auto &r : m_MapRanges)
            {
                dest = r.GetDest(source);
                if(dest != source)
                    break;
            }

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
    std::vector<std::int64_t> seeds;

    std::ifstream fstream("2023/Day5.txt");
    std::string line;

    for (; !fstream.eof();)
    {
        std::getline(fstream, line);

        // Ignore empty lines
        if(line.empty())
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

        if(line.rfind("seeds:", 0) == 0)
        {
            // Read the list of seeds
            std::istringstream seedss(line);
            seedss.seekg(6);
            for (; !seedss.eof();)
            {
                std::int64_t seedNum;
                seedss >> seedNum;
                seeds.push_back(seedNum);
            }
        }
        else if(line == "seed-to-soil map:")
        {
            readMap(seedToSoilMap);
        }
        else if (line == "soil-to-fertilizer map:")
        {
            readMap(soilToFertilizerMap);
        }
        else if(line == "fertilizer-to-water map:")
        {
            readMap(fertilizerToWaterMap);
        }
        else if(line == "water-to-light map:")
        {
            readMap(waterToLightMap);
        }
        else if(line == "light-to-temperature map:")
        {
            readMap(lightToTemperatureMap);
        }
        else if(line == "temperature-to-humidity map:")
        {
            readMap(temperatureToHumidityMap);
        }
        else if(line == "humidity-to-location map:")
        {
            readMap(humidityToLocationMap);
        }
    }

    // Iterate through the seeds and find nearest location
    std::int64_t nearest = std::numeric_limits<std::int64_t>::max();
    for( auto &seed : seeds)
    {
        std::int64_t soil = seedToSoilMap.GetDest(seed);
        std::int64_t fertilizer = soilToFertilizerMap.GetDest(soil);
        std::int64_t water = fertilizerToWaterMap.GetDest(fertilizer);
        std::int64_t light = waterToLightMap.GetDest(water);
        std::int64_t temperature = lightToTemperatureMap.GetDest(light);
        std::int64_t humidity = temperatureToHumidityMap.GetDest(temperature);
        std::int64_t location = humidityToLocationMap.GetDest(humidity);

        if(location < nearest)
            nearest = location;
    }

    std::cout << "Nearest: " << nearest << std::endl;
}
