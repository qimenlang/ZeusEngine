# 智能指针

## 一、智能指针优势

1. RAII自动管理内存

   - 避免内存泄漏：智能指针离开作用域时自动释放内存，确保分配的资源正常释放

     ```c++
     // 裸指针 - 需手动管理内存
     void raw_pointer_example() {
         MyClass* obj = new MyClass();
         // ... 使用 obj ...
         delete obj; // 必须手动释放，否则内存泄漏
     }
     
     // 智能指针 - 自动释放内存
     void smart_pointer_example() {
         std::unique_ptr<MyClass> obj = std::make_unique<MyClass>();
         // ... 使用 obj ...
         // 函数结束时自动调用 delete
     }
     ```

     

   - 异常安全：抛出异常时，智能指针可以确保资源被正确释放，裸指针会因未运行至delete而导致内存泄漏

     ```c++
     // 裸指针 - 异常可能导致泄漏
     void unsafe_function() {
         int* arr = new int[100];
         risky_operation(); // 可能抛出异常
         delete[] arr;      // 异常时不会执行
     }
     
     // 智能指针 - 异常安全
     void safe_function() {
         auto arr = std::make_unique<int[]>(100);
         risky_operation(); // 即使抛出异常，内存也会释放
     }
     ```

     

2. 避免悬空指针：裸指针在被释放之后，该指针所指向的资源已被释放，但是该指针未被置空且继续保存了原资源的地址，继续使用裸指针对原资源进行操作，会引发未定义行为；智能指针在被reset释放之后，会自动置空；

   ```c++
   // 裸指针 - 可能成为悬空指针
   void dangling_pointer() {
       int* raw_ptr = new int(42);
       delete raw_ptr;
       // raw_ptr 现在指向已释放内存 - 危险!
       *raw_ptr = 10; // 未定义行为
   }
   
   // 智能指针 - 自动置空
   void safe_pointer() {
       auto smart_ptr = std::make_unique<int>(42);
       smart_ptr.reset(); // 释放内存
       if (smart_ptr) {   // 自动变为 nullptr
           // 不会进入此分支
       }
   }
   ```

   

3. 明确资源管理所有权：

   - 裸指针：资源所有权不明确

     ```c++
     // 裸指针 - 所有权不明确
     void process_data(int* data) {
         // 我应该删除 data 吗？所有权不明确
     }
     
     // 智能指针 - 所有权清晰
     void process_data(std::unique_ptr<int> data) {
         // 明确获得所有权，将自动释放
     }
     ```

   - 智能指针所有权

     - 共享所有权：多个对象共享同一内存时，使用shared_ptr可以自动管理引用计数
     - 转移所有权：unique_ptr可以方便地转移资源所有权，不需要手动复制和释放
     - 弱引用：当需要观察对象，但不想影响其生命周期时，使用weak_ptr，避免循环引用

4. 容器安全：在容器中存储指针时，使用智能指针，可以避免 容器异常时发生内存泄露

5. 线程安全：Shared_ptr是线程安全的

   ```c++
   // 共享指针的引用计数是线程安全的
   void thread_safe_example() {
       auto shared_data = std::make_shared<Data>();
       
       auto worker = [shared_data] {
           // 安全访问共享数据
       };
       
       std::thread t1(worker);
       std::thread t2(worker);
       t1.join();
       t2.join();
   }
   ```

## 二、三种核心智能指针的功能与用法

1. unique_ptr<T,Deleter> : 独占所有权指针

   - 特点

     - 独占资源所有权
     - 不可复制，但可移动
     - 0额外开销，与裸指针大小相同

   - 用法

     ```c++
     // 创建独占指针
     auto ptr = std::make_unique<MyClass>(arg1, arg2);
     
     // 移动所有权
     auto ptr2 = std::move(ptr); // ptr 现在为空
     
     
     // 自定义删除器（用于特殊资源）
     auto file_deleter = [](FILE* f) { fclose(f); };
     // unique_ptr<class T,class Deleter> 支持删除器，模板参数: FILE[管理资源的类型]，decltype(file_deleter)[自动推到删除器类型]
     std::unique_ptr<FILE, decltype(file_deleter)>  file_ptr(fopen("data.txt", "r"), file_deleter);
     
     
     // 管理数组
     auto array_ptr = std::make_unique<int[]>(10);
     array_ptr[0] = 42;
     ```

     

2. shared_ptr<T>:共享所有权指针

   - 特点

     - 多个指针共享同一对象
     - 使用引用计数管理声明周期
     - 可以自定义计数器、删除器 (尝试使用内存池实践)

   - 原理

     - shared_ptr使用引用计数来管理对象的生命周期，引用计数存储在控制块中（control block）
     - 控制块包含：强引用计数、弱引用计数、指向对象的指针、分配器、删除器

   - 用法

     ```c++
     // 创建共享指针
     auto shared = std::make_shared<MyResource>();
     
     // 复制共享指针
     auto shared2 = shared; // 引用计数+1
     
     // 自定义分配器
     auto custom_alloc = [](size_t size) { 
         return my_memory_pool.allocate(size); 
     };
     auto custom_deleter = [](MyResource* p) {
         p->~MyResource();
         my_memory_pool.deallocate(p);
     };
     
     auto custom_ptr = std::shared_ptr<MyResource>(new (custom_alloc(sizeof(MyResource))) MyResource(),custom_deleter);
     
     // 共享指针数组（C++17+）
     auto arr = std::make_shared<int[]>(5);
     arr[0] = 10;
     ```

     

