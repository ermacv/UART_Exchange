## Build
To build you need to configure and generate project using CMake:
```
cmake -H. -Bbuild
```
Then you need to build the sources. For test executable you need to execute:
```
cmake --build build/ --target uart_exchange_test -- -j8
```
To run the test you need to execute:
```
build/task/uart_exchange_test
```
If you want to build docs you need to execute:
```
doxygen Doxyfile
```
Then open this file in a browser: docs/html/index.html
