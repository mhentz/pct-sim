// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef SENSITIVE_DETECTOR_HH
#define SENSITIVE_DETECTOR_HH

#include "G4VSensitiveDetector.hh"

#include "DetectorHit.hh"

// Records primary protons as they enter a detector plane
class SensitiveDetector : public G4VSensitiveDetector {
public:
  SensitiveDetector(const G4String &name, const G4String &hitsCollectionName);
  ~SensitiveDetector() override = default;

  void Initialize(G4HCofThisEvent *hce) override;
  G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;

private:
  HitsCollection *fHitsCollection = nullptr;
  G4int fHitsCollectionID = -1;
};

#endif
