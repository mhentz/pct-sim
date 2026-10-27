# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2026 Matthieu Hentz

cmake_minimum_required(VERSION 3.16...3.27)
project(pct-sim LANGUAGES CXX)

# Build with the Geant4 UI and visualisation drivers unless WITH_GEANT4_UIVIS=OFF,
# which gives a batch-only executable
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)

if(WITH_GEANT4_UIVIS)
  find_package(Geant4 11.0 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 11.0 REQUIRED)
endif()

include(${Geant4_USE_FILE})

add_executable(pct-sim
  main.cc
  src/ActionInitialization.cc
  src/Analysis.cc
  src/DetectorConstruction.cc
  src/DetectorConstructionMessenger.cc
  src/DetectorHit.cc
  src/EventAction.cc
  src/EventActionMessenger.cc
  src/Phantom.cc
  src/PhantomMessenger.cc
  src/PrimaryGeneratorAction.cc
  src/RunAction.cc
  src/RunActionMessenger.cc
  src/SensitiveDetector.cc
  src/SteppingAction.cc
)
target_include_directories(pct-sim PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(pct-sim PRIVATE ${Geant4_LIBRARIES})

# Copy macros and job scripts next to the executable
foreach(_dir macros scripts)
  file(COPY ${PROJECT_SOURCE_DIR}/${_dir} DESTINATION ${PROJECT_BINARY_DIR})
endforeach()
