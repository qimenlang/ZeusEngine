## ECS架构：Entity-Component-System

1. 核心原理：
   - 组合优于继承：相较于OOP,创建新对象类型只需要重新组合组件，避免了深层次继承；
   - 数据与逻辑分离：提升代码的模块化和清晰度；
   - 面向数据设计：系统统一处理所有实体的特定类型组件，对同类型数据的连续访问，对CPU Cache 友好，提升Cache Hit;
2. 实体Entity：是一个唯一的标识符ID，不包含任何数据和行为，是空壳；作用是将Component组合在一起，形成一个有意义的整体；
3. 组件Component：是纯数据的容器，只包含某个方面的数据，职责单一；
4. 系统System：纯逻辑执行者，处理特定组件组合的实体，执行游戏逻辑，职责清晰；
   - 工作原理：
     - 每个系统声明它关心哪些组件类型（MoveSystem关心Transform、Motion组件）
     - 每帧查询该系统的全部组件、读取数据、运行逻辑、将数据写回组件
5. 优势：
   - 性能
     - 缓存友好
     - 高效查询、
     - 并行友好：由于数据和逻辑分离，且系统通常处理大量同质数据，易于将各系统的执行并行化；
   - 高度灵活性和可组合性：增加新类型只需要组合组件，修改现有类型的行为只需要添加、删除组件和系统（符合开闭原则），代码复用性高：组件和系统都是独立可复用模块；
   - 代码清晰，可维护性高，对系统逻辑可以进行单元测试；
   - 解耦：组件之间不直接通信；系统之间通常通过修改共有组件和事件通信，耦合度低；
6. 实现
   - 实体管理：EntityManager管理{EntityID,Vector{Components}} 的Map；
   - 组件管理：
     - 为每种组件创建一个Component Pool/Manager
     - ComponentPool内存布局：
       - SOA （Structure of Array）:  数组的结构体，以结构体的属性为单位存储进数组； 性能高：缓存命中高，可SIMD；
       - AOS（Array of structures）：结构体数据，以结构体单位存储进数组；代码清晰；
       - AOSOA（Array of Structres of Arrays）：折中方案,SOA 的数组，每组SOA是4-8个实体；
   - 系统管理：
     - SystemManager，提供注册系统的功能	
     - 每个系统在注册时，声明其需要的组件签名
     - 主循环中调用SystemManager::Update，更新所有系统；
     - 系统执行时,向EntityManager和ComponentManager查询该系统所需的所有实体、组件，执行该系统的逻辑：读取组件数据、计算、写回组件
   - 组件签名与匹配：
     - 为每个组件类型分配一个唯一的字位
     - 每个实体维护一个位掩码**EntityMask** ，表示拥有的组件
     - 每个系统维护一个位掩码 **SystemRequiredMask**，表示系统需要的组件
     - 匹配规则： 位掩码按位与， **EntityMask** &**SystemRequiredMask**