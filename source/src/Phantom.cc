// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2026 Matthieu Hentz

#include <vector>

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "Phantom.hh"

Phantom::Phantom(G4LogicalVolume *logicalWorld, const std::map<G4String, G4Material *> &materials,
                 G4UserLimits *userLimits)
    : fLogicalWorld(logicalWorld), fMaterials(materials), fUserLimits(userLimits) {}

void Phantom::Construct(const G4String &phantomType, G4double angle) {
  fPhantomAngle = angle;

  if (phantomType == "ctp528") {
    ConstructCatphan();
  } else if (phantomType == "gammex") {
    ConstructGammex();
  } else if (phantomType == "spiral") {
    ConstructSpiral();
  } else if (phantomType == "spiral-small") {
    ConstructSpiralSmall();
  } else if (phantomType == "water") {
    ConstructWaterPhantom(100. * mm);
  } else if (phantomType == "water-large") {
    ConstructWaterPhantom(165. * mm);
  } else if (phantomType == "toy") {
    ConstructToyPhantom();
  } else {
    G4ExceptionDescription message;
    message << "No such phantom: '" << phantomType << "'";
    G4Exception("Phantom::Construct()", "ProtonCT003", FatalErrorInArgument, message);
    return;
  }

  G4cout << "Constructed phantom: '" << phantomType << "'" << G4endl;
}

G4RotationMatrix *Phantom::CasingRotation() const {
  auto rotation = new G4RotationMatrix();
  // Align the phantom axis with the vertical (y) axis
  rotation->rotateX(-0.5 * pi);
  // Rotate phantom about its own axis (z, as it has not been rotated yet)
  rotation->rotateZ(fPhantomAngle);
  return rotation;
}

void Phantom::SetPhantomRegion(G4LogicalVolume *logicalVolume) const {
  G4Region *region = G4RegionStore::GetInstance()->FindOrCreateRegion("Phantom");
  logicalVolume->SetRegion(region);
  region->AddRootLogicalVolume(logicalVolume);
  region->SetUserLimits(fUserLimits);
}

void Phantom::ConstructWaterPhantom(G4double phantomRadius) {
  auto sCasing = new G4Tubs("water", 0., phantomRadius, 10. * cm / 2, 0., twopi);
  auto lCasing = new G4LogicalVolume(sCasing, fMaterials.at("Water"), "water");

  new G4PVPlacement(CasingRotation(), G4ThreeVector(), lCasing, "water", fLogicalWorld, false, 0, fCheckOverlaps);
}

void Phantom::ConstructToyPhantom() {
  // Small water cylinder for quick tests
  G4double halfHeight = 5. * mm + 0.1 * mm;

  auto sCasing = new G4Tubs("toy", 0., 8. * mm, halfHeight, 0., twopi);
  auto lCasing = new G4LogicalVolume(sCasing, fMaterials.at("Water"), "toy");

  new G4PVPlacement(CasingRotation(), G4ThreeVector(), lCasing, "toy", fLogicalWorld, false, 0, fCheckOverlaps);
}

