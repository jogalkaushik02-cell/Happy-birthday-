cmake_minimum_required(VERSION 3.16)
project(Cessna172FlightSim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
)
set(PLATFORM "Web" CACHE STRING "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

file(GLOB SOURCES "src/*.cpp")
add_executable(flightsim ${SOURCES})

target_link_libraries(flightsim PRIVATE raylib)

set_target_properties(flightsim PROPERTIES
    SUFFIX ".js"
    LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1 -s INITIAL_MEMORY=134217728 -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s EXPORTED_FUNCTIONS=['_main','_SetSimMode','_SetGraphicsQuality','_ResetFlight','_TogglePause'] --shell-file ${CMAKE_SOURCE_DIR}/web/index.html -O2"
)
