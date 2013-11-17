#ifndef PNEUMATICS_H_
#define PNEUMATICS_H_

#include <WPILib.h>
#include <SolenoidBase.h>
#include <DoubleSolenoid.h>
#include <Solenoid.h>
#include <Compressor.h>

#include "../Config/RobotConfig.h"
#include "../Config/Configurable.h"
#include "../Process/SynchronizedProcess.h"
#include "../Config/ConfigManager.h"
#include "../Utils/AsyncPrinter.h"

class Pneumatics : public SynchronizedProcess, public Configurable
{
public:
	enum State
	{
		OFF = false,
		FORWARD = true,
		REVERSE = false
	};
	
	// Double solenoid
	Pneumatics(uint32_t forward, uint32_t reverse, uint8_t module, const char *name);
	Pneumatics(uint32_t forward, uint32_t reverse, const char *name);
	// Single solenoid
	Pneumatics(uint32_t forward, uint8_t module, const char *name);
	Pneumatics(uint32_t forward, const char *name);
	virtual ~Pneumatics();
	
	static void CreateCompressor();
	static void DestroyCompressor();
	static void SetCompressor(bool on);
	
	void Set(bool on, bool force = false);
	bool Get();

	virtual void Configure();
	
	const char* GetName();
	
	static vector<Pneumatics*> pneumatic_vector;
	
protected:
	INT32 Tick();

private:
	void InternalUpdate();
	
	std::string m_configSection;

	int m_pulse_length;

	static Compressor *m_compressor;

	SolenoidBase *solenoid;
	int counter;
	bool pulsed;
	State state;
	
	const char *m_name;
	
	DISALLOW_COPY_AND_ASSIGN(Pneumatics);

};

#endif
