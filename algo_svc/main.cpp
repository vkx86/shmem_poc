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

#include <boost/thread.hpp>
#include <thread>
#include "ShmDataSender.h"

int main() {
    cout << boost::this_thread::get_id() << '\n';

    ShmDataSender dataSender(SHARED_MEM_NAME, SHARED_MEM_SIZE);
    dataSender.Stop();
    dataSender.Start();

    uint32_t frameId = 0;

    while (true)  {

        cout << "Input ('Q' to exit):" << endl;
        char buff[512] = {0 };

        fgets(buff, sizeof(buff), stdin);

        dataSender.SendData(reinterpret_cast<uint8_t *>(buff), strlen(buff));

        if(buff[0] == 'Q')
            break;

    }

    dataSender.Stop();
    return 0;
}
