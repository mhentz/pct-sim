// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4AnalysisManager.hh"

#include "Analysis.hh"

void Analysis::CreateNtuple(const G4String &name, const G4String &title) {
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->CreateNtuple(name, title);
  analysisManager->CreateNtupleFColumn("Ekine");
  analysisManager->CreateNtupleFColumn("X");
  analysisManager->CreateNtupleFColumn("Y");
  analysisManager->CreateNtupleFColumn("dX");
  analysisManager->CreateNtupleFColumn("dY");
  analysisManager->CreateNtupleFColumn("dZ");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleIColumn("RunID");
  analysisManager->FinishNtuple();
}

void Analysis::FillNtuple(G4int ntupleID, const DetectorHit *hit, G4int eventID, G4int runID) {
  auto analysisManager = G4AnalysisManager::Instance();
  const G4ThreeVector position = hit->GetPosition();
  const G4ThreeVector direction = hit->GetDirection();

  analysisManager->FillNtupleFColumn(ntupleID, 0, static_cast<G4float>(hit->GetEnergy()));
  analysisManager->FillNtupleFColumn(ntupleID, 1, static_cast<G4float>(position.x()));
  analysisManager->FillNtupleFColumn(ntupleID, 2, static_cast<G4float>(position.y()));
  analysisManager->FillNtupleFColumn(ntupleID, 3, static_cast<G4float>(direction.x()));
  analysisManager->FillNtupleFColumn(ntupleID, 4, static_cast<G4float>(direction.y()));
  analysisManager->FillNtupleFColumn(ntupleID, 5, static_cast<G4float>(direction.z()));
  analysisManager->FillNtupleIColumn(ntupleID, 6, eventID);
  analysisManager->FillNtupleIColumn(ntupleID, 7, runID);
  analysisManager->AddNtupleRow(ntupleID);
}
