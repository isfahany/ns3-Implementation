#include <iostream>
#include <fstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/bridge-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ProjectAkhir_Milestone-2");

#define vssearch(loc,vec) std::find ((vec).begin (), (vec).end (), (loc)) != (vec).end ()

int
main (int argc, char *argv[])
{
  bool        verbose              = true;

  int         simDurationSeconds   = 60;

  bool        enableUdpMultiSW     = true;
  bool        enableUdpSingleSW    = true;

  std::string pcapLocations        = "";
  uint32_t    snapLen              = PcapFile::SNAPLEN_DEFAULT;

  std::string csmaXLinkDataRate    = "100Mbps";
  std::string csmaXLinkDelay       = "500ns";

  std::string csmaYLinkDataRate    = "10Mbps";
  std::string csmaYLinkDelay       = "500ns";

  std::string p2pLinkDataRate      = "5Mbps";
  std::string p2pLinkDelay         = "50ms";

  uint16_t    udpEchoPort          = 9;  // The well-known UDP echo port


  CommandLine cmd;

  cmd.Usage    ("NOTE: valid --pcap arguments are: 't2,t3,b2,b3,trlan,trwan,brlan,brwan'");

  cmd.AddValue ("verbose",      "Enable printing informational messages",        verbose);

  cmd.AddValue ("duration",     "Duration of simulation.",                       simDurationSeconds);

  cmd.AddValue ("udpMultiSW",   "Enable udp over multi-switch links",            enableUdpMultiSW);
  cmd.AddValue ("udpSingleSW",  "Enable udp over single-switch links",           enableUdpSingleSW);

  cmd.AddValue ("pcap",         "Comma separated list of PCAP Locations to tap", pcapLocations);
  cmd.AddValue ("snapLen",      "PCAP packet capture length",                    snapLen);

  cmd.AddValue ("csmaXRate",    "CSMA X Link data rate",                         csmaXLinkDataRate);
  cmd.AddValue ("csmaXDelay",   "CSMA X Link delay",                             csmaXLinkDelay);

  cmd.AddValue ("csmaYRate",    "CSMA Y Link data rate",                         csmaYLinkDataRate);
  cmd.AddValue ("csmaYDelay",   "CSMA Y Link delay",                             csmaYLinkDelay);

  cmd.AddValue ("p2pRate",      "P2P Link data rate",                            p2pLinkDataRate);
  cmd.AddValue ("p2pDelay",     "P2P Link delay",                                p2pLinkDelay);

  cmd.Parse (argc, argv);

 if (verbose)
    {
      LogComponentEnable ("ProjectAkhir_Milestone-2", LOG_LEVEL_INFO);
    }

 NS_LOG_INFO ("INFO: Create nodes.");    // - - - - - - - - - - - - - - - -
                                          // Node IP     : Description
                                          // - - - - - - - - - - - - - - - -
  Ptr<Node> t3  = CreateObject<Node> ();  // 192.168.1.3 : Top single-switch   udp echo server
  Ptr<Node> t2  = CreateObject<Node> ();  // 192.168.1.2 :
                                          //             :
  Ptr<Node> t4  = CreateObject<Node> ();  // 192.168.1.4 :
                                          //             :
  Ptr<Node> t5  = CreateObject<Node> ();  // 192.168.1.5 :
                                          //
  Ptr<Node> ts1 = CreateObject<Node> ();  // <no IP>     : Top switch #1 (bridge)
                                          //             :
  Ptr<Node> tr  = CreateObject<Node> ();  // 192.168.1.1 : Router connecting top LAN & WAN
                                          // 76.1.1.1    :
                                          //             :
  //=========================================================================
  Ptr<Node> br  = CreateObject<Node> ();  // 76.1.1.2    : Router connecting WAN & bottom LANs
                                          // 192.168.2.1 :
                                          //             :
  Ptr<Node> bs1 = CreateObject<Node> ();  // <no IP>     : Bottom switch #1 (bridge)
                                          //             :

  Ptr<Node> b3  = CreateObject<Node> ();  // 192.168.2.3 : Bottom single-switch   udp echo client
  Ptr<Node> b2  = CreateObject<Node> ();  // 192.168.2.2 :
                                          //             :
  Ptr<Node> b4  = CreateObject<Node> ();  // 192.168.2.4 :
                                          //             :
  Ptr<Node> b5  = CreateObject<Node> ();  // 192.168.2.5 :
                                          //             :
  //=========================================================================
  Ptr<Node> rr  = CreateObject<Node> ();  // 76.1.1.3    : Router connecting WAN & bottom LANs
                                          // 192.168.3.1 :
                                          //             :
  Ptr<Node> rs1 = CreateObject<Node> ();  // <no IP>     : Right switch #1 (bridge)
                                          //             :

  Ptr<Node> r3  = CreateObject<Node> ();  // 192.168.3.3 : Right single-switch   udp echo client
  Ptr<Node> r2  = CreateObject<Node> ();  // 192.168.3.2 :
                                          //             :
  Ptr<Node> r4  = CreateObject<Node> ();  // 192.168.3.4 :
                                          //             :
  Ptr<Node> r5  = CreateObject<Node> ();  // 192.168.3.5 :
                                          //             :
  //=========================================================================
  Ptr<Node> lr  = CreateObject<Node> ();  // 76.1.1.4    : Router connecting WAN & bottom LANs
                                          // 192.168.4.1 :
                                          //             :
  Ptr<Node> ls1 = CreateObject<Node> ();  // <no IP>     : Left switch #1 (bridge)
                                          //             :

  Ptr<Node> l3  = CreateObject<Node> ();  // 192.168.4.3 : Left single-switch   udp echo client
  Ptr<Node> l2  = CreateObject<Node> ();  // 192.168.4.2 :
                                          //             :
  Ptr<Node> l4  = CreateObject<Node> ();  // 192.168.4.4 :
                                          //             :
  Ptr<Node> l5  = CreateObject<Node> ();  // 192.168.4.5 :
                                          //             :
                                          // - - - - - - - - - - - - - - - -

  // ----------------------------------------------------------------------
  // Give the nodes names
  // ----------------------------------------------------------------------
  Names::Add ("t2",  t2);
  Names::Add ("t3",  t3);
  Names::Add ("t4",  t4);
  Names::Add ("t5",  t5);
  Names::Add ("ts1", ts1);
  Names::Add ("tr",  tr);
  Names::Add ("br",  br);
  Names::Add ("bs1", bs1);
  Names::Add ("b2",  b2);
  Names::Add ("b3",  b3);
  Names::Add ("b4",  b4);
  Names::Add ("b5",  b5);
  Names::Add ("rr",  rr);
  Names::Add ("rs1", rs1);
  Names::Add ("r2",  r2);
  Names::Add ("r3",  r3);
  Names::Add ("r4",  r4);
  Names::Add ("r5",  r5);
  Names::Add ("lr",  lr);
  Names::Add ("ls1", ls1);
  Names::Add ("l2",  l2);
  Names::Add ("l3",  l3);
  Names::Add ("l4",  l4);
  Names::Add ("l5",  l5);
  // ======================================================================
  // Create CSMA links to use for connecting LAN nodes together
  // ----------------------------------------------------------------------
  // ----------------------------------------
  // CSMA [X]
  // ----------------------------------------
  NS_LOG_INFO ("L2: Create a " <<
               csmaXLinkDataRate << " " <<
               csmaXLinkDelay << " CSMA link for csmaX for LANs.");
  CsmaHelper csmaX;
  csmaX.SetChannelAttribute ("DataRate", StringValue (csmaXLinkDataRate));
  csmaX.SetChannelAttribute ("Delay",    StringValue (csmaXLinkDelay));

  // ----------------------------------------
  // CSMA [Y]
  // ----------------------------------------
  NS_LOG_INFO ("L2: Create a " <<
               csmaYLinkDataRate << " " <<
               csmaYLinkDelay << " CSMA link for csmaY for LANs.");
  CsmaHelper csmaY;
  csmaY.SetChannelAttribute ("DataRate", StringValue (csmaYLinkDataRate));
  csmaY.SetChannelAttribute ("Delay",    StringValue (csmaYLinkDelay));

  // ----------------------------------------------------------------------
  // Now, connect the top LAN nodes together with csma links.
  // ----------------------------------------------------------------------
  NS_LOG_INFO ("L2: Connect nodes on top LAN together with half-duplex CSMA links.");

  // Node Client to Switch TOP
  NetDeviceContainer link_t2_ts1   = csmaX.Install (NodeContainer (t2,  ts1));
  NetDeviceContainer link_t3_ts1   = csmaX.Install (NodeContainer (t3,  ts1));
  NetDeviceContainer link_t4_ts1   = csmaX.Install (NodeContainer (t4,  ts1));
  NetDeviceContainer link_t5_ts1   = csmaX.Install (NodeContainer (t5,  ts1));
  // Switch ts1 to Router tr
  NetDeviceContainer link_tr_ts1   = csmaY.Install (NodeContainer (tr,  ts1));

  NS_LOG_INFO ("L2: Connect nodes on bottom LAN together with half-duplex CSMA links.");

  // Node Client to Switch BOTTTOM
  NetDeviceContainer link_b2_bs1   = csmaY.Install (NodeContainer (b2,  bs1));
  NetDeviceContainer link_b3_bs1   = csmaY.Install (NodeContainer (b3,  bs1));
  NetDeviceContainer link_b4_bs1   = csmaY.Install (NodeContainer (b4,  bs1));
  NetDeviceContainer link_b5_bs1   = csmaY.Install (NodeContainer (b5,  bs1));

  // Switch to Router
  NetDeviceContainer link_br_bs1   = csmaX.Install (NodeContainer (br,  bs1));
  
  // Node Client to Switch RIGHT
  NetDeviceContainer link_r2_rs1   = csmaY.Install (NodeContainer (r2,  rs1));
  NetDeviceContainer link_r3_rs1   = csmaY.Install (NodeContainer (r3,  rs1));
  NetDeviceContainer link_r4_rs1   = csmaY.Install (NodeContainer (r4,  rs1));
  NetDeviceContainer link_r5_rs1   = csmaY.Install (NodeContainer (r5,  rs1));

  // Switch to Router
  NetDeviceContainer link_rr_rs1   = csmaX.Install (NodeContainer (rr,  rs1));

  // Node Client to Switch LEFT
  NetDeviceContainer link_l2_ls1   = csmaY.Install (NodeContainer (l2,  ls1));
  NetDeviceContainer link_l3_ls1   = csmaY.Install (NodeContainer (l3,  ls1));
  NetDeviceContainer link_l4_ls1   = csmaY.Install (NodeContainer (l4,  ls1));
  NetDeviceContainer link_l5_ls1   = csmaY.Install (NodeContainer (l5,  ls1));

  // Switch to Router
  NetDeviceContainer link_lr_ls1   = csmaX.Install (NodeContainer (lr,  ls1));

  // ======================================================================
  // Create a point-to-point link for connecting WAN nodes together
  // (this type of link is full-duplex)
  // ----------------------------------------------------------------------
  NS_LOG_INFO ("L2: Create a " <<
               p2pLinkDataRate << " " <<
               p2pLinkDelay    << " Point-to-Point link for the WAN.");

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute  ("DataRate", StringValue (p2pLinkDataRate));
  p2p.SetChannelAttribute ("Delay",    StringValue (p2pLinkDelay));

  // ----------------------------------------------------------------------
  // Now, connect top router to bottom router with a p2p WAN link
  // ----------------------------------------------------------------------
  // P2P Router to Router
  // ---------------------------------------------------------------------
  NS_LOG_INFO ("L2: Connect the routers together with the Point-to-Point WAN link.");

  NetDeviceContainer link_tr_br;
  link_tr_br = p2p.Install (NodeContainer (tr,br));
  NetDeviceContainer link_tr_lr;
  link_tr_lr = p2p.Install (NodeContainer (tr,lr));
  NetDeviceContainer link_br_rr;
  link_br_rr = p2p.Install (NodeContainer (br,rr));
  NetDeviceContainer link_rr_lr;
  link_rr_lr = p2p.Install (NodeContainer (rr,lr));
  
  // ======================================================================
  // Manually create the list of NetDevices for each switch
  // ----------------------------------------------------------------------

  // Top Switch 1 NetDevices
  NetDeviceContainer ts1nd;
  ts1nd.Add (link_tr_ts1.Get (1));
  ts1nd.Add (link_t2_ts1.Get (1));
  ts1nd.Add (link_t3_ts1.Get (1));
  ts1nd.Add (link_t4_ts1.Get (1));
  ts1nd.Add (link_t5_ts1.Get (1));
  
  // Bottom Switch 1 NetDevices
  NetDeviceContainer bs1nd;
  bs1nd.Add (link_br_bs1.Get (1));
  bs1nd.Add (link_b2_bs1.Get (1));
  bs1nd.Add (link_b3_bs1.Get (1));
  bs1nd.Add (link_b4_bs1.Get (1));
  bs1nd.Add (link_b5_bs1.Get (1));
  

  // Right Switch 1 NetDevices
  NetDeviceContainer rs1nd;
  rs1nd.Add (link_rr_rs1.Get (1));
  rs1nd.Add (link_r2_rs1.Get (1));
  rs1nd.Add (link_r3_rs1.Get (1));
  rs1nd.Add (link_r4_rs1.Get (1));
  rs1nd.Add (link_r5_rs1.Get (1));
  
  // Left Switch 1 NetDevices
  NetDeviceContainer ls1nd;
  ls1nd.Add (link_lr_ls1.Get (1));
  ls1nd.Add (link_l2_ls1.Get (1));
  ls1nd.Add (link_l3_ls1.Get (1));
  ls1nd.Add (link_l4_ls1.Get (1));
  ls1nd.Add (link_l5_ls1.Get (1));
  
  // ======================================================================
  // Install bridging code on each switch
  // ----------------------------------------------------------------------
  BridgeHelper bridge;

  bridge.Install (ts1, ts1nd);
  bridge.Install (bs1, bs1nd);
  bridge.Install (rs1, rs1nd);
  bridge.Install (ls1, ls1nd);

  // ======================================================================
  // Install the L3 internet stack  (TCP/IP)
  // ----------------------------------------------------------------------
  InternetStackHelper ns3IpStack;

  // ----------------------------------------------------------------------
  // Install the L3 internet stack on UDP endpoints
  // ----------------------------------------------------------------------
  NS_LOG_INFO ("L3: Install the ns3 IP stack on udp client and server nodes.");
  // PERLU DITAMBAHIN ENDPOINT LAGI
  NodeContainer endpointNodes (t2,t3,t4,t5);
  ns3IpStack.Install(endpointNodes);
  
  NodeContainer endpointNodes2 (b2,b3,b4,b5);
  ns3IpStack.Install(endpointNodes2);

  NodeContainer endpointNodes3 (r2,r3,r4,r5);
  ns3IpStack.Install(endpointNodes3);

  NodeContainer endpointNodes4 (l2,l3,l4,l5);
  ns3IpStack.Install(endpointNodes4);

  // ----------------------------------------------------------------------
  // Install the L3 internet stack on routers.
  // ----------------------------------------------------------------------
  NS_LOG_INFO ("L3: Install the ns3 IP stack on routers.");
  NodeContainer routerNodes(tr,br);
  NodeContainer routerNodes2(lr,rr);
  ns3IpStack.Install (routerNodes);
  ns3IpStack.Install (routerNodes2);

  // ======================================================================
  // Assign top LAN IP addresses
  // ----------------------------------------------------------------------
  // PERLU ASSIGN LAN IP ADDRESS LAGI TOP TOP TOP
  NS_LOG_INFO ("L3: Assign top LAN IP Addresses.");

  NetDeviceContainer topLanIpDevices;        // - - - - - -- - - - - - -
  topLanIpDevices.Add (link_tr_ts1.Get (0));  // NOTE: order matters here
                                             //       for IP address
  topLanIpDevices.Add (link_t2_ts1.Get (0));  //       assignment
                                              // - - - - - -- - - - - - -
  topLanIpDevices.Add (link_t3_ts1.Get (0));
  topLanIpDevices.Add (link_t4_ts1.Get (0));
  topLanIpDevices.Add (link_t5_ts1.Get (0));
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("192.168.1.0", "255.255.255.0");
  ipv4.Assign  (topLanIpDevices);

  // ----------------------------------------------------------------------
  // Assign bottom LAN IP addresses
  // ----------------------------------------------------------------------
  // PERLU ASSIGN LAN IP ADDRESS LAGI BOTTOM BOTTOM BOTTOM
  NS_LOG_INFO ("L3: Assign bottom LAN IP Addresses.");

  NetDeviceContainer botLanIpDevices;        // - - - - - -- - - - - - -
  botLanIpDevices.Add (link_br_bs1.Get (0));  // NOTE: order matters here
                                              //       for IP address
  botLanIpDevices.Add (link_b2_bs1.Get (0));  //       assignment
                                              // - - - - - -- - - - - - -
  botLanIpDevices.Add (link_b3_bs1.Get (0));
  botLanIpDevices.Add (link_b4_bs1.Get (0));
  botLanIpDevices.Add (link_b5_bs1.Get (0));
  ipv4.SetBase ("192.168.2.0", "255.255.255.0");
  ipv4.Assign  (botLanIpDevices);

  // ----------------------------------------------------------------------
  // Assign bottom LAN IP addresses
  // ----------------------------------------------------------------------
  // PERLU ASSIGN LAN IP ADDRESS LAGI RIGHT RIGHT RIGHT
  NS_LOG_INFO ("L3: Assign bottom LAN IP Addresses.");

  NetDeviceContainer rightLanIpDevices;        // - - - - - -- - - - - - -
  rightLanIpDevices.Add (link_rr_rs1.Get (0));  // NOTE: order matters here
                                              //       for IP address
  rightLanIpDevices.Add (link_r2_rs1.Get (0));  //       assignment
                                              // - - - - - -- - - - - - -
  rightLanIpDevices.Add (link_r3_rs1.Get (0));
  rightLanIpDevices.Add (link_r4_rs1.Get (0));
  rightLanIpDevices.Add (link_r5_rs1.Get (0));
  ipv4.SetBase ("192.168.3.0", "255.255.255.0");
  ipv4.Assign  (rightLanIpDevices);


  // ----------------------------------------------------------------------
  // Assign bottom LAN IP addresses
  // ----------------------------------------------------------------------
  // PERLU ASSIGN LAN IP ADDRESS LAGI LEFT LEFT LEFT

  NS_LOG_INFO ("L3: Assign bottom LAN IP Addresses.");

  NetDeviceContainer leftLanIpDevices;        // - - - - - -- - - - - - -
  leftLanIpDevices.Add (link_lr_ls1.Get (0));  // NOTE: order matters here
                                              //       for IP address
  leftLanIpDevices.Add (link_l2_ls1.Get (0));  //       assignment
                                              // - - - - - -- - - - - - -
  leftLanIpDevices.Add (link_l3_ls1.Get (0));
  leftLanIpDevices.Add (link_l4_ls1.Get (0));
  leftLanIpDevices.Add (link_l5_ls1.Get (0));
  ipv4.SetBase ("192.168.4.0", "255.255.255.0");
  ipv4.Assign  (leftLanIpDevices);


  // ----------------------------------------------------------------------
  // Assign WAN IP addresses
  // ----------------------------------------------------------------------
  // PERLU DIPERLEBAR LAGI ASSIGN TR BR NYA
  //
  NS_LOG_INFO ("L3: Assign WAN IP Addresses.");
  NetDeviceContainer r2rDevice;
  r2rDevice.Add (link_tr_br.Get(0));
// r2rDevice.Add (link_tr_lr.Get(1));
// r2rDevice.Add (link_br_rr.Get(0));
//r2rDevice.Add (link_rr_lr.Get(0));

  ipv4.SetBase ("76.1.1.0", "255.255.255.0");
  ipv4.Assign  (r2rDevice);


  // ======================================================================
  // Calculate and populate routing tables
  // ----------------------------------------------------------------------
  NS_LOG_INFO ("L3: Populate routing tables.");
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  NS_LOG_INFO ("DEBUGGGING SHITS ");
  ApplicationContainer apps;

  // ======================================================================
  // Single-Switch UDP traffic generation
  // ----------------------------------------------------------------------
  if (enableUdpSingleSW)
    {
      // ------------------------------------------------------------------
      // Install single-switch UDP echo server on t3
      // ------------------------------------------------------------------
      NS_LOG_INFO ("APP: Single-Switch UDP server (on node t3 of top LAN)");

      UdpEchoServerHelper server (udpEchoPort);
      
      //ini untung masang server
      ApplicationContainer serverApp = server.Install (t3);
      serverApp.Start (Seconds (0.1));
      serverApp.Stop  (Seconds (simDurationSeconds));

      // ------------------------------------------------------------------
      // Install single-switch UDP echo client on b3
      // ------------------------------------------------------------------
      NS_LOG_INFO ("APP: Single-Switch UDP client (on node b3 bottom LAN)");

      Time     interPacketInterval = Seconds (0.005);
      uint32_t packetSize          = 1000;
      uint32_t maxPacketCount      = (simDurationSeconds - 2.0) / 0.005;

      UdpEchoClientHelper client (Ipv4Address ("192.168.2.3"), udpEchoPort);

      client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
      client.SetAttribute ("Interval",   TimeValue     (interPacketInterval));
      client.SetAttribute ("PacketSize", UintegerValue (packetSize));

      ApplicationContainer clientApp = client.Install (b3);
      clientApp.Start (Seconds (0.5));
      clientApp.Stop  (Seconds (simDurationSeconds));
    }


  // ======================================================================
  // Print routing tables at T=0.1
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // NOTE: Node 0 and Node 13 must have non-empty tables (except for local 
  //       loopback and local LAN) if routing is operating correctly.
  // ----------------------------------------------------------------------
  NS_LOG_INFO ("Set up to print routing tables at T=0.1s");

  Ptr<OutputStreamWrapper> routingStream =
    Create<OutputStreamWrapper> ("ProjectAkhirV7.routes", std::ios::out);

  Ipv4GlobalRoutingHelper g;
  g.PrintRoutingTableAllAt (Seconds (0.1), routingStream);

  // ======================================================================
  // Now, do the actual simulation.
  // ----------------------------------------------------------------------
  NS_LOG_INFO ("Run Simulation for " << simDurationSeconds << " seconds.");
  AnimationInterface anim("ProjectAkhirV7.xml");
  anim.EnablePacketMetadata (true);
  anim.EnableIpv4RouteTracking ("ProjectAkhir_TableV7.xml", Seconds(0), Seconds(5), Seconds(25));
  anim.SetConstantPosition(tr, 25.0,25.0);
  anim.SetConstantPosition(br, 25.0,75.0);
  anim.SetConstantPosition(rr, 75.0,25.0);
  anim.SetConstantPosition(lr, 75.0,75.0);
  anim.SetConstantPosition(t2, 10.0,10.0);
  anim.SetConstantPosition(t3, 10.0,20.0);
  anim.SetConstantPosition(t4, 10.0,30.0);
  anim.SetConstantPosition(t5, 10.0,40.0);
  anim.SetConstantPosition(ts1,15.0,25.0);
  anim.SetConstantPosition(b2, 10.0,60.0);
  anim.SetConstantPosition(b3, 10.0,70.0);
  anim.SetConstantPosition(b4, 10.0,80.0);
  anim.SetConstantPosition(b5, 10.0,90.0);
  anim.SetConstantPosition(bs1,15.0,75.0);
  anim.SetConstantPosition(r2,90.0,10.0);
  anim.SetConstantPosition(r3,90.0,20.0);
  anim.SetConstantPosition(r4,90.0,30.0);
  anim.SetConstantPosition(r5,90.0,40.0);
  anim.SetConstantPosition(l2,90.0,60.0);
  anim.SetConstantPosition(l3,90.0,70.0);
  anim.SetConstantPosition(l4,90.0,80.0);
  anim.SetConstantPosition(l5,90.0,90.0);
  anim.SetConstantPosition(rs1,85.0,25.0);
  anim.SetConstantPosition(ls1,85.0,75.0);
  Simulator::Stop (Seconds (simDurationSeconds));
  Simulator::Run ();

  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
