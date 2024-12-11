#ifndef PONG_PADDLE
    #define PONG_PADDLE
    #include <box2d/box2d.h>
    #include <raylib.h>

    typedef struct Controls {
        int up;
        int down;
    } Controls;

    typedef struct Paddle {
        b2BodyId bodyId;
        b2ShapeId shapeId;
        Texture* texture;
        Controls controls;
    } Paddle;

    Paddle CreatePaddle(b2WorldId worldId, int x, int y, Texture2D* texture);
    void UpdatePaddle(Paddle* paddle, int lowerBound, int upperBound);
    void DrawPaddle(Paddle* paddle);
#endif
