#ifndef WORMUDP_H
#define WORMUDP_H

#include "worm.h"
//#include "ns3/application.h"
#include "ns3/timer.h"
//#include "ns3/udp-socket-factory.h"
#include "/home/rohit/ns-allinone-3.24.1/ns-3.24.1/src/internet/model/udp-socket-impl.h"
//#include "ns3/udp-l4-protocol.h"
//#include "ns3/udp-client.h"
//#include "ns3/udp-server.h"
//#include "ns3/nstime.h"
//#include "ns3/timer.h" 
//#include "ns3/simulator.h"
//#include "ns3/network-module.h"
//#include "ns3/ipv4.h"
//#include "ns3/udp-socket-factory-impl.h"
#include <map>

using namespace ns3;
using namespace std;
//class UdpSocketImpl;

class WormUDP : public Worm, public Timer {
public:
    WormUDP();
    virtual void SetNode(Ptr<Node>);
    virtual ~WormUDP();
    virtual Ptr<Application> Copy() const;
    static void SetScanRate(uint32_t);
    virtual void Initialize();
    virtual void Activate();
private:
    UdpSocketImpl*	udp;
    Timer       timer;
    static uint32_t 	scanrate;
    
    void SendWorm();
    void ScheduleNextPacket();
    
    virtual void Receive(Ptr<Socket>, uint32_t);

  //  void Timeout(TimerEvent *);

    virtual void StartApplication();
    virtual void StopApplication();
};

#endif

