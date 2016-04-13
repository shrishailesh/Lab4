#ifndef __wormtcp__h__
#define __wormtcp__h__

#include "ns3/application.h"
#include "ns3/random-variable-stream.h"
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
	WormTCP();
	void SetNode (Ptr<Node> arg_node);
	virtual ~WormTCP() {};
	virtual Ptr<Application> Copy() const;
	
	static void SetConnections(uint16_t);
	virtual void DoInitialize(void);
	void Activate();

private:
	std::vector< Ptr<Socket> >	m_tcp_ptr;
	Ptr<Socket>			m_tcp;
	std::map < Ptr<Socket> ,int>	tcp_c_map;
	std::map < Ptr<Socket> ,uint32_t>	tcp_recv;
	std::map < Ptr<Socket> ,bool>	tcp_connected;
	std::map < Ptr<Socket> ,bool>	tcp_is_worm;
	std::vector< uint32_t > 	sentAck;
	Ipv4Address         m_peer;
	uint32_t        m_sendSize;

	static Ptr<UniformRandomVariable> rngD;

	static uint16_t connections;

	void SendWorm(int);

	void StartApplication (void);
	void StopApplication (void);

protected:
	void Receive(Ptr<Socket>); // Data received
	void DataSend(Ptr<Socket>, uint32_t); 
	void ConnectionSucceeded(Ptr<Socket>);
	void ConnectionFailed(Ptr<Socket>);
	void CloseRequest(Ptr<Socket>);

};

#endif
