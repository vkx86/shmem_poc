//
// Created by vadim on 5/22/22.
//

#ifndef SHMEM_POC_SHMDATASENDER_H
#define SHMEM_POC_SHMDATASENDER_H

#include <cstddef>
#include <string>
#include <queue>
#include <cstring>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>

using namespace boost::interprocess;

class ShmDataSender {
public:
    ShmDataSender(const char *name, size_t size);
    virtual ~ShmDataSender();

    void Start();
    void Stop();
    void SendData(uint8_t *data, size_t size);

private:
    shared_memory_object *share_obj{};
    named_mutex *named_mtx{};
    named_condition *named_cnd{};

    std::string _name;
    size_t _size;
};

#endif //SHMEM_POC_SHMDATASENDER_H
