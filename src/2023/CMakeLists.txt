file(GLOB advent_src CONFIGURE_DEPENDS "*.cpp")
file(GLOB advent_txt CONFIGURE_DEPENDS "*.txt")

add_library(
    aoc2023 SHARED
    ${advent_src}
    )

install(
    TARGETS aoc2023
    DESTINATION ${CMAKE_SOURCE_DIR}/install/$<CONFIG>
    )

install(
    FILES
        ${advent_txt}
    DESTINATION ${CMAKE_SOURCE_DIR}/install/$<CONFIG>/2023
    )

target_precompile_headers(aoc2023
    PRIVATE
        "pch.h")

if(MSVC)
    set_target_properties(aoc2023 PROPERTIES LINK_FLAGS "${LINK_FLAGS} /NOIMPLIB")
else()
    # Prevent CMake from prepending "lib"
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()
