@echo off
setlocal enabledelayedexpansion

:: 设置要查找的文件后缀
set extensions=.frag .vert 

echo 编译着色器文件...

for %%e in (%extensions%) do (
    echo === %%e 文件 ===
    set count=0
    for %%f in (*%%e) do (
        echo %%f
        D:\VulkanSDK\1.4.321.1\Bin\glslc.exe %%f -o %%f.spv
        echo %%f.spv
        set /a count+=1
    )
)

echo 编译完成！


pause