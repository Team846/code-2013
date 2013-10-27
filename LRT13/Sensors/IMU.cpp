#include "IMU.h"

IMU* IMU::m_me = NULL;

// TODO: maybe have a create function which allows you to actually use the other constructor
IMU* IMU::Instance()
{
	if(m_me == NULL)
		m_me = new IMU();
	
	return m_me;
}

void IMU::Finalize()
{
	if(m_me != NULL)
	{
		m_me->Abort(0, 0.01);
		DELETE(m_me);
	}
}

IMU::IMU()
	: AsyncProcess("IMU")
{
	construct(kDefaultAddress, SensorBase::GetDefaultDigitalModule());
}

IMU::IMU(UINT8 address, UINT8 module)
	: AsyncProcess("IMU")
{
	construct(address, module);
}

IMU::~IMU()
{
	DELETE(m_i2c);
}

void IMU::construct(UINT8 address, UINT8 moduleNum)
{
	DigitalModule* module = DigitalModule::GetInstance(moduleNum);
	
	if(module == NULL)
	{
		AsyncPrinter::Printf("[ERROR] IMU::construct(): No valid digital module found with id %u.\n", module);
		m_enabled = false;
		return;
	}
	
	m_enabled = true;
	
	m_i2c = module->GetI2C(address << 1);
	
	memset(m_accelerometerGyroscopeValues, 0, sizeof(m_accelerometerGyroscopeValues));
	
	// You might be asking yourself: why a loop and not a memset?
	// The answer is:  the double format is not standardized.  All 0-bits
	// is not guaranteed to be 0.0.  Does it really matter here?  No.
	for(unsigned int i = 0; i < sizeof(m_gyroscopeAngles) / sizeof(double); i++)
	{
		m_gyroscopeAngles[i] = 0.0;
	}
	
	m_lastPacketID = m_bufferIndex = 0;
}

INT32 IMU::Tick()
{
	// read in any packets
	bool failed = false;
	
	for(int i = 0; i < kNumPacketsPerGroup; i++)
	{
		//int bufferIndex = i * kPayloadBytesPerPacket;
		UINT8 data[8] = {0};
		
		bool result = m_i2c->Transaction(NULL, 0, data, sizeof(data) / sizeof(UINT8));
	
		if(!result)
		{
			AsyncPrinter::Printf("[WARNING] IMU::Tick(): I2C transaction failed.\n");
			failed = true;
			break;
		}
		else if((m_lastPacketID++ % 4)!= data[0])
		{
			// we have a transaction error; missed packet or out of order -- just go back to the beginning
			// this will ensure future packets in this group will be ignored until we receive the first packet of a new group.
			m_lastPacketID = 0;
			failed = true;
			break;
		}
		else
		{	
			for(int j = 0; j < kPayloadBytesPerPacket; j++)
			{
				m_internalBuffer[data[0] * 6 + j] = data[j + 1]; 
			}
		}
	}
	
	if(!failed)
	{
		int status = ReadUInt8();
		
		if(status == 0xff)
		{
			AsyncPrinter::Printf("[WARNING] Invalid IMU packet.\n");
			return 1;
		}
		
		for(unsigned int i = 0; i < sizeof(m_gyroscopeAngles) / sizeof(double); i++)
		{
			m_gyroscopeAngles[i] = ReadInt16() / 100.0;
		}
		
		for(unsigned int i = 0; i < sizeof(m_accelerometerGyroscopeValues) / sizeof(INT16); i++)
		{
			m_accelerometerGyroscopeValues[i] = ReadInt16();
		}
	}
	else
		return 1;
	
	return 0;
}

INT16 IMU::Get(IMU_Axis axis)
{
	return m_accelerometerGyroscopeValues[axis];
}

double IMU::GetAngle(IMU_Rotational_Axis axis)
{
	return m_gyroscopeAngles[axis];
}

UINT8 IMU::ReadUInt8()
{
	unsigned int index = m_bufferIndex++;
	
	if(index >= sizeof(m_internalBuffer))
	{
		AsyncPrinter::Printf("[WARNING] IMU::ReadUInt8(): Can't read past the buffer!");
		return 0;
	}
	
	return m_internalBuffer[index];
}

INT16 IMU::ReadInt16()
{
	UINT8 byte1 = ReadUInt8();
	UINT8 byte2 = ReadUInt8();
	
	return (byte2 << 8) | byte1;
}
