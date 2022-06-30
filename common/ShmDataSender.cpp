//
// Created by vadim on 5/22/22.
//

#include <chrono>
#include <iostream>
#include "boost/date_time/posix_time/posix_time_types.hpp"

#include "ShmDataSender.h"
#include "ShmDataExchDefs.h"

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

//    auto time = std::chrono::system_clock::now().time_since_epoch();
//    std::chrono::seconds seconds = std::chrono::duration_cast< std::chrono::seconds >(time);
//    std::chrono::microseconds ms = std::chrono::duration_cast< std::chrono::microseconds >(time);
//    auto result = (double) seconds.count() + ((double) (ms.count() % 1000000)/1000000.0);
//    std::cout << "Data sent at: " << std::to_string(result) << std::endl;

    named_cnd->notify_all();
    named_cnd->wait(lock);

//    boost::posix_time::ptime timeOut(boost::posix_time::second_clock::local_time());
//    std::cout << timeOut.time_of_day().ticks() << std::endl;
//    timeOut += boost::posix_time::seconds(2);
//    std::cout << timeOut.time_of_day().ticks() << std::endl;
//
//    if( !named_cnd->timed_wait(lock, timeOut)){
//        std::cout << "timed out" << std::endl;
//    }

    //std::cout << "unlocked" << std::endl;
}
