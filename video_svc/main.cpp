#include <iostream>

using namespace std;

#include "ShmDataReceiver.h"


int main() {

    uint8_t buffer[4096];
    DataEnvelope *received = reinterpret_cast<DataEnvelope *>(buffer);

    ShmDataReceiver dataReceiver(SHARED_MEM_NAME);
    dataReceiver.Start();

    while (true)  {
        memset(buffer, 0, sizeof(buffer));

        dataReceiver.ReadDataInto(received);

        cout << received->FrameId << " : " << received->DataSize << endl;
        cout << received->Data << endl;

        if(received->Data[0] == 'Q'){
            cout << "Exited!!!" << endl;
            break;
        }


    }

    dataReceiver.Stop();

    return 0;
}
