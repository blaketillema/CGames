#include "bird.h"
#include <stdio.h>

Bird Bird_Create(b2WorldId worldId, b2Vec2 spawn, int jumpKey){
    // a well-a everybody's heard about the bird
    Bird bird;

    //// box2d
    // body
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position.x = spawn.x;
    bodyDef.position.y = spawn.y;
    bodyDef.type = b2_dynamicBody;
    bird.bodyId = b2CreateBody(worldId, &bodyDef);
    // shape
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2Circle circle;
    circle.radius = BIRD_RADIUS;
    circle.center = b2Vec2_zero;
    bird.shapeId = b2CreateCircleShape(bird.bodyId, &shapeDef, &circle);

    // reset and input stuff
    bird.spawn = spawn;
    bird.jumpKey = jumpKey;
    return bird;
}

void Bird_Reset(Bird* bird){
    b2Body_SetTransform(bird->bodyId, bird->spawn, b2Body_GetRotation(bird->bodyId));
}

void Bird_Draw(Bird* bird){
    b2Vec2 pos = b2Body_GetPosition(bird->bodyId);
    DrawCircle(pos.x, pos.y, BIRD_RADIUS, WHITE);
    b2Vec2 vel = b2Body_GetLinearVelocity(bird->bodyId);
    printf("%p -> %f, %f\n", bird, vel.x, vel.y);
}

void Bird_Process(Bird* bird){
    b2Vec2 vel = b2Body_GetLinearVelocity(bird->bodyId);
    if(IsKeyPressed(bird->jumpKey)) {
        b2Vec2 grav = b2World_GetGravity(b2Body_GetWorld(bird->bodyId));
        vel.y = grav.y * -10.f;
    }
    b2Body_SetLinearVelocity(bird->bodyId, vel);
}