//
// Created by vadim on 5/22/22.
//

#include "ShmDataSender.h"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>

using namespace boost::interprocess;


ShmDataSender::ShmDataSender(const char *name, size_t size) {
    _name = name;
    _size = size;
    Init();
}

ShmDataSender::~ShmDataSender(){
    UnInit();
}

void ShmDataSender::Init() {

    shared_memory_object share_obj(create_only, _name.c_str(), read_write);

    named_mutex named_mtx{open_or_create, std::string("mtx_" + _name).c_str()};
    named_condition named_cnd{open_or_create, std::string("mtx_" + _name).c_str()};
    scoped_lock<named_mutex> lock{named_mtx};

    //set the size of the shared memory
    share_obj.truncate(_size);

    //map the shared memory to current process
    mapped_region mmap(share_obj, read_write);

}

void ShmDataSender::UnInit() {
    shared_memory_object::remove(_name.c_str());
    named_mutex::remove(std::string("mtx_" + _name).c_str());
    named_condition::remove(std::string("cnd_" + _name).c_str());
}