# 调试可执行程序
gdb ./your_program

# 调试正在运行的程序（PID 为进程 ID）
gdb -p PID

# 调试 core 文件（程序崩溃后的内存转储）
gdb ./your_program core

设置断点
# 在函数名处设置断点
(gdb) break main         # 在 main 函数入口设置断点
(gdb) break file.c:20    # 在 file.c 文件的第 20 行设置断点
# 条件断点（当条件满足时触发）
(gdb) break file.c:30 if variable == 5
# 查看所有断点
(gdb) info breakpoints
# 删除断点
(gdb) delete 1          # 删除编号为 1 的断点

运行程序
(gdb) run               # 开始运行程序
(gdb) run arg1 arg2     # 带参数运行程序

控制执行流程
(gdb) next              # 执行下一行代码（不进入函数）
(gdb) step              # 执行下一行代码（进入函数内部）
(gdb) continue          # 继续运行直到下一个断点或程序结束
(gdb) finish            # 执行完当前函数并返回
(gdb) return            # 强制从当前函数返回

查看变量和内存
(gdb) print variable    # 打印变量的当前值
(gdb) display variable  # 每次程序暂停时自动显示变量值
(gdb) x/10x &array     # 以十六进制查看数组的前 10 个元素
(gdb) info locals       # 显示当前函数的局部变量
(gdb) info registers    # 查看寄存器值

查看堆栈和源码
(gdb) backtrace         # 查看函数调用栈（缩写 bt）
(gdb) frame 2           # 切换到调用栈的第 2 层（缩写 f 2）
(gdb) list              # 显示当前行附近的源代码
(gdb) list main         # 显示 main 函数的源代码