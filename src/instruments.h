#pragma once
#include "sim_types.h"

class Instruments3D {
public:
    static void DrawPanelGauges(const AircraftState& state);

private:
    static void DrawDialFace(Vector3 pos, float radius, Color bg);
    static void DrawNeedle(Vector3 pos, float radius, float angleDeg, Color color, float width = 0.004f);
    static void DrawAirspeedIndicator(Vector3 pos, float radius, float iasKts);
    static void DrawArtificialHorizon(Vector3 pos, float radius, float pitchRad, float rollRad);
    static void DrawAltimeter(Vector3 pos, float radius, float altFt);
    static void DrawTurnCoordinator(Vector3 pos, float radius, float yawRate, float slipDeg);
    static void DrawHeadingIndicator(Vector3 pos, float radius, float headingDeg);
    static void DrawVerticalSpeed(Vector3 pos, float radius, float vsFpm);
    static void DrawRpmGauge(Vector3 pos, float radius, float rpm);
    static void DrawEngineCluster(Vector3 pos, const EngineState& eng);
};