#include <iostream>
using namespace std;

#include "ShmDataSender.h"
#include "ShmDataExchDefs.h"


int main() {

    ShmDataSender dataSender(SHARED_MEM_NAME);
    dataSender.Stop();
    dataSender.Start(SHARED_MEM_SIZE);

    uint32_t frameId = 0;

    while (true)  {

        cout << "Input ('Q' to exit):" << endl;
        char buff[512] = {0 };

        fgets(buff, sizeof(buff), stdin);

        dataSender.SendData(frameId++, strlen(buff), reinterpret_cast<uint8_t *>(buff));

        if(buff[0] == 'Q'){
            cout << "Exited!!!" << endl;
            break;
        }

    }

    dataSender.Stop();
    return 0;
}