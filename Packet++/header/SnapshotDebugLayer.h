#ifndef PACKETPP_SNAPSHOT_DEBUG_LAYER
#define PACKETPP_SNAPSHOT_DEBUG_LAYER

#include "Layer.h"
#include <vector>
#include <map>
#if defined(WIN32) || defined(WINx64)
#include <winsock2.h>
#elif LINUX
#include <in.h>
#endif

/// @file

/**
 * \namespace pcpp
 * \brief The main namespace for the PcapPlusPlus lib
 */
namespace pcpp
{

    /**
     * @struct snapshot
     * Represents the fixed snapshot header
     */
#pragma pack(push, 1)
    struct snapshotdebughdr // 38 bytes
    {
        /** Higher 16 bits of globalIngressTimestamp */
        uint16_t globalIngressTimestamp_hi_16;
        /** Lower 32 bits of globalIngressTimestamp */
        uint32_t globalIngressTimestamp_lo_32;
        /** Lower 32 bits of globalEgressTimestamp */
        uint32_t globalEgressTimestamp_lo_32;
        /** Enqueue Queue Depth */      
        uint32_t enqQdepth;
        /** Dequeue Queue Depth */
        uint32_t deqQdepth;


        // DEBUGGING RELATED FIELDS
        uint64_t origEgressGlobalTimestamp;
        uint64_t newEgressGlobalTimestamp;
        uint32_t newEnqTimestamp;

    };
#pragma pack(pop)


    /**
     * @class SnapshotDebugLayer
     * Represents the Snapshot debug protocol layer.<BR>
     */
    class SnapshotDebugLayer : public Layer
    {
        
    public:

        /**
         * A constructor that creates the layer from an existing packet raw data
         * @param[in] data A pointer to the raw data
         * @param[in] dataLen Size of the data in bytes
         * @param[in] prevLayer A pointer to the previous layer
         * @param[in] packet A pointer to the Packet instance where layer will be stored in
         */
        SnapshotDebugLayer(uint8_t* data, size_t dataLen, Layer* prevLayer, Packet* packet) : Layer(data, dataLen, prevLayer, packet) { m_Protocol = SNAPSHOTDEBUG; }

        /**
         * Getters for all the different fields.
         */
        uint64_t getGlobalIngressTimestamp();
        uint64_t getGlobalEgressTimestamp();
        uint32_t getEnqQdepth();
        uint32_t getDeqQdepth();
        uint64_t getOrigEgressGlobalTimestamp();
        uint64_t getNewEgressGlobalTimestamp();
        uint32_t getNewEnqTimestamp();   
        /**
         * Get a pointer to the Snapshot header. Notice this points directly to the data, so every change will change the actual packet data
         * @return A pointer to the @ref snapshotdebughdr
         */
        inline snapshotdebughdr* getSnapshotHeader() { return (snapshotdebughdr*)m_Data; }


        // implement abstract methods

        /**
         * Currently identifies the following next layer: PayloadLayer
         */
        void parseNextLayer();

        /**
         * @return Size of @ref snapshotdebughdr
         */
        inline size_t getHeaderLen() { return sizeof(snapshotdebughdr); }

        /**
         * Does nothing for this layer
         */
        void computeCalculateFields() {}

        std::string toString();

        OsiModelLayer getOsiModelLayer() { return OsiModelApplicationLayer; }

    };

} // namespace pcpp

#endif /* PACKETPP_SNAPSHOT_LAYER */
