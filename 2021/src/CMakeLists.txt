add_executable(advent
    main.cpp
    day15.cpp
    day16.cpp
    day17.cpp
    day18.cpp
    day19.cpp)

file(COPY day19test.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY day19.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(MSVC)
    target_compile_options(advent PRIVATE /W4 /WX)
else()
    target_compile_options(advent PRIVATE -Wall -Wextra -Werror)
endif()

