/*
This is the implementation for worm. 
*/

#include "worm.h"



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
