// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef DETECTOR_CONSTRUCTION_MESSENGER_HH
#define DETECTOR_CONSTRUCTION_MESSENGER_HH

#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIcmdWith3VectorAndUnit;
class G4UIdirectory;

class DetectorConstructionMessenger : public G4UImessenger {
public:
  explicit DetectorConstructionMessenger(DetectorConstruction *detectorConstruction);
  ~DetectorConstructionMessenger() override;

  void SetNewValue(G4UIcommand *command, G4String newValue) override;

private:
  DetectorConstruction *fDetectorConstruction;

  G4UIdirectory *fDetDir;
  G4UIcmdWith3VectorAndUnit *fDetSizeCmd;
};

#endif
