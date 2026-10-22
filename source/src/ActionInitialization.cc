// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization(const DetectorConstruction *detectorConstruction)
    : fDetectorConstruction(detectorConstruction) {}

void ActionInitialization::BuildForMaster() const { SetUserAction(new RunAction()); }

void ActionInitialization::Build() const {
  auto runAction = new RunAction();

  SetUserAction(new PrimaryGeneratorAction());
  SetUserAction(runAction);
  SetUserAction(new EventAction(runAction));
  SetUserAction(new SteppingAction(fDetectorConstruction));
}
