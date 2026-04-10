#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("TcpChatExample");
int main(int argc, char *argv[])
{
	// Create 2 nodes
	NodeContainer nodes;
	nodes.Create(2);
	// Install Internet stack
	InternetStackHelper stack;
	stack.Install(nodes);
	// Create point-to-point link
	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
	p2p.SetChannelAttribute("Delay", StringValue("3ms"));
	NetDeviceContainer devices = p2p.Install(nodes);
	// Assign IP addresses
	Ipv4AddressHelper address;
	address.SetBase("10.2.2.0", "255.255.255.0");
	Ipv4InterfaceContainer interfaces = address.Assign(devices);
	uint16_t port = 9090;
	// TCP Server (Packet Sink)
	PacketSinkHelper sink("ns3::TcpSocketFactory",
                      	InetSocketAddress(Ipv4Address::GetAny(), port));
	ApplicationContainer serverApp = sink.Install(nodes.Get(1));
	serverApp.Start(Seconds(1.0));
	serverApp.Stop(Seconds(12.0));
	// TCP Client
	OnOffHelper client("ns3::TcpSocketFactory",
                   	InetSocketAddress(interfaces.GetAddress(1), port));
	client.SetAttribute("DataRate", StringValue("1Mbps"));
	client.SetAttribute("PacketSize", UintegerValue(1024));
	client.SetAttribute("OnTime",
                    	StringValue("ns3::ConstantRandomVariable[Constant=1]"));
	client.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
	ApplicationContainer clientApp = client.Install(nodes.Get(0));
	clientApp.Start(Seconds(2.0));
	clientApp.Stop(Seconds(12.0));
	// Run simulation
	Simulator::Run();
	Simulator::Destroy();
	return 0;
}

