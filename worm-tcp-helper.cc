#include "worm-tcp-helper.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet-socket-address.h"
#include "ns3/string.h"
#include "ns3/names.h"

namespace ns3 {

WormTcpHelper::WormTcpHelper (Ipv4Address address)
{
  m_factory.SetTypeId ("ns3::WormTCP");
}

WormTcpHelper::WormTcpHelper ()
{
  m_factory.SetTypeId ("ns3::WormTCP");
}

/*void
WormTcpHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}*/

ApplicationContainer
WormTcpHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

/*ApplicationContainer
WormTcpHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return ApplicationContainer (InstallPriv (node));
}*/

ApplicationContainer
WormTcpHelper::Install (NodeContainer c) const
{
  int j = 0;
  
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      if(j == 0)
      {
      	//apps.Add (InstallPrivInit (*i));
      	j++;
      }
      else
      {
      	apps.Add (InstallPriv (*i));
      	j++;
      }
    }

  apps.Add (InstallPrivInit (c.Get(0)));

  return apps;
}

Ptr<WormTCP>
WormTcpHelper::InstallPriv (Ptr<Node> node) const
{
  Ptr<WormTCP> app = m_factory.Create<WormTCP> ();
  node->AddApplication (app);
  app->SetNode(node);

  return app;
}

Ptr<WormTCP>
WormTcpHelper::InstallPrivInit (Ptr<Node> node) const
{
  Ptr<WormTCP> app = m_factory.Create<WormTCP> ();
  node->AddApplication (app);
  app->SetNode(node);
  app->Activate();

  return app;
}

} // namespace ns3
