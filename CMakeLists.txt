cmake_minimum_required(VERSION 3.18)

project(BTCS_EDIT LANGUAGES C CXX)

set(PYBIND11_FINDPYTHON ON)
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11
)
FetchContent_MakeAvailable(pybind11)

file(GLOB_RECURSE BTCS_SRC CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

pybind11_add_module(btcs ${BTCS_SRC})
set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

if(CMAKE_HOST_WIN32)
	target_compile_options(btcs PRIVATE /std:c++latest) # MSVC won't let <format> be used on c++20 despite official spec, only c++latest
	# MSVC uses subfolders so extract latest for consistency
	add_custom_command(TARGET btcs POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
				${PROJECT_SOURCE_DIR}/bin/)
endif()
