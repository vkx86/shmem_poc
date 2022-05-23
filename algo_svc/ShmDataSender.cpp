//
// Created by vadim on 5/22/22.
//

#include "ShmDataSender.h"

struct DataEnvelope {
    uint32_t FrameId;
    uint32_t DataSize;
    uint8_t Data[0];
};


ShmDataSender::ShmDataSender(const char *name, size_t size) {
    _name = name;
    _size = size;
}

ShmDataSender::~ShmDataSender(){
    Stop();
}

void ShmDataSender::Start() {
    share_obj = new shared_memory_object(create_only, _name.c_str(), read_write);
    share_obj->truncate(_size);

    named_mtx = new named_mutex(open_or_create, std::string("mtx_" + _name).c_str());
    named_cnd = new named_condition(open_or_create, std::string("mtx_" + _name).c_str());
}

void ShmDataSender::Stop() {

    delete named_mtx;
    delete named_cnd;
    delete share_obj;

    shared_memory_object::remove(_name.c_str());
    named_mutex::remove(std::string("mtx_" + _name).c_str());
    named_condition::remove(std::string("cnd_" + _name).c_str());
}

void ShmDataSender::SendData(uint8_t *data, size_t size) {

    mapped_region mmap(*share_obj, read_write);
    scoped_lock<named_mutex> lock{*named_mtx};

    auto *dataPtr = (DataEnvelope*)mmap.get_address();

    dataPtr->FrameId = 111;
    dataPtr->DataSize = size;
    memcpy(dataPtr->Data, data, size);

    named_cnd->notify_all();
    named_cnd->wait(lock);

}
