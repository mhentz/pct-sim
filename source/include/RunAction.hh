// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include "G4Accumulable.hh"
#include "G4UserRunAction.hh"

class G4Run;
class RunActionMessenger;

class RunAction : public G4UserRunAction {
public:
  RunAction();
  ~RunAction() override;

  void BeginOfRunAction(const G4Run *run) override;
  void EndOfRunAction(const G4Run *run) override;

  void IncrementInCount() { fInCount += 1; }
  void IncrementOutCount() { fOutCount += 1; }

  // Offset added to the Geant4 run ID so that IDs stay unique across batch jobs
  void SetInitialRunID(G4int initialRunID) { fInitialRunID = initialRunID; }
  G4int GetInitialRunID() const { return fInitialRunID; }

private:
  RunActionMessenger *fMessenger;
  G4int fInitialRunID = 0;

  G4Accumulable<G4int> fInCount = 0;
  G4Accumulable<G4int> fOutCount = 0;
};

#endif
