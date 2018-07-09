#include "stdlib.h"
#include "PcapFileDevice.h"
#include "PlatformSpecificUtils.h"
#include "PcapPlusPlusVersion.h"
#include "SystemUtils.h"
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "Packet.h"
#include "TcpLayer.h"
#include "PayloadLayer.h"
#include <arpa/inet.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <sstream>
#include <map>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <unordered_map>

#define DST_IP "40.1.1.2"
#define MAX_CMD_LENGTH 200

std::map<uint32_t, uint32_t> pktCounts;
uint32_t firstSeqNumber;


int recordSeqNo(uint32_t sequenceNumber){
    uint32_t relativeSeqNumber = sequenceNumber;// - firstSeqNumber;

    auto it = pktCounts.find(relativeSeqNumber);
    if(it == pktCounts.end()){ // not found in the map
        pktCounts[relativeSeqNumber] = 1;
    }
    else{
        it->second += 1;
    }
}

void printReTxPkts(){
    printf("#############################\n");
    for(auto it=pktCounts.begin(); it != pktCounts.end(); it++){
        if(it->second > 1 && it->first !=1){
            printf("%u: %u\n",it->first, it->second);
        }
    }
}


int main(int argc, char* argv[]){

    pcpp::AppName::init(argc, argv);

    std::string filename = "";
    char cmd[MAX_CMD_LENGTH];
    FILE *cmd_output;
    unsigned int TOTAL_PKTS;

    if (argc != 3){
        printf("Usage: %s <pcap_file> <target_dst_IP>\n", argv[0]);
        exit(1);
    }
    filename = std::string(argv[1]);
    //char *targetDstIpStr = argv[2];
    pcpp::IPv4Address targetDstIP(argv[2]);
    // printf("Filename is %s\n", filename.c_str());
    // printf("Target dstIP is %s\n", targetDstIP.toString().c_str());


    // Get number of packets in the pcap file
    /*
    sprintf(cmd,"capinfos -Mc %s | grep \"Number\" | tr -d \" \" | cut -d \":\" -f 2",filename.c_str());
    cmd_output = popen(cmd, "r");
    fscanf(cmd_output,"%d",&TOTAL_PKTS);
    printf("Total packets = %d\n", TOTAL_PKTS);  
    */

    // use the IFileReaderDevice interface to automatically identify file type (pcap/pcap-ng)
    // and create an interface instance that both readers implement
    pcpp::IFileReaderDevice* reader = pcpp::IFileReaderDevice::getReader(filename.c_str());

    // verify that a reader interface was indeed created
    if (reader == NULL)
    {
        fprintf(stderr, "Cannot determine reader for file type\n");
        exit(1);
    }

    // open the reader for reading
    if (!reader->open())
    {
        fprintf(stderr, "Cannot open %s for reading\n",filename.c_str());
        exit(1);
    }


    
    pcpp::RawPacket rawPacket;
    pcpp::Packet parsedPacket;
    pcpp::IPv4Layer* ipv4layer;
    pcpp::TcpLayer* tcplayer;
    pcpp::tcphdr *tcphdr;
    uint32_t currSeqNumber, relativeSeqNumber;
    uint32_t packetCounter = 1;
    uint32_t relativePacketCounter = 1;
    
    // get the first seqNumber
    reader->getNextPacket(rawPacket);
    parsedPacket = pcpp::Packet(&rawPacket, pcpp::TCP); // TCP -> parse until this layer only
    tcplayer = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
    tcphdr = tcplayer->getTcpHeader();
    firstSeqNumber = ntohl(tcphdr->sequenceNumber);
    currSeqNumber = firstSeqNumber;
    relativeSeqNumber = currSeqNumber - firstSeqNumber;

    // print the header and the first packet
    printf("RelativePacketCount SequenceNumber RelativeSeqNumber PacketCount\n");
    printf("%u %u %u %u\n",relativePacketCounter++, currSeqNumber, relativeSeqNumber, packetCounter++);

    while (reader->getNextPacket(rawPacket))
    {
        // parse the raw packet
        parsedPacket = pcpp::Packet(&rawPacket, pcpp::TCP); // TCP -> parse until this layer only
        ipv4layer = parsedPacket.getLayerOfType<pcpp::IPv4Layer>();
        tcplayer = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
        tcphdr = tcplayer->getTcpHeader();

        currSeqNumber = ntohl(tcphdr->sequenceNumber);
        relativeSeqNumber = currSeqNumber - firstSeqNumber;

        pcpp::IPv4Address currDstIP = ipv4layer->getDstIpAddress();


        if(currDstIP == targetDstIP){
            printf("%u %u %u %u\n",relativePacketCounter, currSeqNumber, relativeSeqNumber, packetCounter);

            relativePacketCounter++;
        }

        packetCounter++;
    } // end of the while loop


    
    reader->close();

    return 0;
} // end of main()
