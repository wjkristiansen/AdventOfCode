#include <iostream>
#include <vector>
#include <algorithm>

#include <assert.h>

#include "day16.h"

namespace Day16
{
	const uint8_t TestData[] =
	{
		0b10110100,
		0b00110011,
		0b10011010,
		0b00001111,
		0b10010000,
		0b00100111,
		0b11110010,
	};

//110011100110100
//00011111

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
	};

	class BitStreamReader
	{
		BitStream& m_BitStream;

	public:
		BitStreamReader(BitStream &bitStream) :
			m_BitStream(bitStream)
		{
		}

		size_t Read(uint8_t* pBuffer, size_t numBits, size_t bufferSize)
		{
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
				uint8_t CurByte = (m_BitStream.m_pBuffer[m_BitStream.m_ByteOffset]) >> RightShift;
				uint8_t LeftShift = ChunkSize - NumReadBits;
				numBits -= NumReadBits;
				uint8_t Mask = 0xff >> m_BitStream.m_BitOffset;
				pBuffer[BufferIndex] |= ((Mask & CurByte) << LeftShift);

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

			return BufferIndex;
		}
	};

	void Execute()
	{
		BitStream Stream(TestData, sizeof(TestData));
		BitStreamReader Reader(Stream);
		uint8_t Buffer[16];
		Reader.Read(Buffer, 3, sizeof(Buffer));
		assert(Buffer[0] == 0x5);
		Reader.Read(Buffer, 7, sizeof(Buffer));
		assert(Buffer[0] == 0x50);
		Reader.Read(Buffer, 15, sizeof(Buffer));
		assert(Buffer[0] == 0x34);
		assert(Buffer[1] == 0x67);
		Reader.Read(Buffer, 8, sizeof(Buffer));
		assert(Buffer[0] == 0x1f);
	}
}