#pragma once

#include <box2d/box2d.h>

typedef struct Wall {
    b2BodyId bodyId;
    b2ShapeId shapeId;
} Wall;

Wall CreateWall(b2WorldId worldId, int x, int y, int width, int height, bool isSensor);
