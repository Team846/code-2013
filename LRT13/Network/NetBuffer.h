#ifndef NET_BUFFER_H_
#define NET_BUFFER_H_
//stupid hack
#include "Includes.h"
#include <string.h>

#ifdef __VXWORKS__
#include "../Utils/AsyncPrinter.h"
#endif

#include "NetUtil.h"

using namespace std;

namespace Network
{
	/*!
	 * @brief Implements a buffer for data that can be sent over the network.
	 * @author Tony Peng
	 */
	class NetBuffer
	{
	public:
		/*!
		 * @brief Initializes the NetBuffer.
		 */
		NetBuffer();
		
		/*!
		 * @brief Initializes the NetBuffer with a buffer size of bufferDefaultSize.
		 */
		NetBuffer(const int bufferDefaultSize);
		
		/*!
		 * @brief Initializes the NetBuffer with a buffer.
		 */
		NetBuffer(UINT8* buff, int len);
		
		/*!
		 * @brief Releases the resources utilized by the NetBuffer.
		 */
		~NetBuffer();
		
		/*!
		 * @brief Writes a one-byte value to the buffer.
		 */
		void Write(UINT8 c);
		/*!
		 * @brief Writes a length-prefixed byte array to the buffer.
		 */
		void Write(UINT8* c, UINT16 len);
		/*!
		 * @brief Writes a byte array to the buffer.
		 */
		void WriteRaw(UINT8* c, UINT16 len);
		/*!
		 * @brief Wriets a length-prefixed string to the buffer.
		 */
		void Write(string str);
		void Write(INT64 l);
		void Write(UINT64 l);
		/*!
		 * @brief Writes an four-byte value to the buffer. 
		 */
		void Write(INT32 i);
		/*!
		 * @brief Writes a four-byte unsigned value to the buffer.
		 */
		void Write(UINT32 i);
		/*!
		 * @brief Writes a two-byte value to the buffer.
		 */
		void Write(INT16 s);
		/*!
		 * @brief Writes a floating point value to the buffer.
		 */
		void Write(double d);
		/*!
		 * @brief Writes a floating point value to the buffer.
		 */
		void Write(float f);
		/*!
		 * @brief Writes a one-bit boolean value to the buffer.
		 */
		void Write(bool b);
		/*!
		 * @brief Finishes the current byte to improve CPU read/write time.
		 */
		void WritePadBits();
		
		/*!
		 * @brief Reads a one-byte value from the buffer and advances the internal pointer by 8 bits.
		 */
		UINT8 ReadChar();
		/*!
		 * @brief Reads a length-prefixed byte array from the buffer and advances the internal pointer.
		 */
		UINT8* ReadBytes();
		/*!
		 * @brief Reads a length-prefixed STL string from the buffer and advances the internal pointer.
		 */
		string ReadStdString();
		INT64 ReadInt64();
		UINT64 ReadUInt64();
		/*!
		 * @brief Reads a four-byte value from the buffer and advances the internal pointer by 32 bits.
		 */
		INT32 ReadInt32();
		/*!
		 * @brief Reads a four-byte unsigned value from the buffer and advances the internal pointer by 32 bits.
		 */
		UINT32 ReadUInt32();
		/*!
		 * @brief Reads a two-byte value from the buffer and advances the internal pointer by 16 bits.
		 */
		INT16 ReadInt16();
		/*!
		 * @brief Reads a floating point value from the buffer and advances the internal pointer.
		 */
		double ReadDouble();
		/*!
		 * @brief Reads a floating point value from the buffer and advances the internal pointer.
		 */
		float ReadFloat();
		/*!
		 * @brief Reads a one-bit boolean value from the buffer and advances the internal pointer to the next bit.
		 */
		bool ReadBool();
		/*!
		 * @brief Advances the internal pointer to skip over the remaining values in the current byte to improve CPU read time.
		 */
		void SkipPadBits();
		
		int GetBufferLength();

		/*!
		 * @brief Gets the position of the internal pointer in bytes.
		 */
		int GetBytePos();
		
		/*!
		 * @brief Gets the position of the internal pointer in bits.
		 */
		int GetBitIndexInCurrentByte();
		
		/*!
		 * @brief Gets the internal buffer.
		 */
		UINT8* GetBuffer();
		
		//friend int NetPeer::Send(NetBuffer buff, NetChannel::Enum method, int channel);
		
		bool m_sent;
	private:
		void construct(UINT8* buff, int size);

		bool AssertBufferHasSpace(UINT32 bits);
		
		void InternalWriteByte(const UINT8 data, int bit_length);
		void InternalWriteBytes(const UINT8 data[], int bytes);
		void InternalWriteInteger(const UINT64 data, int bits);
		
		UINT8 InternalReadByte(int bit_length);
		UINT8* InternalReadBytes(int length);
		UINT64 InternalReadInteger(int bits);
		
		void FitBufferToSize(UINT32 bits);
		
		static const int kBufferResizeOverAllocateBytes; // TO-DO: make me configurable
		
		UINT8 * m_internalBuffer;
		UINT32 m_internalBufferSize;
		int m_internalBitPos;
		
		bool m_isReadOnly;
	};
};

#endif
