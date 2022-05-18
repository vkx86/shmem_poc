
#include <iostream>
#include <string>

using namespace std;

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>

using namespace boost::interprocess;

#define BUF_SIZE 1025
#define SHARED_MEM_NAME "my_share_memory"

struct DataEnvelope {
    uint32_t FrameId;
    uint32_t DataSize;
    uint8_t Data[0];
};


int main() {

    //open shared memory object
    shared_memory_object share_obj(open_only, SHARED_MEM_NAME, read_only);

    //map the shared memory object to current process
    mapped_region mmap(share_obj, read_only);

    named_mutex named_mtx{open_or_create, "mtx"};
    named_condition named_cnd{open_or_create, "cnd"};
    scoped_lock<named_mutex> lock{named_mtx};

    auto *dataPtr = (DataEnvelope*)mmap.get_address();

    while (true)  {
        cout << dataPtr->FrameId << " : " << dataPtr->DataSize << endl;
        cout << dataPtr->Data << endl;

        named_cnd.notify_all();
        named_cnd.wait(lock);

        if(dataPtr->Data[0] == 'Q')
            break;

    }

    named_cnd.notify_all();

    //remove shared memory object
    shared_memory_object::remove(SHARED_MEM_NAME);
    named_mutex::remove("mtx");
    named_condition::remove("cnd");


    return 0;
}
