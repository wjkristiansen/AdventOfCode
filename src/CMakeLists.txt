add_executable(
    advent
        main.cpp
        2022/Day1.cpp
    )

target_link_libraries(
    advent
        InCommandLib
)

if(MSVC)
    target_compile_options(advent PRIVATE /W4 /WX)
else()
    target_compile_options(advent PRIVATE -Wall -Wextra -Werror)
endif()

install(
    TARGETS advent
    DESTINATION ${CMAKE_SOURCE_DIR}/install/$<CONFIG>
    )

install(
    FILES
        2022/Day1.txt
    DESTINATION ${CMAKE_SOURCE_DIR}/install/$<CONFIG>
    )

if(WIN32)
    set_target_properties(advent 
        PROPERTIES VS_DEBUGGER_COMMAND "${CMAKE_SOURCE_DIR}/install/$<CONFIG>/advent.exe"
    )

    set_target_properties(advent
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/install/$<CONFIG>"
    )
endif()