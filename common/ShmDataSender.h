//
// Created by vadim on 5/22/22.
//

#ifndef SHMEM_POC_SHMDATASENDER_H
#define SHMEM_POC_SHMDATASENDER_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>

using namespace boost::interprocess;

class ShmDataSender {
public:
    explicit ShmDataSender(const char *name);
    virtual ~ShmDataSender();

    void Start(long size);
    void Stop();
    void SendData(uint32_t frameId, uint32_t dataSize, uint8_t *data);

private:
    shared_memory_object *share_obj{};
    named_mutex *named_mtx{};
    named_condition *named_cnd{};

    std::string _name;
};

#endif //SHMEM_POC_SHMDATASENDER_H
