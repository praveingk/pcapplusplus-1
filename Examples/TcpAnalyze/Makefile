include /usr/local/etc/PcapPlusPlus.mk

.PHONY: all clean

all: getpacketdrops getseqnumbers reorderpackets
	

getpacketdrops: getPacketDrops.cpp
	g++ $(PCAPPP_INCLUDES) -std=c++11 -o getPacketDrops.o -c getPacketDrops.cpp
	g++ $(PCAPPP_LIBS_DIR) -static-libstdc++ -o getpacketdrops getPacketDrops.o $(PCAPPP_LIBS)

getseqnumbers: getSeqNumbers.cpp
	g++ $(PCAPPP_INCLUDES) -std=c++11 -o getSeqNumbers.o -c getSeqNumbers.cpp
	g++ $(PCAPPP_LIBS_DIR) -static-libstdc++ -o getseqnumbers getSeqNumbers.o $(PCAPPP_LIBS)

getextrapackets: getExtraPackets.cpp
	g++ $(PCAPPP_INCLUDES) -std=c++11 -o getExtraPackets.o -c getExtraPackets.cpp
	g++ $(PCAPPP_LIBS_DIR) -static-libstdc++ -o getextrapackets getExtraPackets.o $(PCAPPP_LIBS)

reorderpackets: reorderPackets.cpp
	g++ $(PCAPPP_INCLUDES) -std=c++11 -o reorderPackets.o -c reorderPackets.cpp
	g++ $(PCAPPP_LIBS_DIR) -static-libstdc++ -o reorderpackets reorderPackets.o $(PCAPPP_LIBS)	

clean:
	rm *.o getpacketdrops getseqnumbers getextrapackets reorderpackets


