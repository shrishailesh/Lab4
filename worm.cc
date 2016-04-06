/*
This is the implementation for worm. 
*/

#include "worm.h"

using namespace std;

#define DEFAULTSCANRANGE 65536

bool     Worm::initialized = false;
// this is for setting vulnerabilities
Ptr<UniformRandomVariable> Worm::randVar = NULL;
uint32_t Worm::payloadlength = 376;
uint16_t Worm::infectionport = 1434;
//100% machines are vulnerable, please change the folowing for
// for making host's vulnerability be selected randomly
double Worm::vulnerability = 1.0;

Ipv4Address Worm::baseIP.SetBase ("10.1.0.0", "255.255.0.0");

uint32_t  Worm::totalinfected = 0;
uint32_t  Worm::totalvulnerable = 0;
uint32_t  Worm::totalinstances = 0;

string   Worm::signature = "WORM";
char *   Worm::wormdata = NULL;

WormTargetVector* Worm::defaultTV = NULL;

Worm::Worm()
  : infected(false), vulnerable(true), started(false), node(NULL), targetV(NULL)
{

  if (!initialized) {
    DoInitialize();
    initialized = true;
  }

  if (!wormdata) {
    PrepareWormData(wormdata);
  }

  if (vulnerability!=1.0)   {
    if (!randVar) 
	{
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

void Worm::PrepareWormData(char *&buffer)
{
  buffer = new char[payloadlength];
  for (unsigned int i=0; i < payloadlength; i++)
    buffer[i]='0'+i%10;
  strncpy(buffer, signature.c_str(), signature.length());
}

void Worm::Receive(Ptr<Packet> p, Ptr<IpL4Protocol> proto, uint32_t)
{
  delete p;
}

void Worm::ConnectionComplete(Ptr<IpL4Protocol> proto)
{
}

void Worm::Sent(uint32_t c, Ptr<IpL4Protocol> proto)
{
}

void Worm::ConnectionFailed(Ptr<IpL4Protocol> proto, bool)
{
}

void Worm::CloseRequest(Ptr<IpL4Protocol> proto)
{
}

void Worm::Closed(Ptr<IpL4Protocol> proto)
{
}

void Worm::SetNode(Ptr<Node> arg_node) {
    node = arg_node;
    totalinstances ++;
    if (vulnerability != 1.0) {
	vulnerable = (randvar->GetValue()) <= vulnerability;
    }
    if (vulnerable) {
	totalvulnerable++;
    }
}

Ptr<Application> Worm::Copy() const {
    return new Worm(*this);
}

void Worm::Infect() {
    infected = true;
    totalinfected++;
    if (started) {
	Activate();
    }
}

void Worm::Activate() {
    //Yet to be decided. Usually overrided in wormudp and wormtcp
}

void Worm::Initialize() {
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
    if (infected) {
	Activate();
    }
    started = true;
    Application::StartApplication (void) ;
}

void Worm::StopApplication (void) {
    started = false;
    Application::StopApplication(void);
}

Ipv4Address Worm::GenerateNextIPAddress() {
    Ipv4Address IP;
    if (!targetV) {
    	targetV = defaultTV->Copy();
	Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
        Ipv4Address addr = ipv4->GetAddress(0,0);
    }

    IP = baseIP + targetV->Generate();
    return IP;
}
    }
}