void Phantom::ConstructSpiralSmall() {
  // Spiral phantom of Rit et al. (2013): 5 mm diameter aluminium rods in a 20 cm diameter water cylinder
  auto sCasing = new G4Tubs("Spiral", 0., 10. * cm, 40. * cm / 2, 0., twopi);
  auto lCasing = new G4LogicalVolume(sCasing, fMaterials.at("Water"), "Spiral");

  new G4PVPlacement(CasingRotation(), G4ThreeVector(), lCasing, "spiral_small", fLogicalWorld, false, 0,
                    fCheckOverlaps);

  // Aluminium inserts
  auto sInsert = new G4Tubs("Insert", 0., 2.5 * mm, 40. * cm / 2, 0., twopi);
  auto lInsert = new G4LogicalVolume(sInsert, fMaterials.at("Aluminium"), "Insert");

  std::vector<G4ThreeVector> positions;
  positions.push_back(G4ThreeVector(0., 0., 0.));
  positions.push_back(G4ThreeVector(1.1134 * mm, -7.9221 * mm, 0.));
  positions.push_back(G4ThreeVector(6.5357 * mm, 10.0640 * mm, 0.));
  positions.push_back(G4ThreeVector(-15.3802 * mm, -4.4102 * mm, 0.));
  positions.push_back(G4ThreeVector(18.1262 * mm, -8.4524 * mm, 0.));
  positions.push_back(G4ThreeVector(-9.7617 * mm, 21.9251 * mm, 0.));
  positions.push_back(G4ThreeVector(-8.1864 * mm, -26.7765 * mm, 0.));
  positions.push_back(G4ThreeVector(27.1375 * mm, 16.9574 * mm, 0.));
  positions.push_back(G4ThreeVector(-35.5568 * mm, 5.6316 * mm, 0.));
  positions.push_back(G4ThreeVector(25.7115 * mm, -30.6418 * mm, 0.));
  positions.push_back(G4ThreeVector(0.7679 * mm, 43.9933 * mm, 0.));
  positions.push_back(G4ThreeVector(-32.1183 * mm, -35.6710 * mm, 0.));
  positions.push_back(G4ThreeVector(51.6124 * mm, 6.3372 * mm, 0.));
  positions.push_back(G4ThreeVector(-46.4261 * mm, 31.3148 * mm, 0.));
  positions.push_back(G4ThreeVector(15.5291 * mm, -57.9555 * mm, 0.));
  positions.push_back(G4ThreeVector(28.0558 * mm, 57.5228 * mm, 0.));
  positions.push_back(G4ThreeVector(-62.5943 * mm, -26.5697 * mm, 0.));
  positions.push_back(G4ThreeVector(68.4761 * mm, -22.2492 * mm, 0.));
  positions.push_back(G4ThreeVector(-39.1429 * mm, 65.1447 * mm, 0.));
  positions.push_back(G4ThreeVector(-13.8919 * mm, -78.7846 * mm, 0.));
  positions.push_back(G4ThreeVector(65.2803 * mm, 52.8629 * mm, 0.));
  positions.push_back(G4ThreeVector(-87.9464 * mm, 3.0712 * mm, 0.));
  positions.push_back(G4ThreeVector(67.2845 * mm, -62.7438 * mm, 0.));
  positions.push_back(G4ThreeVector(-10.0347 * mm, 95.4741 * mm, 0.));

  G4int copyNo = 0;
  for (const auto &position : positions) {
    new G4PVPlacement(nullptr, position, lInsert, "Insert", lCasing, false, copyNo++, fCheckOverlaps);
  }
}

void Phantom::ConstructSpiral() {
  // Based on the spiral phantom of Rit et al. (2013), with larger rods at different positions
  auto sCasing = new G4Tubs("SpiralBig", 0., 10. * cm, 40. * cm / 2, 0., twopi);
  auto lCasing = new G4LogicalVolume(sCasing, fMaterials.at("Water"), "SpiralBig");

  SetPhantomRegion(lCasing);

  new G4PVPlacement(CasingRotation(), G4ThreeVector(), lCasing, "spiral", fLogicalWorld, false, 0, fCheckOverlaps);

  // Aluminium inserts
  auto sInsert = new G4Tubs("Insert", 0., 6. * mm, 40. * cm / 2, 0., twopi);
  auto lInsert = new G4LogicalVolume(sInsert, fMaterials.at("Aluminium"), "Insert");

  std::vector<G4ThreeVector> positions;
  positions.push_back(G4ThreeVector(0., 0., 0.));
  positions.push_back(G4ThreeVector(-20. * mm, 8. * mm, 0.));
  positions.push_back(G4ThreeVector(-34. * mm, 28. * mm, 0.));
  positions.push_back(G4ThreeVector(-36. * mm, 56. * mm, 0.));
  positions.push_back(G4ThreeVector(-24. * mm, 86. * mm, 0.));
  positions.push_back(G4ThreeVector(1. * mm, 22. * mm, 0.));
  positions.push_back(G4ThreeVector(16. * mm, 41. * mm, 0.));
  positions.push_back(G4ThreeVector(42. * mm, 52. * mm, 0.));
  positions.push_back(G4ThreeVector(74. * mm, 50. * mm, 0.));
  positions.push_back(G4ThreeVector(21. * mm, 5. * mm, 0.));
  positions.push_back(G4ThreeVector(44. * mm, -2. * mm, 0.));
  positions.push_back(G4ThreeVector(63. * mm, -23. * mm, 0.));
  positions.push_back(G4ThreeVector(70. * mm, -55. * mm, 0.));
  positions.push_back(G4ThreeVector(11. * mm, -19. * mm, 0.));
  positions.push_back(G4ThreeVector(11. * mm, -43. * mm, 0.));
  positions.push_back(G4ThreeVector(-3. * mm, -67. * mm, 0.));
  positions.push_back(G4ThreeVector(-30. * mm, -84. * mm, 0.));
  positions.push_back(G4ThreeVector(-14. * mm, -17. * mm, 0.));
  positions.push_back(G4ThreeVector(-38. * mm, -24. * mm, 0.));
  positions.push_back(G4ThreeVector(-65. * mm, -17. * mm, 0.));
  positions.push_back(G4ThreeVector(-89. * mm, 3. * mm, 0.));

  G4int copyNo = 0;
  for (const auto &position : positions) {
    new G4PVPlacement(nullptr, position, lInsert, "Insert", lCasing, false, copyNo++, fCheckOverlaps);
  }
}

