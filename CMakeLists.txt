cmake_minimum_required(VERSION 3.22)
project(project)

set(CMAKE_CXX_STANDARD 14)

add_executable(project main.cpp Database/Mysql.h User/User.h pages.h Department.h Hao.h LoginInfo.h)
target_link_libraries(${PROJECT_NAME} libmysqlclient.so)

