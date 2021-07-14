cmake_minimum_required( VERSION 3.10 )

set(CMAKE_CXX_STANDARD 20)

project( cellworld_rl
        VERSION 2019.1.0
        DESCRIPTION "Cellworld utilities"
        LANGUAGES CXX)

find_package(Cellworld_tools REQUIRED)

###
### MAIN LIBRARY SETUP
###

set(cellworld_rl_files
        src/belief_state.cpp
        src/generative_model.cpp
        src/history.cpp
        src/predictor.cpp
        src/planning_agent.cpp
        )

add_library(cellworld_rl ${cellworld_rl_files})

set_target_properties(cellworld_rl
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED On
        CXX_EXTENSIONS Off
        VERSION ${PROJECT_VERSION})

if(MSVC)
    target_compile_options(cellworld_rl PRIVATE /W4)
else(MSVC)
    target_compile_options(cellworld_rl PRIVATE -Wall -Wextra -pedantic)
endif(MSVC)

target_include_directories(cellworld_rl
        SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_include_directories(cellworld_rl
        PRIVATE
        include )

target_link_libraries( cellworld_rl
        PUBLIC
        ${CELLWORLD_TOOLS_LIBRARIES})


###
### TESTS
###
find_package(CatchTests CONFIG QUIET)

###
### LIBRARY INSTALLATION
###

if (CMAKE_BUILD_TYPE MATCHES Release)

    include(GNUInstallDirs)

    install(TARGETS cellworld_rl EXPORT Cellworld_rlConfig
            ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(EXPORT Cellworld_rlConfig
            DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/Cellworld_rl
            EXPORT_LINK_INTERFACE_LIBRARIES)

    export(TARGETS cellworld_rl FILE Cellworld_rlConfig.cmake)
    install(CODE "FILE(APPEND ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld_rl/Cellworld_rlConfig.cmake \"find_package(Cellworld REQUIRED)\n\") ")
    install(CODE "FILE(APPEND ${CMAKE_INSTALL_FULL_DATADIR}/cmake/Cellworld_rl/Cellworld_rlConfig.cmake \"set(CELLWORLD_RL_LIBRARIES cellworld_rl ${CELLWORLD_LIBRARIES})\n\") ")

endif()
