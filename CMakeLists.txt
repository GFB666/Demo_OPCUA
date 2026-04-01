cmake_minimum_required(VERSION 3.13)
project(OPCUA_Offline_Demo CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 引入第三方 open62541 源码库 ---
add_subdirectory(third_lib/open62541)

# 如果你希望使用本地的示例程序（可选）：
# open62541 子项目自带示例，可根据路径改成 .cpp
# add_executable(ua_server third_lib/open62541/examples/server.cpp)
# add_executable(ua_client third_lib/open62541/examples/client.cpp)
# target_link_libraries(ua_server PRIVATE open62541::open62541)
# target_link_libraries(ua_client PRIVATE open62541::open62541)

# 自己的客户端代码
add_executable(my_client src/my_client.cpp)
target_link_libraries(my_client PRIVATE open62541::open62541)

# Windows 平台需要额外链接
if(WIN32)
    target_link_libraries(ua_server PRIVATE ws2_32 iphlpapi)
    target_link_libraries(ua_client PRIVATE ws2_32 iphlpapi)
    target_link_libraries(my_client PRIVATE ws2_32 iphlpapi)
endif()