void Phantom::ConstructCatphan() {
  //----------------------------------------------------------------------------------------------------------------
  // Catphan phantom
  // Adapted from code written by Charles-Antoine Collins-Fekete (UCL), shared with permission.
  //----------------------------------------------------------------------------------------------------------------
  // Only the CTP528 high-resolution module is built.

  G4Material *water = fMaterials.at("Water");
  G4Material *aluminium = fMaterials.at("Aluminium");
  G4Material *tungstenCarbide = fMaterials.at("TungstenCarbide");

  auto casingVisAtt = new G4VisAttributes(G4Colour(0, 1, 1));
  auto moduleVisAtt = new G4VisAttributes(G4Colour(1, 0, 1));
  auto insertVisAtt = new G4VisAttributes(G4Colour(0, 1, 0));
  insertVisAtt->SetForceSolid(true);

  // Casing
  auto sCasing = new G4Tubs("Casing", 0., 20. * cm / 2, 16. * cm / 2, 0., twopi);
  auto lCasing = new G4LogicalVolume(sCasing, water, "Casing_Log");
  lCasing->SetVisAttributes(casingVisAtt);

  SetPhantomRegion(lCasing);

  // Shift the casing so that the CTP528 module, centred 40 mm along the casing axis, lies in the beam plane
  G4double shift = 40. * mm;
  new G4PVPlacement(CasingRotation(), G4ThreeVector(0., shift, 0.), lCasing, "CasingWorld", fLogicalWorld, false,
                    0, fCheckOverlaps);

  // CTP528 high-resolution module
  auto sModule = new G4Tubs("CTP528", 0., 150. * mm / 2, 40. * mm / 2, 0., twopi);
  auto lModule = new G4LogicalVolume(sModule, water, "CTP528Log");
  lModule->SetVisAttributes(moduleVisAtt);
  new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 40. * mm), lModule, "CTP528World", lCasing, false, 0,
                    fCheckOverlaps);

  // Tungsten carbide beads
  auto sBead = new G4Sphere("Bead", 0., 0.28 * mm / 2, 0., twopi, 0., pi);
  auto lBead = new G4LogicalVolume(sBead, tungstenCarbide, "bead_log");
  lBead->SetVisAttributes(insertVisAtt);
  new G4PVPlacement(nullptr, G4ThreeVector(0., 20. * mm, 0.), lBead, "bead_world", lModule, false, 0,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(0., -20. * mm, 0.), lBead, "bead_world", lModule, false, 1,
                    fCheckOverlaps);

  // Line-pair gauges from 1 to 20 line pairs/cm, each made of aluminium bars in a water container. The
  // polynomial gives the angular position [deg] of each gauge as a function of its line pairs/cm.
  std::vector<float> anglesCoefficients = {-3.8654e-6f, 1.9470e-4f, -2.3863e-3f, -3.6253e-2f,
                                           1.3968f,     -3.0373e1f, 1.9902e2f};
  const int nCoefficients = static_cast<int>(anglesCoefficients.size());

  for (int i = 1; i < 21; i++) {
    // Width of the bars and of the gaps between them
    G4double gapSize = 0.5 / float(i) * cm;

    G4int nLinePairs = 5;
    if (i == 1)
      nLinePairs = 2;
    else if (i == 2)
      nLinePairs = 3;
    else if (i < 6)
      nLinePairs = 4;

    float angle = 0.f;
    float x = 1.;
    for (int j = 0; j < nCoefficients; ++j) {
      angle += x * anglesCoefficients[nCoefficients - 1 - j];
      x *= i;
    }

    G4ThreeVector position(0., 47.5 * mm, 0.);
    position.rotateZ((270 + angle) * degree);
    auto rotation = new G4RotationMatrix();
    rotation->rotateZ((270 - angle) * degree);

    auto sContainer = new G4Box("cont_box", (nLinePairs - 0.5) * gapSize, 4. * mm, 4. * mm);
    auto lContainer = new G4LogicalVolume(sContainer, water, "cont_log");
    new G4PVPlacement(rotation, position, lContainer, "cont_World", lModule, false, i, fCheckOverlaps);

    auto sBar = new G4Box("LinePair", gapSize / 2, 4. * mm, 4. * mm);
    auto lBar = new G4LogicalVolume(sBar, aluminium, "LinePair_Log");
    lBar->SetVisAttributes(insertVisAtt);

    for (int j = 0; j < nLinePairs; j++) {
      G4double barX = -(nLinePairs - 0.5) * gapSize + gapSize / 2 + 2 * j * gapSize;
      new G4PVPlacement(nullptr, G4ThreeVector(barX, 0., 0.), lBar, "LinePair_world", lContainer, false, j,
                        fCheckOverlaps);
    }
  }
}

