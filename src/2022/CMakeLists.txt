file(GLOB advent_src CONFIGURE_DEPENDS "*.cpp")
file(GLOB advent_txt CONFIGURE_DEPENDS "*.txt")

add_library(
    aoc2022 SHARED
    ${advent_src}
    )

install(
    TARGETS aoc2022
    DESTINATION ${CMAKE_SOURCE_DIR}/install/$<CONFIG>
    )

install(
    FILES
        ${advent_txt}
    DESTINATION ${CMAKE_SOURCE_DIR}/install/$<CONFIG>/2022
    )

target_precompile_headers(aoc2022
    PRIVATE
        "pch.h")

set_target_properties(aoc2022 PROPERTIES LINK_FLAGS "${LINK_FLAGS} /NOIMPLIB")