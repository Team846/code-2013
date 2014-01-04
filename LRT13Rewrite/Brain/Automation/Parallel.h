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
     * @param queueIfBlocked whether the routine should be queued if blocked
	 * @param restartable whether the routine can be restarted
	 */
	Parallel(const char *name, bool queueIfBlocked = false, bool restartable = false);
	
	/*!
	 * @brief Creates a parallel routine initialized with a list of routines.
	 * @param name the name of the routine
	 * @param sequence the list of routines to initialize the parallel routine with
     * @param queueIfBlocked whether the routine should be queued if blocked
	 * @param restartable whether the routine can be restarted
	 */
	Parallel(const char *name, vector<Automation*> sequence, bool queueIfBlocked = false, bool restartable = false);
	virtual ~Parallel();

	virtual bool Start();
	virtual bool Abort();
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
	vector<Automation*> running;
};

#endif
