// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4UnitsTable.hh"

#include "DetectorHit.hh"

G4ThreadLocal G4Allocator<DetectorHit> *HitAllocator = nullptr;

void DetectorHit::Print() {
  G4cout << "  energy: " << G4BestUnit(fEnergy, "Energy") << ", position: " << G4BestUnit(fPosition, "Length")
         << ", direction: " << fDirection << G4endl;
}
