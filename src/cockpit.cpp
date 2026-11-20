#include "cockpit.h"
#include "instruments.h"
#include "rlgl.h"

Vector3 CockpitRenderer::GetPilotEyeLocalOffset() {
    return { -0.26f, 0.56f, -0.42f };
}

void CockpitRenderer::DrawInterior(const AircraftState& state) {
    rlPushMatrix();
    rlTranslatef(state.position.x, state.position.y, state.position.z);

    Vector3 axis{};
    float angle = 0.0f;
    QuaternionToAxisAngle(state.orientation, &axis, &angle);
    rlRotatef(angle * RAD2DEG, axis.x, axis.y, axis.z);

    Color dashGrey = { 46, 50, 56, 255 };
    Color glareshield = { 24, 26, 30, 255 };
    Color trimBeige = { 188, 180, 165, 255 };

    DrawCube({ 0.0f, 0.34f, -1.02f }, 1.12f, 0.32f, 0.14f, dashGrey);
    DrawCube({ 0.0f, 0.51f, -1.04f }, 1.14f, 0.03f, 0.26f, glareshield);

    DrawCylinderEx({ -0.54f, 0.48f, -1.02f }, { -0.48f, 0.74f, -0.78f }, 0.022f, 0.022f, 8, trimBeige);
    DrawCylinderEx({  0.54f, 0.48f, -1.02f }, {  0.48f, 0.74f, -0.78f }, 0.022f, 0.022f, 8, trimBeige);
    DrawCylinderEx({  0.00f, 0.51f, -1.08f }, {  0.00f, 0.74f, -0.82f }, 0.014f, 0.014f, 8, glareshield);

    float yokeZ = -0.82f + state.controls.yokePitch * 0.08f;
    DrawCylinderEx({ -0.26f, 0.28f, -0.96f }, { -0.26f, 0.28f, yokeZ }, 0.018f, 0.018f, 10, { 30, 32, 36, 255 });
    rlPushMatrix();
    rlTranslatef(-0.26f, 0.28f, yokeZ);
    rlRotatef(-state.controls.yokeRoll * 45.0f, 0.0f, 0.0f, 1.0f);
    DrawCube({ 0.0f, 0.0f, 0.0f }, 0.24f, 0.032f, 0.025f, { 22, 24, 28, 255 });
    DrawCube({ -0.11f, 0.045f, 0.0f }, 0.028f, 0.11f, 0.025f, { 22, 24, 28, 255 });
    DrawCube({  0.11f, 0.045f, 0.0f }, 0.028f, 0.11f, 0.025f, { 22, 24, 28, 255 });
    rlPopMatrix();

    float throttleZ = -0.88f - state.controls.throttle * 0.07f;
    DrawCylinderEx({ -0.02f, 0.22f, -0.96f }, { -0.02f, 0.22f, throttleZ }, 0.008f, 0.008f, 8, LIGHTGRAY);
    DrawSphere({ -0.02f, 0.22f, throttleZ }, 0.018f, BLACK);

    float mixtureZ = -0.88f - state.controls.mixture * 0.07f;
    DrawCylinderEx({ 0.04f, 0.22f, -0.96f }, { 0.04f, 0.22f, mixtureZ }, 0.008f, 0.008f, 8, LIGHTGRAY);
    DrawSphere({ 0.04f, 0.22f, mixtureZ }, 0.018f, RED);

    Color battColor = state.controls.masterBattery ? RED : DARKGRAY;
    DrawCube({ -0.45f, 0.21f, -0.95f }, 0.035f, 0.045f, 0.02f, battColor);

    Color navColor = state.controls.navLights ? GREEN : DARKGRAY;
    DrawCube({ -0.39f, 0.21f, -0.95f }, 0.025f, 0.035f, 0.02f, navColor);

    Color landColor = state.controls.landingLights ? YELLOW : DARKGRAY;
    DrawCube({ -0.35f, 0.21f, -0.95f }, 0.025f, 0.035f, 0.02f, landColor);

    Instruments3D::DrawPanelGauges(state);

    rlPopMatrix();
}

bool CockpitRenderer::HandleTouchInteraction(const Camera3D& camera, AircraftState& state, Vector2 touchPos) {
    Ray ray = GetMouseRay(touchPos, camera);
    auto checkSwitch = [&](Vector3 localPos, float radius) -> bool {
        Vector3 worldPos = Vector3Add(state.position, Vector3RotateByQuaternion(localPos, state.orientation));
        RayCollision col = GetRayCollisionSphere(ray, worldPos, radius);
        return col.hit;
    };

    if (checkSwitch({ -0.45f, 0.21f, -0.95f }, 0.045f)) {
        state.controls.masterBattery = !state.controls.masterBattery;
        return true;
    }
    if (checkSwitch({ -0.39f, 0.21f, -0.95f }, 0.040f)) {
        state.controls.navLights = !state.controls.navLights;
        state.controls.beaconLights = state.controls.navLights;
        return true;
    }
    if (checkSwitch({ -0.35f, 0.21f, -0.95f }, 0.040f)) {
        state.controls.landingLights = !state.controls.landingLights;
        state.controls.strobeLights = state.controls.landingLights;
        return true;
    }
    return false;
}