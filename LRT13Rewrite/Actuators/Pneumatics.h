#ifndef PNEUMATICS_H_
#define PNEUMATICS_H_

#include <WPILib.h>
#include <SolenoidBase.h>
#include <DoubleSolenoid.h>
#include <Solenoid.h>
#include <Compressor.h>

#include "Actuator.h"

#include "../Config/ConfigPortMappings.h"
#include "../Config/Configurable.h"
#include "../Logging/Loggable.h"
#include "../Config/ConfigRuntime.h"
#include "../Utils/AsyncPrinter.h"

/*!
 * @brief Container for single solenoids and double solenoids.
 */
class Pneumatics : public Actuator, public Configurable, public Loggable
{
public:
	enum State
	{
		OFF,
		FORWARD,
		REVERSE
	};
	
	// Double solenoid
	Pneumatics(uint32_t forward, uint32_t reverse, uint8_t module, const char *name);
	Pneumatics(uint32_t forward, uint32_t reverse, const char *name);
	// Single solenoid
	Pneumatics(uint32_t forward, uint8_t module, const char *name);
	Pneumatics(uint32_t forward, const char *name);
	virtual ~Pneumatics();
	
	void Update();
	
	static void CreateCompressor();
	static void DestroyCompressor();
	static void SetCompressor(bool on);
	
	void Set(State on, bool force = false);
	State Get();
	State GetHardwareValue();

	void Configure();
	void Log();
	
	static vector<Pneumatics*> pneumatic_vector;
	
private:
	std::string m_configSection;

	int m_pulse_length;

	static Compressor *m_compressor;

	SolenoidBase *solenoid;
	int counter;
	bool pulsed;
	State state;
	
	DISALLOW_COPY_AND_ASSIGN(Pneumatics);

};

#endif
