project(server)

set(SRC_LIST src/server_main.cpp src/Server.cpp src/ThreadPool.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} -lpthread)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
