#include "instruments.h"
#include "rlgl.h"
#include <algorithm>
#include <cmath>

void Instruments3D::DrawDialFace(Vector3 pos, float radius, Color bg) {
    rlPushMatrix();
    rlTranslatef(pos.x, pos.y, pos.z);
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    DrawCylinder({ 0.0f, -0.004f, 0.0f }, radius * 1.08f, radius * 1.08f, 0.008f, 24, { 28, 30, 34, 255 });
    DrawCylinder({ 0.0f, 0.001f, 0.0f }, radius, radius, 0.004f, 24, bg);
    rlPopMatrix();
}

void Instruments3D::DrawNeedle(Vector3 pos, float radius, float angleDeg, Color color, float width) {
    rlPushMatrix();
    rlTranslatef(pos.x, pos.y, pos.z + 0.005f);
    rlRotatef(-angleDeg, 0.0f, 0.0f, 1.0f);
    DrawCube({ 0.0f, radius * 0.36f, 0.0f }, width, radius * 0.78f, 0.002f, color);
    DrawSphere({ 0.0f, 0.0f, 0.001f }, width * 1.8f, { 45, 45, 48, 255 });
    rlPopMatrix();
}

void Instruments3D::DrawAirspeedIndicator(Vector3 pos, float radius, float iasKts) {
    DrawDialFace(pos, radius, { 18, 20, 24, 255 });
    for (int i = 0; i < 16; ++i) {
        float a = 20.0f + i * 20.0f;
        float rad = a * DEG2RAD;
        Vector3 tick = { pos.x + std::sin(rad) * radius * 0.82f, pos.y + std::cos(rad) * radius * 0.82f, pos.z + 0.004f };
        Color c = (i >= 3 && i <= 11) ? GREEN : ((i > 12) ? RED : WHITE);
        DrawSphere(tick, 0.0025f, c);
    }
    float clamped = std::clamp(iasKts, 0.0f, 180.0f);
    float angle = (clamped / 180.0f) * 320.0f + 15.0f;
    DrawNeedle(pos, radius, angle, WHITE);
}

void Instruments3D::DrawArtificialHorizon(Vector3 pos, float radius, float pitchRad, float rollRad) {
    DrawDialFace(pos, radius, { 35, 110, 185, 255 });
    rlPushMatrix();
    rlTranslatef(pos.x, pos.y, pos.z + 0.004f);
    rlRotatef(rollRad * RAD2DEG, 0.0f, 0.0f, 1.0f);

    float pitchOffset = std::clamp(pitchRad * RAD2DEG, -30.0f, 30.0f) * (radius / 45.0f);
    DrawCube({ 0.0f, -radius * 0.42f - pitchOffset, 0.001f }, radius * 1.45f, radius * 0.84f, 0.002f, { 138, 86, 46, 255 });
    DrawCube({ 0.0f, -pitchOffset, 0.002f }, radius * 1.55f, 0.003f, 0.002f, WHITE);
    rlPopMatrix();

    DrawCube({ pos.x, pos.y, pos.z + 0.008f }, radius * 0.75f, 0.004f, 0.002f, YELLOW);
}

void Instruments3D::DrawAltimeter(Vector3 pos, float radius, float altFt) {
    DrawDialFace(pos, radius, { 18, 20, 24, 255 });
    for (int i = 0; i < 10; ++i) {
        float rad = (i * 36.0f) * DEG2RAD;
        Vector3 tick = { pos.x + std::sin(rad) * radius * 0.82f, pos.y + std::cos(rad) * radius * 0.82f, pos.z + 0.004f };
        DrawSphere(tick, 0.0025f, WHITE);
    }
    float hundredsAngle = std::fmod(altFt, 1000.0f) / 1000.0f * 360.0f;
    float thousandsAngle = std::fmod(altFt, 10000.0f) / 10000.0f * 360.0f;
    DrawNeedle(pos, radius * 0.62f, thousandsAngle, { 210, 210, 215, 255 }, 0.006f);
    DrawNeedle(pos, radius * 0.95f, hundredsAngle, WHITE, 0.0035f);
}

