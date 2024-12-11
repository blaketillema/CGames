#include "wall.h"

Wall CreateWall(b2WorldId worldId, int x, int y, int width, int height, bool isSensor){
    Wall wall;
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
