// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"

class EventActionMessenger;
class RunAction;

class EventAction : public G4UserEventAction {
public:
  explicit EventAction(RunAction *runAction);
  ~EventAction() override;

  void BeginOfEventAction(const G4Event *event) override;
  void EndOfEventAction(const G4Event *event) override;

  void SetPrintModulo(G4int value) { fPrintModulo = value; }

private:
  RunAction *fRunAction;
  EventActionMessenger *fMessenger;

  G4int fPrintModulo = 10000;
  G4int fHitsCollectionInID = -1;
  G4int fHitsCollectionOutID = -1;
};

#endif
