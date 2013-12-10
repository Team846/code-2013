#ifndef ASYNC_CAN_JAGUAR_H_
#define ASYNC_CAN_JAGUAR_H_

#include <taskLib.h>

#include <CANJaguar.h>
#include <vector>

#include "../Process/SynchronizedProcess.h"
#include "../Utils/Util.h"
#include "../Utils/PrintInConstructor.h"
#include "../Utils/CachedValue.h"
#include "../RobotState.h"

#include "LRTSpeedController.h"
#include "../Logging/Loggable.h"

/*!
 * @brief Asynchronous implementation of a CAN driver for the Jaguar.
 * @brief Reimplements the large majority of useful operations on the Jaguar in a threaded fashion.
 */
class AsyncCANJaguar : public SynchronizedProcess, public CANJaguar, public LRTSpeedController
{
public:
	/*!
	 * @brief Constructs a new Jaguar on the CAN network
	 * @param channel Channel that the Jaguar is assigned to
	 * @param name The name of the Jaguar
	 */
	AsyncCANJaguar(UINT8 channel, std::string name);

	/*!
	 * @brief Destroys the AsyncCANJaguar object
	 */
	~AsyncCANJaguar();

	/*!
	 * @brief Linked list of all of the constructed AsyncCANJaguars
	 */
	static vector<AsyncCANJaguar*> jaguar_vector;

	void Send();
	
	int GetChannel();
	
	/*!
	 * @brief Set the duty cycle of the Jaguar
	 * @param duty_cycle
	 */
	void SetDutyCycle(float duty_cycle);

	/*!
	 * @brief Set the output position of the Jaguar
	 * @param position
	 */
	void SetPosition(float position);

	/*!
	 * @brief Set the output velocity of the Jaguar
	 * @param velocity
	 */
	void SetVelocity(float velocity);
	
	/*!
	 * @brief Set the PID parameters
	 * @param p proportional gain
	 * @param i integral gain
	 * @param d derivative gain
	 */
	void SetPID(double p, double i, double d);

	/*!
	 * @brief Set the new neutral mode
	 * @param mode the new mode
	 */
	void ConfigNeutralMode(LRTSpeedController::NeutralMode mode);

	/*!
	 * @brief Set the new speed reference
	 * @param reference the new reference
	 */
	void SetSpeedReference(SpeedReference reference);

	/*!
	 * @brief Set the new control mode
	 * @param mode the new mode
	 */
	void ChangeControlMode(CANJaguar::ControlMode mode);

	/*!
	 * @brief Enables control
	 * @param encoderInitialPosition the initial position of the encoder
	 */
	void EnableControl(double encoderInitialPosition = 0.0);

	/*!
	 * @brief Set ramp rate of voltage output
	 * @param rampRate ramp rate
	 */
	void SetVoltageRampRate(double rampRate);

	/*!
	 * @brief Set number of encoder codes per revolution
	 * @param codesPerRev codes per revolution
	 */
	void ConfigEncoderCodesPerRev(UINT16 codesPerRev);

	/*!
	 * @brief Set number of potentiometer turns
	 * @param turns number of potentiometer turns
	 */
	void ConfigPotentiometerTurns(UINT16 turns);

	/*!
	 * @brief Sets soft position limits
	 * @param forwardLimitPosition forward soft position limit
	 * @param reverseLimitPosition reverse soft position limit
	 */
	void ConfigSoftPositionLimits(double forwardLimitPosition,
			double reverseLimitPosition);

	/*!
	 * @brief Disables soft position limits
	 */
	void DisableSoftPositionLimits();

	/*!
	 * @brief Disables control
	 */
	void DisableControl();

	/*!
	 * @brief Sets maximum output voltage
	 * @param voltage The voltage
	 */
	void ConfigMaxOutputVoltage(double voltage);

	/*!
	 * @brief Configure Fault time
	 * @param faultTime Fault time
	 */
	void ConfigFaultTime(float faultTime);

	/*!
	 * @brief Configure Expiration time
	 * @param timeout Expiration timeout
	 */
	void SetExpiration(float timeout);

	/*!
	 * @brief Set the new position reference
	 * @param reference The new reference
	 */
	void SetPositionReference(CANJaguar::PositionReference reference);

	/*!
	 * @brief Sets (does not clear) new collection flags
	 * @param flags The flags to be set, defaults to all
	 */
	void AddCollectionFlags(uint32_t flags = 0xffffffff);

	/*!
	 * @brief Clears (does not set) existing collection flags
	 * @param flags The flags to be cleared, defaults to all
	 */
	void RemoveCollectionFlags(uint32_t flags = 0xffffffff);

	/*!
	 * @brief Sets collection flags to input (will clear old flags)
	 * @param flags Flag register
	 */
	void SetCollectionFlags(uint32_t flags);

	/*!
	 * @brief Get the duty cycle of the Jaguar that is pending to be sent
	 * @return duty cycle
	 */
	float GetDutyCycle();

	/*!
	 * @brief Get the duty cycle of the Jaguar that was last received
	 * @return duty cycle
	 */
	float GetHardwareValue();

