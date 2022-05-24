//
// Created by vadim on 5/22/22.
//

#include "ShmDataSender.h"
#include "DataEnvelope.h"

ShmDataSender::ShmDataSender(const char *name) {
    _name = name;
}

ShmDataSender::~ShmDataSender(){
    Stop();
}

void ShmDataSender::Start(long size) {
    //Stop();

    share_obj = new shared_memory_object(create_only, _name.c_str(), read_write);
    share_obj->truncate(size);

    named_mtx = new named_mutex(open_or_create, std::string("mtx_" + _name).c_str());
    named_cnd = new named_condition(open_or_create, std::string("cnd_" + _name).c_str());
}

void ShmDataSender::Stop() {

    shared_memory_object::remove(_name.c_str());
    named_mutex::remove(std::string("mtx_" + _name).c_str());
    named_condition::remove(std::string("cnd_" + _name).c_str());

    delete named_mtx;
    delete named_cnd;
    delete share_obj;

    named_mtx = nullptr;
    named_cnd = nullptr;
    share_obj = nullptr;
}

void ShmDataSender::SendData(uint32_t frameId, uint32_t dataSize, uint8_t *data) {

    mapped_region mmap(*share_obj, read_write);
    auto *dataPtr = (DataEnvelope*)mmap.get_address();

    scoped_lock<named_mutex> lock{*named_mtx};

    dataPtr->FrameId = frameId;
    dataPtr->DataSize = dataSize;
    memcpy(dataPtr->Data, data, dataSize);

    named_cnd->notify_all();
    named_cnd->wait(lock);
}
