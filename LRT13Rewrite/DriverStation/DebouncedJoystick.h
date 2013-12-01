#ifndef DEBOUNCED_JOYSTICK_H_
#define DEBOUNCED_JOYSTICK_H_

#include "Joystick.h"
#include "../Logging/Loggable.h"

/*!
 * @brief Implements a joystick with last state checking ability.
 */
class DebouncedJoystick : public Joystick, public Loggable
{
public:
	DebouncedJoystick(UINT32 port, int nBtns, int nAxes);
	~DebouncedJoystick();

	/*!
	 * @brief Initializes the values of the joystick.
	 */
	void Init();
	
	/*!
	 * @brief Updates the values of the joystick.
	 */
	void Update();

	/*!
	 * @brief Checks if a button is in bounds.
	 * @param button the button to check
	 * @return whether the button is in bounds
	 */
	bool ButtonInBounds(int button);
	
	/*!
	 * @brief Checks if an axis is in bounds.
	 * @param axis the axis to check
	 * @return whether the axis is in bounds
	 */
	bool AxisInBounds(int axis);

	/*!
	 * @brief Returns whether a button is first pressed in the current cycle.
	 * @param button the button to check
	 * @return whether the button was just pressed
	 */
	bool IsButtonJustPressed(int button);
	
	/*!
	 * @brief Returns whether a button is first released in the current cycle.
	 * @param button the button to check
	 * @return whether the button was just released
	 */
	bool IsButtonJustReleased(int button);
	
	/*!
	 * @brief Returns if the button is down.
	 * @param button the button to check
	 * @return whether the button is down
	 */
	bool IsButtonDown(int button);

	/*!
	 * @brief Returns if the button was down in the previous cycle.
	 * @param button the button to check
	 * @return whether the button was down in the previous cycle
	 */
	bool WasButtonDown(int button);

	/*!
	 * @brief Returns if the hat switch is first pressed in the current cycle.
	 * @param axis the axis of the switch to check
	 * @param direction the direction of the switch to check
	 * @return whether the hat switch was just pressed
	 */
	bool IsHatSwitchJustPressed(int axis, int direction);

	/*!
	 * @brief Returns if the hat switch is first released in the current cycle.
	 * @param axis the axis of the switch to check
	 * @param direction the direction of the switch to check
	 * @return whether the hat switch was just released
	 */
	bool IsHatSwitchJustReleased(int axis, int direction);

	/*!
	 * @brief Returns if the hat switch is down.
	 * @param axis the axis of the switch to check
	 * @param direction the direction of the switch to check
	 * @return whether the hat switch is down
	 */
	bool IsHatSwitchDown(int axis, int direction);

	/*!
	 * @brief Returns if the hat switch was down in the previous cycle.
	 * @param axis the axis of the switch to check
	 * @param direction the direction of the switch to check
	 * @return whether the hat switch was down in the previous cycle
	 */
	bool WasHatSwitchDown(int axis, int direction);

	/*!
	 * @brief Gets the change in axis value.
	 * @param axis the axis to check
	 * @return the change in value of the axis
	 */
	double GetRawAxisDelta(int axis);
	
	/*!
	 * @brief Gets the value of the axis in the previous cycle.
	 * @param axis the axis to check
	 * @return the value of the axis in the previous cycle
	 */
	double GetLastAxis(int axis);
	
	/*!
	 * @brief Gets the number of buttons on this joystick.
	 * @return the number of buttons
	 */
	int GetNumButtons();

	/*!
	 * @brief Gets the number of axes on this joystick.
	 * @return the number of axes
	 */
	int GetNumAxes();
	
	/*!
	 * @brief Gets the port that this joystick is on.
	 * @return the port of the joystick
	 */
	UINT32 GetPort();
	
	void Log();

private:
	int m_num_buttons, m_num_axes;
	bool* wasPressed;
	bool* isPressed;
	double* axisPrevValue;
	double* axisValue;
	UINT32 m_port;
};

#endif
