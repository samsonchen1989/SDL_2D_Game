#ifndef TILE_LAYER_H
#define TILE_LAYER_H

#include <vector>
#include "Layer.h"
#include "Level.h"
#include "Vector2D.h"

class TileLayer : public Layer
{
public:
    TileLayer(int tileSize, int mapWidth, int mapHeight,
        const std::vector<Tileset>& tilesets);

    virtual void update(Level* pLevel);
    virtual void render();

    void setTileIDs(const std::vector<std::vector<int> >& data) {
        m_tileIDs = data;
    }

    void setTileSize(int tileSize) { m_tileSize = tileSize; }

    void setMapWidth(int mapWidth) { m_mapWidth = mapWidth; }

    Tileset getTilesetByID(int tileID);

    int getTileSize() { return m_tileSize; }

    const std::vector<std::vector<int> >& getTileIDs() { return m_tileIDs; }

    const Vector2D getPosition() { return m_position; }

    int getMapWidth() { return m_mapWidth; }

private:
    int m_numColumns;
    int m_numRows;
    int m_tileSize;
    int m_mapWidth;

    Vector2D m_position;
    Vector2D m_velocity;

    const std::vector<Tileset> &m_tilesets;
    std::vector<std::vector<int> > m_tileIDs;
};

#endif