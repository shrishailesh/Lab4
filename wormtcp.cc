#include "ns3/wormtcp.h"
#include "ns3/socket-factory.h"
#include "ns3/socket.h"
#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/tcp-tahoe.h"
#include "ns3/inet-socket-address.h"
#include "ns3/string.h"

using namespace std;

NS_OBJECT_ENSURE_REGISTERED (WormTCP);

uint16_t  WormTCP::connections = 1;

TypeId WormTCP::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WormTCP")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<WormTCP> ()
  ;

  return tid;
}

WormTCP::WormTCP()
  : m_tcp(NULL)
{

	cout << "Inside WormTCP Constructor(). " << endl;

	m_tcp_ptr.clear();
	tcp_c_map.clear();
	tcp_recv.clear();
	tcp_is_worm.clear();
	tcp_connected.clear();
	sentAck.clear();
}

void WormTCP::StartApplication()     // Called at time specified by Start
{
  Worm:: StartApplication();

  cout << "Inside TCP worm start app function" << endl;

  //Ptr<Node> arg_node = GetNode();
}

void WormTCP::StopApplication()    // Called at time specified by Stop
{
  cout << "Inside WormTCP StopApp(). " << endl;

  Worm :: StopApplication();
}

void WormTCP::SetNode (Ptr<Node> arg_node)
{
  //ns3::Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpTahoe"));

  cout << "Inside WormTCP SetNode(). " << endl;

  Worm::SetNode(arg_node);

  m_tcp = Socket::CreateSocket (arg_node, TcpSocketFactory::GetTypeId());

  cout << "Server socket created" << endl;

  Ptr<Ipv4> ipv4; //= arg_node->GetObject<Ipv4>();
  Ipv4InterfaceAddress iaddr; //= ipv4->GetAddress (1,0);
  Ipv4Address addri; //= iaddr.GetLocal ();
  //m_tcp->Bind(InetSocketAddress((Ipv4Address)addri, infectionport));
  m_tcp->Bind(InetSocketAddress(Ipv4Address::GetAny(), infectionport));
  m_tcp->Listen();
  m_tcp->ShutdownSend ();
  m_tcp->SetRecvCallback (
	MakeCallback (&WormTCP::Receive, this));
  m_tcp->SetAcceptCallback (
	MakeNullCallback<bool, Ptr<Socket>, const Address &> (),
	MakeCallback (&WormTCP::ConnectionAccept, this));

  for(int i=0; i<(int) connections;i++)
  {
	Ptr<Socket> temp_socket = Socket::CreateSocket (arg_node, TcpSocketFactory::GetTypeId());
	temp_socket->Bind(InetSocketAddress((Ipv4Address)addri, infectionport));

	cout << "Bound=" << i << endl;

	m_tcp_ptr.push_back(temp_socket);
	sentAck.push_back(0);
	tcp_c_map[temp_socket]=i;
	tcp_connected[temp_socket] = false;

	m_tcp_ptr[i]->SetConnectCallback (
		MakeCallback (&WormTCP::ConnectionSucceeded, this),
		MakeCallback (&WormTCP::ConnectionFailed, this));
	m_tcp_ptr[i]->SetSendCallback (
		MakeCallback (&WormTCP::DataSend, this));
	m_tcp_ptr[i]->SetCloseCallbacks (
		MakeCallback (&WormTCP::CloseRequest, this),
		MakeNullCallback<void, Ptr<Socket> > ());
  }
}

Ptr<Node> WormTCP::GetNode ()
{
  cout << "Inside WormTCP GetNode(). " << endl;

  return node; 
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
  cout << "Inside WormTCP Activate(). " << endl;

  for (int i =0; i<(int) connections; i++) 
  	SendWorm(i);
}

void WormTCP::DoInitialize()
{
  cout<<"TCP worm initialize"<<endl;
  // override this in child classes
}

