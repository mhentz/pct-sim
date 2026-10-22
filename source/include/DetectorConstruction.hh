// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include <map>

#include "G4ThreeVector.hh"
#include "G4VUserDetectorConstruction.hh"

class DetectorConstructionMessenger;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class G4VPhysicalVolume;
class PhantomMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  ~DetectorConstruction() override;

  G4VPhysicalVolume *Construct() override;
  void ConstructSDandField() override;

  void SetDetSize(const G4ThreeVector &size) { fDetSize = size; }

  void SetPhantomType(const G4String &phantomType) { fPhantomType = phantomType; }
  void SetPhantomSetup(const G4String &phantomSetup) { fPhantomSetup = phantomSetup; }
  void SetPhantomRotation(G4double phantomAngle) { fPhantomAngle = phantomAngle; }

  G4double GetDetDistance() const { return fDetDistance; }

private:
  void DefineMaterials();

  DetectorConstructionMessenger *fDetectorConstructionMessenger = nullptr;
  PhantomMessenger *fPhantomMessenger = nullptr;

  // Materials available to the phantoms, by name
  std::map<G4String, G4Material *> fMaterials;

  // Step limit inside the phantom region; owned here because the region keeps a pointer to it
  G4UserLimits *fPhantomUserLimits = nullptr;

  // Dimensions (full widths)
  G4ThreeVector fWorldSize;
  G4ThreeVector fDetSize;

  // Distance from the phantom centre to the recording face of each detector
  G4double fDetDistance = 0.;

  // Detector logical volumes for sensitive detector definition
  G4LogicalVolume *fDetectorInLV = nullptr;
  G4LogicalVolume *fDetectorOutLV = nullptr;

  // Phantom
  G4String fPhantomType;
  G4String fPhantomSetup;
  G4double fPhantomAngle = 0.;
};

#endif
