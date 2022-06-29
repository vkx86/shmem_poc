//
// Created by vadim on 5/24/22.
//

#ifndef SHMEM_POC_SHMDATAEXCHDEFS_H
#define SHMEM_POC_SHMDATAEXCHDEFS_H

#define SHARED_MEM_NAME "my_share_memory"
#define SHARED_MEM_SIZE 16384

struct DataEnvelope {
    uint32_t FrameId;
    uint32_t DataSize;
    uint8_t Data[];
};

#endif //SHMEM_POC_SHMDATAEXCHDEFS_H
