// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4AccumulableManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"

#include "Analysis.hh"
#include "RunAction.hh"
#include "RunActionMessenger.hh"

RunAction::RunAction() {
  fMessenger = new RunActionMessenger(this);

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetVerboseLevel(2);
  analysisManager->SetNtupleMerging(true);

  Analysis::CreateNtuple("In", "Phase space on entry detector");
  Analysis::CreateNtuple("Out", "Phase space on exit detector");

  auto accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Register(fInCount);
  accumulableManager->Register(fOutCount);
}

RunAction::~RunAction() { delete fMessenger; }

void RunAction::BeginOfRunAction(const G4Run *) {
  G4AccumulableManager::Instance()->Reset();
  G4AnalysisManager::Instance()->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run *) {
  G4cout << "\n### End of run" << G4endl;

  G4AccumulableManager::Instance()->Merge();

  if (IsMaster()) {
    G4cout << "Total incoming protons detected: " << fInCount.GetValue() << G4endl;
    G4cout << "Total outgoing protons detected: " << fOutCount.GetValue() << G4endl;
  }

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
