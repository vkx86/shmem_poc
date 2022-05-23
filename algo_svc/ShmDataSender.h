//
// Created by vadim on 5/22/22.
//

#ifndef SHMEM_POC_SHMDATASENDER_H
#define SHMEM_POC_SHMDATASENDER_H

#include <cstddef>
#include <string>
#include <queue>
#include <cstring>

struct DataEnvelope {
    uint32_t FrameId;
    uint32_t DataSize;
    uint8_t *Data;

    DataEnvelope(uint8_t *data, uint32_t size) {
        Data = static_cast<uint8_t *>(malloc(size));
        memcpy(Data, data, size);
        DataSize = size;
    }

    ~DataEnvelope() {
        free(Data);
    }

};


class ShmDataSender {
public:
    ShmDataSender(const char *name, size_t size);
    virtual ~ShmDataSender();

    void Start();
    void Stop();
    void SendData(uint8_t *data, size_t size);

private:
    bool _running;
    std::queue<DataEnvelope*> _queue;

    std::string _name;
    size_t _size;

};

#endif //SHMEM_POC_SHMDATASENDER_H
