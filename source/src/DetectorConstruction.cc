// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
#include "Phantom.hh"
#include "PhantomMessenger.hh"
#include "SensitiveDetector.hh"

DetectorConstruction::DetectorConstruction()
    : fWorldSize(2. * m, 2. * m, 2. * m), fDetSize(40. * cm, 40. * cm, 0.1 * mm) {
  DefineMaterials();

  fPhantomUserLimits = new G4UserLimits();
  fPhantomUserLimits->SetMaxAllowedStep(0.1 * mm);

  fDetectorConstructionMessenger = new DetectorConstructionMessenger(this);
  fPhantomMessenger = new PhantomMessenger(this);
}

DetectorConstruction::~DetectorConstruction() {
  delete fDetectorConstructionMessenger;
  delete fPhantomMessenger;
  delete fPhantomUserLimits;
}

void DetectorConstruction::DefineMaterials() {
  auto nistMan = G4NistManager::Instance();

  //------------------------------------------------------------------------------------
  // Define elements
  //------------------------------------------------------------------------------------
  G4Element *H = nistMan->FindOrBuildElement("H");
  G4Element *C = nistMan->FindOrBuildElement("C");
  G4Element *N = nistMan->FindOrBuildElement("N");
  G4Element *O = nistMan->FindOrBuildElement("O");
  G4Element *Mg = nistMan->FindOrBuildElement("Mg");
  G4Element *Si = nistMan->FindOrBuildElement("Si");
  G4Element *P = nistMan->FindOrBuildElement("P");
  G4Element *Cl = nistMan->FindOrBuildElement("Cl");
  G4Element *Ca = nistMan->FindOrBuildElement("Ca");
  G4Element *W = nistMan->FindOrBuildElement("W");

  //------------------------------------------------------------------------------------
  // Define materials
  //------------------------------------------------------------------------------------
  G4double density, fractionmass;
  G4int natoms, ncomponents;

  auto air = nistMan->FindOrBuildMaterial("G4_AIR");
  auto aluminium = nistMan->FindOrBuildMaterial("G4_Al");
  auto vacuum = nistMan->FindOrBuildMaterial("G4_Galactic");

  auto water = new G4Material("Water", density = 1. * g / cm3, ncomponents = 2);
  water->AddElement(H, natoms = 2);
  water->AddElement(O, natoms = 1);
  water->GetIonisation()->SetMeanExcitationEnergy(78.0 * eV);

  // Catphan line-pair beads
  auto tungstenCarbide = new G4Material("TungstenCarbide", density = 15.63 * g / cm3, ncomponents = 2);
  tungstenCarbide->AddElement(C, natoms = 1);
  tungstenCarbide->AddElement(W, natoms = 1);

  // Gammex 467 materials
  auto AP6 = new G4Material("AP6", density = 0.947 * g / cm3, ncomponents = 5);
  AP6->AddElement(H, fractionmass = 0.0906);
  AP6->AddElement(C, fractionmass = 0.7230);
  AP6->AddElement(N, fractionmass = 0.0225);
  AP6->AddElement(O, fractionmass = 0.1627);
  AP6->AddElement(Cl, fractionmass = 0.0013);

  auto B200 = new G4Material("B200", density = 1.152 * g / cm3, ncomponents = 7);
  B200->AddElement(H, fractionmass = 0.0665);
  B200->AddElement(C, fractionmass = 0.5552);
  B200->AddElement(N, fractionmass = 0.0198);
  B200->AddElement(O, fractionmass = 0.2364);
  B200->AddElement(P, fractionmass = 0.0324);
  B200->AddElement(Cl, fractionmass = 0.0011);
  B200->AddElement(Ca, fractionmass = 0.0887);

  auto BR12 = new G4Material("BR12", density = 0.98 * g / cm3, ncomponents = 6);
  BR12->AddElement(H, fractionmass = 0.0859);
  BR12->AddElement(C, fractionmass = 0.7011);
  BR12->AddElement(N, fractionmass = 0.0233);
  BR12->AddElement(O, fractionmass = 0.1790);
  BR12->AddElement(Cl, fractionmass = 0.0013);
  BR12->AddElement(Ca, fractionmass = 0.0095);

  auto CB2_30 = new G4Material("CB2_30", density = 1.331 * g / cm3, ncomponents = 6);
  CB2_30->AddElement(H, fractionmass = 0.0668);
  CB2_30->AddElement(C, fractionmass = 0.5348);
  CB2_30->AddElement(N, fractionmass = 0.0212);
  CB2_30->AddElement(O, fractionmass = 0.2561);
  CB2_30->AddElement(Cl, fractionmass = 0.0011);
  CB2_30->AddElement(Ca, fractionmass = 0.1201);

  auto CB2_50 = new G4Material("CB2_50", density = 1.559 * g / cm3, ncomponents = 6);
  CB2_50->AddElement(H, fractionmass = 0.0477);
  CB2_50->AddElement(C, fractionmass = 0.4163);
  CB2_50->AddElement(N, fractionmass = 0.0152);
  CB2_50->AddElement(O, fractionmass = 0.3200);
  CB2_50->AddElement(Cl, fractionmass = 0.0008);
  CB2_50->AddElement(Ca, fractionmass = 0.2002);

  auto IB3 = new G4Material("IB3", density = 1.134 * g / cm3, ncomponents = 7);
  IB3->AddElement(H, fractionmass = 0.0667);
  IB3->AddElement(C, fractionmass = 0.5564);
  IB3->AddElement(N, fractionmass = 0.0196);
  IB3->AddElement(O, fractionmass = 0.2352);
  IB3->AddElement(P, fractionmass = 0.0323);
  IB3->AddElement(Cl, fractionmass = 0.0011);
  IB3->AddElement(Ca, fractionmass = 0.0886);

  auto LN300 = new G4Material("LN300", density = 0.3 * g / cm3, ncomponents = 7);
  LN300->AddElement(H, fractionmass = 0.0846);
  LN300->AddElement(C, fractionmass = 0.5938);
  LN300->AddElement(N, fractionmass = 0.0196);
  LN300->AddElement(O, fractionmass = 0.1814);
  LN300->AddElement(Mg, fractionmass = 0.1119);
  LN300->AddElement(Si, fractionmass = 0.0078);
  LN300->AddElement(Cl, fractionmass = 0.001);

  auto LN450 = new G4Material("LN450", density = 0.46 * g / cm3, ncomponents = 7);
  LN450->AddElement(H, fractionmass = 0.0847);
  LN450->AddElement(C, fractionmass = 0.5957);
  LN450->AddElement(N, fractionmass = 0.0197);
  LN450->AddElement(O, fractionmass = 0.1811);
  LN450->AddElement(Mg, fractionmass = 0.1121);
  LN450->AddElement(Si, fractionmass = 0.0058);
  LN450->AddElement(Cl, fractionmass = 0.001);

  auto LV1 = new G4Material("LV1", density = 1.096 * g / cm3, ncomponents = 6);
  LV1->AddElement(H, fractionmass = 0.0806);
  LV1->AddElement(C, fractionmass = 0.6701);
  LV1->AddElement(N, fractionmass = 0.0247);
  LV1->AddElement(O, fractionmass = 0.2001);
  LV1->AddElement(Cl, fractionmass = 0.0014);
  LV1->AddElement(Ca, fractionmass = 0.0231);

  auto SB3 = new G4Material("SB3", density = 1.822 * g / cm3, ncomponents = 6);
  SB3->AddElement(H, fractionmass = 0.0341);
  SB3->AddElement(C, fractionmass = 0.3141);
  SB3->AddElement(N, fractionmass = 0.0184);
  SB3->AddElement(O, fractionmass = 0.3650);
  SB3->AddElement(Cl, fractionmass = 0.0004);
  SB3->AddElement(Ca, fractionmass = 0.2681);

  auto SR2 = new G4Material("SR2", density = 1.051 * g / cm3, ncomponents = 5);
  SR2->AddElement(H, fractionmass = 0.1083);
  SR2->AddElement(C, fractionmass = 0.7254);
  SR2->AddElement(N, fractionmass = 0.0169);
  SR2->AddElement(O, fractionmass = 0.1486);
  SR2->AddElement(Cl, fractionmass = 0.0008);

  auto WaterSolidCT = new G4Material("WaterSolid", density = 1.022 * g / cm3, ncomponents = 6);
  WaterSolidCT->AddElement(H, fractionmass = 0.0800);
  WaterSolidCT->AddElement(C, fractionmass = 0.6730);
  WaterSolidCT->AddElement(N, fractionmass = 0.0239);
  WaterSolidCT->AddElement(O, fractionmass = 0.1987);
  WaterSolidCT->AddElement(Cl, fractionmass = 0.0014);
  WaterSolidCT->AddElement(Ca, fractionmass = 0.0231);

  //------------------------------------------------------------------------------------
  // Fill materials map
  //------------------------------------------------------------------------------------
  fMaterials["Air"] = air;
  fMaterials["Aluminium"] = aluminium;
  fMaterials["Vacuum"] = vacuum;
  fMaterials["Water"] = water;
  fMaterials["TungstenCarbide"] = tungstenCarbide;

  // gammex 467
  fMaterials["AP6"] = AP6;
  fMaterials["B200"] = B200;
  fMaterials["BR12"] = BR12;
  fMaterials["CB2_30"] = CB2_30;
  fMaterials["CB2_50"] = CB2_50;
  fMaterials["IB3"] = IB3;
  fMaterials["LN300"] = LN300;
  fMaterials["LN450"] = LN450;
  fMaterials["LV1"] = LV1;
  fMaterials["SB3"] = SB3;
  fMaterials["SR2"] = SR2;
  fMaterials["WaterSolidCT"] = WaterSolidCT;
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  G4bool checkOverlaps = true;

  // Set visAttributes for various components
  auto wireframeVisAtt = new G4VisAttributes();
  wireframeVisAtt->SetForceWireframe(true);

  auto detectorVisAtt = new G4VisAttributes(G4Colour(0.71, 0.651, 0.259, 1.));

  // World
  // - G4Box needs to be supplied half-lengths
  auto sWorld = new G4Box("World", fWorldSize.x() / 2, fWorldSize.y() / 2, fWorldSize.z() / 2);
  auto lWorld = new G4LogicalVolume(sWorld, fMaterials.at("Air"), "World");
  auto pWorld = new G4PVPlacement(nullptr, G4ThreeVector(), lWorld, "World", nullptr, false, 0);

  lWorld->SetVisAttributes(wireframeVisAtt);

  // Define detectors
  G4double phantomHalfWidth = 0.;

  if (fPhantomType == "gammex" || fPhantomType == "water-large") {
    phantomHalfWidth = 165. * mm;
  } else if (fPhantomType == "ctp528" || fPhantomType == "spiral" || fPhantomType == "spiral-small" ||
             fPhantomType == "water") {
    phantomHalfWidth = 100. * mm;
  } else if (fPhantomType == "toy") {
    phantomHalfWidth = 8. * mm;
  } else {
    G4ExceptionDescription message;
    message << "Unknown phantom type '" << fPhantomType << "'. Set /phantom/type to one of: "
            << "ctp528, gammex, spiral, spiral-small, water, water-large, toy.";
    G4Exception("DetectorConstruction::Construct()", "ProtonCT001", FatalErrorInArgument, message);
  }

  if (fPhantomSetup == "ideal") {
    fDetDistance = phantomHalfWidth + 10. * mm;

    if (fPhantomType == "toy") {
      fDetDistance = phantomHalfWidth + 2. * mm;
    }

  } else if (fPhantomSetup == "realistic") {
    fDetDistance = phantomHalfWidth + 150. * mm;
  } else {
    G4ExceptionDescription message;
    message << "Unknown setup '" << fPhantomSetup << "'. Set /phantom/setup to either 'ideal' or 'realistic'.";
    G4Exception("DetectorConstruction::Construct()", "ProtonCT002", FatalErrorInArgument, message);
  }

  // Shift the detector positions by half the detector's width so the face that
  // is supposed to be hit first is where we want to record particles.
  G4ThreeVector detInPosition(0., 0., fDetSize.z() / 2 - fDetDistance);
  G4ThreeVector detOutPosition(0., 0., fDetDistance + fDetSize.z() / 2);

  auto sDetector = new G4Box("Detector", fDetSize.x() / 2, fDetSize.y() / 2, fDetSize.z() / 2);

  // Entry detector
  fDetectorInLV = new G4LogicalVolume(sDetector, fMaterials.at("Vacuum"), "DetectorIn");
  new G4PVPlacement(nullptr, detInPosition, fDetectorInLV, "DetectorIn", lWorld, false, 0, checkOverlaps);

  fDetectorInLV->SetVisAttributes(detectorVisAtt);

  // Exit detector
  fDetectorOutLV = new G4LogicalVolume(sDetector, fMaterials.at("Vacuum"), "DetectorOut");
  new G4PVPlacement(nullptr, detOutPosition, fDetectorOutLV, "DetectorOut", lWorld, false, 0, checkOverlaps);

  fDetectorOutLV->SetVisAttributes(detectorVisAtt);

  // Phantom
  Phantom phantom(lWorld, fMaterials, fPhantomUserLimits);
  phantom.Construct(fPhantomType, fPhantomAngle);

  return pWorld; // always return world physical vol
}

void DetectorConstruction::ConstructSDandField() {
  auto detMan = G4SDManager::GetSDMpointer();

  auto sdIn = detMan->FindSensitiveDetector("/PhaseSpace/in", false);

  if (!sdIn) {
    sdIn = new SensitiveDetector("/PhaseSpace/in", "HitsCollectionIn");
    detMan->AddNewDetector(sdIn);
    G4cout << "Sensitive detector added to entry detector" << G4endl;
  }
  fDetectorInLV->SetSensitiveDetector(sdIn);

  auto sdOut = detMan->FindSensitiveDetector("/PhaseSpace/out", false);

  if (!sdOut) {
    sdOut = new SensitiveDetector("/PhaseSpace/out", "HitsCollectionOut");
    detMan->AddNewDetector(sdOut);
    G4cout << "Sensitive detector added to exit detector" << G4endl;
  }
  fDetectorOutLV->SetSensitiveDetector(sdOut);
}
