cmake_minimum_required(VERSION 3.16)

project(AtlasBoot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(atlasboot
    src/atlasboot/core/BootManager.cpp
    src/atlasboot/core/Image.cpp
    src/atlasboot/core/UpdateManager.cpp
)

target_include_directories(atlasboot
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/atlasboot
)

add_library(atlasboot_port_native_sim
    src/atlasboot/ports/native_sim/NativeSimApplicationLauncher.cpp
    src/atlasboot/ports/native_sim/NativeSimPort.cpp
    src/atlasboot/ports/native_sim/VirtualFlash.cpp
)

target_link_libraries(atlasboot_port_native_sim PUBLIC atlasboot)

add_executable(desktop_update_sim
    examples/desktop_update_sim/main.cpp
)

target_link_libraries(desktop_update_sim PRIVATE atlasboot_port_native_sim)

add_executable(porting_minimal
    examples/porting_minimal/main.cpp
)

target_link_libraries(porting_minimal PRIVATE atlasboot_port_native_sim)

add_executable(atlasboot_core_flow_tests
    tests/core_flow_tests.cpp
)

target_link_libraries(atlasboot_core_flow_tests PRIVATE atlasboot_port_native_sim)

enable_testing()
add_test(NAME atlasboot_core_flow_tests COMMAND atlasboot_core_flow_tests)
