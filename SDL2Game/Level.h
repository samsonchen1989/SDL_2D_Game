#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include "Layer.h"
#include "LevelParser.h"

struct Tileset
{
    int firstGridID;
    int tileWidth;
    int tileHeight;
    int spacing;
    int margin;
    int width;
    int height;
    int numColumns;
    std::string name;
};

class Level
{
public:
    ~Level();

    void update();
    void render();

    std::vector<Tileset>* getTilesets() {
        return &m_tilesets;
    }

    std::vector<Layer*>* getLayers() {
        return &m_layers;
    }

private:
    friend class LevelParser;
    Level();

    std::vector<Tileset> m_tilesets;
    std::vector<Layer*> m_layers;
};

#endif