#ifndef UNIT_TEST_H_
#define UNIT_TEST_H_

#include <WPILib.h>

/*!
 * @brief Provides a unit test.
 * @author Tony Peng
 */
class UnitTest : public RobotBase
{
public:
	UnitTest();
	virtual ~UnitTest();
	
	/*!
	 * @brief Bootstrapping function - called from WPILib
	 */
	void StartCompetition();
	
	/*!
	 * @brief Runs the unit test.
	 */
	virtual void Run() = 0;
};

#endif
