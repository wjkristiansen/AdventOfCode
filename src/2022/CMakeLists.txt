add_executable(
    advent
        main.cpp
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
