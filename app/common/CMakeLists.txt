add_library(common STATIC
    src/logger/Logger.cpp

    src/model/Message.cpp

    src/utils/utils.cpp
    src/utils/RandomGenerator.cpp
    src/utils/ConfigParser.cpp
)

target_include_directories(common
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
