#ifndef __worm_targetvector_h__
#define __worm_targetvector_h__

#include "ns3/random-variable-stream.h"
#include "ns3/application.h"
#include "ns3/ipv4-address.h"
#include "ns3/ptr.h"

#define IPADDR_NONE    (0x0)

class WormTargetVector : public Object {
public:
  WormTargetVector(uint32_t range);
  WormTargetVector(uint32_t m);

  virtual WormTargetVector* Copy() const;
  virtual uint32_t Generate();
  virtual void Initialize(Ipv4Address);
 private:
  bool initialized;
 protected:
  uint32_t scanrange;
  Ipv4Address localIP = new Ipv4Address();
};

class WTVUniform : public WormTargetVector {
 public:
  WTVUniform(uint32_t range);
  WTVUniform(Ipv4Address m);

  virtual WormTargetVector* Copy() const;
  virtual uint32_t Generate();
  virtual void Initialize(Ipv4Address);

 private:
  Ptr<UniformRandomVariable> randGen;
};

#endif
