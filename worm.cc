/*
This is the implementation for worm. 
*/

#include "worm.h"
#include <new>

using namespace std;
using namespace ns3;

#define DEFAULTSCANRANGE 65536

bool     Worm::initialized = false;
uint32_t Worm::scan_range = DEFAULTSCANRANGE; //scan_ramge = 65536 
// this is for setting vulnerabilities
Ptr<UniformRandomVariable> Worm::randVar = NULL;
uint32_t Worm::payloadlength = 376;
uint16_t Worm::infectionport = 1434;
//100% machines are vulnerable, please change the folowing for
// for making host's vulnerability be selected randomly
double Worm::vulnerability = 1.0;

Ipv4Address Worm::baseIP("10.1.0.0");

uint32_t  Worm::totalinfected = 0;
uint32_t  Worm::totalvulnerable = 0;
uint32_t  Worm::totalinstances = 0;

string   Worm::signature = "WORM";
char *   Worm::wormdata = NULL;

WormTargetVector* Worm::defaultTV = NULL;

Worm::Worm()
  : infected(false), vulnerable(true), started(false), node(NULL), targetV(NULL)
{
  cout << "Inside Worm Constructor." << endl;

  if (!initialized) {
    DoInitialize();
    initialized = true;
  }

  if (!wormdata) {
    PrepareWormData(wormdata);
/* wormdata is the buffer. It is static which means that all objects of type worm 
   will share the same data/buffer. It is initially set to NULL. Thus when the 
   first object of type worm is created, we set it to a series of 1s followed
   by the worm signature which is "WORM"
*/
  }

  if (vulnerability!=1.0)   {
    if (!randVar) 
	{
		cout << "Inside vulnerability" << endl;
		
		randVar = CreateObject<UniformRandomVariable> ();
		randVar-> SetAttribute ("Min", DoubleValue(0.0));
    		randVar-> SetAttribute ("Max", DoubleValue(1.0));
	}
  }

  // if there is no target vector object defined then we are setting the 
  // it to be the Uniform random target vector class.
  if (!defaultTV) {
    defaultTV = new WTVUniform(DEFAULTSCANRANGE);
  }
}

void Worm::PrepareWormData(char *(&buffer)) //equivalent to char **
{
  cout << "Inside PrepareWormData" << endl;

  buffer = new char[payloadlength];
  for (unsigned int i=0; i < payloadlength; i++){
    //buffer[i]='0'+i%10;
    buffer[i] = '1';
  }
  strncpy(buffer, signature.c_str(), signature.length());
  std::cout<<"WORM CONTENTS "<<buffer<<std::endl;
}

void Worm::Receive(Ptr<Socket> proto, uint32_t val )
{
//implement the same thing in udp and tcp
//Note: Class Socket is an abstract class and we cannot instantiate any object of this class
    
}

void Worm::ConnectionSucceeded(Ptr<Socket> proto)
{
}

void Worm::DataSend(Ptr<Socket> proto, uint32_t c)
{
}

void Worm::ConnectionFailed(Ptr<Socket> proto)
{
}

void Worm::CloseRequest(Ptr<Socket> proto)
{
}

void Worm::SetNode(Ptr<Node> arg_node) {

    cout << "Inside Worm set node. " << endl;

    node = arg_node;
    totalinstances ++;
    if (vulnerability != 1.0) {
	vulnerable = (randVar->GetValue()) <= vulnerability;
    }
    if (vulnerable) {
	totalvulnerable++;
    }
}

Ptr<Application> Worm::Copy() const {
    return new Worm(*this);
}

void Worm::Infect() {

    if(infected)
	return;

    cout << "Inside Worm infect(). " << endl;

    infected = true;
    totalinfected++;

    cout << "Worm:TotalInfected = " <<  totalinfected << endl;

    if (started) {
	Activate();
    }
}

void Worm::Activate() {
    //Yet to be decided. Usually overrided in wormudp and wormtcp
    //lets just colour the nodes

    cout << "Inside Worm Activate(). " << endl;

#ifdef HAVE_QT
    node->Color(Qt::red);
#endif
}

void Worm::DoInitialize() {
    std::cout<<"Initializing the Worm"<<std::endl;
    Application::DoInitialize();
}

bool Worm::IsInfected() {
    return infected;
}

void Worm::SetPayloadLength(uint32_t size) {
    payloadlength = size;    
}

void Worm::SetBaseIP(Ipv4Address addr) {
   baseIP = addr; 
}

void Worm::SetScanRange(uint32_t range) {
   scan_range = range; 
}

void Worm::SetVulnerability(double v) {
    vulnerability = v;
}

void Worm::SetInfectionPort(uint16_t arg_port) {
    infectionport = arg_port;
}

void Worm::SetTargetVector(const WormTargetVector &targetvector) {
    defaultTV = targetvector.Copy();
}

void Worm::SetSignature(std::string s) {
    signature = s;
}

uint32_t Worm::TotalInfected() {
    return totalinfected;
}

uint32_t Worm::TotalVulnerable() {
    return totalvulnerable;
}

uint32_t Worm::TotalInstances() {
    return totalinstances;
}

void Worm::StartApplication (void) {

    cout << "Entering WORM StartApplication()" << endl;
    
    if (infected) {
	Activate();
    }
    started = true;
}

void Worm::StopApplication (void) {
    started = false;
}

Ipv4Address Worm::GenerateNextIPAddress() {
    Ipv4Address IP;
    std::cout<<"Generating the next IP address"<<std::endl;
    if (!targetV) {
	cout << "No target vector."<<endl;
    	targetV = defaultTV->Copy();
	//cout << "1"<<endl;
        Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> (); 
	//cout << "2"<<endl;        
	Ipv4InterfaceAddress iaddr = ipv4->GetAddress (1,0);
	//cout << "3"<<endl;  
        Ipv4Address ipAddr = iaddr.GetLocal ();
	
	cout << "Raw form = " << ipAddr << " ,uint32_t form =" << ipAddr.Get() << endl; 
        targetV->Initialize(ipAddr.Get());
    }

    //cout <<baseIP.Get() << endl;
    //cout <<targetV->Generate() << endl;

    IP.Set(baseIP.Get() + targetV->Generate());

    //cout << IP.Get() << endl;
    return IP;
}

bool Worm::PacketIsWorm(Ptr<Packet> p) {
    /*Currently, every packet is a worm
    This can be changed later by analyzing the signature of the packet.
    Current signature is "WORM" and this is appended at the end of the 
    wormdata.
    */
    return true;
}

void Worm::SetStartTime(Time start) {
    std::cout<<"Worm Application started at "<<start<<std::endl;
    Application::SetStartTime(start);
}

void Worm::SetStopTime(Time stop) {
    std::cout<<"Worm Application stopping at "<<stop<<std::endl;
    Application::SetStopTime(stop);
}






