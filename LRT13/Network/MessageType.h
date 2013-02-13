#ifndef MESSAGETYPE_H_
#define MESSAGETYPE_H_

namespace Network
{
	// minor hack because pre-C++11 compilers move the enum values into the parent namespace.
	/*!
	 * @brief Contains an enumeration defining networked message headers.
	 * @author Tony Peng
	 */
	namespace MessageType
	{
		enum Enum
		{
			// in
			AUTO_AIM_DATA = 0x00,
			DISC_LOCATOR_DATA = 0x10,
			
			// out
			DASHBOARD_DATA = 0x20,
			ROBOTDATA_DATA = 0x21,
		};
	};
};

#endif
