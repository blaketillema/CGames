// #define SCALE 10

#include <box2d/box2d.h>
#include <raylib.h>
#include <rlgl.h>

#include "objects/bird.h"

#define RENDER_W 640
#define RENDER_H 960
#define WIDTH 960
#define HEIGHT 640

int main(){

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.y = 980.f;
    b2SetLengthUnitsPerMeter(100.f);
    b2WorldId worldId = b2CreateWorld(&worldDef);

    InitWindow(RENDER_W, RENDER_H, "FlappyBird");
    SetTargetFPS(120);

    const int halfWidth = WIDTH / 2;
    const int quarterWidth = WIDTH / 4;
    const int halfHeight = HEIGHT / 2;

    Bird bird = Bird_Create(worldId, (b2Vec2){quarterWidth, halfHeight}, KEY_SPACE);

    bool paused = false;

    while (!WindowShouldClose())
    {
        if(paused == false){
            Bird_Process(&bird);
            b2World_Step(worldId, GetFrameTime(), 4);
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(0, 0);
        Bird_Draw(&bird);
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}