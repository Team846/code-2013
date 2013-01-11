#ifndef IROBOT_ACTION_H_
#define IROBOT_ACTION_H_

#warning proposed code

/*!
 * @brief Provides an interface for robot actions.
 * @author Tony Peng
 */
class IRobotAction
{
public:
	IRobotAction();
	virtual ~IRobotAction();
	
	virtual int Run();
};

#endif
