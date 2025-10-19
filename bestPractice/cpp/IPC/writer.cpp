#include <iostream>
#include <windows.h>
#include <string>

struct SharedData {
    int counter;
    char message[100];
};

int main() {
    // 创建文件映射对象
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // 使用系统分页文件
        NULL,                    // 默认安全属性
        PAGE_READWRITE,          // 可读可写访问
        0,                       // 对象大小的高位DWORD
        sizeof(SharedData),      // 对象大小的低位DWORD
        TEXT("Local\\MySharedMemory") // 共享内存名称
    );

    if (hMapFile == NULL) {
        std::cerr << "CreateFileMapping failed: " << GetLastError() << std::endl;
        return 1;
    }

    // 映射共享内存到进程地址空间
    SharedData* shared_data = (SharedData*)MapViewOfFile(
        hMapFile,               // 映射对象的句柄
        FILE_MAP_ALL_ACCESS,    // 读写权限
        0,
        0,
        sizeof(SharedData)
    );

    if (shared_data == NULL) {
        std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    // 写入数据
    shared_data->counter = 0;
    for (int i = 0; i < 5; ++i) {
        shared_data->counter++;
        snprintf(shared_data->message, sizeof(shared_data->message), 
                 "Hello from writer! Count: %d", shared_data->counter);
        
        std::cout << "Writer: Sent - " << shared_data->message << std::endl;
        Sleep(2000); // 等待读取端读取
    }

    // 清理
    UnmapViewOfFile(shared_data);
    CloseHandle(hMapFile);

    return 0;
}