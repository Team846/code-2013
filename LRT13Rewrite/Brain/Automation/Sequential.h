#ifndef SEQUENTIAL_H_
#define SEQUENTIAL_H_

#include "Automation.h"
#include <queue>
#include <vector>

using namespace std;

/*!
 * @brief Container for sequentially executing automation routines.
 */
class Sequential : public Automation
{
public:
	/*!
	 * @brief Creates a sequential routine.
	 * @param name the name of the routine
     * @param queueIfBlocked whether the routine should be queued if blocked
	 * @param restartable whether the routine can be restarted
	 */
	Sequential(const char *name, bool queueIfBlocked = false, bool restartable = false);
	
	/*!
	 * @brief Creates a sequential routine initialized with a sequence of routines.
	 * @param name the name of the routine
	 * @param sequence the list of routines to initialize the sequential routine with
     * @param queueIfBlocked whether the routine should be queued if blocked
	 * @param restartable whether the routine can be restarted
	 */
	Sequential(const char *name, vector<Automation*> sequence, bool queueIfBlocked = false, bool restartable = false);
	virtual ~Sequential();

	virtual bool Start();
	virtual bool Abort();
	virtual bool Run();
	virtual void AllocateResources();
	
	/*!
	 * @brief Adds an automation routine to the sequence of routines.
	 * @param automation the routine to add
	 */
	void AddAutomation(Automation *automation);
	
	/*!
	 * @brief Adds a list of automation routine to the sequence of routines.
	 * @param automation the list of routines to add
	 */
	void AddAutomation(vector<Automation*> automation);
	
	/*!
	 * @brief Clears the sequence of routines.
	 */
	void ClearSequence();
	
protected:
	/*!
	 * @brief Checks whether the next step in the sequence can be started.
	 * @return whether the next step in the sequence can be started
	 */
	virtual bool ContinueNextStep();
	
private:
	vector<Automation*> routines;
	queue<Automation*> queued;
    bool started;
};

#endif
