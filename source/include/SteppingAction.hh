// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include "G4UserSteppingAction.hh"

class DetectorConstruction;

class SteppingAction : public G4UserSteppingAction {
public:
  explicit SteppingAction(const DetectorConstruction *detectorConstruction);
  ~SteppingAction() override = default;

  void UserSteppingAction(const G4Step *step) override;

private:
  const DetectorConstruction *fDetectorConstruction;
};

#endif
