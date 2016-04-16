#ifndef __wormtcp__h__
#define __wormtcp__h__

#include "ns3/application.h"
#include "worm.h"
#include "ns3/ptr.h"
#include "ns3/event-id.h"
#include "ns3/socket.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/node.h"
#include "ns3/ipv4-address.h"

#include <vector>
#include <map>

using namespace std;
using namespace ns3;

class WormTCP : public Worm {

public:
	static TypeId GetTypeId (void);
	WormTCP();
	void SetNode (Ptr<Node> arg_node);
	Ptr<Node> GetNode ();
	virtual ~WormTCP() {};
	virtual Ptr<Application> Copy() const;
	
	static void SetConnections(uint16_t);
	virtual void DoInitialize(void);
	void Activate();

private:
	std::vector< Ptr<Socket> >	m_tcp_ptr; // Client side
	Ptr<Socket>			m_tcp; // Server side
	std::map < Ptr<Socket> ,int>	tcp_c_map; // client side to know how much ack'ed
	std::map < Ptr<Socket> ,uint32_t>	tcp_recv; // Server side to know how much received
	std::map < Ptr<Socket> ,bool>	tcp_connected; // Is connected
	std::map < Ptr<Socket> ,bool>	tcp_is_worm; // Is worm
	std::vector< uint32_t > 	sentAck;
	Ipv4Address         m_peer;

	static uint16_t connections;

	void SendWorm(int);

	void StartApplication (void);
	void StopApplication (void);

protected:
	void Receive(Ptr<Socket>); // Call back when data received
	void DataSend(Ptr<Socket>, uint32_t); // Call back for sending data
	void ConnectionSucceeded(Ptr<Socket>); // Call back once connection succeeds
	void ConnectionFailed(Ptr<Socket>); // Call back if connection fails
	void CloseRequest(Ptr<Socket>); // Call back for close command
	void ConnectionAccept(Ptr<Socket>, const Address&);

};

#endif
