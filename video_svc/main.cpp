#include <iostream>
#include <thread>
#include <csignal>

using namespace std;

#include "ShmDataReceiver.h"
#include "ShmDataExchDefs.h"

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main() {

    signal(SIGINT, intHandler);
    //signal(SIGTSTP, intHandler);

    ShmDataReceiver dataReceiver(SHARED_MEM_NAME);
    while (keepRunning && !dataReceiver.Start()){
        cout << "Connecting..." << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    cout << "Started listening..." << endl;

    uint8_t buffer[4096];
    auto *received = reinterpret_cast<DataEnvelope *>(buffer);

    while (keepRunning)  {
        memset(buffer, 0, sizeof(buffer));

        dataReceiver.ReadDataInto(buffer, sizeof(buffer));

        cout << received->FrameId << " : " << received->DataSize << endl;
        cout << received->Data << endl;

        if(received->Data[0] == 'Q'){
            cout << "Exited!!!" << endl;
            break;
        }
    }

    cout << "Stopping receiver..." << endl;
    dataReceiver.Stop();
    return 0;
}
