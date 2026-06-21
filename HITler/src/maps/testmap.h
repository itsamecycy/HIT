#ifndef TESTMAP_H
#define TESTMAP_H

#include "map.h"
#include "raylib.h"

class TestMap : public Map
{
public:
    void Draw() override;
};

#endif