3. weak_ptr : 弱引用指针

   - 特点：

     - 不增加引用计数
     - 用于解决循环引用问题
     - 需转换为shared_ptr才能访问对象（通过lock()）

   - 用法：

     ```c++
     // 创建弱引用
     auto shared = std::make_shared<MyObject>();
     std::weak_ptr<MyObject> weak = shared;
     
     // 安全访问对象
     if (auto locked = weak.lock()) {
         // 成功获取 shared_ptr，对象存在
         locked->do_something();
     } else {
         // 对象已被释放
     }
     
     // 检查对象是否存在
     if (!weak.expired()) {
         // 对象仍然存在
     }
     ```

     

## 三、智能指针最佳实践

1. 创建智能指针

   ```c++
   // 避免：双重内存分配（控制块和对象两次内存分配）
   std::shared_ptr<MyClass> bad(new MyClass());
   //双重内存分配可能带来以下问题：
   // 1. 性能开销：内存分配通常是比较昂贵的操作，尤其是当频繁创建和销毁`shared_ptr`时。
   // 2. 内存碎片：多次分配可能增加内存碎片。
   // 3. 缓存不友好：控制块和对象可能位于内存的不同位置，访问它们可能导致缓存未命中。
   // 4. MyClass()构造函数抛出异常时，会造成内存泄漏
   
   // 优先使用 make_* 系列函数，能够将对象和控制块的分配合并为一次内存分配,避免上述问题
   auto uptr = std::make_unique<MyClass>();
   auto sptr = std::make_shared<MyClass>();
   ```

   

2. 所有权转移

   ```cpp
   // 在函数间转移所有权
   std::unique_ptr<Resource> create_resource() {
       return std::make_unique<Resource>();
   }
   
   void consume_resource(std::unique_ptr<Resource> res) {
       // 获取资源所有权
   }
   
   auto resource = create_resource();
   consume_resource(std::move(resource));
   ```

   

3. 与裸指针交互

   - 获取裸指针，转移所有权

     ```cpp
     // 获取裸指针（不转移所有权）
     void legacy_function(Resource* res);
     
     auto smart_ptr = std::make_unique<Resource>();
     legacy_function(smart_ptr.get());
     
     ```

   - 从裸指针创建智能指针

     ```cpp
     // 从裸指针创建智能指针（谨慎使用）
     Resource* raw = new Resource();
     std::unique_ptr<Resource> smart(raw); // 取得所有权
     ```

     

4. 处理循环引用

   ```cpp
   class Node {
   public:
       std::shared_ptr<Node> next;
       std::weak_ptr<Node> prev; // 使用 weak_ptr 避免循环引用
   };
   
   auto node1 = std::make_shared<Node>();
   auto node2 = std::make_shared<Node>();
   
   node1->next = node2;
   node2->prev = node1; // 不会增加引用计数
   ```

   

## 四、智能指针性能

| 指针类型     | 内存开销          | 性能开销         | 适用场景                 |
| ------------ | ----------------- | ---------------- | ------------------------ |
| `unique_ptr` | 0（与裸指针相同） | 0                | 独占所有权、局部资源管理 |
| `shared_ptr` | 2个指针（控制块） | 原子操作引用计数 | 共享所有权、多线程环境   |
| `weak_ptr`   | 2个指针（控制块） | 原子操作引用计数 | 观察者模式、解决循环引用 |

#### 性能优化技巧：

1. 优先使用unique_ptr
2. 避免不必要的shared_ptr拷贝
3. 使用make_shared减少内存分配次数
4. 在性能关键路径避免频繁创建、销毁

## 五、智能指针对比总结

| 特性           | `unique_ptr`     | `shared_ptr` | `weak_ptr`       |
| -------------- | ---------------- | ------------ | ---------------- |
| **所有权**     | 独占             | 共享         | 无（观察）       |
| **是否可复制** | ❌                | ✔️            | ✔️                |
| **是否可移动** | ✔️                | ✔️            | ✔️                |
| **线程安全**   | 否（需外部同步） | 引用计数安全 | 引用计数安全     |
| **内存开销**   | 0                | 2指针大小    | 2指针大小        |
| **典型应用**   | 工厂模式返回值   | 共享资源管理 | 缓存、观察者模式 |

## 六、何时使用裸指针

尽管智能指针功能强大，但在以下场景下裸指针仍有价值

1. **性能极致优化**：对内存和性能有极端要求场景

2. **与C接口交互**：当与C库或操作系统API交互时

3. **非所有权引用**：仅作为观察者且对象生命周期明确时

   ```cpp
   void process(const Data& data) {
       // 仅读取data，不涉及所有权
       const Data* ptr = &data; // 安全使用裸指针
   }
   ```



智能指针是现代C++内存管理的核心工具，合理使用可显著提高代码的安全性和可维护性。掌握`unique_ptr`、`shared_ptr`和`weak_ptr`的适用场景与最佳实践，是成为高效C++开发者的关键一步。
