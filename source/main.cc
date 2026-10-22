// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4RunManagerFactory.hh"
#include "G4ScoringManager.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "QGSP_BIC_HP.hh"
#include "Randomize.hh"

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

int main(int argc, char **argv) {
  // Start an interactive session if no macro is given
  G4UIExecutive *ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4Random::setTheEngine(new CLHEP::RanecuEngine());

  auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Activate command-based scoring, which must happen right after creating the run manager
  G4ScoringManager::GetScoringManager()->SetVerboseLevel(1);

  auto detectorConstruction = new DetectorConstruction();

  auto physicsList = new QGSP_BIC_HP();
  physicsList->SetVerboseLevel(1);
  physicsList->RegisterPhysics(new G4StepLimiterPhysics());

  runManager->SetUserInitialization(detectorConstruction);
  runManager->SetUserInitialization(physicsList);
  runManager->SetUserInitialization(new ActionInitialization(detectorConstruction));

  auto visManager = new G4VisExecutive();
  visManager->Initialize();

  auto UImanager = G4UImanager::GetUIpointer();

  if (ui) {
    UImanager->ApplyCommand("/control/execute macros/debug/ui_vis.mac");
    ui->SessionStart();
    delete ui;
  } else {
    UImanager->ApplyCommand(G4String("/control/execute ") + argv[1]);
  }

  delete visManager;
  delete runManager;
}
