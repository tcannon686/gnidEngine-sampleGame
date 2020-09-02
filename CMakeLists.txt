cmake_minimum_required(VERSION 3.18)
project(
    game
    LANGUAGES                          C CXX
    DESCRIPTION                        "An example game using the gnidEngine")

set(CMAKE_CXX_STANDARD                 17)
set(CMAKE_CXX_STANDARD_REQUIRED        True)

################################### SOURCES ####################################

file(
    GLOB                               ${PROJECT_NAME}_SOURCES
    LIST_DIRECTORIES                   false
    src/game/*.cpp)

file(
    GLOB                               ${PROJECT_NAME}_HEADERS
    LIST_DIRECTORIES                   false
    include/game/*.hpp)

################################### TARGETS ####################################

add_executable(${PROJECT_NAME}         ${${PROJECT_NAME}_SOURCES}
                                       ${glad_SOURCES}
                                       ${matrix_SOURCES})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC                             include)

target_link_libraries(${PROJECT_NAME} glfw gnid dl)

