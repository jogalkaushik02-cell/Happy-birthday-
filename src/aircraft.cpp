#include "aircraft.h"
#include "rlgl.h"

void AircraftRenderer::Init() {}
void AircraftRenderer::Unload() {}

void AircraftRenderer::DrawExternal(const AircraftState& state, GraphicsQuality quality) {
    rlPushMatrix();
    rlTranslatef(state.position.x, state.position.y, state.position.z);

    // Apply orientation quaternion via axis-angle
    Vector3 axis{};
    float angle = 0.0f;
    QuaternionToAxisAngle(state.orientation, &axis, &angle);
    rlRotatef(angle * RAD2DEG, axis.x, axis.y, axis.z);

    Color whiteFuselage = { 244, 246, 249, 255 };
    Color navyStripe    = { 24, 52, 98, 255 };
    Color goldAccent    = { 205, 162, 65, 255 };
    Color darkMetal     = { 42, 46, 52, 255 };
    Color tireBlack     = { 25, 25, 28, 255 };
    Color glassTint     = { 130, 175, 210, 125 };

    // 1. FUSELAGE (Engine cowling, cabin, tail cone)
    // Nose cowling
    DrawCylinderEx({ 0.0f, 0.08f, -2.55f }, { 0.0f, 0.12f, -1.35f }, 0.42f, 0.58f, 16, whiteFuselage);
    // Spinner cone
    DrawCylinderEx({ 0.0f, 0.08f, -2.92f }, { 0.0f, 0.08f, -2.55f }, 0.03f, 0.22f, 16, { 210, 215, 222, 255 });
    // Cabin main body
    DrawCube({ 0.0f, 0.18f, -0.35f }, 1.18f, 1.12f, 2.10f, whiteFuselage);
    // Cabin stripe
    DrawCube({ 0.0f, 0.02f, -0.35f }, 1.20f, 0.16f, 2.15f, navyStripe);
    DrawCube({ 0.0f, 0.11f, -0.35f }, 1.205f, 0.04f, 2.15f, goldAccent);

    // Tail cone tapering toward empennage
    DrawCylinderEx({ 0.0f, 0.22f, 0.65f }, { 0.0f, 0.34f, 4.10f }, 0.56f, 0.14f, 14, whiteFuselage);
    DrawCylinderEx({ 0.0f, 0.14f, 0.65f }, { 0.0f, 0.30f, 4.05f }, 0.57f, 0.15f, 14, navyStripe);

    // 2. COCKPIT WINDOWS
    DrawCube({ 0.0f, 0.52f, -1.25f }, 1.06f, 0.48f, 0.55f, glassTint); // Windshield
    DrawCube({ 0.0f, 0.52f, -0.35f }, 1.21f, 0.44f, 1.55f, glassTint); // Side windows
    DrawCube({ 0.0f, 0.74f, -0.35f }, 1.16f, 0.08f, 1.95f, whiteFuselage); // Cabin roof

    // 3. HIGH MAIN WINGS & STRUTS (Cessna 172 signature high wing)
    DrawCube({ 0.0f, 0.78f, -0.32f }, 10.90f, 0.14f, 1.48f, whiteFuselage);
    DrawCube({ 0.0f, 0.855f, -0.32f }, 10.60f, 0.015f, 0.32f, navyStripe); // Wing top stripe
    // Wing struts
    DrawCylinderEx({ -0.58f, -0.22f, -0.45f }, { -2.65f, 0.74f, -0.45f }, 0.03f, 0.03f, 8, whiteFuselage);
    DrawCylinderEx({  0.58f, -0.22f, -0.45f }, {  2.65f, 0.74f, -0.45f }, 0.03f, 0.03f, 8, whiteFuselage);

    // 4. ANIMATED FLAPS (Inboard trailing edge)
    float flapAngle = state.controls.currentFlapDeg;
    for (int side = -1; side <= 1; side += 2) {
        rlPushMatrix();
        rlTranslatef(side * 1.95f, 0.75f, 0.40f);
        rlRotatef(flapAngle, 1.0f, 0.0f, 0.0f);
        DrawCube({ 0.0f, 0.0f, 0.22f }, 2.20f, 0.06f, 0.42f, { 232, 236, 240, 255 });
        rlPopMatrix();
    }

    // 5. ANIMATED AILERONS (Outboard trailing edge)
    float aileronAngle = state.controls.yokeRoll * 20.0f;
    for (int side = -1; side <= 1; side += 2) {
        rlPushMatrix();
        rlTranslatef(side * 4.15f, 0.76f, 0.40f);
        rlRotatef(-side * aileronAngle, 1.0f, 0.0f, 0.0f);
        DrawCube({ 0.0f, 0.0f, 0.18f }, 2.10f, 0.05f, 0.35f, whiteFuselage);
        rlPopMatrix();
    }

    // 6. EMPENNAGE: HORIZONTAL STABILIZER & ANIMATED ELEVATOR
    DrawCube({ 0.0f, 0.36f, 3.75f }, 3.45f, 0.09f, 0.68f, whiteFuselage);
    rlPushMatrix();
    rlTranslatef(0.0f, 0.36f, 4.08f);
    rlRotatef(-state.controls.yokePitch * 22.0f, 1.0f, 0.0f, 0.0f);
    DrawCube({ 0.0f, 0.0f, 0.18f }, 3.40f, 0.06f, 0.34f, whiteFuselage);
    rlPopMatrix();

    // 7. VERTICAL STABILIZER & ANIMATED RUDDER
    DrawCube({ 0.0f, 1.08f, 3.65f }, 0.09f, 1.48f, 0.85f, whiteFuselage);
    DrawCube({ 0.0f, 1.72f, 3.75f }, 0.10f, 0.18f, 0.65f, navyStripe);
    rlPushMatrix();
    rlTranslatef(0.0f, 1.05f, 4.08f);
    rlRotatef(-state.controls.rudder * 24.0f, 0.0f, 1.0f, 0.0f);
    DrawCube({ 0.0f, 0.0f, 0.22f }, 0.07f, 1.42f, 0.42f, whiteFuselage);
    rlPopMatrix();

    // 8. LANDING GEAR & WHEELS (Retractable scale based on gearPosition)
    float gearScale = state.controls.gearPosition;
    if (gearScale > 0.05f) {
        rlPushMatrix();
        rlScalef(1.0f, gearScale, 1.0f);
        // Nose gear strut & wheel
        DrawCylinderEx({ 0.0f, -0.25f, -1.95f }, { 0.0f, -1.12f, -2.05f }, 0.04f, 0.035f, 8, darkMetal);
        rlPushMatrix();
        rlTranslatef(0.0f, -1.15f, -2.05f);
        rlRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        DrawCylinder({ 0.0f, -0.07f, 0.0f }, 0.18f, 0.18f, 0.14f, 12, tireBlack);
        rlPopMatrix();

        // Main landing gear struts & wheels
        for (int side = -1; side <= 1; side += 2) {
            DrawCylinderEx({ side * 0.45f, -0.25f, -0.25f }, { side * 1.25f, -1.10f, -0.25f }, 0.055f, 0.04f, 8, whiteFuselage);
            rlPushMatrix();
            rlTranslatef(side * 1.28f, -1.12f, -0.25f);
            rlRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            DrawCylinder({ 0.0f, -0.09f, 0.0f }, 0.22f, 0.22f, 0.18f, 14, tireBlack);
            rlPopMatrix();
        }
        rlPopMatrix();
    }

    // 9. AIRCRAFT EXTERIOR LIGHTING
    if (state.controls.navLights) {
        DrawSphere({ -5.48f, 0.78f, -0.32f }, 0.07f, RED);   // Port red
        DrawSphere({  5.48f, 0.78f, -0.32f }, 0.07f, GREEN); // Starboard green
        DrawSphere({  0.0f,  0.36f,  4.35f }, 0.06f, WHITE); // Tail white
    }
    double t = GetTime();
    if (state.controls.beaconLights && std::fmod(t, 1.1) < 0.22) {
        DrawSphere({ 0.0f, 1.84f, 3.85f }, 0.09f, RED);
    }
    if (state.controls.strobeLights && std::fmod(t, 1.4) < 0.12) {
        DrawSphere({ -5.52f, 0.78f, -0.20f }, 0.11f, WHITE);
        DrawSphere({  5.52f, 0.78f, -0.20f }, 0.11f, WHITE);
    }
    if (state.controls.landingLights) {
        DrawSphere({ -2.45f, 0.76f, -1.06f }, 0.09f, { 255, 250, 220, 255 });
    }

    // 10. SPINNING PROPELLER
    DrawPropellerBlur(state);

    rlPopMatrix();
}

void AircraftRenderer::DrawPropellerBlur(const AircraftState& state) {
    rlPushMatrix();
    rlTranslatef(0.0f, 0.08f, -2.72f);
    rlRotatef(state.propAngleDeg, 0.0f, 0.0f, 1.0f);

    if (state.engine.rpm < 450.0f) {
        // Distinct blades visible at low RPM
        DrawCube({ 0.0f, 0.0f, 0.0f }, 1.92f, 0.14f, 0.03f, { 35, 38, 42, 255 });
        DrawCube({ 0.92f, 0.0f, 0.0f }, 0.08f, 0.14f, 0.032f, YELLOW);
        DrawCube({ -0.92f, 0.0f, 0.0f }, 0.08f, 0.14f, 0.032f, YELLOW);
    } else {
        // Semi-transparent motion blur disc at high RPM
        rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        DrawCylinder({ 0.0f, -0.01f, 0.0f }, 0.96f, 0.96f, 0.02f, 24, { 60, 65, 72, 75 });
    }
    rlPopMatrix();
}