# CMakeLists.txt (��������)
cmake_minimum_required(VERSION 3.10)
project(FileRAII LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ������ ����������
add_library(FileRAII STATIC
    src/FileRAII.cpp
)
target_include_directories(FileRAII PUBLIC include)

# ������ ����������������� ����������
add_executable(demo_app
    demo/main.cpp
)
target_link_libraries(demo_app PRIVATE FileRAII)
