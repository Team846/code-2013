#include "NetBuffer.h"
//stupid hack
using namespace Network;

const int NetBuffer::kBufferResizeOverAllocateBytes = 4;

NetBuffer::NetBuffer()
{
	construct(NULL, 0);
}

NetBuffer::NetBuffer(int bufferDefaultSize)
{
	construct(NULL, bufferDefaultSize);
}

NetBuffer::NetBuffer(UINT8* buff, int len)
{
	construct(buff, len);
}

NetBuffer::~NetBuffer()
{
	DELETE_ARR(m_internalBuffer);
}

void NetBuffer::construct(UINT8* buff, int size)
{
	m_internalBitPos = 0;
	m_internalBuffer = NULL;

	if(size > 0)
	{
		m_internalBuffer = new UINT8[size];

		if(buff == NULL)
			memset(m_internalBuffer, 0, size * sizeof(UINT8));
		else
			memcpy(m_internalBuffer, buff, size * sizeof(UINT8));
	}

	m_internalBufferSize = size;
	
	m_isReadOnly = buff != NULL;
	m_sent = false;
}

void NetBuffer::Write(UINT8 c)
{
	InternalWriteByte(c, sizeof(UINT8)*8);
}

void NetBuffer::Write(UINT8* c, UINT16 len)
{
	InternalWriteInteger(len, sizeof(UINT16)*8);
	InternalWriteBytes(c, len);
}

void NetBuffer::WriteRaw(UINT8* c, UINT16 len)
{
	InternalWriteBytes(c, len);
}

void NetBuffer::Write(string str)
{
	InternalWriteInteger((ULONG)str.length(), sizeof(UINT16) * 8);
	InternalWriteBytes((UINT8*)str.c_str(), str.length());
}

void NetBuffer::Write(bool b)
{
	InternalWriteByte(b ? 1 : 0, 1);
}

void NetBuffer::Write(double d)
{
	UINT64 packed = NetUtil::PackDouble(d);

	Write(packed);
}

void NetBuffer::Write(float f)
{
	UINT32 packed = NetUtil::PackFloat(f);

	Write(packed);
}

void NetBuffer::Write(INT64 l)
{
	InternalWriteInteger((UINT64)l, sizeof(INT64) * 8);
}

void NetBuffer::Write(UINT64 l)
{
	InternalWriteInteger(l, sizeof(UINT64) * 8);
}

void NetBuffer::Write(INT32 i)
{
	InternalWriteInteger(i, sizeof(INT32)*8);
}

void NetBuffer::Write(UINT32 i)
{
	InternalWriteInteger(i, sizeof(UINT32)*8);
}

void NetBuffer::Write(INT16 s)
{
	InternalWriteInteger(s, sizeof(INT16)*8);
}

void NetBuffer::WritePadBits()
{	
	if(m_isReadOnly)
	{
		#ifdef __VXWORKS__
AsyncPrinter::Println("[NetBuffer] Can't write to a read-only buffer!");
#else
printf("[NetBuffer] Can't write to a read-only buffer!\n");
#endif
		return;
	}
	
	m_internalBitPos += 8 - m_internalBitPos % 8;
}

UINT8 NetBuffer::ReadChar()
{
	return InternalReadByte(sizeof(UINT8)*8);
}

UINT8* NetBuffer::ReadBytes()
{
	UINT16 len = InternalReadInteger(sizeof(UINT16) * 8);
	
	return InternalReadBytes(len);
}

string NetBuffer::ReadStdString()
{
	UINT16 len = InternalReadInteger(sizeof(UINT16) * 8);
	
	return (char*)InternalReadBytes(len);
}

INT64 NetBuffer::ReadInt64()
{
	return (INT64)InternalReadInteger(sizeof(INT64) * 8);
}

UINT64 NetBuffer::ReadUInt64()
{
	return InternalReadInteger(sizeof(UINT64) * 8);
}

INT32 NetBuffer::ReadInt32()
{
	return (INT32)InternalReadInteger(sizeof(INT32) * 8);
}

