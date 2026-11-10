// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
#include "Materials.hh"
#include "Phantom.hh"
#include "PhantomMessenger.hh"
#include "SensitiveDetector.hh"

DetectorConstruction::DetectorConstruction()
    : fMaterials(BuildMaterials()), fWorldSize(2. * m, 2. * m, 2. * m),
      fDetSize(40. * cm, 40. * cm, 0.1 * mm) {
  fPhantomUserLimits = new G4UserLimits();
  fPhantomUserLimits->SetMaxAllowedStep(0.1 * mm);

  fDetectorConstructionMessenger = new DetectorConstructionMessenger(this);
  fPhantomMessenger = new PhantomMessenger(this);
}

DetectorConstruction::~DetectorConstruction() {
  delete fDetectorConstructionMessenger;
  delete fPhantomMessenger;
  delete fPhantomUserLimits;
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  G4bool checkOverlaps = true;

  // Set visAttributes for various components
  auto wireframeVisAtt = new G4VisAttributes();
  wireframeVisAtt->SetForceWireframe(true);

  auto detectorVisAtt = new G4VisAttributes(G4Colour(0.71, 0.651, 0.259, 1.));

  // World
  // - G4Box needs to be supplied half-lengths
  auto sWorld = new G4Box("World", fWorldSize.x() / 2, fWorldSize.y() / 2, fWorldSize.z() / 2);
  auto lWorld = new G4LogicalVolume(sWorld, fMaterials.at("Air"), "World");
  auto pWorld = new G4PVPlacement(nullptr, G4ThreeVector(), lWorld, "World", nullptr, false, 0);

  lWorld->SetVisAttributes(wireframeVisAtt);

  // Define detectors
  G4double phantomHalfWidth = 0.;

  if (fPhantomType == "gammex" || fPhantomType == "water-large") {
    phantomHalfWidth = 165. * mm;
  } else if (fPhantomType == "ctp528" || fPhantomType == "spiral" || fPhantomType == "spiral-small" ||
             fPhantomType == "water") {
    phantomHalfWidth = 100. * mm;
  } else if (fPhantomType == "toy") {
    phantomHalfWidth = 8. * mm;
  } else {
    G4ExceptionDescription message;
    message << "Unknown phantom type '" << fPhantomType << "'. Set /phantom/type to one of: "
            << "ctp528, gammex, spiral, spiral-small, water, water-large, toy.";
    G4Exception("DetectorConstruction::Construct()", "ProtonCT001", FatalErrorInArgument, message);
  }

  if (fPhantomSetup == "ideal") {
    fDetDistance = phantomHalfWidth + 10. * mm;

    if (fPhantomType == "toy") {
      fDetDistance = phantomHalfWidth + 2. * mm;
    }

  } else if (fPhantomSetup == "realistic") {
    fDetDistance = phantomHalfWidth + 150. * mm;
  } else {
    G4ExceptionDescription message;
    message << "Unknown setup '" << fPhantomSetup << "'. Set /phantom/setup to either 'ideal' or 'realistic'.";
    G4Exception("DetectorConstruction::Construct()", "ProtonCT002", FatalErrorInArgument, message);
  }

  // Shift the detector positions by half the detector's width so the face that
  // is supposed to be hit first is where we want to record particles.
  G4ThreeVector detInPosition(0., 0., fDetSize.z() / 2 - fDetDistance);
  G4ThreeVector detOutPosition(0., 0., fDetDistance + fDetSize.z() / 2);

  auto sDetector = new G4Box("Detector", fDetSize.x() / 2, fDetSize.y() / 2, fDetSize.z() / 2);

  // Entry detector
  fDetectorInLV = new G4LogicalVolume(sDetector, fMaterials.at("Vacuum"), "DetectorIn");
  new G4PVPlacement(nullptr, detInPosition, fDetectorInLV, "DetectorIn", lWorld, false, 0, checkOverlaps);

  fDetectorInLV->SetVisAttributes(detectorVisAtt);

  // Exit detector
  fDetectorOutLV = new G4LogicalVolume(sDetector, fMaterials.at("Vacuum"), "DetectorOut");
  new G4PVPlacement(nullptr, detOutPosition, fDetectorOutLV, "DetectorOut", lWorld, false, 0, checkOverlaps);

  fDetectorOutLV->SetVisAttributes(detectorVisAtt);

  // Phantom
  Phantom phantom(lWorld, fMaterials, fPhantomUserLimits);
  phantom.Construct(fPhantomType, fPhantomAngle);

  return pWorld; // always return world physical vol
}

void DetectorConstruction::ConstructSDandField() {
  auto detMan = G4SDManager::GetSDMpointer();

  auto sdIn = detMan->FindSensitiveDetector("/PhaseSpace/in", false);

  if (!sdIn) {
    sdIn = new SensitiveDetector("/PhaseSpace/in", "HitsCollectionIn");
    detMan->AddNewDetector(sdIn);
    G4cout << "Sensitive detector added to entry detector" << G4endl;
  }
  fDetectorInLV->SetSensitiveDetector(sdIn);

  auto sdOut = detMan->FindSensitiveDetector("/PhaseSpace/out", false);

  if (!sdOut) {
    sdOut = new SensitiveDetector("/PhaseSpace/out", "HitsCollectionOut");
    detMan->AddNewDetector(sdOut);
    G4cout << "Sensitive detector added to exit detector" << G4endl;
  }
  fDetectorOutLV->SetSensitiveDetector(sdOut);
}
