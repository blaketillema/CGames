#ifndef PONG_OBSTACLE
    #define PONG_OBSTACLE
    #include <box2d/box2d.h>
    #include <raylib.h>

    enum SPAWN_LOCATION {TOP, BOTTOM};

    typedef struct Obstacle {
        float spawnX;
        float spawnY;
        b2BodyId bodyId;
        b2ShapeId shapeId;
        int size;
    } Obstacle;

    Obstacle CreateObstacle(b2WorldId worldId, int spawnX, int spawnY, int minSize, int maxSize, int minSpeed, int maxSpeed);
    void DrawObstacle(Obstacle* obstacle, Color color);
    void ResetObstacle(Obstacle* obstacle, int where);
#endif