#ifndef NET_CHANNEL_H_
#define NET_CHANNEL_H_

namespace Network
{
	// minor hack because pre-C++11 compilers move the enum values into the parent namespace.
	/*!
	 * @brief Contains an enumeration defining network channels.  Each channel has 16 subchannels.
	 * @author Tony Peng
	 */
	namespace NetChannel
	{
		// Network subchannels are identified by base_channel + [0, 15] for a total of 16 subchannels. -tp
		enum Enum
		{
// UNRELIABLE CHANNELS
			/*!
			 * @brief Base channel for an unreliable message.  The message is sent and not tracked.
			 */
			NET_UNRELIABLE = 0x00,
			/*!
			 * @brief Base channel for an unreliably sequenced message.  If packets arrive out of order, older ones are discarded.
			 */
			NET_UNRELIABLE_SEQUENCED = 0x16,
			
// RELIABLE CHANNELS
			/*!
			 * @brief Base channel for a reliably sent message.  Packets are guaranteed to arrive at the destination, but order is not guaranteed.
			 */
			NET_RELIABLE = 0x32,
			/*!
			 * @brief Base channel for a reliable sequenced message.  Packets are guaranteed to arrive at the destination, but old packets are dropped.
			 */
			NET_RELIABLE_SEQUENCED=0x48,
			/*!
			 * @brief Base channel for a reliable ordered message.  Packets are guaranteed to arrive at the destination in the order they were sent.
			 */
			NET_RELIABLE_IN_ORDER = 0x64,
		};
	};
}

#endif
