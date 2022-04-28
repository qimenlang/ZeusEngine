In memory of being dismissed during the probation period of xiaohongshu

##### 编译环境配置

1. IDE  使用vscode + cmake + cpp,配置方式参考[这里](https://computingonplains.wordpress.com/building-c-applications-with-cmake-and-visual-studio-code/)
1. 代码风格使用 [Google c++编程规范](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents/)

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
3. 大自然的渲染

   - 地形

   - 大气

   - 云
4. 人物的真实感渲染
