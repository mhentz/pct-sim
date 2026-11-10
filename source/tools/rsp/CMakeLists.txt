# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2026 Matthieu Hentz

# Relative stopping powers of the simulation materials. A separate project so that it
# can be built against Geant4 10.7, which the simulation itself no longer supports.
cmake_minimum_required(VERSION 3.16...3.27)
project(pct-sim-rsp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geant4 10.7 REQUIRED)
include(${Geant4_USE_FILE})

set(_sim_source ${PROJECT_SOURCE_DIR}/../..)
add_executable(rsp rsp.cc ${_sim_source}/src/Materials.cc)
target_include_directories(rsp PRIVATE ${_sim_source}/include)
target_link_libraries(rsp PRIVATE ${Geant4_LIBRARIES})
