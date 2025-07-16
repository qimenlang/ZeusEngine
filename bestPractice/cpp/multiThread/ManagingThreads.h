#pragma once
#include <iostream>
#include <thread>

namespace ManagingThreads {
// threadCreateAndJoin
struct CallableStruct {
    void operator()() const;
};

void threadCreateAndJoin();

// workWithLocalVariable
struct CallableStructWithLocalPtr {
    CallableStructWithLocalPtr(int *localPtr) : varPtr(localPtr) {}
    void operator()();

   private:
    int *varPtr;
};

void workWithLocalPtr();

struct CallableStructWithLocalRef {
    CallableStructWithLocalRef(int &localVar) : varRef(localVar) {}
    void operator()();

   private:
    int &varRef;
};

void workWithLocalRef();

// pass args to thread
void workWithLocalRef2();

void threadGuardTest();

// pass unique_ptr to thread
void passUniquePtrToThread();
}  // namespace ManagingThreads
