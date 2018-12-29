function(describe_tool)

    cmake_parse_arguments(
            ARGS
            ""
            "NAME"
            "DEPENDENCIES;INCLUDE;DEFINITIONS"
            ${ARGN}
    )

    # Marking project
    project(HG${ARGS_NAME})

    # Setting C++ standard to C++17
    set(CMAKE_CXX_STANDARD 17)

    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/libraries)
        add_subdirectory(libraries)
    endif()

    # Globbing sources and headers
    file(GLOB_RECURSE ${ARGS_NAME}_SOURCES src/*.cpp)
    file(GLOB_RECURSE ${ARGS_NAME}_HEADERS include/*.hpp)

    # Creating executable
    add_executable(${PROJECT_NAME} ${${ARGS_NAME}_SOURCES})

    # Include headers directory
    target_include_directories(${PROJECT_NAME} PUBLIC include ${ARGS_INCLUDE})

    # Linking libraries
    target_link_libraries(${PROJECT_NAME} ${ARGS_DEPENDENCIES})

    # Definitions
    target_compile_definitions(${PROJECT_NAME} PUBLIC ${ARGS_DEFINITIONS})

    # Enabling test coverate if required
    if (${HG_TEST_COVERAGE})
        target_compile_options(${PROJECT_NAME} BEFORE
            PUBLIC
                -fprofile-arcs
                -ftest-coverage
        )

        list(APPEND ARGS_DEPENDENCIES -lgcov)
    endif()

    if (${HG_BUILD_WARNINGS})
        target_compile_options(${PROJECT_NAME}
            PRIVATE
                -ansi
                -pedantic
                -Wall
                -Wextra
                -Woverloaded-virtual
                -Winit-self
                -Wunreachable-code
        )
    endif()

endfunction(describe_tool)


function(describe_module)

    find_package(Sanitizers)

    cmake_parse_arguments(
            ARGS
            ""
            "NAME"
            "DEPENDENCIES;INCLUDE;DEFINITIONS"
            ${ARGN}
    )

    # Marking project
    project(HG${ARGS_NAME})

    # Setting C++ standard to C++17
    set(CMAKE_CXX_STANDARD 17)

    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/libraries)
        add_subdirectory(libraries)
    endif()

    # Creating option for building tests for this module if tests even exists
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/tests)
        message(STATUS "Tests for ${ARGS_NAME} found.")

        set(TEST_KEY "HG_BUILD_${ARGS_NAME}_TESTS")
        option(${TEST_KEY} "Build ${ARGS_NAME} tests" Off)

        # If required - include tests
        if (${${TEST_KEY}} OR ${HG_BUILD_ALL_TESTS})

            # Finding all tests
            file(GLOB TEST_FILES tests/*.cpp)

            set(HG_TEST_CASES ${HG_TEST_CASES} "${TEST_FILES}" CACHE INTERNAL "Test cases files")

            # Setting current module as dependency
            set(HG_TEST_LIBS ${HG_TEST_LIBS} "${PROJECT_NAME}" CACHE INTERNAL "Test libraries")

            # Finding static testing assets
            file(GLOB_RECURSE TEST_ASSETS tests/static/*)

            # Setting it to global variable
            set(${PROJECT_NAME}_TEST_ASSETS
                ${CMAKE_CURRENT_LIST_DIR}/tests/static/
                ${TEST_ASSETS}

                CACHE INTERNAL "" FORCE
            )

            # Adding this variable to var list
            set(TEST_ASSETS_VARIABLES
                ${TEST_ASSETS_VARIABLES}
                ${PROJECT_NAME}_TEST_ASSETS

                CACHE INTERNAL "" FORCE
            )

        endif()
    endif()

    # Globbing sources and headers
    file(GLOB_RECURSE ${ARGS_NAME}_SOURCES src/*.cpp)
    file(GLOB_RECURSE ${ARGS_NAME}_HEADERS include/*.hpp)

    # Creating static library
    add_library(${PROJECT_NAME} STATIC ${${ARGS_NAME}_SOURCES})

    # Setting that it's cxx code
    set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

    # Include headers directory
    target_include_directories(${PROJECT_NAME} PUBLIC include ${ARGS_INCLUDE})

    # Enabling test coverate if required
    if (${HG_TEST_COVERAGE})
        message(STATUS "Enabling test coverage for ${ARGS_NAME}")
        target_compile_options(${PROJECT_NAME} BEFORE
            PUBLIC
                -fprofile-arcs
                -ftest-coverage
        )

        list(APPEND ARGS_DEPENDENCIES -lgcov)
    endif()

    # Linking libraries
    target_link_libraries(${PROJECT_NAME} ${ARGS_DEPENDENCIES})

    # Adding definitions
    target_compile_definitions(${PROJECT_NAME} PUBLIC ${ARGS_DEFINITIONS})

    add_sanitizers(${PROJECT_NAME})

    if (${HG_BUILD_WARNINGS})
        target_compile_options(${PROJECT_NAME}
            PRIVATE
            -ansi
            -pedantic
            -Wall
            -Wextra
            -Woverloaded-virtual
            -Winit-self
            -Wunreachable-code
        )
    endif()

    # Adding this module to global variable with all projects
    if (NOT PROJECT_NAME IN_LIST MODULES_LIST)
        set(MODULES_LIST ${MODULES_LIST} "${PROJECT_NAME}" CACHE INTERNAL "List of engine modules")
    endif()

endfunction(describe_module)

function(add_example)
    cmake_parse_arguments(
        ARGS
        ""
        "NAME"
        "DEPENDENCIES"
        ${ARGN}
    )

    project(HGExample${ARGS_NAME})

    set(CMAKE_CXX_STANDARD 17)

    file(GLOB_RECURSE SOURCES *.cpp)

    add_executable(${PROJECT_NAME}
        ${SOURCES}
    )

    target_include_directories(${PROJECT_NAME} PRIVATE .)

    target_link_libraries(${PROJECT_NAME} ${ARGS_DEPENDENCIES})

    file(GLOB_RECURSE ASSETS *.png *.mtl *.obj *.tga)

    foreach (ASSET_FULL_PATH ${ASSETS})
        string(REPLACE ${CMAKE_CURRENT_LIST_DIR}/ "" ASSET_RELATIVE ${ASSET_FULL_PATH})

        message(STATUS "Copying example asset file \"${ASSET_RELATIVE}\"")

        configure_file(${ASSET_RELATIVE} ${ASSET_RELATIVE} COPYONLY)
    endforeach()

endfunction()

function(clear_cached_variables)
    set(HG_TEST_CASES  "" CACHE STRING "" FORCE)
    set(HG_TEST_LIBS   "" CACHE STRING "" FORCE)
    set(HG_TEST_ASSETS "" CACHE STRING "" FORCE)

    foreach (VARIABLE ${TEST_ASSETS_VARIABLES})
        set(${VARIABLE} "" CACHE LIST "" FORCE)
    endforeach()

    set(TEST_ASSETS_VARIABLES "" CACHE LIST "" FORCE)
endfunction(clear_cached_variables)

function(pack_package)

    cmake_parse_arguments(
            ARGS # prefix of output variables
            "" # list of names of the boolean arguments (only defined ones will be true)
            "PATH;NAME;AUTHOR;VERSION_MAJOR;VERSION_MINOR" # list of names of mono-valued arguments
            "" # list of names of multi-valued arguments (output variables are lists)
            ${ARGN} # arguments of the function to parse, here we take the all original ones
    )

    # note: if it remains unparsed arguments, here, they can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS

    add_custom_command(
        OUTPUT ${ARGS_OUTPUT}
        COMMAND $<TARGET_FILE:HGPackageProcessor>
            --command pack
            --path ${ARGS_PATH}
            --output ${CMAKE_CURRENT_BINARY_DIR}/${ARGS_NAME}.hgpackage
            --name ${ARGS_NAME}
            --author ${ARGS_AUTHOR}
            --major ${ARGS_VERSION_MAJOR}
            --minor ${ARGS_VERSION_MINOR}

        COMMENT "Building ${ARGS_NAME} package"
        DEPENDS HGPackageProcessor
    )

    add_custom_target(Create${ARGS_NAME}Package
        DEPENDS ${ARGS_OUTPUT}
    )

endfunction()

function(unpack_package)

    cmake_parse_arguments(
        ARGS # prefix of output variables
        "" # list of names of the boolean arguments
        "NAME;OUTPUT" # list of names of mono-valued
        "" # list of names of multi-valued arguments
        ${ARGN}
    )

    add_custom_command(
        OUTPUT ${ARGS_OUTPUT}
        COMMAND $<TARGET_FILE:HGPackageProcessor>
            --command unpack
            --path ${CMAKE_CURRENT_BINARY_DIR}/${ARGS_NAME}.hgpackage
            --output ${ARGS_OUTPUT}

        COMMENT "Unpacking ${ARGS_PATH} package"
        DEPENDS HGPackageProcessor Create${ARGS_NAME}Package
    )

    add_custom_target(Unpack${ARGS_NAME}Package
        DEPENDS ${ARGS_OUTPUT}
    )

endfunction()