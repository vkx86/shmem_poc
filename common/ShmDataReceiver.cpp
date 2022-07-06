//
// Created by vadim on 5/22/22.
//

#include <chrono>
#include <iostream>
#include <thread>

#include "ShmDataReceiver.h"

ShmDataReceiver::ShmDataReceiver(const char *name) {
    _name = name;
}

ShmDataReceiver::~ShmDataReceiver() {
    Stop();
}

bool ShmDataReceiver::Start() {
    _isOpened = CheckIsOpened();

    if(!_isOpened){
        return _isOpened;
    }

    share_obj = new shared_memory_object(
            //open_or_create,
            open_only,
            _name.c_str(),read_only);

    named_mtx = new named_mutex(
            //open_or_create,
            open_only,
            std::string("mtx_" + _name).c_str());
    named_cnd = new named_condition(
            //open_or_create,
            open_only,
            std::string("cnd_" + _name).c_str());

    _isOpened = true;
    return _isOpened;
}

void ShmDataReceiver::Stop() {

//    shared_memory_object::remove(_name.c_str());
//    named_mutex::remove(std::string("mtx_" + _name).c_str());
//    named_condition::remove(std::string("cnd_" + _name).c_str());

    delete named_mtx;
    delete named_cnd;
    delete share_obj;

    named_mtx = nullptr;
    named_cnd = nullptr;
    share_obj = nullptr;

}

void ShmDataReceiver::ReadDataInto(DataEnvelope *outData) {

    if(!_isOpened)
        return;

    scoped_lock<named_mutex> lock{*named_mtx};
    named_cnd->wait(lock);

    mapped_region mmap(*share_obj, read_only);
    auto *dataPtr = (DataEnvelope*)mmap.get_address();

    outData->FrameId = dataPtr->FrameId;
    outData->DataSize = dataPtr->DataSize;
    memcpy(outData->Data, dataPtr->Data, dataPtr->DataSize);

//    auto time = std::chrono::system_clock::now().time_since_epoch();
//    std::chrono::seconds seconds = std::chrono::duration_cast< std::chrono::seconds >(time);
//    std::chrono::microseconds ms = std::chrono::duration_cast< std::chrono::microseconds >(time);
//    auto result = (double) seconds.count() + ((double) (ms.count() % 1000000)/1000000.0);
//    std::cout << "Data received at: " << std::to_string(result) << std::endl;


//    std::cout << "Sleeping..." << std::endl;
//    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
//    std::cout << "Finished Sleeping !!!" << std::endl;

    named_cnd->notify_all();
}

void ShmDataReceiver::NotifyDataRead() {
    named_cnd->notify_all();
}

bool ShmDataReceiver::CheckIsOpened() {

    auto tmpHandle = shm_open(_name.c_str(), O_CREAT | O_EXCL, O_RDWR);
    if (tmpHandle == -1) {
        if (EEXIST == errno)
            return true;

        return false;
    }

    close(tmpHandle);
    shm_unlink(_name.c_str());
    return false;
}
