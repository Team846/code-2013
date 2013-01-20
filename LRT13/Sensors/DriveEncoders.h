#ifndef DRIVE_ENCODERS_H_
#define DRIVE_ENCODERS_H_

#include "WPILib.h"
#include "LRTEncoder.h"
#include "../ComponentData/DrivetrainData.h"
#include "../Config/Configurable.h"
#include "../Log/Loggable.h"
#include "../Utils/Defines.h"

/*!
 * @brief Class to keep track of drivetrain encoders
 */
class DriveEncoders : public Configurable, public Loggable
{
private:
	static DriveEncoders* m_instance;
	std::string m_configsection;

	LRTEncoder* m_encoders[4];//not all of the array is used

	double PULSES_PER_REVOLUTION; // pulses per wheel revolution [BA]

	double MAX_ENCODER_RATE;
	double MAX_TURNING_RATE;

	double TICKS_PER_FULL_TURN;
	double WHEEL_DIAMETER; // in
public:
	const static double PI = 3.14159;
	/*!
	 * @brief Gets the singleton instance
	 * @return the instance
	 */
	static DriveEncoders& GetInstance();

	/*!
	 * @brief Destructor
	 */
	virtual ~DriveEncoders();
	
	/*!
	 * @brief Configures overall settings
	 */
	virtual void Configure();

	/*!
	 * @brief logs data to the smartdashboard
	 */
	virtual void Log();

	/*!
	 * @brief Gets the fastest attainable encoder rate
	 * @return maxEncoderRate
	 */
	double getMaxEncoderRate();


	/*!
	 * @brief Get non-normalized average forward speed
	 * @return forward speed
	 */
	double GetRawForwardSpeed(); //not normalized

	/*!
	 * @brief Get normalized average forward speed
	 * @return forward speed
	 */
	double getNormalizedForwardSpeed();

	/*!
	 * @brief Get non-normalized turning speed
	 * @return turning speed
	 */
	double getRawTurningSpeed();

	/*!
	 * @brief Get normalized turning speed assuming high gear
	 * @return turning speed
	 */
	double getNormalizedTurningSpeed();

	/*!
	 * @brief Get robot traveled distance in engineering units
	 * @return distance in inches (iff properly calibrated)
	 */
	double getRobotDist();

	/*!
	 * @brief Get number of ticks during turn
	 * @return turn ticks
	 */
	int getTurnTicks();

	/*!
	 * @brief Get number of revolutions during turn
	 * @return turn revolutions
	 */
	double getTurnRevolutions();

	/*!
	 * @brief Get calibrated turning angle
	 * @return turn agnle
	 */
	double getTurnAngle();

	/*!
	 * @brief Get the distance of a particular wheel
	 * @param side LEFT or RIGHT
	 * @return
	 */
	double getWheelDist(data::drivetrain::Side side);


	/*!
	 * @brief Normalized speed of a particular side
	 * @param which side
	 * @return normalized speed of that side
	 */
	double getNormalizedSpeed(data::drivetrain::Side side);
	
	/*!
	 * @brief access the LRT Encoder.
	 */
	LRTEncoder *getEncoder(data::drivetrain::Side side);
protected:
	DriveEncoders();
	DISALLOW_COPY_AND_ASSIGN(DriveEncoders);
};

#endif
