# Output

For a phantom `<phantom>` and set-up `<setup>`, the batch macros write to
`data/<phantom>-<setup>/` in the working directory.

## Phase space: `PhaseSpace/PhaseSpace<angle>.root`

One ROOT file per projection (with a `-<chunk>` suffix for split acquisitions),
containing two ntuples:

- `In`: protons recorded on the entry detector;
- `Out`: the same protons recorded on the exit detector.

A proton appears in both ntuples or in neither: only primary protons that reach both
detectors are stored. Each ntuple has the following columns.

| Column | Type | Unit | Description |
| --- | --- | --- | --- |
| `Ekine` | float | MeV | Kinetic energy |
| `X` | float | mm | Horizontal position on the detector face |
| `Y` | float | mm | Vertical position on the detector face |
| `dX`, `dY`, `dZ` | float | – | Direction cosines |
| `EventID` | int | – | Geant4 event ID within the run |
| `RunID` | int | – | Geant4 run ID plus the offset set with `/user/run/setInitialRunID` |

Positions and directions are given in the fixed scanner frame described in
[architecture.md](architecture.md), with the beam along $+z$. The $z$ position is not
stored, as it is the same for every proton on a given detector: the recording face
lies at a distance $-d$ (entry) or $+d$ (exit) from the phantom centre.

The ntuples are filled on several threads and merged at the end of each run. The rows
of `In` and `Out` are not guaranteed to end up in the same order (they usually do, but
not always), so pair them on `EventID` and `RunID` rather than on row number.

For example, with [uproot](https://github.com/scikit-hep/uproot5):

```python
import uproot

with uproot.open("PhaseSpace0.root") as f:
    entry = f["In"].arrays(library="pd")
    exit_ = f["Out"].arrays(library="pd")

protons = entry.merge(exit_, on=["EventID", "RunID"], suffixes=("_in", "_out"))
```

## Scoring: `scoring/*.csv`

Each projection also produces the CSV files written by the Geant4 command-based
scoring (`/score/dumpQuantityToFile`), using the meshes defined in
`macros/init_scoring*.mac`:

| File | Content |
| --- | --- |
| `sliceDose<angle>.csv` | Dose deposited by protons in a 3 mm thick cylindrical slice through the phantom |
| `sliceEnergy<angle>.csv` | Energy deposited by protons in the same slice |
| `fluenceIn<angle>.csv`, `fluenceCentre<angle>.csv`, `fluenceOut<angle>.csv` | Proton fluence through thin boxes in front of, at the centre of and behind the phantom |

Each mesh has a single bin. `scripts/total_slice_dose.sh` adds up the slice dose over
all projections.

## Logs

The batch jobs copy `macros/` and `scripts/` into `data/<phantom>-<setup>/logs/` and
write the standard output of each job to `logs/output/`, so that the settings used for
an acquisition are kept with its data.
