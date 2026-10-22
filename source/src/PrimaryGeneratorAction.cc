// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() { fParticleSource = new G4GeneralParticleSource(); }

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleSource; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *event) { fParticleSource->GeneratePrimaryVertex(event); }