	/*!
	 * @brief Gets the neutral mode (brake or coast).
	 * @return the neutral mode (brake or coast)
	 */
	LRTSpeedController::NeutralMode GetNeutralMode();
	
	/*!
	 * @brief Gets the P gain
	 * @return proportional gain
	 */
	double GetP();

	/*!
	 * @brief Gets the I gain
	 * @return integral gain
	 */
	double GetI();

	/*!
	 * @brief Gets the D gain
	 * @return derivative gain
	 */
	double GetD();

	/*!
	 * @brief Gets the current speed reference
	 * @return speed reference
	 */
	SpeedReference GetSpeedReference();

	/*!
	 * @brief Gets the current position reference
	 * @return position reference
	 */
	PositionReference GetPositionReference();
	
	/*!
	 * @brief Gets the control mode
	 * @return control mode
	 */
	ControlMode GetControlMode();

	/*!
	 * @brief Gets the bus voltage
	 * @return bus voltage
	 */
	float GetBusVoltage();

	/*!
	 * @brief Gets the output voltage
	 * @return output voltage
	 */
	float GetOutputVoltage();

	/*!
	 * @brief Gets the output current
	 * @return output current
	 */
	double GetOutputCurrent();
	/*!
	 * @brief Gets the temperature
	 * @return temperature
	 */
	float GetTemperature();

	/*!
	 * @brief Gets the position
	 * @return position
	 */
	float GetPosition();

	/*!
	 * @brief Gets the speed
	 * @return speed
	 */
	double GetSpeed();
	/*!
	 * @brief Get forward limit m_status
	 * @return forward limit m_status
	 */
	bool GetForwardLimitOK();

	/*!
	 * @brief Get reverse limit m_status
	 * @return reverse limit m_status
	 */
	bool GetReverseLimitOK();

	/*!
	 * @brief Get power cycle m_status
	 * @return power cycle m_status
	 */
	bool GetPowerCycled();

	/*!
	 * @brief Get expiration time
	 * @return expiration time
	 */
	float GetExpiration();

	/*!
	 * @brief Simple wrapper for GetFaults() == 0
	 * @return true if no faults
	 */
	bool StatusOK();

	/*!
	 * @brief Clear the cache
	 */
	void ResetCache();
	
	// collection flags
	const static uint32_t SPEEDREF = (1 << 0);
	const static uint32_t POSREF = (1 << 1);
	const static uint32_t PID = (1 << 2);
	const static uint32_t CTRLMODE = (1 << 3);
	const static uint32_t BUSVOLT = (1 << 4);
	const static uint32_t OUTVOLT = (1 << 5);
	const static uint32_t OUTCURR = (1 << 6);
	const static uint32_t TEMP = (1 << 7);
	const static uint32_t POS = (1 << 8);
	const static uint32_t SPEED = (1 << 9);
	const static uint32_t FWDLIMOK = (1 << 10);
	const static uint32_t REVLIMOK = (1 << 11);
	const static uint32_t PWRCYCLE = (1 << 12);
	const static uint32_t EXPIRE = (1 << 13);
	const static uint32_t VALUE = (1 << 14);
	
protected:
	/*!
	 * @brief Does the actual communication, not including timing
	 */
	void Tick();
	
private:
	void Set(float setpoint, UINT8 syncGroup = 0);

	void Println(const char *str);
	
	string m_task_name;
	int m_channel;

	CachedValue<float> m_setpoint;
	CachedValue<LRTSpeedController::NeutralMode> m_neutral_mode;
	CachedValue<AsyncCANJaguar::ControlMode> m_control_mode;
	CachedValue<AsyncCANJaguar::PositionReference> m_position_reference;
	CachedValue<AsyncCANJaguar::SpeedReference> m_speed_reference;
	CachedValue<float> m_pid_p;
	CachedValue<float> m_pid_i;
	CachedValue<float> m_pid_d;
	CachedValue<float> m_enable_control;
	bool m_should_disable_control;
	CachedValue<float> m_voltage_ramp_rate;
	CachedValue<float> m_max_output_voltage;
	CachedValue<float> m_fault_time;
	CachedValue<float> m_expiration;
	CachedValue<int> m_encoder_codes_per_rev;
	CachedValue<int> m_potentiometer_turns;
	bool m_should_disable_position_limits;
	CachedValue<float> m_forward_limit_position;
	CachedValue<float> m_reverse_limit_position;
	
	volatile uint32_t m_collection_flags;
	volatile float m_output_voltage;
	volatile float m_output_current;
	volatile float m_position;
	volatile float m_p, m_i, m_d;
	volatile SpeedReference m_speed_ref;
	volatile PositionReference m_position_ref;
	volatile ControlMode m_ctrl_mode;
	volatile float m_bus_voltage;
	volatile float m_temperature;
	volatile float m_speed;
	volatile bool m_fwd_limit_ok;
	volatile bool m_rev_limit_ok;
	volatile bool m_pwr_cyc;
	volatile float m_expire;
	volatile float m_value;

	RobotState::Mode m_last_game_mode;
};

#endif /* ASYNC_CAN_JAGUAR_H_ */
