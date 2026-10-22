// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include <cmath>

#include "G4Step.hh"
#include "G4SystemOfUnits.hh"

#include "DetectorConstruction.hh"
#include "SteppingAction.hh"

SteppingAction::SteppingAction(const DetectorConstruction *detectorConstruction)
    : fDetectorConstruction(detectorConstruction) {}

void SteppingAction::UserSteppingAction(const G4Step *step) {
  const G4ThreeVector position = step->GetPreStepPoint()->GetPosition();

  // Kill particle if it has gone past the exit detector in the z-direction
  // or if has travelled too far in the x or y direction and will never
  // reach the detector
  G4double zThreshold = fDetectorConstruction->GetDetDistance() + 10. * mm;

  if (position.z() > zThreshold || std::abs(position.x()) > 210. * mm || std::abs(position.y()) > 210. * mm) {
    step->GetTrack()->SetTrackStatus(fStopAndKill);
  }
}
