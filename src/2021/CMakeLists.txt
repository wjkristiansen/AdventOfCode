add_executable(advent21
    main.cpp
    day15.cpp
    day16.cpp
    day17.cpp
    day18.cpp
    day19.cpp
    day20.cpp
    day21.cpp
    day22.cpp)

if(MSVC)
    target_compile_options(advent21 PRIVATE /W4 /WX)
else()
    target_compile_options(advent21 PRIVATE -Wall -Wextra -Werror)
endif()

install(
    TARGETS advent21
    DESTINATION ${CMAKE_SOURCE_DIR}/install
    )

install(
    FILES
        day19test.txt
        day19.txt
        day20test.txt
        day20.txt
        day22.txt
        day22test.txt
    DESTINATION ${CMAKE_SOURCE_DIR}/install/2021
    )