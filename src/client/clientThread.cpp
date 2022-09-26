#include "clientThread.h"
#include "client.h"
ClientThread::ClientThread(int msgCount) { _msgCount = msgCount; }
void ClientThread::Update() {
    if (_pClient == nullptr) {
        _pClient = new Client(_msgCount, _thread.get_id());
        if (!_pClient->Connect("127.0.0.1", 4399)) {
            // std::cout << "客户端线程连接失败" << std::endl;
        }else{
            std::cout << "连接服务器成功" << std::endl;
        }
    } else {
        // std::cout << "帧" << std::endl;
        _pClient->Update();
        _pClient->DataHandler();
        if (_pClient->IsCompleted()) {
            Thread::Stop();
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void ClientThread::Dispose() {

    if (_pClient != nullptr) {
        _pClient->Dispose();
        delete _pClient;
        _pClient = nullptr;
    }

    Thread::Dispose();
}
