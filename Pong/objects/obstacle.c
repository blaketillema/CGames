#include "obstacle.h"

Obstacle CreateObstacle(b2WorldId worldId, int spawnX, int spawnY, int minSize, int maxSize, int minSpeed, int maxSpeed){
    // obby
    Obstacle obstacle;
    obstacle.spawnX = spawnX;
    obstacle.spawnY = spawnY;

    // body
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.x = spawnX;
    bodyDef.position.y = spawnY;
    obstacle.bodyId = b2CreateBody(worldId, &bodyDef);

    // shape stuff
    // b2Polygon polygon = b2MakeBox(obstacle.size / 2, obstacle.size / 2);
    // obstacle.shapeId = b2CreatePolygonShape(obstacle.bodyId, &shapeDef, &polygon);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    obstacle.size = GetRandomValue(minSize, maxSize);
    b2Circle circle;
    circle.center = b2Vec2_zero;
    circle.radius = obstacle.size / 2.f;
    obstacle.shapeId = b2CreateCircleShape(obstacle.bodyId, &shapeDef, &circle);

    // velocity
    b2Vec2 vel = b2Vec2_zero;
    vel.y = GetRandomValue(minSpeed, maxSpeed);
    b2Body_SetLinearVelocity(obstacle.bodyId, vel);
    return obstacle;
}

void DrawObstacle(Obstacle* obstacle, Color color){
    b2Vec2 pos = b2Body_GetPosition(obstacle->bodyId);
    DrawCircle(pos.x, pos.y, obstacle->size / 2.f, color);
    // DrawRectangle(pos.x - obstacle->size / 2, pos.y - obstacle->size / 2, obstacle->size, obstacle->size, color);
}

void ResetObstacle(Obstacle* obstacle, int where){
    b2Vec2 pos = b2Body_GetPosition(obstacle->bodyId);
    if(where == TOP) pos.y = -obstacle->size*2;
    if(where == BOTTOM) obstacle->spawnY;
    b2Body_SetTransform(obstacle->bodyId, pos, b2Body_GetRotation(obstacle->bodyId));
    // b2Vec2 vel = b2Body_GetLinearVelocity(obstacle->bodyId);
    // vel.y += vel.y * 1.1f;
    // b2Body_SetLinearVelocity(obstacle->bodyId, vel);
}