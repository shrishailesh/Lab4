#include "wormudp.h"
#include "ns3/node.h"
#include "ns3/udp-socket-factory-impl.h"
#include "ns3/udp-socket-impl.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/udp-socket.h"
#include "ns3/udp-client.h"
#include "ns3/udp-server.h"
#include "ns3/udp-client.h"

using namespace ns3;


uint32_t WormUDP::scanrate = 50;

WormUDP::WormUDP() {
}

WormUDP::~WormUDP() {
}

void WormUDP::StartApplication() {
    Worm::StartApplication();
}

void WormUDP::StopApplication() {
    Worm::StopApplication();
}

Ptr<Application> WormUDP::Copy() const {
    return new WormUDP(*this);
}

void WormUDP::SetNode(Ptr<Node> n) {
    Worm::SetNode(n);
/*  udp = new UDP(n);
    udp->Attach(n);
    udp->Bind(infectionport);
    udp->AttachApplication(this);
*/
    Ptr<Ipv4> ipv4 = PtrNode->GetObject<Ipv4> ();
    IpvInterfaceAddress iaddr = ipv4.GetAddress (1,0);
    Ipv4Address ipAddr = iaddr.GetLocal ();

    udp = new UdpSocketImpl;
    udp->SetNode(n);
    udp->Bind(ipAddr);

    //Do we need to do anything else??
}

void WormUDP::Activate() {
#ifdef HAVE_QT
    node->Color(Qt::red);
#endif
    Ptr<Ipv4> ipv4 = PtrNode->GetObject<Ipv4> ();
    IpvInterfaceAddress iaddr = ipv4.GetAddress (1,0);
    Ipv4Address ipAddr = iaddr.GetLocal ();         
    SendWorm();
    ScheduleNextPacket();
}

void WormUDP::Initialize() {

}

void WormUDP::SendWorm() {
    Ipv4Address target = GenerateNextIPAdress();
    Ptr<Ipv4> ipv4 = node->GetObject<Ipv4>();
    Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
    Ipv4Address addri = iaddr.GetLocal ();
    InetSocketAddress inetAddress(target, infectionport);
    Ptr<ns3::Packet> packet = new ns3::Packet(payloadlength);
    udp->SendTo(packet, (uint32_t)0, inetAddress);
}

void WormUDP::SetScanRate(uint32_t rate) {
    scanrate = rate;
}

void WormUDP::ScheduleNextPacket() {

}

void WormUDP::Timeout(TimerEvent *event) {

}

void WormUDP::Receive(Ptr<Packet> p, Ptr<IpL4Protocol> proto, Seq_t seq) {
    if (PacketIsWorm(p)) {
	if (vulnerable && !infected) {
	    Infect();
	}
     }
     delete p;
}
}



