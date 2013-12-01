#ifndef LRT_DRIVER_STATION_H_
#define LRT_DRIVER_STATION_H_

#include "../Config/DriverStationConfig.h"
#include "DebouncedJoystick.h"

class LRTDriverStation
{
public:
	static void Initialize();
    static LRTDriverStation* Instance();
    static void Finalize();
    
    DebouncedJoystick* GetDriverWheel();
    DebouncedJoystick* GetDriverStick();
    DebouncedJoystick* GetOperatorStick();
    
    static void Update();
    
private:
    LRTDriverStation();
    ~LRTDriverStation();
    
    static LRTDriverStation *m_instance;
    
	DebouncedJoystick *m_driver_stick;
	DebouncedJoystick *m_operator_stick;
	DebouncedJoystick *m_driver_wheel;
	
	DISALLOW_COPY_AND_ASSIGN(LRTDriverStation);
};


#endif
