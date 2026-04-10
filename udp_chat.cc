#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h" 
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("UdpChatExample");
int main(int argc, char *argv[])
{
	// Create 2 nodes (client + server)
	NodeContainer nodes;
	nodes.Create(2);
	// Install Internet stack
	InternetStackHelper stack;
	stack.Install(nodes);
	// Create point-to-point link
	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate", StringValue("8Mbps"));
	p2p.SetChannelAttribute("Delay", StringValue("2ms"));
	NetDeviceContainer devices = p2p.Install(nodes);
	// Assign IP addresses
	Ipv4AddressHelper address;
	address.SetBase("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer interfaces = address.Assign(devices);
	// UDP Server
	uint16_t port = 8080;
	UdpServerHelper server(port);
	ApplicationContainer serverApp = server.Install(nodes.Get(1));
	serverApp.Start(Seconds(1.0));
	serverApp.Stop(Seconds(12.0));
	// UDP Client
	UdpClientHelper client(interfaces.GetAddress(1), port);
	client.SetAttribute("MaxPackets", UintegerValue(7));
	client.SetAttribute("Interval", TimeValue(Seconds(1.0)));
	client.SetAttribute("PacketSize", UintegerValue(1024));
	ApplicationContainer clientApp = client.Install(nodes.Get(0));
	clientApp.Start(Seconds(2.0));
	clientApp.Stop(Seconds(12.0));
	// Run simulation
	Simulator::Run();
	Simulator::Destroy();
	return 0;
}
