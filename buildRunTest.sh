cmake -B build 
cd build/
cmake --build .

echo "$OSTYPE"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  echo "Linux"
  ./engine/unit_test/UnitTest
elif [[ "$OSTYPE" == "darwin"* ]]; then
  echo "MacOS"
elif [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "msys" ]]; then
  echo "Windows"
  ./engine/unit_test/Debug/UnitTest
fi
