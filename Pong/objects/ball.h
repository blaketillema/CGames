#pragma once

#include <box2d/box2d.h>
#include <raylib.h>

typedef struct Ball {
    b2BodyId bodyId;
    b2ShapeId shapeId;
    Texture* texture;
} Ball;

Ball CreateBall(b2WorldId worldId, Texture2D* texture);
void DrawBall(Ball* ball);
void ResetBall(Ball* ball, int x, int y);
void LaunchBall(Ball* ball);
