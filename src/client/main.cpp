#include "clientThread.h"

#include <atomic>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {

    int threadNum = 1, msgCount = 1;
    if (argc >= 2) {
        threadNum = std::atoi(argv[1]);
        // std::cout << "机器人个数为：" << threadNum << std::endl;
    }
    if (argc == 3) {
        msgCount = std::atoi(argv[2]);
        // std::cout << "每个机器人发送的消息数量为：" << msgCount << std::endl;
    }

    std::vector<ClientThread *> threads(threadNum);
    for (auto i = 0; i < threadNum; ++i) {
        threads[i] = new ClientThread(msgCount);
        threads[i]->Start();
    }

    //获取开始时间
    auto start = std::chrono::system_clock::now();

    // std::cout << "个数为" << threads.size() << std::endl;

    //主线程判断线程数量
    while (true) {
        if (threads.size() <= 0) {
            // std::cout << "线程数量为空 结束!!!!!!!!!!!!!!!!---------"
            //     << std::endl;
            break;
        }

        //便利移除停止执行的线程
        for (auto bIter = threads.begin(); bIter < threads.end();
                ++bIter) {

            ClientThread *pThread = (*bIter);
            //如果线程是停止状态
            if (!pThread->IsRun()) {
                // std::cout << "结束-------------------------123" << std::endl;

                pThread->Dispose();
                if (pThread != nullptr)
                    delete pThread;
                threads.erase(bIter);
                --bIter;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    auto end = std::chrono::system_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    //输出消耗的时间
    std::cout << "time:"
        << double(duration.count()) *
        std::chrono::microseconds::period::num /
        std::chrono::microseconds::period::den
        << "s" << std::endl;

    threads.clear();

    return 0;
}
