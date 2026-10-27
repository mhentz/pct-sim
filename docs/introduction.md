# Introduction

The `pct-sim` application, built using [Geant4](https://geant4.web.cern.ch), simulates the passage of protons
through a range of user-defined phantoms and records their energy, lateral
position, and direction of travel at a specified position. The output of these
simulations can then be used to produce images using different reconstruction
algorithms. The use of simulations for this purpose is desirable as it allows
for an easy comparison of the performance of different proton CT reconstruction
algorithms in terms of their spatial resolution, density resolution, relative
error, noise and percentage of over- and under-dosed voxels.


## Proton Computed Tomography

The energy deposited per unit length by a heavy charged particle in an absorber
is known as the stopping power and the total energy deposited by a particle
along its trajectory through the medium is given by the integral of its stopping
power along the path taken through the material.

The inverse problem central to proton computed tomography (pCT) is described by
the equation

$$
\int_{\Gamma_i} \text{RSP}(x) \:\! \text{d}l = \int_{E_{out}}^{E_{in}} \frac{\text{d}E}{S(E(x), I_W(x))}
$$

from which we wish to reconstruct the 3-dimensional distribution of the relative
stopping power (RSP) given a measurement of the right hand side of the equation
above which is each proton's water-equivalent path length (WEPL) through the
material. The path along which the RSP is integrated is given by
$\Gamma_i$ in this equation, where the subscript $i$ indicates that
we are considering the $i$-th proton. In x-ray CT we can assume
$\Gamma_i$ to be a straight line between the source position of the
x-rays and the position where they are detected and therefore only a measurement
of the ingoing and outgoing intensities (the intensity of an x-ray beam plays
the role that the energy of a single proton plays in proton CT) is required. Unfortunately, the
problem is more complicated when considering protons because they undergo many
small-angle deflections due to interactions with the Coulomb field of the nuclei
in the material. This process is known as multiple Coulomb scattering (MCS) and
leads to considerable blurring in the proton CT reconstructions because the path
$\Gamma_i$ is now no longer known exactly and needs to be estimated given
the incoming and outgoing position and direction of each proton.
