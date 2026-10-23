add_executable(loadbalancer
    src/main.cpp

    src/LoadBalancer.cpp
)

target_include_directories(loadbalancer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(loadbalancer
    PRIVATE
        common
)