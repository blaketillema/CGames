#include "paddle.h"

Paddle CreatePaddle(b2WorldId worldId, int x, int y, Texture2D* texture){
    Paddle paddle;
    paddle.controls.up = 0;
    paddle.controls.down = 0;
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.x = x;
    bodyDef.position.y = y;
    paddle.bodyId = b2CreateBody(worldId, &bodyDef);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2Capsule capsule;
    capsule.center1 = (b2Vec2){0.0f, -texture->height / 2.0f + texture->width / 2.0f};
    capsule.center2 = (b2Vec2){0.0f, texture->height / 2.0f - texture->width / 2.0f};
    capsule.radius = texture->width / 2.0f;
    paddle.shapeId = b2CreateCapsuleShape(paddle.bodyId, &shapeDef, &capsule);
    paddle.texture = texture;
    return paddle;
}

void UpdatePaddle(Paddle* paddle, int lowerBound, int upperBound){
    b2Vec2 pos = b2Body_GetPosition(paddle->bodyId);
    b2Vec2 vel = b2Body_GetLinearVelocity(paddle->bodyId);

    float speed = 500.0f;
    if(IsKeyDown(paddle->controls.up)) vel.y = -speed;
    else if(IsKeyDown(paddle->controls.down)) vel.y = speed;
    else vel.y = 0.0f;

    float heightOffset = paddle->texture->height / 2.0f;
    if(pos.y - heightOffset < lowerBound){
        pos.y = lowerBound + heightOffset;
        b2Body_SetTransform(paddle->bodyId, pos, b2Body_GetRotation(paddle->bodyId));
        b2Body_SetLinearVelocity(paddle->bodyId, b2Vec2_zero);
    }
    else if (pos.y + heightOffset > upperBound) {
        pos.y = upperBound - heightOffset;
        b2Body_SetTransform(paddle->bodyId, pos, b2Body_GetRotation(paddle->bodyId));
        b2Body_SetLinearVelocity(paddle->bodyId, b2Vec2_zero);
    }
    else {
        b2Body_SetLinearVelocity(paddle->bodyId, vel);
    }
}

void DrawPaddle(Paddle* paddle){
    b2Vec2 position = b2Body_GetPosition(paddle->bodyId);
    b2Rot rotation = b2Body_GetRotation(paddle->bodyId);
    float radians = b2Rot_GetAngle(rotation);
    float widthOffset = paddle->texture->width / 2.0f;
    float heightOffset = paddle->texture->height / 2.0f;
    DrawTexture((*paddle->texture), position.x - widthOffset, position.y - heightOffset, WHITE);
}