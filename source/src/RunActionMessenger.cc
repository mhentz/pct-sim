// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"

#include "RunAction.hh"
#include "RunActionMessenger.hh"

RunActionMessenger::RunActionMessenger(RunAction *runAction) : fRunAction(runAction) {
  fRunActionDir = new G4UIdirectory("/user/run/");
  fRunActionDir->SetGuidance("User run action control.");

  fSetRunIDCmd = new G4UIcmdWithAnInteger("/user/run/setInitialRunID", this);
  fSetRunIDCmd->SetGuidance("Set the offset added to the run ID stored in the output ntuples.");
  fSetRunIDCmd->SetParameterName("runID", false);
  fSetRunIDCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

RunActionMessenger::~RunActionMessenger() {
  delete fSetRunIDCmd;
  delete fRunActionDir;
}

void RunActionMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
  if (command == fSetRunIDCmd) {
    fRunAction->SetInitialRunID(fSetRunIDCmd->GetNewIntValue(newValue));
  }
}
