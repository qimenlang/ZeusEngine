In memory of being dismissed during the probation period of xiaohongshu

##### 编译环境配置

1. IDE  使用vscode + cmake + cpp,配置方式参考[这里](https://computingonplains.wordpress.com/building-c-applications-with-cmake-and-visual-studio-code/)
1. 代码风格使用 [Google c++编程规范](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents/)

##### TODO

1. 优化UI，引入imgui配合glfw
2. 配置C++编程规范静态检查
3. 搭建opengl渲染管线
4. 完善引擎五层架构

##### 第三方库

1. [glfw](https://github.com/glfw/glfw) ：用于绘制window
2. glad 参考https://rpxomi.github.io/ 引入

##### 实现功能

1. 引擎五层架构
   - 工具层
   - 资源层
   - 功能层
     - 渲染
     - 动画
     - 物理
     - ECS
   - 核心层
     - 数学计算
     - 内存管理
   - 平台层
     - RHI
2. 渲染管线搭建
   - 前向渲染
   - 延迟渲染
   - Cluster-Based渲染管线
3. 大自然的渲染

   - 地形

   - 大气

   - 云
4. 基于物理的渲染
   - PBR
5. 


