#include <iostream>
#include <memory>
#include <string>
#include <thread>

void testSharedPtrThreadSafe() {
  struct Data {
    int num;
    std::string name;
  };
  auto shared_data = std::make_shared<Data>();

  auto worker = [shared_data] {
    shared_data->name = "name1";
    std::cout << "name:" << shared_data->name
              << ", use_count:" << shared_data.use_count() << std::endl;
  };

  std::cout << __LINE__ << std::endl;
  std::thread t1(worker);
  std::cout << __LINE__ << std::endl;
  std::thread t2(worker);
  std::cout << __LINE__ << std::endl;
  t1.join();
  std::cout << __LINE__ << std::endl;
  t2.join();
  std::cout << __LINE__ << std::endl;
}