//
// Created by vadim on 5/24/22.
//

#ifndef SHMEM_POC_DATAENVELOPE_H
#define SHMEM_POC_DATAENVELOPE_H

struct DataEnvelope {
    uint32_t FrameId;
    uint32_t DataSize;
    uint8_t Data[];
};

#endif //SHMEM_POC_DATAENVELOPE_H
