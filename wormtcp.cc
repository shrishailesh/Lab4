#include "ns3/wormtcp.h"
#include "ns3/socket-factory.h"
#include "ns3/socket.h"
#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/tcp-tahoe.h"

using namespace std;

Ptr<UniformRandomVariable>  WormTCP::rngD = NULL;
uint16_t  WormTCP::connections = 2;

WormTCP::WormTCP()
  : m_tcp(NULL)
{
	m_tcp_ptr.clear();
	tcp_c_map.clear();
	tcp_recv.clear();
	tcp_is_worm.clear();
	tcp_connected.clear();
	sentAck.clear();
	if (!rngD)
	{
		rngD = CreateObject<UniformRandomVariable> ();
		rngD-> SetAttribute ("Min", DoubleValue(0.0));
    		rngD-> SetAttribute ("Max", DoubleValue(1.0));
	}
}

void WormTCP::StartApplication()     // Called at time specified by Start
{
  Worm:: StartApplication();
}

void WormTCP::StopApplication()    // Called at time specified by Stop
{
  Worm :: StopApplication();
}

void WormTCP::SetNode (Ptr<Node> arg_node)
{
	
	Worm::SetNode(arg_node);

	Ptr<Socket> m_tcp = Socket::CreateSocket (arg_node, TcpTahoe::GetTypeId());
       	Ptr<Ipv4> ipv4 = arg_node->GetObject<Ipv4>();
       	Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
       	Ipv4Address addri = iaddr.GetLocal ();
	m_tcp->Bind(InetSocketAddress((Ipv4Address)addri, infectionport));
	m_tcp->Listen();
	arg_node->AddApplication(this);

	for(int i=0; i<(int) connections;i++)
	{
		Ptr<Socket> temp_socket = Socket::CreateSocket (arg_node, TcpTahoe::GetTypeId());
        	ipv4 = arg_node->GetObject<Ipv4>();
        	iaddr = ipv4->GetAddress (1,0);
        	addri = iaddr.GetLocal ();
		temp_socket->Bind(InetSocketAddress((Ipv4Address)addri, infectionport));
		m_tcp_ptr.push_back(temp_socket);
		sentAck.push_back(0);
		arg_node->AddApplication(this);
		tcp_c_map[temp_socket]=i;
		tcp_connected[temp_socket] = false;
		m_tcp_ptr[i]->Connect (m_peer);
      		m_tcp_ptr[i]->ShutdownRecv ();
      		m_tcp_ptr[i]->SetConnectCallback (
        		MakeCallback (&WormTCP::ConnectionSucceeded, this),
        		MakeCallback (&WormTCP::ConnectionFailed, this));
      		m_tcp_ptr[i]->SetSendCallback (
        		MakeCallback (&WormTCP::DataSend, this));
		m_tcp_ptr[i]->SetRecvCallback (
			MakeCallback (&WormTCP::Receive, this));
		m_tcp_ptr[i]->SetCloseCallbacks (
    			MakeCallback (&WormTCP::CloseRequest, this),
    			MakeNullCallback<void, Ptr<Socket> > ());
	}
}

Ptr<Application> WormTCP::Copy() const
{
  return new WormTCP(*this);
}

void WormTCP::SetConnections(uint16_t val)
{
  connections = val;
}

void WormTCP::Activate()
{
  for (int i =0; i<(int) connections; i++) 
  	SendWorm(i);
}

void WormTCP::DoInitialize()
{
  cout<<"TCP initialize"<<endl;
  // override this in child classes
}

void WormTCP::SendWorm(int threadno)
{ 
  m_peer.Set(baseIP.Get() + (GenerateNextIPAddress()).Get());
  
  m_tcp_ptr[threadno]->Connect(m_peer);
  Ptr<Packet> packet = Create<Packet> ((uint8_t * const)wormdata, payloadlength);
  int actual = m_tcp_ptr[threadno]->Send(packet);
  sentAck[threadno]+=(actual);

  if(sentAck[threadno]>=payloadlength)
  {
  	m_tcp_ptr[threadno]->Close();
  	tcp_connected[m_tcp_ptr[threadno]]=false;
  }

}

void WormTCP::ConnectionSucceeded(Ptr<Socket> sock)
{
  int threadno = tcp_c_map[sock];
  sentAck[threadno] = 0;
  tcp_connected[m_tcp_ptr[threadno]]=true;
  SendWorm(threadno);
}

void WormTCP::ConnectionFailed(Ptr<Socket> sock)
{
  int threadno = tcp_c_map[sock];
  SendWorm(threadno);
}

void WormTCP::DataSend (Ptr<Socket> sock, uint32_t)
{
  int threadno = tcp_c_map[sock];
  //sentAck[threadno]+=val;
  SendWorm(threadno);
}

void WormTCP::Receive(Ptr<Socket> sock)
{
  //int threadno = tcp_c_map[sock];
  Address from;
  Ptr<Packet> pack = sock->RecvFrom (from);
  tcp_is_worm[sock] = PacketIsWorm(pack);
  uint32_t packet = pack->GetSize();
  tcp_recv[sock] = tcp_recv[sock] + packet;
  
  if (tcp_is_worm[sock] && tcp_recv[sock]>=payloadlength) {
    if (vulnerable && !infected) {
      Infect();
    }
  }
}

void WormTCP::CloseRequest(Ptr<Socket> sock)
{
  int threadno = tcp_c_map[sock];
  tcp_recv.erase(sock);
  tcp_is_worm.erase(sock);
  m_tcp_ptr[threadno]->Close();
}


