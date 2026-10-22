// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef EVENT_ACTION_MESSENGER_HH
#define EVENT_ACTION_MESSENGER_HH

#include "G4UImessenger.hh"

class EventAction;
class G4UIcmdWithAnInteger;
class G4UIdirectory;

class EventActionMessenger : public G4UImessenger {
public:
  explicit EventActionMessenger(EventAction *eventAction);
  ~EventActionMessenger() override;

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

private:
  EventAction *fEventAction;
  G4UIdirectory *fEventDir;
  G4UIcmdWithAnInteger *fPrintCmd;
};

#endif
