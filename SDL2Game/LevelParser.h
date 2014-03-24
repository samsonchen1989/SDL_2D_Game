#ifndef LEVEL_PARSER_H
#define LEVEL_PARSER_H

#include <vector>

#include "tinyxml/tinyxml.h"

class Level;
struct Tileset;
class Layer;
class TileLayer;

class LevelParser
{
public:
    Level* parseLevel(const char* levelFile);

private:
    void parseTilesets(TiXmlElement* pTilesetRoot,
        std::vector<Tileset>* pTilesets);

    void parseTileLayer(TiXmlElement* pTileElement,
        std::vector<Layer*> *pLayers, const std::vector<Tileset>* pTilesets,
        std::vector<TileLayer*> *pCollisionLayers);

    void parseTextures(TiXmlElement* pTextureRoot);

    void parseObjectLayer(TiXmlElement* pObjectElement, std::vector<Layer*> *pLayers, Level* pLevel);

    int m_tileSize;
    int m_width;
    int m_height;
};

#endif