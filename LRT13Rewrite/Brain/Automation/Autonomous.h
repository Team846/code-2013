#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_

#include "Sequential.h"

#include <fstream>
#include <string>

/*!
 * @brief Performs autonomous routine.
 */
class Autonomous : public Sequential
{
public:
	Autonomous();
	~Autonomous();
	
	bool Start();
	void AllocateResources();
	
private:
	/*!
	 * @brief Reads the autonomous routine file and loads the routine sequence.
	 * @param path the path of the autonomous routine file
	 */
    void LoadRoutine(std::string path);
    
	double m_autonomousStartTime;
};

#endif
