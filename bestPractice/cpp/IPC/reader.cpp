#include <windows.h>

#include <iostream>

struct SharedData {
    int counter;
    char message[100];
};

int main() {
    // 打开已存在的文件映射对象
    HANDLE hMapFile =
        OpenFileMapping(FILE_MAP_ALL_ACCESS,           // 读写访问
                        FALSE,                         // 不继承句柄
                        TEXT("Local\\MySharedMemory")  // 共享内存名称
        );

    if (hMapFile == NULL) {
        std::cerr << "OpenFileMapping failed: " << GetLastError() << std::endl;
        return 1;
    }

    // 映射共享内存到进程地址空间
    SharedData* shared_data =
        (SharedData*)MapViewOfFile(hMapFile,             // 映射对象的句柄
                                   FILE_MAP_ALL_ACCESS,  // 读写权限
                                   0, 0, sizeof(SharedData));

    if (shared_data == NULL) {
        std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    // 读取数据
    for (int i = 0; i < 5; ++i) {
        std::cout << "Reader: Received - " << shared_data->message
                  << " (Counter: " << shared_data->counter << ")" << std::endl;
        Sleep(2000);  // 等待写入端更新
    }

    // 清理
    UnmapViewOfFile(shared_data);
    CloseHandle(hMapFile);

    return 0;
}