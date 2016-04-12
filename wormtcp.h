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
#include "ns3/address.h"

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

private:
	std::vector< Ptr<Socket> >	m_tcp_ptr;
	Ptr<Socket>			m_tcp;
	std::map < Ptr<Socket> ,int>	tcp_c_map;
	std::map < Ptr<Socket> ,uint32_t>	tcp_recv;
	std::map < Ptr<Socket> ,bool>	tcp_connected;
	std::map < Ptr<Socket> ,bool>	tcp_is_worm;
	std::vector< uint32_t > 	sentAck;
	Address         m_peer;
	uint32_t        m_sendSize;

	static Ptr<UniformRandomVariable> rngD;

	static uint16_t connections;

	void SendWorm(int);

	virtual void Receive(Ptr<Socket>, uint32_t); // Data received
	virtual void DataSend(uint32_t , Ptr<Socket>); 
	virtual void ConnectionComplete(Ptr<Socket>);
	virtual void ConnectionFailed(Ptr<Socket>);
	virtual void CloseRequest(Ptr<Socket>);

	virtual void StartApplication (void);
	virtual void StopApplication (void);
};

#endif
