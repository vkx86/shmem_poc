//
// Created by vadim on 5/22/22.
//

#ifndef SHMEM_POC_SHMDATASENDER_H
#define SHMEM_POC_SHMDATASENDER_H


#include <cstddef>
#include <string>

class ShmDataSender {
public:
    ShmDataSender(const char *name, size_t size);
    virtual ~ShmDataSender();

private:
    std::string _name;
    size_t _size;
    void Init();
    void UnInit();
};


#endif //SHMEM_POC_SHMDATASENDER_H
