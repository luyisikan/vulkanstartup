cmake_minimum_required(VERSION 3.7.1)

set(PROJECT_NAME "vkStart")
project(${PROJECT_NAME})

find_package(Vulkan)

message(STATUS "Vulkan_FOUND = ${Vulkan_FOUND}")
message(STATUS "Vulkan_INCLUDE_DIRS = ${Vulkan_INCLUDE_DIRS}")
message(STATUS "Vulkan_LIBRARIES = ${Vulkan_LIBRARIES}")
message(STATUS "Vulkan_LIBRARY = ${Vulkan_LIBRARY}")
message(STATUS "Vulkan_GLSLC_EXECUTABLE = ${Vulkan_GLSLC_EXECUTABLE}")

# try to find up vulkan lib
if (NOT Vulkan_FOUND)
    # exit cmake files
    message(SEND_ERROR "Unable to find Vulkan Library")
endif()

# include vulkan header files and libs
include_directories(AFTER ${Vulkan_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
### Each file is one main entry point
# file(GLOB_RECURSE PROJECT_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
# string(STRIP "${PROJECT_SOURCE_FILES}" PROJECT_SOURCE_FILES)
# message(STATUS "PROJECT_SOURCE_FILES = ${PROJECT_SOURCE_FILES}")


if(WIN32)
    # Add any required preprocessor definitions here
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR) # added to include vulkan_win32.h

    # Define directories and the contained folder and files inside.
    source_group("include" REGULAR_EXPRESSION "include/*")
    source_group("lib" REGULAR_EXPRESSION "lib/*")
endif(WIN32)

set(MAIN_APP_NAMES
    CheckExtension
    CreateInstance
    CheckLogicalDevice
    CheckLayers
)

foreach(app ${MAIN_APP_NAMES})

    set(APP_TARGET ${PROJECT_NAME}${app})
    message(STATUS "Generate executable ${APP_TARGET} with ${app}.cpp")
    add_executable(${APP_TARGET} ${app}.cpp)
    target_link_libraries(${APP_TARGET} ${Vulkan_LIBRARIES} )

    # Define C++ version to be used for building the project
    set_property(TARGET ${APP_TARGET} PROPERTY CXX_STANDARD 11)
    set_property(TARGET ${APP_TARGET} PROPERTY CXX_STANDARD_REQUIRED ON)

    # Define C version to be used for building the project
    set_property(TARGET ${APP_TARGET} PROPERTY C_STANDARD 99)
    set_property(TARGET ${APP_TARGET} PROPERTY C_STANDARD_REQUIRED ON)

endforeach(app)