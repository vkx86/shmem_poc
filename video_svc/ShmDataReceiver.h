//
// Created by vadim on 5/22/22.
//

#ifndef SHMEM_POC_SHMDATARECEIVER_H
#define SHMEM_POC_SHMDATARECEIVER_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>

using namespace boost::interprocess;

struct DataEnvelope {
    uint32_t FrameId;
    uint32_t DataSize;
    uint8_t *Data;
};

class ShmDataReceiver {

    ShmDataReceiver(const char *name);
    virtual ~ShmDataReceiver();

    void Start();
    void Stop();
    DataEnvelope* ReadData();

private:
    shared_memory_object *share_obj{};
    named_mutex *named_mtx{};
    named_condition *named_cnd{};

    std::string _name;
};


#endif //SHMEM_POC_SHMDATARECEIVER_H
