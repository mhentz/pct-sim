# Running the simulation

The commands below assume the simulation has been built in `build/` as described in the
[README](../README.md). CMake copies `macros/` and `scripts/` into the build directory,
and the macros expect to be run from there.

## Interactive mode

```
./pct-sim
```

starts a Qt (or terminal) session and executes `macros/debug/ui_vis.mac`, which builds
the phantom selected by the `phantom` and `setup` aliases at the top of that file,
opens the OpenGL viewer and simulates one projection. Edit the aliases to look at a
different phantom.

## Batch mode

```
./pct-sim my_acquisition.mac
```

executes the given macro. A minimal macro for a full 360° acquisition of the Gammex
phantom in 0.5° steps could look like this:

```
/control/alias phantom gammex
/control/alias setup ideal

/phantom/type {phantom}
/phantom/setup {setup}
/user/det/size 400. 400. 0.001 mm

/run/numberOfThreads 8
/run/initialize

/control/macroPath macros
/control/execute init_scoring_gammex.mac
/control/execute gps_gammex_{setup}.mac

/user/run/setInitialRunID 0
/control/alias angular_step_size 0.5
/control/loop run_projection.mac angle 0 359.5 {angular_step_size}
```

`macros/run_projection.mac` defines the number of protons per projection only for the
`ctp528`, `gammex` and `spiral` phantoms. For the other phantoms, `macros/debug/run_projection.mac`
also covers `water` and `water-large`; otherwise add a line for the phantom of interest.

Note that `init_scoring.mac` places the scoring meshes for a 200 mm phantom and
`init_scoring_gammex.mac` for the 330 mm Gammex phantom. The fluence boxes sit just
outside the phantom surface (9 to 11 mm) in both set-ups: they score the fluence
entering and leaving the phantom and are independent of the detector distance. The `water-large`
phantom has the same 330 mm diameter as the Gammex phantom and must therefore be run with
`init_scoring_gammex.mac` and `gps_gammex_{setup}.mac`: with the 200 mm source macros the
source plane sits inside the phantom and no protons reach the entry detector.

## Commands

In addition to the standard Geant4 commands, the following are defined.

| Command | Available | Description |
| --- | --- | --- |
| `/phantom/type <type>` | PreInit | Phantom to build: `ctp528`, `gammex`, `spiral`, `spiral-small`, `water`, `water-large` or `toy` (see [phantoms.md](phantoms.md)). |
| `/phantom/setup <setup>` | PreInit | `ideal` or `realistic`; sets the distance between the phantom and the detectors. |
| `/phantom/rotate <angle> <unit>` | Idle | Angle of the phantom about the vertical axis. |
| `/phantom/update` | PreInit, Idle | Rebuild the geometry; required after `/phantom/rotate`. |
| `/user/det/size <x> <y> <z> <unit>` | PreInit | Full widths of the entry and exit detectors. |
| `/user/run/setInitialRunID <id>` | PreInit, Idle | Offset added to the run ID stored in the ntuples. |
| `/user/event/printModulo <n>` | Idle | Print the event number every `n` events. |

## Batch jobs on a cluster

The scripts in `scripts/` split an acquisition into jobs for a Sun Grid Engine
scheduler (`qsub` with `#$` directives). They were written for a particular cluster, so
the module names and resource requests in `simulation.job` and `split_simulation.job`
will most likely need adapting.

Before submitting, set `GEANT4_INSTALL` to the Geant4 installation prefix (the
directory containing `bin/geant4.sh`) and, optionally, `WORKDIR` to the build
directory that holds `pct-sim`, `macros/` and `scripts/`:

```
export GEANT4_INSTALL=/path/to/geant4
export WORKDIR=/path/to/pct-sim/build
./scripts/submit_simulation.sh gi 0.5
```

The first argument selects the phantom and set-up: `c`, `g` or `s` for `ctp528`, `gammex`
or `spiral`, followed by `i` or `r` for `ideal` or `realistic`. The second argument is
the angular step in degrees. Each job simulates several consecutive projections by
filling in `macros/sim_template.mac`, and writes to `data/<phantom>-<setup>/` in the
working directory.

| Script | Purpose |
| --- | --- |
| `submit_simulation.sh <sim> <step>` | Submit a full 360° acquisition. |
| `resubmit_simulation.sh <sim> <step> <batch_size> <start>` | Resubmit one job starting at angle `<start>`. |
| `submit_split_simulation.sh <sim> <step> <batch_size>` | Submit an acquisition in which each batch of projections is run as 8 independent chunks with different seeds. Each chunk simulates the full number of protons per projection, and the output files carry a `-<chunk>` suffix. |
| `resubmit_split_simulation.sh <sim> <step> <batch_size> <start> <chunk>` | Resubmit one chunk. |
| `move_logs.sh` | Move scheduler logs into `data/logs/`. |
| `total_slice_dose.sh <dir>` | Sum the slice dose over all projections in a scoring directory. |

If `WORKDIR` is not set, all four scripts default to
`~/simulations/pct-sim/build-<sim>`, i.e. one build directory per phantom and
set-up (for example `build-ci`), so that the data of the `ideal` and `realistic`
acquisitions end up in separate directories.
