#pragma once
#include "sim_types.h"

class CockpitRenderer {
public:
    static void DrawInterior(const AircraftState& state);
    static Vector3 GetPilotEyeLocalOffset();
    static bool HandleTouchInteraction(const Camera3D& camera, AircraftState& state, Vector2 touchPos);
};