UINT32 NetBuffer::ReadUInt32()
{
	return (UINT32)InternalReadInteger(sizeof(UINT32) * 8);
}

INT16 NetBuffer::ReadInt16()
{
	return (INT16)InternalReadInteger(sizeof(INT16) * 8);
}

double NetBuffer::ReadDouble()
{
	UINT64 packed = ReadUInt64();

	return NetUtil::UnpackDouble(packed);
}

float NetBuffer::ReadFloat()
{
	UINT32 packed = ReadUInt32();
	
	return NetUtil::UnpackFloat(packed);
}

bool NetBuffer::ReadBool()
{
	return InternalReadByte(1) == 1;
}

void NetBuffer::SkipPadBits()
{
	m_internalBitPos += 8 - m_internalBitPos % 8;
}

bool NetBuffer::AssertBufferHasSpace(UINT32 bits)
{
	return ((bits + 7) >> 3) <= m_internalBufferSize;
}

void NetBuffer::InternalWriteByte(const UINT8 data, int bit_length)
{
	if(m_isReadOnly)
	{
		#ifdef __VXWORKS__
AsyncPrinter::Println("[NetBuffer] Can't write to a read-only buffer!");
#else
printf("[NetBuffer] Can't write to a read-only buffer!\n");
#endif
		return;
	}
	
	if(bit_length < 1 || bit_length > 8)
	{
		#ifdef __VXWORKS__
AsyncPrinter::Println("[NetBuffer] Can't write less than one bit or more than eight bits!");
#else
printf("[NetBuffer] Can't write less than one bit or more than eight bits!\n");
#endif
		return;
	}
	
	FitBufferToSize(GetBytePos() * 8 + bit_length);
	
	int bit_pos = GetBitIndexInCurrentByte();
	
	// this operation performs a logical AND on the given data and the bit_length
	// in order to get rid of the unnecessary bits.
	// The data is AND'ed with 0xFF (1111 1111) shifted to the right by 8 - bit_length, creating the masker.
	UINT8 data_masked = (UINT8)(((UINT32)data & ((UINT32)(~(UINT32)(0)) >> (8 - bit_length))) << (8 - bit_length));
	
	int remainingBits = 8 - bit_pos;
	int overflow = bit_length - remainingBits;

	m_internalBuffer[GetBytePos()] |= (data_masked >> ((8 - bit_length) + overflow));
	
	// this byte is finished
	if(overflow <= 0)
	{
	}
	// write into the next byte
	else if(overflow > 0)
	{
		remainingBits = overflow;
		
		// mask off written bits
		data_masked &= ((UINT8)((UINT32)(~(UINT32)(0)) >> (8 - remainingBits)));
		
		m_internalBuffer[GetBytePos() + 1] |= (data_masked << (8 - remainingBits));
	}

	m_internalBitPos += bit_length;
}

void NetBuffer::InternalWriteBytes(const UINT8 data[], int bytes)
{
	if(m_isReadOnly)
	{
		#ifdef __VXWORKS__
AsyncPrinter::Println("[NetBuffer] Can't write to a read-only buffer!");
#else
printf("[NetBuffer] Can't write to a read-only buffer!\n");
#endif
		return;
	}
	
	for(int i = 0; i < bytes; i++)
	{
		InternalWriteByte(data[i], 8);
	}
}

void NetBuffer::InternalWriteInteger(const UINT64 data, int bits)
{	
	if(m_isReadOnly)
	{
		#ifdef __VXWORKS__
AsyncPrinter::Println("[NetBuffer] Can't write to a read-only buffer!");
#else
printf("[NetBuffer] Can't write to a read-only buffer!\n");
#endif
		return;
	}
	
	for(int i = 0; i < bits; i += 8)
	{
		if(i + 8 > bits)
		{
			// this is the last bitset
			int rem_bits = bits - i;
			InternalWriteByte((UINT8)(data >> i), rem_bits);
		}
		else
		{
			InternalWriteByte((UINT8)(data >> i), 8);
		}
	}
}

