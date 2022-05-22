#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>

using namespace boost::interprocess;

#define SHARED_MEM_SIZE 16384
#define SHARED_MEM_NAME "my_share_memory"

#include "ShmDataSender.h"

struct DataEnvelope {
    uint32_t FrameId;
    uint32_t DataSize;
    uint8_t Data[0];
};

int main() {

    ShmDataSender sendr1("hey_ya1", 1024);

    shared_memory_object::remove(SHARED_MEM_NAME);
    named_mutex::remove("mtx");
    named_condition::remove("cnd");

    //create shared memory object
    shared_memory_object share_obj(create_only, SHARED_MEM_NAME, read_write);

    named_mutex named_mtx{open_or_create, "mtx"};
    named_condition named_cnd{open_or_create, "cnd"};
    scoped_lock<named_mutex> lock{named_mtx};

    //set the size of the shared memory
    share_obj.truncate(SHARED_MEM_SIZE);

    //map the shared memory to current process
    mapped_region mmap(share_obj, read_write);
    auto *dataPtr = (DataEnvelope*)mmap.get_address();

    uint32_t frameId = 0;

    while (true)  {
        cout << "Input ('Q' to exit):" << endl;
        char buff[512] = {0 };

        fgets(buff, sizeof(buff), stdin);

        auto data_len = std::strlen(buff);
        dataPtr->FrameId = frameId++;
        dataPtr->DataSize = data_len;

        memset(dataPtr->Data, 0, SHARED_MEM_SIZE - (sizeof(DataEnvelope::FrameId) + sizeof(DataEnvelope::DataSize)));
        std::strncpy(reinterpret_cast<char *>(dataPtr->Data), buff, data_len);

        named_cnd.notify_all();
        named_cnd.wait(lock);

        if(buff[0] == 'Q')
            break;

    }

    named_cnd.notify_all();

    shared_memory_object::remove(SHARED_MEM_NAME);
    named_mutex::remove("mtx");
    named_condition::remove("cnd");

    return 0;

}
