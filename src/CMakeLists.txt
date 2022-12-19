file(GLOB advent_src CONFIGURE_DEPENDS "2022/*.cpp")

add_executable(
    advent
        main.cpp
        ${advent_src}
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

file(GLOB advent_txt CONFIGURE_DEPENDS "2022/*.txt")

install(
    FILES
        ${advent_txt}
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

target_precompile_headers(advent
    PRIVATE
        "pch.h")