UINT8 NetBuffer::InternalReadByte(int bit_length)
{
	if(bit_length < 1 || bit_length > 8)
	{
		#ifdef __VXWORKS__
AsyncPrinter::Println("[NetBuffer] Can't read less than one bit or more than eight bits!");
#else
printf("[NetBuffer] Can't read less than one bit or more than eight bits!\n");
#endif
		return 0;
	}
	
	if(!AssertBufferHasSpace(m_internalBitPos + bit_length))
	{
		#ifdef __VXWORKS__
AsyncPrinter::Println("[NetBuffer] Can't read past the buffer!");
#else
printf("[NetBuffer] Can't read past the buffer!\n");
#endif
		return 0;
	}
	
	int bit_pos = GetBitIndexInCurrentByte();
	
	int remainingBits = 8 - bit_pos;
	int overflow = bit_length - remainingBits;
	
	int pos = GetBytePos();
	
	UINT32 masker = 0;

	if(overflow < 0)
	{
		masker = ((UINT32)0xff) << (-overflow);
	}
	else
	{
		masker = ((UINT32)0xff) >> (overflow);
	}

	UINT32 retrieved = (UINT32)(((UINT32)m_internalBuffer[pos]) & masker);
	
	if(overflow <= 0)
	{
		// we're done.
	}
	else if(overflow > 0)
	{
		retrieved = retrieved << (overflow);
		retrieved = (UINT8)((UINT32)(retrieved) | ((m_internalBuffer[GetBytePos() + 1] & (~(UINT32)(0) << (8 - overflow))) >> (8 - overflow))); 
	}
	
	m_internalBitPos += bit_length;
	return (UINT8)(retrieved >> (8 - bit_length));
}

UINT8* NetBuffer::InternalReadBytes(int bytes)
{	
	// done like this so that the array stays alive after the function returns
	UINT8* retrieved = (UINT8*)malloc(bytes);
	
	for(int i = 0; i < bytes; i++)
	{
		retrieved[i] = InternalReadByte(8);
	}
	
	return retrieved;
}

UINT64 NetBuffer::InternalReadInteger(int bits)
{	
	UINT64 retrieved = 0;
	
	for(int i = 0; i < bits; i += 8)
	{
		if(i + 8 > bits)
		{
			// this is the last bitset
			int rem_bits = bits - i;
			retrieved |= (UINT64)InternalReadByte(rem_bits) << i;
		}
		else
		{
			retrieved |= (UINT64)InternalReadByte(8) << i;
		}
	}
	
	return retrieved;
}

int NetBuffer::GetBufferLength()
{
	return ((m_internalBitPos + 7) / 8);
}

int NetBuffer::GetBytePos()
{
	return (int)(m_internalBitPos / 8);
}

int NetBuffer::GetBitIndexInCurrentByte()
{
	return m_internalBitPos % 8;
}

void NetBuffer::FitBufferToSize(UINT32 bits)
{
	UINT32 bytes = (bits + 7) >> 3;
	
	if(m_internalBuffer == NULL)
	{
		int len = bytes + NetBuffer::kBufferResizeOverAllocateBytes;
		m_internalBuffer = new UINT8[len];
		m_internalBufferSize = len;
		memset(m_internalBuffer, 0, sizeof(UINT8) * len);
	}
	else if(bytes > m_internalBufferSize)
	{
		int len = bytes + NetBuffer::kBufferResizeOverAllocateBytes;
		UINT8* newBuff = new UINT8[len];
		
		memset(newBuff, 0, sizeof(UINT8) * len);

		memcpy(newBuff, m_internalBuffer, m_internalBufferSize);
		m_internalBufferSize = len;
		
		delete[] m_internalBuffer;
		
		m_internalBuffer = newBuff;
	}
}

UINT8* NetBuffer::GetBuffer()
{
	return m_internalBuffer;
}
