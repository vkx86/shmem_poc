
#include <iostream>
#include <string>

using namespace std;

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

#define BUF_SIZE 1025
#define SHARED_MEM_NAME "my_share_memory"

int main() {
//open shared memory object
    //shared_memory_object share_obj(open_only, "shared_memory", read_only);
    shared_memory_object share_obj(open_only, SHARED_MEM_NAME, read_only);

    //map the shared memory object to current process
    //mapped_region mmap(share_obj, read_only);
    mapped_region mmap(share_obj, read_only);

    while (true)
    {
        cout << "Press any button to receive data ('Q' to exit)..." << endl;

        auto ch = getchar();
        if(ch == 'Q')
            break;

        cout << static_cast<char*>(mmap.get_address()) << endl;
        //shared_memory_object::remove(SHARED_MEM_NAME);
    }

    //remove shared memory object

    return 0;
}
