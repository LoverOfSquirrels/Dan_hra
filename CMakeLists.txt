cmake_minimum_required(VERSION 3.20)
project(VilaRubin VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(vila_core
    src/model.cpp
    src/text.cpp
    src/world.cpp
    src/locations.cpp
    src/parser.cpp
    src/session.cpp
    src/finder.cpp
    src/view.cpp
    src/navigation.cpp
    src/items.cpp
    src/puzzles.cpp
    src/dialogue.cpp
    src/report.cpp
    src/hints.cpp
    src/dispatcher.cpp
    src/console.cpp
)
target_include_directories(vila_core PUBLIC include)

add_executable(vila_rubin src/main.cpp)
target_link_libraries(vila_rubin PRIVATE vila_core)

add_executable(vila_tests
    tests/main.cpp tests/parser_tests.cpp tests/world_tests.cpp
    tests/story_tests.cpp tests/puzzle_tests.cpp tests/save_tests.cpp tests/console_tests.cpp
)
target_link_libraries(vila_tests PRIVATE vila_core)

foreach(target vila_core vila_rubin vila_tests)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX /utf-8 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endforeach()

if(MINGW)
    target_link_options(vila_rubin PRIVATE -static)
    target_link_options(vila_tests PRIVATE -static)
endif()

enable_testing()
add_test(NAME herni_scenare COMMAND vila_tests)
install(TARGETS vila_rubin RUNTIME DESTINATION .)
