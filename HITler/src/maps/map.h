#ifndef MAP_H
#define MAP_H

class Map
{
public:
    virtual ~Map() = default;
    virtual void Draw() = 0;
};

#endif