// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

#include "Analysis.hh"
#include "DetectorHit.hh"
#include "EventAction.hh"
#include "EventActionMessenger.hh"
#include "RunAction.hh"

EventAction::EventAction(RunAction *runAction) : fRunAction(runAction) {
  fMessenger = new EventActionMessenger(this);
}

EventAction::~EventAction() { delete fMessenger; }

void EventAction::BeginOfEventAction(const G4Event *event) {
  G4int eventID = event->GetEventID();

  if (eventID % fPrintModulo == 0) {
    G4cout << "\n---> Begin of event: " << eventID << G4endl;
  }
}

void EventAction::EndOfEventAction(const G4Event *event) {
  auto hce = event->GetHCofThisEvent();
  if (!hce) {
    return;
  }

  if (fHitsCollectionInID < 0) {
    auto sdManager = G4SDManager::GetSDMpointer();
    fHitsCollectionInID = sdManager->GetCollectionID("HitsCollectionIn");
    fHitsCollectionOutID = sdManager->GetCollectionID("HitsCollectionOut");
  }

  auto hcIn = static_cast<HitsCollection *>(hce->GetHC(fHitsCollectionInID));
  auto hcOut = static_cast<HitsCollection *>(hce->GetHC(fHitsCollectionOutID));

  if (hcIn->entries() > 0) {
    fRunAction->IncrementInCount();
  }
  if (hcOut->entries() > 0) {
    fRunAction->IncrementOutCount();
  }

  // Only record protons seen by both detectors, using their first crossing of each
  if (hcIn->entries() > 0 && hcOut->entries() > 0) {
    G4int eventID = event->GetEventID();
    G4int runID = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID() + fRunAction->GetInitialRunID();

    Analysis::FillNtuple(0, (*hcIn)[0], eventID, runID);
    Analysis::FillNtuple(1, (*hcOut)[0], eventID, runID);
  }
}
