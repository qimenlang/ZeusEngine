#include <iostream>

#include "ManagingThreads.h"
#include "sharingDataBetweenThreads.h"
#include "synchronization.h"

int main() {
    std::cout << "Multi Thread Tests!" << std::endl;
    // Run the multi-thread test
    // ManagingThreads::threadCreateAndJoin();
    // ManagingThreads::workWithLocalPtr();
    // ManagingThreads::workWithLocalRef();
    // ManagingThreads::workWithLocalRef2();
    // ManagingThreads::threadGuardTest();
    // ManagingThreads::passUniquePtrToThread();
    // sharingDataBetweenThreads::threadsafeStackTest();
    // sharingDataBetweenThreads::threadsafeStackSwapNoDeadLockTest();
    // sharingDataBetweenThreads::dataRaceTest();
    // sharingDataBetweenThreads::mutexDataRaceTest();

    // synchronization::testConditionVariable();
    // synchronization::testConditionVariable2();
    // synchronization::testasync();
    // synchronization::testPromise();
    synchronization::testSharedFuture();
    return 0;
}