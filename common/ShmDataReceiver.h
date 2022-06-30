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

#include "ShmDataExchDefs.h"

class ShmDataReceiver {
public:
    explicit ShmDataReceiver(const char *name);
    virtual ~ShmDataReceiver();

    bool Start();
    void Stop();
    void ReadDataInto(DataEnvelope* outData);
    void NotifyDataRead();

private:
    shared_memory_object *share_obj{};
    named_mutex *named_mtx{};
    named_condition *named_cnd{};

    std::string _name;
    bool _isOpened;
    bool CheckIsOpened();
};


#endif //SHMEM_POC_SHMDATARECEIVER_H
