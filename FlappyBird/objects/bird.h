#pragma once

#include <box2d/box2d.h>
#include <raylib.h>

#define BIRD_RADIUS 20

typedef struct Bird {
    b2BodyId bodyId;
    b2ShapeId shapeId;
    b2Vec2 spawn;
    int jumpKey;
} Bird;

Bird Bird_Create(b2WorldId worldId, b2Vec2 spawn, int jumpKey);
void Bird_Reset(Bird* bird);
void Bird_Draw(Bird* bird);
void Bird_Process(Bird* bird);
