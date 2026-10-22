// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "PhantomMessenger.hh"

PhantomMessenger::PhantomMessenger(DetectorConstruction *detectorConstruction)
    : fDetectorConstruction(detectorConstruction) {
  fPhantomDir = new G4UIdirectory("/phantom/");
  fPhantomDir->SetGuidance("Commands to change phantom parameters.");

  // Choose phantom
  fPhantomTypeCmd = new G4UIcmdWithAString("/phantom/type", this);
  fPhantomTypeCmd->SetGuidance("Choose the phantom type.");
  fPhantomTypeCmd->SetParameterName("PhantomType", false);
  fPhantomTypeCmd->SetCandidates("ctp528 gammex spiral spiral-small water water-large toy");
  fPhantomTypeCmd->AvailableForStates(G4State_PreInit);

  // Choose ideal or realistic setup
  fPhantomSetupCmd = new G4UIcmdWithAString("/phantom/setup", this);
  fPhantomSetupCmd->SetGuidance("Choose the setup, which sets the detector distance from the phantom.");
  fPhantomSetupCmd->SetParameterName("PhantomSetup", false);
  fPhantomSetupCmd->SetCandidates("ideal realistic");
  fPhantomSetupCmd->AvailableForStates(G4State_PreInit);

  // Rotate phantom
  fRotatePhantomCmd = new G4UIcmdWithADoubleAndUnit("/phantom/rotate", this);
  fRotatePhantomCmd->SetGuidance("Set the angle the phantom is rotated by about the vertical axis.");
  fRotatePhantomCmd->SetGuidance("Apply /phantom/update afterwards for it to take effect.");
  fRotatePhantomCmd->SetParameterName("angle", false);
  fRotatePhantomCmd->SetDefaultUnit("deg");
  fRotatePhantomCmd->SetUnitCandidates("deg rad");
  fRotatePhantomCmd->AvailableForStates(G4State_Idle);

  // Update phantom
  fUpdateCmd = new G4UIcmdWithoutParameter("/phantom/update", this);
  fUpdateCmd->SetGuidance("Update phantom geometry.");
  fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
  fUpdateCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PhantomMessenger::~PhantomMessenger() {
  delete fPhantomTypeCmd;
  delete fPhantomSetupCmd;
  delete fRotatePhantomCmd;
  delete fUpdateCmd;
  delete fPhantomDir;
}

void PhantomMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
  if (command == fPhantomTypeCmd) {
    fDetectorConstruction->SetPhantomType(newValue);
  }

  if (command == fPhantomSetupCmd) {
    fDetectorConstruction->SetPhantomSetup(newValue);
  }

  if (command == fRotatePhantomCmd) {
    fDetectorConstruction->SetPhantomRotation(fRotatePhantomCmd->GetNewDoubleValue(newValue));
  }

  if (command == fUpdateCmd) {
    G4UImanager::GetUIpointer()->ApplyCommand("/run/reinitializeGeometry");
  }
}
