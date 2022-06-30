#include <iostream>
#include <csignal>

using namespace std;

#include "ShmDataSender.h"
#include "ShmDataExchDefs.h"

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main() {

    signal(SIGINT, intHandler);
    //signal(SIGTSTP, intHandler);

    ShmDataSender dataSender(SHARED_MEM_NAME);
    dataSender.Stop();
    dataSender.Start(SHARED_MEM_SIZE);

    uint32_t frameId = 0;

    while (keepRunning)  {

        cout << "Input ('Q' to exit):" << endl;
        char buff[512] = {0 };

        fgets(buff, sizeof(buff), stdin);

        dataSender.SendData(frameId++, strlen(buff), reinterpret_cast<uint8_t *>(buff));

        if(buff[0] == 'Q'){
            cout << "Exited!!!" << endl;
            break;
        }

    }

    cout << "Stopping sender..." << endl;
    dataSender.Stop();
    return 0;
}