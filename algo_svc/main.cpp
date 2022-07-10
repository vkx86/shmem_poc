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
    dataSender.Start(SHARED_MEM_SIZE);

    uint32_t frameId = 0;
    uint8_t sendBuff[1024];
    auto *dataPtr = (DataEnvelope *)sendBuff;

    while (keepRunning)  {

        cout << "Input ('Q' to exit):" << endl;
        char buff[512] = {0 };

        fgets(buff, sizeof(buff), stdin);

        memset(sendBuff, 0, sizeof(sendBuff));

        dataPtr->FrameId = frameId;
        strcpy(reinterpret_cast<char *>(dataPtr->FrameRelated), "Frame related!");

        dataPtr->DataSize = strlen(buff);
        strcpy(reinterpret_cast<char *>(dataPtr->Data), buff);

        dataSender.SendData(sendBuff, sizeof (sendBuff));

        if(buff[0] == 'Q'){
            cout << "Exited!!!" << endl;
            break;
        }

    }

    cout << "Stopping sender..." << endl;
    dataSender.Stop();
    return 0;
}