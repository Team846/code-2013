#ifndef PNEUMATICS_H_
#define PNEUMATICS_H_

#include <WPILib.h>
#include <DoubleSolenoid.h>
#include <Compressor.h>

#include "../Config/RobotConfig.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Process/SynchronizedProcess.h"
#include "../Config/ConfigManager.h"
#include "../Utils/AsyncPrinter.h"

typedef struct
{
	DoubleSolenoid * solenoid;
	int counter;
	bool state;
	bool pulsed;
} PulsedSolenoid;

class Pneumatics : public SynchronizedProcess, public Configurable, public Loggable
{
public:
	/*!
	 * Destructor
	 */
	virtual ~Pneumatics();

	/*!
	 * Gets an instance of Pneumatics
	 * @return pointer to instance
	 */
	static Pneumatics* Instance();

	/*!
	 * Deletes instance
	 */
	static void Finalize();
	
	void setCollector(bool on, bool force = false);

	void setClimberLowerInner(bool on, bool force = false);

	void setClimberLowerOuter(bool on, bool force = false);

	void setClimberMiddle(bool on, bool force = false);

	void setClimberUpper(bool on, bool force = false);

	void setStorageExit(bool on, bool force = false);
	
	/*!
	 * Sets the state of the compressor
	 * @param on
	 */
	void setCompressor(bool on);

	/*!
	 * Configures values
	 */
	virtual void Configure();

	/*!
	 * Logs value
	 */
	virtual void Log();

protected:
	INT32 Tick();

private:
	/*!
	 * Constructor
	 */
	Pneumatics();

	/*!
	 * Pulses a solenoid
	 * @param ptr the pulsed solenoid
	 */
	void pulse(PulsedSolenoid * ptr);

	PulsedSolenoid m_collector, m_climberLowerInner, m_climberLowerOuter, m_climberMiddle, m_climberUpper, m_storageExit;

	static Pneumatics* m_instance;

	std::string m_configSection;

	int m_pulse_length;
	bool m_mutex;

	Compressor* m_compressor;

	DISALLOW_COPY_AND_ASSIGN(Pneumatics);

};

#endif
