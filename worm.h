#ifndef ns3_worm_h_
#define ns3_worm_h_

#include "ns3/application.h"
#include "ns3/ipv4-address.h"
#include "ns3/random-variable-stream.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/socket.h"
#include "WormTargetVector.h"
#include "ns3/ptr.h"
#include "ns3/double.h"
#include "ns3/ipv4.h"
#include <stdint.h>
#include <string.h>

class Worm : public Application
{
protected:
	bool infected;      // indicates if this node is in infected stage
	bool vulnerable;
	bool started;       // used to know if the application of worm has started
  	Ptr<Node> node;
	
	static uint32_t payloadlength;
	static uint16_t infectionport;
	static double vulnerability;
	static Ipv4Address baseIP;
	static std::string signature;    // signature string that is sent in packets
	static char* wormdata;           // the contents of the worm  (this is sent usually sent)

	Ipv4Address GenerateNextIPAddress();
	bool PacketIsWorm(Ptr<Packet>);   // I am going to use this to check if packet is a worm
	virtual void PrepareWormData(char *&);
	virtual void Activate();
	virtual void StartApplication (void);    // Called at time specified by Start
	virtual void StopApplication (void);     // Called at time specified by Stop

	virtual void Receive(Ptr<Socket>, uint32_t); // Data received
	virtual void DataSend(Ptr<Socket>, uint32_t); 
	virtual void ConnectionSucceeded(Ptr<Socket>);
	virtual void ConnectionFailed(Ptr<Socket>);
	virtual void CloseRequest(Ptr<Socket>);

 private:
	static uint32_t  totalinfected;    // keeps count of total infected instances
	static uint32_t  totalvulnerable;  // keeps count of total vulnerable instances
	static uint32_t  totalinstances;
        static uint32_t  scan_range;

	WormTargetVector *targetV;
	static WormTargetVector *defaultTV;

	static bool initialized;  // set to see if worm class was initialized
	static Ptr<UniformRandomVariable> randVar;

public:

	Worm(); // Constructor.
	void SetNode (Ptr<Node> arg_node); // Note which node attached to
	virtual ~Worm() {};
	virtual Ptr<Application> Copy() const;// Make a copy of the application
	virtual void DoInitialize(void);
	void Infect();
	bool IsInfected();
	static void SetPayloadLength(uint32_t);
	static void SetBaseIP(Ipv4Address);
	static void SetScanRange(uint32_t);
	static void SetVulnerability(double);
	static void SetInfectionPort(uint16_t);
	static void SetTargetVector(const WormTargetVector &);
	static void SetSignature(std::string s);
	static uint32_t  TotalInfected() ;
	static uint32_t  TotalVulnerable() ;
	static uint32_t  TotalInstances() ;
	//void SetStartTime (Time start);
	//void SetStopTime (Time stop);	
};

#endif