void Phantom::ConstructGammex() {
  // Gammex 467 tissue characterisation phantom
  auto sCasing = new G4Tubs("gammex", 0., 165. * mm, 400. * mm / 2, 0., twopi);
  auto lCasing = new G4LogicalVolume(sCasing, fMaterials.at("Water"), "gammex");

  SetPhantomRegion(lCasing);

  new G4PVPlacement(CasingRotation(), G4ThreeVector(), lCasing, "gammex", fLogicalWorld, false, 0, fCheckOverlaps);

  auto sInsert = new G4Tubs("Insert", 0., 14. * mm, 400. * mm / 2, 0., twopi);

  auto lInsert_BR12 = new G4LogicalVolume(sInsert, fMaterials.at("BR12"), "Insert");
  auto lInsert_SB3 = new G4LogicalVolume(sInsert, fMaterials.at("SB3"), "Insert");
  auto lInsert_AP6 = new G4LogicalVolume(sInsert, fMaterials.at("AP6"), "Insert");
  auto lInsert_LV1 = new G4LogicalVolume(sInsert, fMaterials.at("LV1"), "Insert");
  auto lInsert_SR2 = new G4LogicalVolume(sInsert, fMaterials.at("SR2"), "Insert");
  auto lInsert_WaterSolidCT = new G4LogicalVolume(sInsert, fMaterials.at("WaterSolidCT"), "Insert");
  auto lInsert_LN300 = new G4LogicalVolume(sInsert, fMaterials.at("LN300"), "Insert");
  auto lInsert_CB2_50 = new G4LogicalVolume(sInsert, fMaterials.at("CB2_50"), "Insert");
  auto lInsert_LN450 = new G4LogicalVolume(sInsert, fMaterials.at("LN450"), "Insert");
  auto lInsert_CB2_30 = new G4LogicalVolume(sInsert, fMaterials.at("CB2_30"), "Insert");
  auto lInsert_IB3 = new G4LogicalVolume(sInsert, fMaterials.at("IB3"), "Insert");
  auto lInsert_B200 = new G4LogicalVolume(sInsert, fMaterials.at("B200"), "Insert");

  // Inserts inside
  new G4PVPlacement(nullptr, G4ThreeVector(55. * mm, 0. * mm, 0.), lInsert_BR12, "Insert", lCasing, false, 0,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(38. * mm, 38. * mm, 0.), lInsert_SB3, "Insert", lCasing, false, 1,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(0. * mm, 55. * mm, 0.), lInsert_AP6, "Insert", lCasing, false, 2,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(-38. * mm, 38. * mm, 0.), lInsert_LV1, "Insert", lCasing, false, 3,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(-55. * mm, 0. * mm, 0.), lInsert_SR2, "Insert", lCasing, false, 4,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(-38. * mm, -38. * mm, 0.), lInsert_WaterSolidCT, "Insert", lCasing,
                    false, 5, fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(0. * mm, -55. * mm, 0.), lInsert_LN300, "Insert", lCasing, false, 6,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(38. * mm, -38. * mm, 0.), lInsert_CB2_50, "Insert", lCasing, false, 7,
                    fCheckOverlaps);

  // Inserts outside
  new G4PVPlacement(nullptr, G4ThreeVector(101. * mm, 42. * mm, 0.), lInsert_LN450, "Insert", lCasing, false, 8,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(42. * mm, 101. * mm, 0.), lInsert_CB2_30, "Insert", lCasing, false, 9,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(-42. * mm, 101. * mm, 0.), lInsert_WaterSolidCT, "Insert", lCasing,
                    false, 10, fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(-101. * mm, 42. * mm, 0.), lInsert_IB3, "Insert", lCasing, false, 11,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(-101. * mm, -42. * mm, 0.), lInsert_LV1, "Insert", lCasing, false, 12,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(-42. * mm, -101. * mm, 0.), lInsert_AP6, "Insert", lCasing, false, 13,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(42. * mm, -101. * mm, 0.), lInsert_B200, "Insert", lCasing, false, 14,
                    fCheckOverlaps);
  new G4PVPlacement(nullptr, G4ThreeVector(101. * mm, -42. * mm, 0.), lInsert_LN300, "Insert", lCasing, false, 15,
                    fCheckOverlaps);
}