void Instruments3D::DrawTurnCoordinator(Vector3 pos, float radius, float yawRate, float slipDeg) {
    DrawDialFace(pos, radius, { 18, 20, 24, 255 });
    rlPushMatrix();
    rlTranslatef(pos.x, pos.y, pos.z + 0.005f);
    float bankGraphic = std::clamp(-yawRate * 120.0f, -35.0f, 35.0f);
    rlRotatef(bankGraphic, 0.0f, 0.0f, 1.0f);
    DrawCube({ 0.0f, 0.0f, 0.0f }, radius * 1.25f, 0.004f, 0.002f, WHITE);
    DrawCube({ 0.0f, 0.012f, 0.0f }, 0.004f, 0.022f, 0.002f, WHITE);
    rlPopMatrix();

    float ballOffset = std::clamp(slipDeg * 0.0018f, -radius * 0.45f, radius * 0.45f);
    DrawSphere({ pos.x + ballOffset, pos.y - radius * 0.55f, pos.z + 0.006f }, 0.0055f, WHITE);
}

void Instruments3D::DrawHeadingIndicator(Vector3 pos, float radius, float headingDeg) {
    DrawDialFace(pos, radius, { 18, 20, 24, 255 });
    rlPushMatrix();
    rlTranslatef(pos.x, pos.y, pos.z + 0.004f);
    rlRotatef(headingDeg, 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < 12; ++i) {
        float rad = (i * 30.0f) * DEG2RAD;
        Color c = (i == 0) ? RED : WHITE;
        DrawSphere({ std::sin(rad) * radius * 0.78f, std::cos(rad) * radius * 0.78f, 0.0f }, 0.003f, c);
    }
    rlPopMatrix();
    DrawCube({ pos.x, pos.y + radius * 0.85f, pos.z + 0.007f }, 0.004f, 0.014f, 0.002f, ORANGE);
}

void Instruments3D::DrawVerticalSpeed(Vector3 pos, float radius, float vsFpm) {
    DrawDialFace(pos, radius, { 18, 20, 24, 255 });
    float clamped = std::clamp(vsFpm, -2000.0f, 2000.0f);
    float angle = -90.0f + (clamped / 2000.0f) * 165.0f;
    DrawNeedle(pos, radius, angle, WHITE);
}

void Instruments3D::DrawRpmGauge(Vector3 pos, float radius, float rpm) {
    DrawDialFace(pos, radius, { 18, 20, 24, 255 });
    float clamped = std::clamp(rpm, 0.0f, 3000.0f);
    float angle = -130.0f + (clamped / 3000.0f) * 260.0f;
    Color c = (rpm > 2650.0f) ? RED : GREEN;
    DrawNeedle(pos, radius, angle, c);
}

void Instruments3D::DrawEngineCluster(Vector3 pos, const EngineState& eng) {
    float r = 0.036f;
    DrawDialFace({ pos.x - 0.045f, pos.y + 0.045f, pos.z }, r, { 20, 22, 26, 255 });
    DrawNeedle({ pos.x - 0.045f, pos.y + 0.045f, pos.z }, r, -60.0f + (eng.fuelGallons / 53.0f) * 120.0f, YELLOW, 0.003f);

    DrawDialFace({ pos.x + 0.045f, pos.y + 0.045f, pos.z }, r, { 20, 22, 26, 255 });
    DrawNeedle({ pos.x + 0.045f, pos.y + 0.045f, pos.z }, r, -60.0f + ((eng.oilTempF - 70.0f) / 180.0f) * 120.0f, GREEN, 0.003f);

    DrawDialFace({ pos.x - 0.045f, pos.y - 0.045f, pos.z }, r, { 20, 22, 26, 255 });
    DrawNeedle({ pos.x - 0.045f, pos.y - 0.045f, pos.z }, r, -60.0f + (eng.oilPressurePsi / 100.0f) * 120.0f, GREEN, 0.003f);
}

void Instruments3D::DrawPanelGauges(const AircraftState& state) {
    float z = -0.96f;
    float r = 0.056f;

    DrawAirspeedIndicator({ -0.40f, 0.42f, z }, r, state.indicatedAirspeedKts);
    DrawArtificialHorizon({ -0.26f, 0.42f, z }, r, state.pitch, state.roll);
    DrawAltimeter({ -0.12f, 0.42f, z }, r, state.altitudeFt);

    DrawTurnCoordinator({ -0.40f, 0.29f, z }, r, state.angularVelocity.y, state.sideslipDeg);
    DrawHeadingIndicator({ -0.26f, 0.29f, z }, r, state.headingDeg);
    DrawVerticalSpeed({ -0.12f, 0.29f, z }, r, state.verticalSpeedFpm);

    DrawRpmGauge({ 0.24f, 0.30f, z }, r, state.engine.rpm);
    DrawEngineCluster({ 0.06f, 0.36f, z }, state.engine);
}