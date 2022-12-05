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

configure_file(day19test.txt ${CMAKE_CURRENT_BINARY_DIR}/day19test.txt)
configure_file(day19.txt ${CMAKE_CURRENT_BINARY_DIR}/day19.txt)
configure_file(day20test.txt ${CMAKE_CURRENT_BINARY_DIR}/day20test.txt)
configure_file(day20.txt ${CMAKE_CURRENT_BINARY_DIR}/day20.txt)
configure_file(day22.txt ${CMAKE_CURRENT_BINARY_DIR}/day22.txt)
configure_file(day22test.txt ${CMAKE_CURRENT_BINARY_DIR}/day22test.txt)

if(MSVC)
    target_compile_options(advent21 PRIVATE /W4 /WX)
else()
    target_compile_options(advent21 PRIVATE -Wall -Wextra -Werror)
endif()

