#include "WormTargetVector.h"

using namespace std;

WormTargetVector::WormTargetVector(uint32_t range) 
  : initialized(false), scanrange(range)
{
  localIP = new Ipv4Address(IPADDR_NONE);
  //localIP->Set(IPADDR_NONE);
}

/*WormTargetVector::WormTargetVector(uint32_t m)
{
  scanrange = 1<<(32-m);
}*/

WormTargetVector* WormTargetVector::Copy() const
{
  return new WormTargetVector(*this);
}

void WormTargetVector::Initialize(uint32_t IP)
{
  //cout<<"Initialize called"<<endl;
  localIP->Set(IP);
  initialized = true;
}

uint32_t WormTargetVector::Generate()
{
  if (!initialized) 
    cout<<"TargetVector Not Initialized!!"<<endl;
  return IPADDR_NONE;
}

WTVUniform::WTVUniform(uint32_t scanrange) : WormTargetVector(scanrange)
{
}

WormTargetVector* WTVUniform::Copy() const
{
  return new WTVUniform(*this);
}

void WTVUniform::Initialize(uint32_t IP)
{
  WormTargetVector::Initialize(IP);

  randGen = CreateObject<UniformRandomVariable> ();
  randGen-> SetAttribute ("Min", IntegerValue(0.0));
  randGen-> SetAttribute ("Max", IntegerValue(scanrange));
}

uint32_t WTVUniform::Generate()
{
  WormTargetVector::Generate();

  return ((uint32_t)randGen->GetValue());
}
