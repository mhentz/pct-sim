// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIdirectory.hh"

#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction *detectorConstruction)
    : fDetectorConstruction(detectorConstruction) {
  fDetDir = new G4UIdirectory("/user/det/");
  fDetDir->SetGuidance("Detector construction commands.");

  fDetSizeCmd = new G4UIcmdWith3VectorAndUnit("/user/det/size", this);
  fDetSizeCmd->SetGuidance("Set the full widths of the entry and exit detectors.");
  fDetSizeCmd->SetParameterName("DetSizeX", "DetSizeY", "DetSizeZ", false);
  fDetSizeCmd->SetUnitCategory("Length");
  fDetSizeCmd->AvailableForStates(G4State_PreInit);
}

DetectorConstructionMessenger::~DetectorConstructionMessenger() {
  delete fDetSizeCmd;
  delete fDetDir;
}

void DetectorConstructionMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
  if (command == fDetSizeCmd) {
    fDetectorConstruction->SetDetSize(fDetSizeCmd->GetNew3VectorValue(newValue));
  }
}
