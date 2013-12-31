#ifndef DRIVE_ENCODERS_H_
#define DRIVE_ENCODERS_H_

#include "WPILib.h"
#include "LRTEncoder.h"
#include "../Config/Configurable.h"
#include "../Logging/Loggable.h"
#include "../Utils/Defines.h"

/*!
 * @brief Class containing left and right drivetrain encoders.
 */
class DriveEncoders : public Configurable, public Loggable
{
public:
	enum Side
	{
		LEFT = 0,
		RIGHT = 1
	};
	
	DriveEncoders(UINT32 leftSourceA, UINT32 leftSourceB, UINT32 rightSourceA, UINT32 rightSourceB);
	
	/*!
	 * @brief Destructor
	 */
	~DriveEncoders();
	
	static DriveEncoders* Instance();
	
	/*!
	 * @brief Configures constants.
	 */
	void Configure();

	/*!
	 * @brief Logs values to the log file.
	 */
	void Log();

	/*!
	 * @brief Get non-normalized average forward speed.
	 * @return forward speed
	 */
	double GetRawForwardSpeed();

	/*!
	 * @brief Get normalized average forward speed.
	 * @return normalized forward speed
	 */
	double GetNormalizedForwardSpeed();

	/*!
	 * @brief Get non-normalized turning speed.
	 * @return turning speed
	 */
	double GetRawTurningSpeed();

	/*!
	 * @brief Get normalized turning speed assuming high gear.
	 * @return turning speed
	 */
	double GetNormalizedTurningSpeed();

	/*!
	 * @brief Get robot traveled distance in engineering units.
	 * @return distance in inches (if properly calibrated)
	 */
	double GetRobotDist();

	/*!
	 * @brief Get number of ticks during turn.
	 * @return turn ticks
	 */
	int GetTurnTicks();

	/*!
	 * @brief Get number of revolutions during turn. Counter-clockwise is positive, clockwise is negative.
	 * @return turn revolutions
	 */
	double GetTurnRevolutions();

	/*!
	 * @brief Get calibrated turning angle.
	 * @return turn agnle
	 */
	double GetTurnAngle();

	/*!
	 * @brief Get the distance travelled by a wheel.
	 * @param side LEFT or RIGHT
	 * @return the distance of the wheel
	 */
	double GetWheelDist(Side side);


	/*!
	 * @brief Normalized speed of a side.
	 * @param side LEFT or RIGHT
	 * @return normalized speed of the side
	 */
	double GetNormalizedSpeed(Side side);
	
	/*!
	 * @brief Arc radius calculated from turn and forward speed, defined as distance between the center of rotation and the center of the robot.
	 * 
	 * @return radius of turn
	 */
	double GetTurnRadius();
	
	/*!
	 * @brief Access the LRTEncoder.
	 * @return the LRTEncoder
	 */
	LRTEncoder *GetEncoder(Side side);
	
	/*!
	 * @brief Gets the fastest attainable encoder rate.
	 * @return maxEncoderRate
	 */
	static double GetMaxEncoderRate();
	
	/*!
	 * @brief Gets the fastest attainable encoder rate.
	 * @return fastest possible encoder pulse rate
	 */
	static double GetMaxSpeed();
	
	/*!
	 * @brief Gets the fastest attainable turning rate.
	 * @return fastest possible turning rate
	 */
	static double GetMaxTurnRate();
	
private:
	static DriveEncoders *m_instance;
	
	const static double PI = 3.14159;

	LRTEncoder* m_encoders[2];

	static double PULSES_PER_REVOLUTION; // Encoder pulses per wheel revolution

	static double MAX_ENCODER_RATE;
	static double MAX_TURNING_RATE;

	static double TICKS_PER_FULL_TURN;
	static double WHEEL_DIAMETER; // Inches
};

#endif
