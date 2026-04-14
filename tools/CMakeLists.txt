cmake_minimum_required(VERSION 3.15.7)

project(farado_tools)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опции сборки проекта
# ---------------------------------------------------------------------------- #
option(BUILD_TOOL_MIGRATOR "Build database migration tool" ON)
option(BUILD_ALL_TOOLS "Build all tools" OFF)

# TODO : реализовать запуск и сборку генераторов
option(RUN_TOOL_DTO_GEN "Run DTO code generator" OFF)
option(RUN_TOOL_DTO_TEST_GEN "Run DTO code generator" OFF)
# ---------------------------------------------------------------------------- #

enable_testing()

if(BUILD_TOOL_MIGRATOR OR BUILD_ALL_TOOLS)
    add_subdirectory(migrator)
endif()
