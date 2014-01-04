#ifndef ROBOT_LOCATION_H_
#define ROBOT_LOCATION_H_

#include "WPILib.h"
#include "DriveEncoders.h"
#include "../Logging/Loggable.h"
#include "semLib.h"

/*!
 * @brief Localizes the robot's location relative to its start position using changes in encoder values. Uses arcs to approximate path.
 */
class RobotLocation : public Loggable
{
public:
	static void Initialize();
	static RobotLocation* Instance();
	static void Finalize();

	/*!
	 * @brief Calculates the new position of the robot.
	 */
	void Update();
	
	/*!
	 * @brief Get the x position of the robot relative to the zeroed location.
	 * @return x position
	 */
	double GetX();
	
	/*!
	 * @brief Get the y position of the robot relative to the zeroed location.
	 * @return y position
	 */
	double GetY();
	
	/*!
	 * @brief Gets the angle of the robot relative to the zeroed angle.
	 * @return angle of the robot
	 */
	double GetTheta();

	/*!
	 * @brief Sets the current position and angle of the robot as zero.
	 */
	void Zero();
	
	/*!
	 * @brief Logs values to the log file.
	 */
	void Log();
	
	/*!
	 * @brief Periodically called by notifier to update.
	 */
	static void Periodic(void *param);
	
private:
	RobotLocation();
	
	static RobotLocation *m_instance;
	
	DriveEncoders *m_encoders;
	
	Notifier m_notifier;
	
	SEM_ID sem;
	
	double x, y;
	double x_last, y_last;
	double theta_zero;
	double d, theta;
	double d_last, theta_last;
	
	DISALLOW_COPY_AND_ASSIGN(RobotLocation);
};

#endif
