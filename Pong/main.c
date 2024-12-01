#include <box2d/collision.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <raylib.h>
#include <box2d/box2d.h>
#include <limits.h>
#include <stdio.h>

#define WIDTH 960
#define HEIGHT 640

struct Object {
    b2BodyId bodyId;
    b2ShapeId shapeId;
    Texture2D* texture;
} Object;

struct Object CreateBall(b2WorldId worldId, int x, int y, Texture2D* texture){
    struct Object ball;
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.isBullet = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.x = x;
    bodyDef.position.y = y;
    float radians = (float)GetRandomValue(0, INT_MAX) / (float)INT_MAX * PI / 4.0f;
    bodyDef.linearVelocity.x = cos(radians) * 200.0f;
    bodyDef.linearVelocity.y = sin(radians) * 200.0f;
    // flip x and y velocity
    if(GetRandomValue(0, 1) == 1) bodyDef.linearVelocity.x = -bodyDef.linearVelocity.x;
    if(GetRandomValue(0, 1) == 1) bodyDef.linearVelocity.y = -bodyDef.linearVelocity.y;
    ball.bodyId = b2CreateBody(worldId, &bodyDef);
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

struct Object CreatePaddle(b2WorldId worldId, int x, int y, Texture2D* texture){
    struct Object paddle;
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

struct Object CreateWall(b2WorldId worldId, int x, int y, int width, int height, bool isSensor){
    struct Object wall;
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.x = x;
    bodyDef.position.y = y;
    wall.bodyId = b2CreateBody(worldId, &bodyDef);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = isSensor;
    b2Polygon polygon = b2MakeBox(width/2.0f, height/2.0f);
    wall.shapeId = b2CreatePolygonShape(wall.bodyId, &shapeDef, &polygon);
    return wall;
}

void UpdatePaddle(struct Object* paddle, int upKey, int downKey, float dt){
    b2Vec2 pos = b2Body_GetPosition(paddle->bodyId);
    b2Vec2 vel = b2Body_GetLinearVelocity(paddle->bodyId);

    float speed = 500.0f;
    if(IsKeyDown(upKey)) vel.y = -speed;
    else if(IsKeyDown(downKey)) vel.y = speed;
    else vel.y = 0.0f;

    float heightOffset = paddle->texture->height / 2.0f;
    if(pos.y - heightOffset < 0){
        pos.y = heightOffset;
        b2Body_SetTransform(paddle->bodyId, pos, b2Body_GetRotation(paddle->bodyId));
        b2Body_SetLinearVelocity(paddle->bodyId, b2Vec2_zero);
    }
    else if (pos.y + heightOffset > HEIGHT) {
        pos.y = HEIGHT - heightOffset;
        b2Body_SetTransform(paddle->bodyId, pos, b2Body_GetRotation(paddle->bodyId));
        b2Body_SetLinearVelocity(paddle->bodyId, b2Vec2_zero);
    }
    else {
        b2Body_SetLinearVelocity(paddle->bodyId, vel);
    }
}

void ProcessInput(struct Object* leftPaddle, struct Object* rightPaddle, float dt){
    // right paddle
    UpdatePaddle(rightPaddle, KEY_UP, KEY_DOWN, dt);
    // left paddle
    UpdatePaddle(leftPaddle, KEY_W, KEY_S, dt);
}

bool CheckGoal(b2WorldId worldId, struct Object* wall, struct Object* ball){
    b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);
    for(int i = 0; i < sensorEvents.beginCount; i++){
        b2SensorBeginTouchEvent* event = sensorEvents.beginEvents + i;
        if(B2_ID_EQUALS(event->sensorShapeId, wall->shapeId) && B2_ID_EQUALS(event->visitorShapeId, ball->shapeId)) return true;
    }
    return false;
}

void DrawObject(struct Object* obj){
    b2Vec2 position = b2Body_GetPosition(obj->bodyId);
    b2Rot rotation = b2Body_GetRotation(obj->bodyId);
    float radians = b2Rot_GetAngle(rotation);
    float widthOffset = (*obj->texture).width / 2.0f;
    float heightOffset = (*obj->texture).height / 2.0f;
    DrawTexture((*obj->texture), position.x - widthOffset, position.y - heightOffset, WHITE);
}

int main(){

    // colours
    const Color DARKER = (Color){15, 56, 15, 255};
    const Color DARK = (Color){48, 98, 48, 255};
    const Color LIGHT = (Color){139, 172, 15, 255};
    const Color LIGHTER = (Color){155, 188, 15, 255};

    InitWindow(WIDTH, HEIGHT, "Pong");

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2_zero;
    b2WorldId worldId = b2CreateWorld(&worldDef);
    Texture2D ballTexture = LoadTexture("ball.png");
    Texture2D paddleTexture = LoadTexture("paddle.png");

    const int halfWidth = WIDTH / 2, halfHeight = HEIGHT / 2;
    struct Object ball = CreateBall(worldId, halfWidth, halfHeight, &ballTexture);
    struct Object leftPaddle = CreatePaddle(worldId, paddleTexture.width, halfHeight, &paddleTexture);
    struct Object rightPaddle = CreatePaddle(worldId, WIDTH - paddleTexture.width, halfHeight, &paddleTexture);

    // walls: top, bottom, left, right
    struct Object topWall = CreateWall(worldId, halfWidth, -10, WIDTH, 20, false);
    struct Object bottomWall = CreateWall(worldId, halfWidth, HEIGHT+10, WIDTH, 20, false);
    struct Object leftWall = CreateWall(worldId, -10, halfHeight, 20, HEIGHT, true);
    struct Object rightWall = CreateWall(worldId, WIDTH+10, halfHeight, 20, HEIGHT, true);

    bool paused = false;
    float dt;
    while(!WindowShouldClose()){
        paused = !IsWindowFocused();
        dt = GetFrameTime();
        if(!paused){
            ProcessInput(&leftPaddle, &rightPaddle, dt);
            b2World_Step(worldId, dt, 4);
            if(CheckGoal(worldId, &leftWall, &ball)){
                printf("right paddle scored!\n");
            }
            if(CheckGoal(worldId, &rightWall, &ball)){
                printf("left paddle scored!\n");
            }
        }
        BeginDrawing();
        ClearBackground(DARKER);
        DrawObject(&ball);
        DrawObject(&leftPaddle);
        DrawObject(&rightPaddle);
        EndDrawing();
    }
}