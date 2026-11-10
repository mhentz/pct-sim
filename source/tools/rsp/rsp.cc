// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

// Print the relative stopping power (RSP) of each simulation material for protons: the
// electronic stopping power of the material from G4BetheBlochModel divided by that of
// water. Usage: rsp [energy in MeV ...], 200 and 250 MeV by default.

#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "G4BetheBlochModel.hh"
#include "G4Material.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"
#include "G4Version.hh"

#include "Materials.hh"

int main(int argc, char **argv) {
  std::vector<G4double> energies;
  for (int i = 1; i < argc; ++i) {
    energies.push_back(std::atof(argv[i]) * MeV);
  }
  if (energies.empty()) {
    energies = {200. * MeV, 250. * MeV};
  }

  auto materials = BuildMaterials();
  auto proton = G4Proton::Proton();
  G4BetheBlochModel model;
  auto stoppingPower = [&](G4Material *material, G4double energy) {
    return model.ComputeDEDXPerVolume(material, proton, energy, DBL_MAX);
  };

  std::printf("%s\n", G4Version.c_str());
  std::printf("%-16s", "material");
  for (auto energy : energies) {
    std::printf(" %9.1f MeV", energy / MeV);
  }
  std::printf("\n");
  for (const auto &[name, material] : materials) {
    std::printf("%-16s", name.c_str());
    for (auto energy : energies) {
      std::printf(" %13.6f", stoppingPower(material, energy) / stoppingPower(materials.at("Water"), energy));
    }
    std::printf("\n");
  }
  return 0;
}
