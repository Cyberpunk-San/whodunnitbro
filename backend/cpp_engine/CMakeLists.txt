cmake_minimum_required(VERSION 3.15)
project(WhoDunnitEngine VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Find Python
find_package(Python REQUIRED COMPONENTS Interpreter Development)

# Find pybind11
find_package(pybind11 REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/models
    ${CMAKE_SOURCE_DIR}/src/data_structures
)

# Source files
set(ENGINE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/core/engine.cpp
    ${CMAKE_SOURCE_DIR}/src/core/story_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/core/utils.cpp
    ${CMAKE_SOURCE_DIR}/src/models/case.cpp
    ${CMAKE_SOURCE_DIR}/src/models/character.cpp
    ${CMAKE_SOURCE_DIR}/src/models/suspect.cpp
    ${CMAKE_SOURCE_DIR}/src/data_structures/avl_tree.cpp
    ${CMAKE_SOURCE_DIR}/src/data_structures/rb_tree.cpp
    ${CMAKE_SOURCE_DIR}/src/data_structures/linked_list.cpp
    ${CMAKE_SOURCE_DIR}/src/data_structures/graph.cpp
)

# Create the Python module
pybind11_add_module(whodunnit_engine 
    ${CMAKE_SOURCE_DIR}/py_wrapper.cpp
    ${ENGINE_SOURCES}
)

# Platform-specific extension
if (WIN32)
    set(MODULE_SUFFIX ".pyd")
else()
    set(MODULE_SUFFIX ".so")
endif()

set_target_properties(whodunnit_engine PROPERTIES
    PREFIX ""
    SUFFIX ${MODULE_SUFFIX}
)

# Include directories for the module
target_include_directories(whodunnit_engine PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/models
    ${CMAKE_SOURCE_DIR}/src/data_structures
    ${Python_INCLUDE_DIRS}
)

# Link Python libraries
target_link_libraries(whodunnit_engine PRIVATE 
    pybind11::module
    ${Python_LIBRARIES}
)

# Post-build copy
add_custom_command(TARGET whodunnit_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
        $<TARGET_FILE:whodunnit_engine> 
        ${CMAKE_SOURCE_DIR}/whodunnit_engine${MODULE_SUFFIX}
    COMMENT "Copying whodunnit_engine${MODULE_SUFFIX} to source directory"
)

# Configuration info
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Python executable: ${Python_EXECUTABLE}")
message(STATUS "Python version: ${Python_VERSION}")
message(STATUS "Python include dirs: ${Python_INCLUDE_DIRS}")
message(STATUS "Python libraries: ${Python_LIBRARIES}")
message(STATUS "Pybind11 found: ${pybind11_FOUND}")
