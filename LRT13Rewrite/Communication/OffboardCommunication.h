#ifndef OFFBOARD_COMMUNICATION_H_
#define OFFBOARD_COMMUNICATION_H_

#include "../Process/AsyncProcess.h"
#include <SerialPort.h>
#include <vector>
#include <semLib.h>

using namespace std;

/*!
 * @brief Intermediate interface for serial communications between cRIO and offboard computer. Assumes that the stream has been properly escaped by the offboard computer.
 */
class OffboardCommunication : public AsyncProcess
{
public:
	enum Stream
	{
		UNSET = -1,
		AUTO_AIM = 0,
		GAME_PIECE_TRACKING = 1,
		LIDAR = 2,
	};
	enum Flag
	{
		AUTO_AIM_FLAG = 0x00,
		GAME_PIECE_TRACKING_FLAG = 0x01,
		LIDAR_FLAG = 0x02,
		ESCAPE = 0x7D
	};
	enum ReadState
	{
		WAIT_HEADER,
		IN_MSG,
		AFTER_ESC
	};
	
	static OffboardCommunication* Instance();
	static void Initialize();
	static void Finalize();

	~OffboardCommunication();
	
	/*!
	 * @brief Read the current message from the specified stream.
	 * @param stream the stream to read from
	 * @param buffer the buffer to read the message into
	 */
	void Read(Stream stream, vector<char> &buffer);
	
protected:
	void Tick();
	
private:
	OffboardCommunication();
	
	inline Stream FlagToStream(Flag flag)
	{
		if (flag == AUTO_AIM_FLAG)
			return AUTO_AIM;
		if (flag == GAME_PIECE_TRACKING_FLAG)
			return GAME_PIECE_TRACKING;
		if (flag == LIDAR_FLAG)
			return LIDAR;
		return UNSET;
	}
	
	static OffboardCommunication *m_instance;
	
	SerialPort *m_serial;
	
	vector<char> buffers[3];
	vector<char> buffer;
	
	Stream currentStream;
	ReadState currentState;
	
	SEM_ID sem[3];
};

#endif /* OFFBOARD_COMMUNICATION_H_ */
