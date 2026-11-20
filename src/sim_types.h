#pragma once
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <string>
#include <vector>

enum class FlightMode {
    COLD_AND_DARK = 0,
    READY_FOR_TAXI,
    TAKEOFF,
    FREE_FLIGHT,
    LANDING_PRACTICE
};

enum class CameraMode {
    COCKPIT = 0,
    CHASE,
    SIDE,
    FRONT,
    FREE
};

enum class GraphicsQuality {
    LOW = 0,
    MEDIUM,
    HIGH
};

enum class FuelSelector {
    OFF = 0,
    LEFT,
    BOTH,
    RIGHT
};

struct ControlInputs {
    float yokePitch = 0.0f;      // -1.0 (push down) to +1.0 (pull up)
    float yokeRoll = 0.0f;       // -1.0 (left) to +1.0 (right)
    float rudder = 0.0f;         // -1.0 (left) to +1.0 (right)
    float elevatorTrim = 0.0f;   // -1.0 to +1.0
    float throttle = 0.0f;       // 0.0 to 1.0
    float mixture = 1.0f;        // 0.0 (cutoff) to 1.0 (rich)
    int flapStep = 0;            // 0 (0 deg), 1 (10 deg), 2 (20 deg), 3 (30 deg)
    float currentFlapDeg = 0.0f; // Smoothly interpolated flap angle
    bool gearDown = true;        // Cessna 172RG retractable option simulated
    float gearPosition = 1.0f;   // 0.0 (retracted) to 1.0 (extended)
    bool wheelBrakes = false;
    bool parkingBrake = true;
    int ignitionKey = 0;         // 0:OFF, 1:R, 2:L, 3:BOTH, 4:START
    FuelSelector fuelSelector = FuelSelector::BOTH;
    bool pitotHeat = false;
    bool navLights = false;
    bool beaconLights = false;
    bool strobeLights = false;
    bool landingLights = false;
    bool masterBattery = false;
};

struct EngineState {
    bool running = false;
    float rpm = 0.0f;            // 0 to 2700 RPM
    float oilTempF = 75.0f;      // Fahrenheit (normal 100-245)
    float oilPressurePsi = 0.0f; // PSI (normal 50-90)
    float egtF = 75.0f;          // Exhaust gas temp
    float fuelGallons = 48.0f;   // Max 53 gal usable (C172S)
    float fuelFlowGph = 0.0f;
    float starterTimer = 0.0f;
};

struct AircraftState {
    // World coordinates (Y is Up, -Z is North/Runway heading 360/0)
    Vector3 position = { 0.0f, 1.45f, 950.0f };
    Vector3 velocity = { 0.0f, 0.0f, 0.0f };
    Vector3 acceleration = { 0.0f, 0.0f, 0.0f };

    // Orientation quaternion & Euler angles (radians)
    Quaternion orientation = QuaternionIdentity();
    float pitch = 0.0f;          // Nose up positive
    float roll = 0.0f;           // Right wing down positive
    float yaw = 0.0f;            // Heading clockwise from North

    // Angular velocity in body frame (rad/s): x=pitchRate, y=yawRate, z=rollRate
    Vector3 angularVelocity = { 0.0f, 0.0f, 0.0f };

    // Aerodynamic parameters
    float trueAirspeedMps = 0.0f;
    float indicatedAirspeedKts = 0.0f;
    float groundSpeedKts = 0.0f;
    float altitudeFt = 0.0f;
    float verticalSpeedFpm = 0.0f;
    float headingDeg = 0.0f;
    float angleOfAttackDeg = 0.0f;
    float sideslipDeg = 0.0f;
    float gForce = 1.0f;
    float dynamicPressure = 0.0f;

    // Ground & Contact states
    bool onGround = true;
    bool noseWheelContact = true;
    bool leftMainContact = true;
    bool rightMainContact = true;
    float gearCompression = 0.05f;
    float propAngleDeg = 0.0f;

    // Warnings
    bool stallWarning = false;
    bool lowAirspeedWarning = false;
    bool overspeedWarning = false;
    bool lowFuelWarning = false;
    bool engineFailureWarning = false;
    bool gearWarning = false;
    float structuralStress = 0.0f;
    float touchdownBounceTimer = 0.0f;

    ControlInputs controls;
    EngineState engine;
};

struct EnvironmentState {
    Vector3 windVelocityMps = { 1.8f, 0.0f, 1.2f }; // Light crosswind/headwind
    float airDensity = 1.225f; // kg/m^3 at sea level
    float outsideAirTempC = 18.0f;
    float sunElevationDeg = 42.0f;
    float sunAzimuthDeg = 135.0f;
    float timeOfDay = 10.5f;
};

struct SimSettings {
    GraphicsQuality quality = GraphicsQuality::HIGH;
    float uiScale = 1.0f;
    bool hudCollapsed = false;
    bool soundEnabled = true;
    float masterVolume = 0.8f;
    bool paused = false;
    FlightMode currentMode = FlightMode::TAKEOFF;
    CameraMode cameraMode = CameraMode::COCKPIT;
};