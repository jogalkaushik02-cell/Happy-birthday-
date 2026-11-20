#pragma once
#include "sim_types.h"

class AircraftRenderer {
public:
    static void Init();
    static void Unload();
    static void DrawExternal(const AircraftState& state, GraphicsQuality quality);
    static void DrawPropellerBlur(const AircraftState& state);
};