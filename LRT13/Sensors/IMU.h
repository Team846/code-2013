#ifndef IMU_H_
#define IMU_H_

#include <WPILib.h>
#include <I2C.h>
#include <DigitalModule.h>

#include "../Utils/AsyncPrinter.h"
#include "../Process/AsyncProcess.h"
#include "../ComponentData/ComponentData.h"
#include "../Utils/Defines.h"

class IMU : public AsyncProcess
{
	enum IMU_Axis
	{
		ACCELEROMETER_X = 0,
		ACCELEROMETER_Y = 1,
		ACCELEROMETER_Z = 2,
		GYROSCOPE_X = 3,
		GYROSCOPE_Y = 4,
		GYROSCOPE_Z = 5,
	};
	
	enum IMU_Rotational_Axis
	{
		GYROSCOPE_PITCH = 0,
		GYROSCOPE_ROLL = 1,
		GYROSCOPE_YAW = 2,
	};
	
public:
	static IMU* Instance();
	static void Finalize();
	
	IMU();
	IMU(UINT8 address, UINT8 module);
	
	~IMU();
	
	void construct(UINT8 address, UINT8 moduleNum);
	
	INT16 Get(IMU_Axis axis);
	double GetAngle(IMU_Rotational_Axis axis);
	
private:
	static IMU* m_me;
	
	const static UINT8 kDefaultAddress = 0x29;
	const static UINT8 kNumPacketsPerGroup = 4;
	const static UINT8 kPayloadBytesPerPacket = 7 - 1; // -1 for header
	
	bool m_enabled;
	INT16 m_accelerometerGyroscopeValues[6]; // indices correspond to those defined in IMU_Axis
	double m_gyroscopeAngles[3];
	
	UINT8 m_bufferIndex;
	UINT8 ReadUInt8();
	INT16 ReadInt16();
	
	I2C* m_i2c; // the heart of this code!
	UINT8 m_internalBuffer[kNumPacketsPerGroup*kPayloadBytesPerPacket];
	
	INT8 m_lastPacketID;
	
	INT32 Tick();
};

#endif
