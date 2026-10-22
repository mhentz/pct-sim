// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"

#include "EventAction.hh"
#include "EventActionMessenger.hh"

EventActionMessenger::EventActionMessenger(EventAction *eventAction) : fEventAction(eventAction) {
  fEventDir = new G4UIdirectory("/user/event/");
  fEventDir->SetGuidance("Event control.");

  fPrintCmd = new G4UIcmdWithAnInteger("/user/event/printModulo", this);
  fPrintCmd->SetGuidance("Print the event number every n events.");
  fPrintCmd->SetParameterName("EventPrintMod", false);
  fPrintCmd->SetRange("EventPrintMod > 0");
  fPrintCmd->AvailableForStates(G4State_Idle);
}

EventActionMessenger::~EventActionMessenger() {
  delete fPrintCmd;
  delete fEventDir;
}

void EventActionMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
  if (command == fPrintCmd) {
    fEventAction->SetPrintModulo(fPrintCmd->GetNewIntValue(newValue));
  }
}
