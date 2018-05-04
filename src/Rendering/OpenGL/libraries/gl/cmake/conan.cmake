find_file(CONAN_COMMAND NAMES conan conan.exe PATHS $ENV{PATH} $ENV{CONAN_DIR})
if(CONAN_COMMAND)
  option(USE_CONAN "Use conan for dependency management." ON)
endif()

find_file(CONAN_BUILD_INFO conanbuildinfo.cmake NO_DEFAULT_PATH PATHS ${CMAKE_BINARY_DIR})
if(USE_CONAN AND NOT CONAN_BUILD_INFO)
  message(FATAL_ERROR "Error using conan: You need to execute 'conan install' before running cmake! Aborting.")
endif()

if(USE_CONAN AND CONAN_BUILD_INFO)
  include                     (${CONAN_BUILD_INFO})
  conan_check_compiler        ()
  conan_set_find_library_paths()
  conan_define_targets        ()
  conan_set_rpath             ()
  conan_set_vs_runtime        ()
  conan_set_libcxx            ()
  conan_set_find_paths        ()
endif()

macro(find_package_or_conan PKG)
  set(OPTIONS)
  set(SINGLE_ARGS CONAN_NAME)
  set(MULTI_ARGS)
  cmake_parse_arguments(ARG "${OPTIONS}" "${SINGLE_ARGS}" "${MULTI_ARGS}" ${ARGN})
  string(REPLACE ";" " " ADDITIONAL_ARGS "${ARG_UNPARSED_ARGUMENTS}")

  if   (ARG_CONAN_NAME)
    set(CONAN_PACKAGE_NAME CONAN_PKG::${ARG_CONAN_NAME})
  else ()
    set(CONAN_PACKAGE_NAME CONAN_PKG::${PKG})
  endif()

  if   (USE_CONAN)
    set(${PKG} ${CONAN_PACKAGE_NAME})
  else ()
    find_package(${PKG} ${ADDITIONAL_ARGS})
  endif()
endmacro()
