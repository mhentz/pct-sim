// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef PHANTOM_HH
#define PHANTOM_HH

#include <map>

#include "G4RotationMatrix.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Material;
class G4UserLimits;

// Builds the selected phantom at the centre of the world volume
class Phantom {
public:
  Phantom(G4LogicalVolume *logicalWorld, const std::map<G4String, G4Material *> &materials,
          G4UserLimits *userLimits);

  void Construct(const G4String &phantomType, G4double angle);

private:
  void ConstructCatphan();
  void ConstructGammex();
  void ConstructSpiral();
  void ConstructSpiralSmall();
  void ConstructWaterPhantom(G4double phantomRadius);
  void ConstructToyPhantom();

  // Orientation of a phantom whose axis is along its local z-axis
  G4RotationMatrix *CasingRotation() const;
  // Apply the phantom step limit to a volume and its daughters
  void SetPhantomRegion(G4LogicalVolume *logicalVolume) const;

  G4LogicalVolume *fLogicalWorld;
  const std::map<G4String, G4Material *> &fMaterials;
  G4UserLimits *fUserLimits;

  G4double fPhantomAngle = 0.;
  G4bool fCheckOverlaps = true;
};

#endif
