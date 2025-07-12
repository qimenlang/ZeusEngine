# 获取传递的文件夹参数
target_dir="$1"

echo $target_dir

find $target_dir -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp" \) -exec clang-format -i -style=file {} +

echo "format all file in $target_dir Done!"