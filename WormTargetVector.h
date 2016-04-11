#ifndef __worm_targetvector_h__
#define __worm_targetvector_h__

#include "ns3/random-variable-stream.h"
#include "ns3/application.h"
#include "ns3/ipv4-address.h"
#include "ns3/ptr.h"
#include "ns3/integer.h"

#define IPADDR_NONE (uint32_t)(0x0)

using namespace ns3;

class WormTargetVector : public Object {
public:
  WormTargetVector(uint32_t range);
  //WormTargetVector(Ipv4Address m);

  virtual WormTargetVector* Copy() const;
  virtual uint32_t Generate();
  virtual void Initialize(uint32_t);
 private:
  bool initialized;
 protected:
  uint32_t scanrange;
  Ipv4Address* localIP;
};

class WTVUniform : public WormTargetVector {
 public:
  WTVUniform(uint32_t range);
  //WTVUniform(Ipv4Address m);

  virtual WormTargetVector* Copy() const;
  virtual uint32_t Generate();
  virtual void Initialize(uint32_t);

 private:
  Ptr<UniformRandomVariable> randGen;
};

#endif
