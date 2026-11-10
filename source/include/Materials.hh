// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef MATERIALS_HH
#define MATERIALS_HH

#include <map>

#include "G4String.hh"

class G4Material;

// Build the materials used by the phantoms and detectors, keyed by name
std::map<G4String, G4Material *> BuildMaterials();

#endif
