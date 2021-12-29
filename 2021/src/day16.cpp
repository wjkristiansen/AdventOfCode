#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <assert.h>

#include "day16.h"

namespace Day16
{
	class BitStream
	{
	public:
		const uint8_t* m_pBuffer;
		size_t m_BufferSize;
		size_t m_ByteOffset = 0;
		uint8_t m_BitOffset = 0;

		BitStream(const uint8_t* pBuffer, size_t bufferSize) :
			m_pBuffer(pBuffer),
			m_BufferSize(bufferSize) {}

		size_t GetOffsetInBits() const { return m_ByteOffset * 8 + m_BitOffset; }
	};

	class BitStreamReader
	{
		BitStream& m_BitStream;

	public:
		BitStreamReader(BitStream &bitStream) :
			m_BitStream(bitStream)
		{
		}

		size_t Read(void* pOutputBuffer, size_t numBits, size_t bufferSize)
		{
			size_t result = numBits;
			uint8_t* pBuffer = reinterpret_cast<uint8_t*>(pOutputBuffer);
			if (numBits == 0)
				return 0;

			size_t BufferIndex = ((numBits - 1) / 8);
			if (BufferIndex >= bufferSize)
				return 0;

			uint8_t ChunkSize = 1 + uint8_t((numBits - 1) % 8);
			pBuffer[BufferIndex] = 0;
			while (numBits > 0)
			{
				// Read from the current byte
				uint8_t NumReadBits = std::min<uint8_t>(ChunkSize, 8 - m_BitStream.m_BitOffset);
				uint8_t RightShift = uint8_t(std::max<int>(0, 8 - m_BitStream.m_BitOffset - ChunkSize));
				uint8_t Mask = 0xff >> m_BitStream.m_BitOffset;
				uint8_t CurByte = (Mask & (m_BitStream.m_pBuffer[m_BitStream.m_ByteOffset])) >> RightShift;
				uint8_t LeftShift = ChunkSize - NumReadBits;
				numBits -= NumReadBits;
				pBuffer[BufferIndex] |= (CurByte << LeftShift);

				// Update the stream state 
				size_t NewBitOffset = m_BitStream.m_BitOffset + NumReadBits;
				m_BitStream.m_ByteOffset += NewBitOffset / 8;
				m_BitStream.m_BitOffset = uint8_t(NewBitOffset & 0x7);

				ChunkSize -= NumReadBits;
				if (ChunkSize == 0)
				{
					ChunkSize = 8;
					if (BufferIndex == 0)
					{
						assert(numBits == 0);
						break;
					}
					BufferIndex--;
					pBuffer[BufferIndex] = 0;
				}
			}

			return result;
		}
	};

	class PacketDecoder
	{
		BitStream m_Stream;
		BitStreamReader m_Reader;
		size_t m_SumOfVersions = 0;

		uint8_t ReadUInt8(size_t numBits)
		{
			uint8_t Value = 0;
			if (0 == m_Reader.Read(&Value, numBits, sizeof(Value)))
				throw std::exception();
			return Value;
		}

		uint16_t ReadUInt16(size_t numBits)
		{
			uint16_t Value = 0;
			if (0 == m_Reader.Read(&Value, numBits, sizeof(Value)))
				throw std::exception();
			return Value;
		}

		uint64_t ReadLiteral()
		{
			uint64_t Literal = 0;
			uint8_t NumNibbles = 0;
			uint8_t Nibbles[16] = { 0 };
			for (NumNibbles = 0;;)
			{
				uint8_t Value = ReadUInt8(5);
				Nibbles[NumNibbles] = Value & 0x0f;
				++NumNibbles;
				if (!(Value & 0x10))
					break;
			}
			int Shift = 4 * (NumNibbles - 1);
			for (int i = 0; i < NumNibbles; ++i)
			{
				Literal |= Nibbles[i] << Shift;
				Shift -= 4;
			}
			return Literal;
		}

		int ReadVersion()
		{
			return ReadUInt8(3);
		}

		int ReadTypeId()
		{
			return ReadUInt8(3);
		}

		void ReadPacket(const std::string &indent, bool IsSubPacket)
		{
			int Version = ReadVersion();
			m_SumOfVersions += Version;
			int Type = ReadTypeId();
			if(IsSubPacket)
				std::cout << indent << "Begin SubPacket" << std::endl;
			else
				std::cout << indent << "Begin Packet" << std::endl;
			std::cout << indent << " -Version: " << Version << std::endl;
			std::cout << indent << " -TypeId: " << Type << std::endl;

			switch (Type)
			{
			case 4: {
				uint64_t Literal = ReadLiteral();
				std::cout << indent << "    Literal: " << Literal << std::endl;
				break; }
			default: {
				// Operator packet
				int SubTypeId = ReadUInt8(1);
				std::string SubIndent = indent + "  ";
				if (SubTypeId == 0)
				{
					uint16_t SubPacketSizeInBits = ReadUInt16(15);
					size_t CurOffsetInBits = m_Stream.GetOffsetInBits();
					size_t EndOffsetInBits = CurOffsetInBits + SubPacketSizeInBits;
					for (;m_Stream.GetOffsetInBits() < EndOffsetInBits;)
					{
						ReadPacket(SubIndent, true);
					}
				}
				else
				{
					uint16_t NumSubPackets = ReadUInt16(11);

					for (uint16_t subIndex = 0; subIndex < NumSubPackets; ++subIndex)
					{
						ReadPacket(SubIndent, true);
					}
				}

				break; }
			}
		}

	public:
		PacketDecoder(uint8_t *pData, size_t dataSize) :
			m_Stream(pData, dataSize),
			m_Reader(m_Stream)
		{
		}

		void Execute()
		{
			try
			{
				ReadPacket("", false); // throws when stream is fully parsed
			}
			catch (std::exception& )
			{

			}
		}

		size_t SumOfVersions() const { return m_SumOfVersions; }
	};

	void Execute()
	{
		// Convert the input data from a string to a buffer of bytes
		std::vector<uint8_t> m_Data;
		const size_t Len = sizeof(DataString) - 1;
		for (size_t i = 0; i < Len; i += 2)
		{
			m_Data.push_back(static_cast<uint8_t>(std::stoi(std::string(&DataString[i], 2), nullptr, 16)));
		}

		PacketDecoder Decoder(m_Data.data(), m_Data.size());
		Decoder.Execute();
		std::cout << "SumOfVersions=" << Decoder.SumOfVersions();
	}
}