void WormTCP::SendWorm(int threadno)
{ 
  cout << "Inside WormTCP SendWorm(). " << endl;

  //cout << "TCP worm " << baseIP.Get() << endl;

  //cout << "Middle" << endl;

  //cout << "TCP worm " << (GenerateNextIPAddress()).Get() << endl;

  //m_peer.Set((GenerateNextIPAddress()).Get());
  
  Ptr<Node> temp_node = GetNode();

  Ptr<Ipv4> ipv4= temp_node->GetObject<Ipv4>();
  Ipv4InterfaceAddress iaddr= ipv4->GetAddress (1,0);
  Ipv4Address addri= iaddr.GetLocal ();

  m_peer.Set((uint32_t)167837954);

  if(m_peer.Get() == addri.Get())
	m_peer.Set((uint32_t)167837953);

  cout << "peer set with IP " << m_peer << " ,uint32_t form " << m_peer.Get() << endl;

  m_tcp_ptr[threadno]->Connect(InetSocketAddress(m_peer, infectionport));
  m_tcp_ptr[threadno]->ShutdownRecv ();

  cout << "connected" << endl;

  Ptr<Packet> packet = Create<Packet> ((uint8_t * const)wormdata, payloadlength);
  int actual = m_tcp_ptr[threadno]->Send(packet);
  sentAck[threadno]+=(actual);

  cout << "sent packets " << sentAck[threadno] << endl;

  if(sentAck[threadno]>=payloadlength)
  {
  	m_tcp_ptr[threadno]->Close();
  	tcp_connected[m_tcp_ptr[threadno]]=false;
  }
  cout << "Exiting SendWorm()" << endl;
}

void WormTCP::ConnectionSucceeded(Ptr<Socket> sock)
{
  cout << "Inside WormTCP ConnectionSucceeded(). " << endl;

  int threadno = tcp_c_map[sock];
  sentAck[threadno] = 0;
  tcp_connected[m_tcp_ptr[threadno]]=true;
  SendWorm(threadno);
}

void WormTCP::ConnectionFailed(Ptr<Socket> sock)
{
  cout << "Inside WormTCP ConnectionFailed(). " << endl;

  //int threadno = tcp_c_map[sock];
  //SendWorm(threadno);
}

void WormTCP::DataSend (Ptr<Socket> sock, uint32_t)
{
  cout << "Inside WormTCP DataSend(). " << endl;

  int threadno = tcp_c_map[sock];
  SendWorm(threadno);
}

void WormTCP::Receive(Ptr<Socket> sock)
{
  cout << "Inside WormTCP Receive(). " << endl;

  Address from;
  Ptr<Packet> pack = sock->RecvFrom (from);
  tcp_is_worm[sock] = PacketIsWorm(pack);
  uint32_t packet = pack->GetSize();
  tcp_recv[sock] += packet;
  
  if (tcp_is_worm[sock] && tcp_recv[sock]>=payloadlength) {
    if (vulnerable && !infected) {
      Infect();
      //SendWorm(tcp_c_map[sock]);
    }
  }
  cout << "Exiting Receive" << endl;
}

void WormTCP::CloseRequest(Ptr<Socket> sock)
{
  cout << "Inside WormTCP CloseRequest(). " << endl;

  int threadno = tcp_c_map[sock];
  tcp_recv.erase(sock);
  tcp_is_worm.erase(sock);
  tcp_connected.erase(sock);
  m_tcp_ptr[threadno]->Close();

  //cout << "total infected = " << TotalInfected() << endl;

  //if(TotalInfected()==1)
//{
  Ptr<Node> arg_node = GetNode();

  Ptr<Ipv4> ipv4 = arg_node->GetObject<Ipv4>();
  Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
  Ipv4Address addri = iaddr.GetLocal ();

  Ptr<Socket> temp_socket = Socket::CreateSocket (arg_node, TcpSocketFactory::GetTypeId());
  temp_socket->Bind(InetSocketAddress((Ipv4Address)addri, infectionport));
  m_tcp_ptr[threadno]=temp_socket;
  sentAck[threadno]=0;
  tcp_c_map[temp_socket]=threadno;
  tcp_connected[temp_socket] = false;

  m_tcp_ptr[threadno]->SetConnectCallback (
	MakeCallback (&WormTCP::ConnectionSucceeded, this),
	MakeCallback (&WormTCP::ConnectionFailed, this));
  m_tcp_ptr[threadno]->SetSendCallback (
	MakeCallback (&WormTCP::DataSend, this));
  m_tcp_ptr[threadno]->SetCloseCallbacks (
	MakeCallback (&WormTCP::CloseRequest, this),
	MakeNullCallback<void, Ptr<Socket> > ());

  SendWorm(threadno);
//}
}

void WormTCP::ConnectionAccept (Ptr<Socket> s, const Address& from)
{
  cout << "inside TCP accept connection function" << endl;
  s->SetRecvCallback (MakeCallback (&WormTCP::Receive, this));
}
