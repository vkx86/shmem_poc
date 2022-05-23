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
    _running = false;
    _name = name;
    _size = size;
}

ShmDataSender::~ShmDataSender(){
    Stop();
}

void ShmDataSender::Start() {

    _running = true;

    shared_memory_object share_obj(create_only, _name.c_str(), read_write);

    named_mutex named_mtx{open_or_create, std::string("mtx_" + _name).c_str()};
    named_condition named_cnd{open_or_create, std::string("mtx_" + _name).c_str()};

    scoped_lock<named_mutex> lock{named_mtx};

    //set the size of the shared memory
    share_obj.truncate(_size);

    //map the shared memory to current process
    mapped_region mmap(share_obj, read_write);

    auto *dataPtr = (DataEnvelope*)mmap.get_address();

    while (_running)  {

        //run on some queue
        while( !_queue.empty()){

            auto  *curr = _queue.front();

            dataPtr->FrameId = curr->FrameId;
            dataPtr->DataSize = curr->DataSize;

            memcpy(dataPtr->Data, curr->Data, curr->DataSize);

            _queue.pop();

            named_cnd.notify_all();
            named_cnd.wait(lock);
        }

        usleep(500);

    }

}

void ShmDataSender::Stop() {

    _running = false;

    shared_memory_object::remove(_name.c_str());
    named_mutex::remove(std::string("mtx_" + _name).c_str());
    named_condition::remove(std::string("cnd_" + _name).c_str());
}

void ShmDataSender::SendData(uint8_t *data, size_t size) {
    auto *new_data = new DataEnvelope(data, size);
    _queue.push(new_data);
}
