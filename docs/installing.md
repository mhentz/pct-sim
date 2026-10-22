# Installation

## Installing Geant4

The simulation requires Geant4 11.0 or later, built with multi-threading. If your
machine or cluster does not provide a suitable installation, you can build one from
source following the [Geant4 installation guide][geant4-install]. The approach used
during development keeps the source, build and install trees side by side:

    software/
    ├── src/geant4                   # git checkout of the Geant4 tag
    ├── build/geant4-v11.4.3         # CMake build tree
    └── install/geant4-v11.4.3       # installation prefix

The only external dependencies are a C++17 compiler and CMake 3.16 or higher (on
macOS, the Xcode command-line tools and CMake from Homebrew are sufficient). Geant4
downloads its data sets during the build when `GEANT4_INSTALL_DATA` is on.

    mkdir -p software/src software/build/geant4-v11.4.3 software/install
    cd software/src
    git clone --branch v11.4.3 --depth 1 https://github.com/Geant4/geant4.git
    cd ../build/geant4-v11.4.3
    cmake ../../src/geant4 \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_STANDARD=17 \
      -DCMAKE_INSTALL_PREFIX=$PWD/../../install/geant4-v11.4.3 \
      -DGEANT4_BUILD_MULTITHREADED=ON \
      -DGEANT4_INSTALL_DATA=ON \
      -DGEANT4_USE_SYSTEM_EXPAT=ON
    make -j8
    make install

Visualisation drivers (Qt, OpenGL) are optional; the simulation runs in batch mode
without them, and the interactive session described in [usage.md](usage.md) then
falls back to the terminal. Before building or running the simulation, set up the
environment with

    source /path/to/install/geant4-v11.4.3/bin/geant4.sh

## Installing the simulation

Clone the repository, create a build directory, configure with CMake and build:

    git clone https://github.com/mhentz/protonct-sim.git
    cd protonct-sim
    mkdir build
    cd build
    cmake ../source
    make -j8

CMake finds Geant4 through the environment set up by `geant4.sh`. The executable
`protonct-sim` is placed in the build directory together with copies of the
`macros/` and `scripts/` directories.

## Running on a cluster

The simulations for the thesis were run on the plus1 cluster of the UCL High Energy
Physics group. The scripts in `source/scripts/` can be used on Sun Grid Engine-style
clusters to run several jobs in parallel: they submit, resubmit and post-process
acquisitions, and the job files load the Geant4 environment from `GEANT4_INSTALL`.
The `module` lines in the job files are site-specific and may need adapting. See
[usage.md](usage.md) for how to use the scripts.

[geant4-install]: https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html
