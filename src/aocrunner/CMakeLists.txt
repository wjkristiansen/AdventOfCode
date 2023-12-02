add_executable(
    aocrunner
        main.cpp
    )

target_link_libraries(
    aocrunner
        InCommandLib
)

if(MSVC)
    target_compile_options(aocrunner PRIVATE /W4 /WX)
else()
    target_compile_options(aocrunner PRIVATE -Wall -Wextra -Werror)
endif()

install(
    TARGETS aocrunner
    DESTINATION ${CMAKE_SOURCE_DIR}/install/$<CONFIG>
    )

if(WIN32)
    set_target_properties(aocrunner 
        PROPERTIES VS_DEBUGGER_COMMAND "${CMAKE_SOURCE_DIR}/install/$<CONFIG>/aocrunner.exe"
    )

    set_target_properties(aocrunner
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/install/$<CONFIG>"
    )

    install(FILES $<TARGET_PDB_FILE:aocrunner> DESTINATION ${CMAKE_SOURCE_DIR}/install/$<CONFIG> OPTIONAL)
endif()

target_precompile_headers(aocrunner
    PRIVATE
        "pch.h")

