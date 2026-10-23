add_executable(autoscaler-loadbalancer
    src/main.cpp

    src/LoadBalancer.cpp
)

target_include_directories(autoscaler-loadbalancer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(autoscaler-loadbalancer
    PRIVATE
        autoscaler-common
)