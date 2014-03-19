#include <iostream>
#include "TextureManager.h"
#include "Game.h"
#include "LevelParser.h"
#include "TileLayer.h"
#include "GameObjectFactory.h"
#include "base64/base64.h"
#include "zlib.h"
#include "Level.h"

Level* LevelParser::parseLevel(const char* levelFile)
{
    //create a TinyXML document and load the map xml
    TiXmlDocument levelDocument;
    levelDocument.LoadFile(levelFile);

    //create the level object
    Level* pLevel = new Level();

    //get the root node
    TiXmlElement* pRoot = levelDocument.RootElement();
    pRoot->Attribute("tilewidth", &m_tileSize);
    pRoot->Attribute("width", &m_width);
    pRoot->Attribute("height", &m_height);

    //parse the tilesets
    for (TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        if (e->Value() == std::string("tileset")) {
            parseTilesets(e, pLevel->getTilesets());
        }
    }

    //parse any object layers
    for (TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        if (e->Value() == std::string("layer")) {
            parseTileLayer(e, pLevel->getLayers(), pLevel->getTilesets());
        }
    }

    return pLevel;
}

void LevelParser::parseTilesets(TiXmlElement* pTilesetRoot, std::vector<Tileset>* pTilesets) {
    //first add the tileset to texture manager
    TheTextureManager::Instance()->load(
        pTilesetRoot->FirstChildElement()->Attribute("source"),
        pTilesetRoot->Attribute("name"),
        TheGame::Instance()->getRenderer());

    //create a tileset object
    Tileset tileset;
    pTilesetRoot->FirstChildElement()->Attribute("width", &tileset.width);
    pTilesetRoot->FirstChildElement()->Attribute("height", &tileset.height);
    pTilesetRoot->Attribute("firstgid", &tileset.firstGridID);
    pTilesetRoot->Attribute("tilewidth", &tileset.tileWidth);
    pTilesetRoot->Attribute("tileheight", &tileset.tileHeight);
    pTilesetRoot->Attribute("spacing", &tileset.spacing);
    pTilesetRoot->Attribute("margin", &tileset.margin);
    tileset.name = pTilesetRoot->Attribute("name");

    tileset.numColumns = tileset.width / (tileset.tileWidth + tileset.spacing);

    pTilesets->push_back(tileset);
}

void LevelParser::parseTileLayer(TiXmlElement* pTileElement, std::vector<Layer*> *pLayers,
    const std::vector<Tileset>* pTilesets)
{
    TileLayer* pTileLayer = new TileLayer(m_tileSize, *pTilesets);
    //tile data
    std::vector<std::vector<int> > data;
    std::string decodedIDs;
    TiXmlElement* pDataNode;

    for(TiXmlElement* e = pTileElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        if (e->Value() == std::string("data")) {
            pDataNode = e;
        }
    }

    for(TiXmlNode* e = pDataNode->FirstChild(); e != NULL; e = e->NextSibling()) {
        TiXmlText* text = e->ToText();
        std::string t = text->Value();
        decodedIDs = base64_decode(t);
    }

    //uncompress zlib compression
    uLongf numGids = m_width * m_height * sizeof(int);
    std::vector<unsigned> gids(numGids);
    uncompress((Bytef*)&gids[0], &numGids, (const Bytef*)decodedIDs.c_str(), decodedIDs.size());

    std::vector<int> LayerRow(m_width);

    for (int j = 0; j < m_height; j++) {
        data.push_back(LayerRow);
    }

    for (int rows = 0; rows < m_height; rows++) {
        for (int cols = 0; cols < m_width; cols++) {
            data[rows][cols] = gids[rows * m_width + cols];
        }
    }

    std::cout << "data, m_height:" << m_height << ", m_width:" << m_width << std::endl;
    pTileLayer->setMapWidth(m_width);
    pTileLayer->setTileIDs(data);

    pLayers->push_back(pTileLayer);
}