#include <raylib.h>
#include <box2d/box2d.h>
#include <stdio.h>

#include "objects/ball.h"
#include "objects/obstacle.h"
#include "objects/paddle.h"
#include "objects/wall.h"


#define WIDTH 960
#define HEIGHT 640

typedef struct Timer {
    float time;
    float remaining;
} Timer;

Timer CreateTimer(float time){
    Timer timer;
    timer.time = time;
    timer.remaining = time;
    return timer;
}

void ProcessInput(Paddle* leftPaddle, Paddle* rightPaddle, float dt){
    // right paddle
    UpdatePaddle(rightPaddle, 0, HEIGHT);
    // left paddle
    UpdatePaddle(leftPaddle, 0, HEIGHT);
}

bool CheckGoal(b2WorldId worldId, Wall* wall, Ball* ball){
    b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);
    for(int i = 0; i < sensorEvents.beginCount; i++){
        b2SensorBeginTouchEvent* event = sensorEvents.beginEvents + i;
        if(B2_ID_EQUALS(event->sensorShapeId, wall->shapeId) && B2_ID_EQUALS(event->visitorShapeId, ball->shapeId)) return true;
    }
    return false;
}

int main(){

    // colours
    const Color DARKER = (Color){15, 56, 15, 255};
    const Color DARK = (Color){48, 98, 48, 255};
    const Color LIGHT = (Color){139, 172, 15, 255};
    const Color LIGHTER = (Color){155, 188, 15, 255};
    const Color PAUSED = (Color){0, 0, 0, 191};

    // this needs to be done before loading textures
    InitWindow(WIDTH, HEIGHT, "Pong");

    // make the world
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2_zero;
    b2WorldId worldId = b2CreateWorld(&worldDef);

    // load the textures
    Texture2D ballTexture = LoadTexture("ball.png");
    Texture2D paddleTexture = LoadTexture("paddle.png");

    // save calculations?
    const int halfWidth = WIDTH / 2, halfHeight = HEIGHT / 2;
    const int quarterWidth = WIDTH / 4;
    
    // we ballin'
    Ball ball = CreateBall(worldId, &ballTexture);
    ResetBall(&ball, halfWidth, halfHeight);

    // left paddle - W, S for controls
    Paddle leftPaddle = CreatePaddle(worldId, paddleTexture.width / 2, halfHeight, &paddleTexture);
    leftPaddle.controls.up = KEY_W;
    leftPaddle.controls.down = KEY_S;
    int leftScore = 0;

    // right paddle - Up, Down for controls
    Paddle rightPaddle = CreatePaddle(worldId, WIDTH - paddleTexture.width / 2, halfHeight, &paddleTexture);
    rightPaddle.controls.up = KEY_UP;
    rightPaddle.controls.down = KEY_DOWN;
    int rightScore = 0;

    // walls: top, bottom, left, right
    Wall topWall = CreateWall(worldId, halfWidth, -10, WIDTH, 20, false);
    Wall bottomWall = CreateWall(worldId, halfWidth, HEIGHT+10, WIDTH, 20, false);
    Wall leftWall = CreateWall(worldId, -10, halfHeight, 20, HEIGHT, true);
    Wall rightWall = CreateWall(worldId, WIDTH+10, halfHeight, 20, HEIGHT, true);

    // falling obstacles
    const int leftX = quarterWidth;
    const int rightX = 3 * quarterWidth;
    const int minSize = 10;
    const int maxSize = 100;
    const int minSpeed = 100;
    const int maxSpeed = 1000;
    const int numObstacles = 24;
    const int widthStep = (rightX - leftX) / numObstacles;
    Obstacle obstacles[numObstacles];
    for(int i = 0; i < numObstacles; i++){
        obstacles[i] = CreateObstacle(worldId, leftX + i * widthStep + widthStep / 2, 2 * HEIGHT, minSize, maxSize, minSpeed, maxSpeed);
    }

    // timer to drop obstacles
    Timer timer = CreateTimer(0.5f);
    float dt;

    // game states
    bool paused = true;
    bool reset = false;

    // text
    const char* helpMsg = "ESC=QUIT SPACE=(UN)PAUSE";
    int msgWidth = MeasureText(helpMsg, 20);
    const char* controlsMsg = "<(W=UP S=DOWN) (UP=UP DOWN=DOWN)>";
    int controlsWidth = MeasureText(controlsMsg, 40);

    int boxIdx = 0;
    Obstacle* box;

    SetTargetFPS(120);

    while(!WindowShouldClose()){
        // delta time
        dt = GetFrameTime();

        // do we need to reset
        // this just puts the ball back in the middle of the field for now
        if(reset == true){
            ResetBall(&ball, halfWidth, halfHeight);
            reset = false;
            paused = true;
            for(int i = 0; i < numObstacles; i++){
                ResetObstacle(&obstacles[i], BOTTOM);
            }
        }

        // doesnt matter if input is processed for the paddles
        ProcessInput(&leftPaddle, &rightPaddle, dt);
        if(IsKeyPressed(KEY_SPACE)) paused = !paused;
        if(!paused){
            b2World_Step(worldId, dt, 4);
            if(CheckGoal(worldId, &leftWall, &ball)){
                rightScore++;
                reset = true;
            }
            if(CheckGoal(worldId, &rightWall, &ball)){
                leftScore++;
                reset = true;
            }
            timer.remaining -= dt;
            if(timer.remaining < 0.f){
                timer.remaining += timer.time;
                // select a random box
                // if it's below the screen, reset it
                // otherwise, try next time
                boxIdx = GetRandomValue(0, numObstacles-1);
                box = &obstacles[0] + boxIdx;
                if(b2Body_GetPosition(box->bodyId).y - box->size > HEIGHT){
                    ResetObstacle(box, TOP);
                }
            }
        }

        // draw stuff
        BeginDrawing();
        ClearBackground(DARKER);
        DrawBall(&ball);
        DrawPaddle(&leftPaddle);
        DrawPaddle(&rightPaddle);
        for(int i = 0; i < numObstacles; i++){
            DrawObstacle(&obstacles[i], LIGHT);
        }
        if(paused){
            DrawRectangle(0, 0, WIDTH, HEIGHT, PAUSED);
            DrawText(controlsMsg, halfWidth - controlsWidth / 2, halfHeight, 40, LIGHTER);
        }
        DrawText(helpMsg, halfWidth - msgWidth / 2, HEIGHT - 20, 20, LIGHTER);
        DrawText(TextFormat("%d", leftScore), quarterWidth, 0, 40, LIGHTER);
        DrawText(TextFormat("%d", rightScore), quarterWidth * 3, 0, 40, LIGHTER);
        EndDrawing();
    }
}