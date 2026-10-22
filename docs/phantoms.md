# Phantoms

The phantom is chosen with `/phantom/type` before `/run/initialize`. All phantoms are
cylinders whose axis is vertical ($y$) and whose background material is water
(density 1 g/cm³, mean excitation energy 78 eV), unless stated otherwise.

| Type | Description | Diameter | Step limit |
| --- | --- | --- | --- |
| `ctp528` | Catphan CTP528 high-resolution module | 200 mm | 0.1 mm |
| `gammex` | Gammex 467 tissue characterisation phantom | 330 mm | 0.1 mm |
| `spiral` | Aluminium rods along a spiral | 200 mm | 0.1 mm |
| `spiral-small` | Spiral phantom of Rit et al. (2013) | 200 mm | none |
| `water` | Plain water cylinder | 200 mm | none |
| `water-large` | Plain water cylinder | 330 mm | none |
| `toy` | Small water cylinder for quick tests | 16 mm | none |

"Step limit" refers to the maximum step length applied in the `Phantom` region; the
phantoms without one use the default stepping of the physics list. `ctp528`, `gammex`
and `spiral` are the phantoms used for the actual experiments; the others are test
phantoms, which is why no step limit is applied to them.

## `ctp528`

A water cylinder 200 mm in diameter and 160 mm long that contains only the CTP528
high-resolution module of the Catphan phantom: a 150 mm diameter, 40 mm thick water
disc holding

- twenty line-pair gauges from 1 to 20 line pairs/cm, each made of 8 mm deep
  aluminium bars, arranged around a 47.5 mm radius;
- two tungsten carbide beads, 0.28 mm in diameter, 20 mm either side of the centre.

The other Catphan modules are not modelled. The casing is shifted 40 mm vertically so
that the CTP528 module lies in the beam plane. The geometry of this module was adapted
from code written by Charles-Antoine Collins-Fekete.

## `gammex`

A model of the Gammex 467 phantom: a water cylinder 330 mm in diameter and 400 mm long
with sixteen 28 mm diameter inserts, eight on an inner ring (55 mm from the centre) and
eight on an outer ring (about 109 mm from the centre). The insert materials are LN-300
and LN-450 (lung), AP6 (adipose), BR12 (breast), CT solid water (a solid resin water
equivalent), SR2 (brain), LV1 (liver), IB3 (inner bone), B200 (porous bone with a mineral
density of 200 mg/cm³), CB2-30 % and CB2-50 % (CB2 resin with 30 % and 50 % calcium
carbonate) and SB3 (cortical bone). Their elemental compositions and densities, defined
in `DetectorConstruction::DefineMaterials()`, are taken from Table 1 of G. Landry et al.,
"Deriving effective atomic numbers from DECT based on a parameterization of the ratio
of high and low linear attenuation coefficients", *Phys. Med. Biol.* 58, 6851 (2013),
and are tabulated in M. Hentz, *Iterative Reconstruction using Structural Priors in
Proton Computed Tomography*, PhD thesis, University College London (2026).

## `spiral` and `spiral-small`

`spiral-small` reproduces the spiral phantom of Rit et al. (2013), with 5 mm diameter
aluminium inserts in a 200 mm diameter water cylinder; `spiral` is based on the same
design but uses larger (12 mm diameter) inserts at different positions. Both are
400 mm long. The varying distance of the inserts from the centre is intended for
studying how spatial resolution changes with depth.

## `water`, `water-large` and `toy`

Homogeneous water cylinders, 100 mm long for `water` and `water-large`, and 10.2 mm
long for `toy`. They may be useful for checking the energy loss and scattering
models, or for quick tests of the full chain.

## References

S. Rit, G. Dedes, N. Freud, D. Sarrut and J. M. Létang, "Filtered backprojection proton
CT reconstruction along most likely paths", *Medical Physics* 40(3), 031103 (2013).
<https://doi.org/10.1118/1.4789589>
