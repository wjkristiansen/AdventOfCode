add_executable(advent
    main.cpp
    day15.cpp
    day16.cpp
    day17.cpp)

if(MSVC)
    target_compile_options(advent PRIVATE /W4 /WX)
else()
    target_compile_options(advent PRIVATE -Wall -Wextra -Werror)
endif()

