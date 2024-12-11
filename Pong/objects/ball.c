#include <limits.h>

#include "ball.h"

Ball CreateBall(b2WorldId worldId, Texture2D* texture){
    Ball ball;
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.isBullet = true;
    bodyDef.type = b2_dynamicBody;
    ball.bodyId = b2CreateBody(worldId, &bodyDef);
    LaunchBall(&ball);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.restitution = 1.0f;
    shapeDef.friction = 0.0f;
    b2Circle circle;
    circle.center = b2Vec2_zero;
    circle.radius = texture->width/2.0f;
    ball.shapeId = b2CreateCircleShape(ball.bodyId, &shapeDef, &circle);
    ball.texture = texture;
    return ball;
}

void DrawBall(Ball* ball){
    b2Vec2 position = b2Body_GetPosition(ball->bodyId);
    b2Rot rotation = b2Body_GetRotation(ball->bodyId);
    float radians = b2Rot_GetAngle(rotation);
    float widthOffset = ball->texture->width / 2.0f;
    float heightOffset = ball->texture->height / 2.0f;
    DrawTexture((*ball->texture), position.x - widthOffset, position.y - heightOffset, WHITE);
}

void ResetBall(Ball* ball, int x, int y){
    b2Rot rot;
    b2Body_SetTransform(ball->bodyId, (b2Vec2){x, y}, b2Body_GetRotation(ball->bodyId));
    LaunchBall(ball);
}

void LaunchBall(Ball* ball){
    b2Vec2 vel = b2Vec2_zero;
    float rad = (float)GetRandomValue(0, INT_MAX) / (float)INT_MAX * PI / 4.0f;
    vel.x = cos(rad) * 300.f * 1000.f;
    vel.y = sin(rad) * 300.f * 1000.f;
    if(GetRandomValue(0, 1) == 1) vel.x = -vel.x;
    if(GetRandomValue(0, 1) == 1) vel.y = -vel.y;
    b2Body_SetLinearVelocity(ball->bodyId, b2Vec2_zero);
    b2Body_ApplyLinearImpulseToCenter(ball->bodyId, vel, true);
}