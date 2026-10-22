# How the simulation works

This page outlines how a proton CT acquisition is modelled and which classes are
involved. Details of the phantoms, the commands and the output files are given in
[phantoms.md](phantoms.md), [usage.md](usage.md) and [output.md](output.md).

## Set-up

The coordinate system is fixed to the scanner. Protons travel along $+z$, $x$ runs
across the beam and $y$ is vertical. Rather than rotating the source and detectors
around the object, the phantom is rotated about the vertical axis, which should be
equivalent for a parallel or near-parallel beam.

- **World**: a 2 m air cube.
- **Phantom**: centred at the origin with its cylinder axis along $y$, so that each
  projection images one horizontal slice.
- **Entry and exit detectors**: thin vacuum boxes placed upstream and downstream of
  the phantom. They are not modelled as physical trackers; they only mark the planes
  where protons are recorded. The upstream face of each box sits at a distance $d$
  from the phantom centre, with $d$ equal to the phantom half-width plus 10 mm in the
  `ideal` set-up (2 mm for `toy`) or plus 150 mm in the `realistic` set-up. Their
  lateral size is set with `/user/det/size`; the batch macros use
  400 mm × 400 mm (`ideal`) or 400 mm × 100 mm (`realistic`), 1 µm thick.
- **Source**: a General Particle Source configured in `macros/gps_*.mac`. Protons start
  10 mm upstream of the entry detector from a rectangular plane that is wide in $x$ and
  3 mm tall in $y$. In the `ideal` set-up the beam is monoenergetic and parallel; in
  the `realistic` set-up it has a Gaussian energy spread (σ = 0.2 % of the nominal
  energy) and a 3 mrad angular spread. The nominal energy is 200 MeV, or 250 MeV for the
  larger Gammex phantom.

## Physics

The reference physics list `QGSP_BIC_HP` is used together with `G4StepLimiterPhysics`.
This list is the one generally recommended for proton CT simulation; the choice and its
validation are discussed in M. Hentz, *Iterative Reconstruction using Structural Priors
in Proton Computed Tomography*, PhD thesis, University College London (2026). For the `ctp528`, `gammex` and `spiral` phantoms the phantom volume belongs to a
`Phantom` region with a maximum step length of 0.1 mm.

## What happens during an event

1. `PrimaryGeneratorAction` asks the General Particle Source for one proton.
2. Geant4 transports it and any secondaries through the geometry.
3. `SensitiveDetector`, attached to both detector planes, creates a `DetectorHit` when a
   *primary* proton enters a detector through its boundary while moving downstream.
   The hit holds the kinetic energy, position and direction at that point.
   Secondaries and backward-going protons are ignored.
4. `SteppingAction` stops tracks that have gone more than 10 mm past the exit detector,
   or more than 210 mm sideways, since they can no longer contribute.
5. At the end of the event, `EventAction` writes one row to each of the `In` and `Out`
   ntuples if the proton was recorded by *both* detectors, using its first crossing of
   each. Protons that stop in the phantom or miss a detector are therefore not stored,
   although they are still counted in the totals printed at the end of each run.

Alongside the phase-space ntuples, command-based scoring meshes defined in
`macros/init_scoring*.mac` record the proton dose and deposited energy in a 3 mm thick
slice through the phantom, and the proton fluence through thin boxes in front of,
at the centre of and behind the phantom.

## Projections and runs

Each projection is a separate Geant4 run. The macro `macros/run_projection.mac`:

1. sets the phantom angle with `/phantom/rotate` and rebuilds the geometry with
   `/phantom/update`;
2. seeds the random number generator from the angle, so that each projection has its
   own reproducible sequence;
3. chooses the output file name from the phantom, set-up and angle;
4. runs a number of protons that depends on the phantom and set-up, scaled with the
   angular step so that the total number of protons in an acquisition stays roughly
   constant;
5. dumps the scoring meshes to CSV files.

`macros/sim_template.mac` loops this macro over a range of angles. The run ID written
to the ntuples is the Geant4 run number plus an offset set with
`/user/run/setInitialRunID`, which the template sets to twice the first angle of the
batch job. This is meant to keep run IDs distinct when an acquisition is split across
several jobs.

## Classes

| Class | Role |
| --- | --- |
| `DetectorConstruction` | Defines the materials, builds the world and detectors, and calls `Phantom`; attaches the sensitive detectors. |
| `Phantom` | Builds the selected phantom at the requested angle. |
| `DetectorConstructionMessenger`, `PhantomMessenger` | UI commands for the detector size and the phantom (`/user/det/`, `/phantom/`). |
| `ActionInitialization` | Creates the user actions for the master and worker threads. |
| `PrimaryGeneratorAction` | Wraps the General Particle Source. |
| `SensitiveDetector`, `DetectorHit` | Record primary protons entering a detector plane. |
| `EventAction`, `EventActionMessenger` | Fill the ntuples at the end of each event; `/user/event/printModulo`. |
| `RunAction`, `RunActionMessenger` | Open and write the output file for each run and count detected protons; `/user/run/setInitialRunID`. |
| `Analysis` | Defines and fills the `In` and `Out` ntuples. |
| `SteppingAction` | Stops tracks that can no longer reach the exit detector. |

The application runs in multi-threaded mode. The ntuples from the worker threads are
merged into a single ROOT file per projection.
