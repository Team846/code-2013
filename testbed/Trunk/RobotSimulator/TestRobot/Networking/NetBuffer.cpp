#include "NetBuffer.h"

using namespace Network;

const int NetBuffer::kBufferResizeOverAllocateBytes = 4;

NetBuffer::NetBuffer()
{
	construct(NULL, 0);
}

NetBuffer::NetBuffer(int bufferDefaultSize)
{
	construct(new char[bufferDefaultSize], bufferDefaultSize);
}

NetBuffer::NetBuffer(char* buff, int len)
{
	construct(buff, len);
}

NetBuffer::~NetBuffer()
{
	DELETE_ARR(m_internalBuffer);
}

void NetBuffer::construct(char* buff, int size)
{
	m_internalBitPos = 0;
	m_internalBuffer = buff;
	m_internalBufferSize = size;
	
	m_isReadOnly = buff != NULL;
	m_sent = false;
}

void NetBuffer::Write(char c)
{
	InternalWriteByte(c, 8);
}

void NetBuffer::Write(char* c, UINT16 len)
{
	InternalWriteInteger(len, sizeof(UINT16));
	InternalWriteBytes(c, len);
}

void NetBuffer::WriteRaw(char* c, UINT16 len)
{
	InternalWriteBytes(c, len);
}

void NetBuffer::Write(string str)
{
	InternalWriteInteger((ULONG)str.length(), sizeof(UINT16) * 8);
	InternalWriteBytes(str.c_str(), str.length());
}

void NetBuffer::Write(bool b)
{
	InternalWriteByte(b ? 1 : 0, 1);
}

void NetBuffer::Write(double d)
{
	char *byteArray = reinterpret_cast<char*>(&d);
	
	InternalWriteBytes(byteArray,sizeof(double));
}

void NetBuffer::Write(float f)
{
	UINT32 packed = NetUtil::PackFloat(f);

	Write(packed);
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
		AsyncPrinter::Println("[NetBuffer] Can't write to a read-only buffer!");
		return;
	}
	
	m_internalBitPos += 8 - m_internalBitPos % 8;
}

char NetBuffer::ReadChar()
{
	return InternalReadByte(8);
}

char* NetBuffer::ReadBytes()
{
	UINT16 len = InternalReadInteger(sizeof(UINT16) * 8);
	
	return InternalReadBytes(len);
}

string NetBuffer::ReadStdString()
{
	UINT16 len = InternalReadInteger(sizeof(UINT16) * 8);
	
	return InternalReadBytes(len);
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
	char *byteArray = InternalReadBytes(sizeof(double));
		
	double d = *reinterpret_cast<double*>(byteArray);
	
	return d;
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

void NetBuffer::InternalWriteByte(const char data, int bit_length)
{
	if(m_isReadOnly)
	{
		AsyncPrinter::Println("[NetBuffer] Can't write to a read-only buffer!");
		return;
	}
	
	if(bit_length < 1 || bit_length > 8)
	{
		AsyncPrinter::Println("[NetBuffer] Can't write less than one bit or more than eight bits!");
		return;
	}
	
	FitBufferToSize(GetBytePos() * 8 + bit_length);
	
	int bit_pos = GetBitIndexInCurrentByte();
	
	// this operation performs a logical AND on the given data and the bit_length
	// in order to get rid of the unnecessary bits.
	// The data is AND'ed with 0xFF (1111 1111) shifted to the right by 8 - bit_length, creating the masker.
	char data_masked = (char)(((UINT32)data & ((UINT32)(~(UINT32)(0)) >> (8 - bit_length))));
	
	int remainingBits = 8 - bit_pos;
	int overflow = bit_length - remainingBits;
	
	m_internalBuffer[GetBytePos()] |= (data_masked >> (overflow));
	
	// this byte is finished
	if(overflow <= 0)
	{
	}
	// write into the next byte
	else if(overflow > 0)
	{
		remainingBits = overflow;
		
		// mask off written bits
		data_masked &= ((char)((UINT32)(~(UINT32)(0)) >> (8 - remainingBits)));
		
		m_internalBuffer[GetBytePos()] &= (data_masked << (8 - remainingBits));
	}
	
	m_internalBitPos += bit_length;
}

void NetBuffer::InternalWriteBytes(const char data[], int bytes)
{
	if(m_isReadOnly)
	{
		AsyncPrinter::Println("[NetBuffer] Can't write to a read-only buffer!");
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
		AsyncPrinter::Println("[NetBuffer] Can't write to a read-only buffer!");
		return;
	}
	
	for(int i = 0; i < bits; i += 8)
	{
		if(i + 8 > bits)
		{
			// this is the last bitset
			int rem_bits = bits - i;
			InternalWriteByte((char)(data >> i), rem_bits);
		}
		else
		{
			InternalWriteByte((char)(data >> i), 8);
		}
	}
}

char NetBuffer::InternalReadByte(int bit_length)
{
	if(bit_length < 1 || bit_length > 8)
	{
		AsyncPrinter::Println("[NetBuffer] Can't read less than one bit or more than eight bits!");
		return 0;
	}
	
	if(!AssertBufferHasSpace(m_internalBitPos + bit_length))
	{
		AsyncPrinter::Println("[NetBuffer] Can't read past the buffer!");
		return 0;
	}
	
	int bit_pos = GetBitIndexInCurrentByte();
	
	int remainingBits = 8 - bit_pos;
	int overflow = bit_length - remainingBits;
	
	int pos = GetBytePos();
	
	char retrieved = (char)((UINT32)m_internalBuffer[pos] & (~(UINT32)(0) >> (overflow)));
	
	if(overflow <= 0)
	{
		// we're done.
	}
	else if(overflow > 0)
	{
		retrieved = (char)((UINT32)(retrieved) & m_internalBuffer[GetBytePos() + 1] & (~(UINT32)(0) << (8 - overflow))); 
	}
	
	m_internalBitPos += bit_length;
	return (char)retrieved;
}

char* NetBuffer::InternalReadBytes(int bytes)
{	
	// done like this so that the array stays alive after the function returns
	char* retrieved = (char*)malloc(bytes);
	
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
		m_internalBuffer = new char[len];
		m_internalBufferSize = len;
		memset(m_internalBuffer, 0, sizeof(char) * len);
	}
	else if(bytes > m_internalBufferSize)
	{
		int len = bytes + NetBuffer::kBufferResizeOverAllocateBytes;
		char* newBuff = new char[len];
		
		memset(newBuff, 0, sizeof(char) * len);

		memcpy(newBuff, m_internalBuffer, m_internalBufferSize);
		m_internalBufferSize = len;
		
		delete[] m_internalBuffer;
		
		m_internalBuffer = newBuff;
	}
}

char* NetBuffer::GetBuffer()
{
	return m_internalBuffer;
}
