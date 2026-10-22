// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4HCofThisEvent.hh"
#include "G4Proton.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"

#include "SensitiveDetector.hh"

SensitiveDetector::SensitiveDetector(const G4String &name, const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name) {
  collectionName.insert(hitsCollectionName);
}

void SensitiveDetector::Initialize(G4HCofThisEvent *hce) {
  fHitsCollection = new HitsCollection(SensitiveDetectorName, collectionName[0]);

  if (fHitsCollectionID < 0) {
    fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  }
  hce->AddHitsCollection(fHitsCollectionID, fHitsCollection);
}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *) {
  const G4Track *track = step->GetTrack();
  const G4StepPoint *preStep = step->GetPreStepPoint();

  // Only score primary protons entering the detector, ie when the step starts on its boundary,
  // and travelling downstream
  if (track->GetParentID() != 0 || track->GetDefinition() != G4Proton::Definition() ||
      preStep->GetStepStatus() != fGeomBoundary || preStep->GetMomentumDirection().z() <= 0.) {
    return false;
  }

  auto hit = new DetectorHit();
  hit->SetEnergy(preStep->GetKineticEnergy());
  hit->SetPosition(preStep->GetPosition());
  hit->SetDirection(preStep->GetMomentumDirection());
  fHitsCollection->insert(hit);

  return true;
}
