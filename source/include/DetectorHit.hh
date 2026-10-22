// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#ifndef DETECTOR_HIT_HH
#define DETECTOR_HIT_HH

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"

// Phase-space point of a primary proton entering a detector plane
class DetectorHit : public G4VHit {
public:
  DetectorHit() = default;
  ~DetectorHit() override = default;

  inline void *operator new(size_t);
  inline void operator delete(void *);

  void Print() override;

  void SetEnergy(G4double energy) { fEnergy = energy; }
  void SetPosition(const G4ThreeVector &position) { fPosition = position; }
  void SetDirection(const G4ThreeVector &direction) { fDirection = direction; }

  G4double GetEnergy() const { return fEnergy; }
  G4ThreeVector GetPosition() const { return fPosition; }
  G4ThreeVector GetDirection() const { return fDirection; }

private:
  G4double fEnergy = 0.;
  G4ThreeVector fPosition;
  G4ThreeVector fDirection;
};

using HitsCollection = G4THitsCollection<DetectorHit>;
extern G4ThreadLocal G4Allocator<DetectorHit> *HitAllocator;

//------------------------------------------------------------------------------------
// Memory management
//------------------------------------------------------------------------------------

inline void *DetectorHit::operator new(size_t) {
  if (!HitAllocator)
    HitAllocator = new G4Allocator<DetectorHit>;
  return (void *)HitAllocator->MallocSingle();
}

inline void DetectorHit::operator delete(void *hit) { HitAllocator->FreeSingle((DetectorHit *)hit); }

#endif
