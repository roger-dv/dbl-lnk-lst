cmake_minimum_required(VERSION 3.16)

project(dbl-lnk-lst)

if(DEFINED ENV{OS} AND "$ENV{OS}" STREQUAL "Windows_NT")
    set(OS_PLATFORM "win32")
else()
    set(OS_PLATFORM "linux")
endif()

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wno-unknown-pragmas -std=c11")

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -std=gnu++20 -static-libstdc++")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CLANG_LINKER_OPTN} -Wl,-rpath,'$ORIGIN/'")

set(SOURCE_FILES main.cpp)

SET(LIBRARY_OUTPUT_PATH "${dbl-lnk-lst_SOURCE_DIR}/${CMAKE_BUILD_TYPE}")

SET(EXECUTABLE_OUTPUT_PATH "${LIBRARY_OUTPUT_PATH}")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} pthread)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}"
)

enable_testing()

add_executable(
        dbl-lnk-lst_test
        dbl-lnk-lst_test.cpp
)
target_link_libraries(
        dbl-lnk-lst_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dbl-lnk-lst_test)