#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

#define BUF_SIZE 1025
#define SHARED_MEM_NAME "my_share_memory"

int main() {

    shared_memory_object::remove(SHARED_MEM_NAME);

    //create shared memory object
    shared_memory_object share_obj(create_only, SHARED_MEM_NAME, read_write);

    //set the size of the shared memory
    share_obj.truncate(BUF_SIZE-1);

    //map the shared memory to current process
    mapped_region mmap(share_obj, read_write);

    //access the mapped region
    //strcpy(static_cast<char*>(mmap.get_address()), "Hello world!");

    while (true)
    {
        cout << "Input ('Q' to exit):" << endl;
        char szInfo[BUF_SIZE] = { 0 };

        fgets(szInfo, BUF_SIZE, stdin);

        if(szInfo[0] == 'Q')
            break;

        //strncpy(mmap.get_address(), szInfo, BUF_SIZE - 1);
        std::strncpy((char*)mmap.get_address(), szInfo, BUF_SIZE - 1);
        //share_obj[BUF_SIZE - 1] = '\0';
    }

	shared_memory_object::remove(SHARED_MEM_NAME);

    return 0;

}
