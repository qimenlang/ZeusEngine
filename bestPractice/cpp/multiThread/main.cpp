#include "ManagingThreads.h"
#include "sharingDataBetweenThreads.h"
#include <iostream>

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
  sharingDataBetweenThreads::dataRaceTest();
  sharingDataBetweenThreads::mutexDataRaceTest();
  return 0;
}