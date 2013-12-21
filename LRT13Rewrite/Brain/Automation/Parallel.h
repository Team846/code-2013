#ifndef PARALLEL_H_
#define PARALLEL_H_

#include "Automation.h"
#include <vector>

using namespace std;

/*!
 * @brief Container for routines executing in parallel.
 */
class Parallel : public Automation
{
public:
	/*!
	 * @brief Creates a parallel routine.
	 * @param name the name of the routine
	 * @param restartable whether the routine can be restarted
	 */
	Parallel(const char *name, bool restartable = false);
	
	/*!
	 * @brief Creates a parallel routine initialized with a list of routines.
	 * @param name the name of the routine
	 * @param sequence the list of routines to initialize the parallel routine with
	 * @param restartable whether the routine can be restarted
	 */
	Parallel(const char *name, vector<Automation*> sequence, bool restartable = false);
	virtual ~Parallel();

	virtual Status Start(Event *trigger);
	virtual Status Abort(Event *trigger);
	virtual bool Run();
	virtual void AllocateResources();
	
	/*!
	 * @brief Adds an automation routine to the parallel routines.
	 * @param automation the routine to add
	 */
	void AddAutomation(Automation *automation);
	
	/*!
	 * @brief Adds a list of automation routine to the parallel routines.
	 * @param automation the list of routines to add
	 */
	void AddAutomation(vector<Automation*> automation);
	
	/*!
	 * @brief Clears the list of parallel routines.
	 */
	void ClearAutomation();
	
private:
	vector<Automation*> routines;
};

#endif
