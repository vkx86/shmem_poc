//
// Created by vadim on 5/22/22.
//

#include "ShmDataReceiver.h"

ShmDataReceiver::ShmDataReceiver(const char *name) {
    _name = name;
}

ShmDataReceiver::~ShmDataReceiver() {
    Stop();
}

void ShmDataReceiver::Start() {
    share_obj = new shared_memory_object(open_only, _name.c_str(), read_only);
    named_mtx = new named_mutex(open_or_create, std::string("mtx_" + _name).c_str());
    named_cnd = new named_condition(open_or_create, std::string("cnd_" + _name).c_str());
}

void ShmDataReceiver::Stop() {

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

DataEnvelope *ShmDataReceiver::ReadData() {

    scoped_lock<named_mutex> lock{*named_mtx};

    mapped_region mmap(*share_obj, read_only);
    auto *dataPtr = (DataEnvelope*)mmap.get_address();

    auto *result = new DataEnvelope();
    result->FrameId = dataPtr->FrameId;
    result->DataSize = dataPtr->DataSize;
    result->Data = static_cast<uint8_t *>(malloc(dataPtr->DataSize));
    memcpy(result->Data, dataPtr->Data, dataPtr->DataSize);

    named_cnd->notify_all();
    named_cnd->wait(lock);

    return result;
}