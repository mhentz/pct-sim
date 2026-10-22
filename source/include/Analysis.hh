// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef ANALYSIS_HH
#define ANALYSIS_HH

#include "DetectorHit.hh"

// Phase-space ntuples recorded on the entry and exit detectors
class Analysis {
public:
  static void CreateNtuple(const G4String &name, const G4String &title);
  static void FillNtuple(G4int ntupleID, const DetectorHit *hit, G4int eventID, G4int runID);
};

#endif
