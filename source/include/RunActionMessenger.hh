// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef RUN_ACTION_MESSENGER_HH
#define RUN_ACTION_MESSENGER_HH

#include "G4UImessenger.hh"

class G4UIcmdWithAnInteger;
class G4UIdirectory;
class RunAction;

class RunActionMessenger : public G4UImessenger {
public:
  explicit RunActionMessenger(RunAction *runAction);
  ~RunActionMessenger() override;

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

private:
  RunAction *fRunAction;
  G4UIdirectory *fRunActionDir;
  G4UIcmdWithAnInteger *fSetRunIDCmd;
};

#endif
