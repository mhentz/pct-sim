// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef PHANTOM_MESSENGER_HH
#define PHANTOM_MESSENGER_HH

#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIdirectory;

class PhantomMessenger : public G4UImessenger {
public:
  explicit PhantomMessenger(DetectorConstruction *detectorConstruction);
  ~PhantomMessenger() override;

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

private:
  DetectorConstruction *fDetectorConstruction;

  G4UIdirectory *fPhantomDir;
  G4UIcmdWithAString *fPhantomTypeCmd;
  G4UIcmdWithAString *fPhantomSetupCmd;
  G4UIcmdWithADoubleAndUnit *fRotatePhantomCmd;
  G4UIcmdWithoutParameter *fUpdateCmd;
};

#endif
