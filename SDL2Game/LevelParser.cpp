#include <iostream>
#include "TextureManager.h"
#include "Game.h"
#include "LevelParser.h"
#include "TileLayer.h"
#include "GameObjectFactory.h"
#include "base64/base64.h"
#include "zlib.h"
#include "Level.h"
#include "ObjectLayer.h"

Level* LevelParser::parseLevel(const char* levelFile)
{
    //std::cout << "Parse level:" << levelFile << std::endl;
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

    //we know that properties is the first child of the root
    TiXmlElement* pProperties = pRoot->FirstChildElement();

    // we must parse the textures needed for this level, which have been added to properties
    for(TiXmlElement* e = pProperties->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        if(e->Value() == std::string("property"))
        {
            //std::cout << "Parse property." << std::endl;
            parseTextures(e);
        }
    }

    //parse the tilesets
    for (TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        if (e->Value() == std::string("tileset")) {
            //std::cout << "Parse tileset." << std::endl;
            parseTilesets(e, pLevel->getTilesets());
        }
    }

    // parse any object layers
    for(TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        if(e->Value() == std::string("objectgroup") || e->Value() == std::string("layer"))
        {
            if(e->FirstChildElement()->Value() == std::string("object"))
            {
                //std::cout << "Parse object." << std::endl;
                parseObjectLayer(e, pLevel->getLayers(), pLevel);
            }
            else if(e->FirstChildElement()->Value() == std::string("data") ||
               (e->FirstChildElement()->NextSiblingElement() != 0 &&
                e->FirstChildElement()->NextSiblingElement()->Value() == std::string("data")))
            {
                //std::cout << "Parse tile layer." << std::endl;
                parseTileLayer(e, pLevel->getLayers(), pLevel->getTilesets(), pLevel->getCollisionLayers());
            }
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
    const std::vector<Tileset>* pTilesets, std::vector<TileLayer*> *pCollisionLayers)
{
    TileLayer* pTileLayer = new TileLayer(m_tileSize, m_width, m_height, *pTilesets);
    bool collidable = false;
    //tile data
    std::vector<std::vector<int> > data;
    std::string decodedIDs;
    TiXmlElement* pDataNode;

    for(TiXmlElement* e = pTileElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        if(e->Value() == std::string("properties"))
        {
            for(TiXmlElement* property = e->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
            {
                if(property->Value() == std::string("property"))
                {
                    if(property->Attribute("name") == std::string("collidable"))
                    {
                        collidable = true;
                    }
                }
            }
        }

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

    //std::cout << "data, m_height:" << m_height << ", m_width:" << m_width << std::endl;
    pTileLayer->setMapWidth(m_width);
    pTileLayer->setTileIDs(data);

    if(collidable)
    {
        pCollisionLayers->push_back(pTileLayer);
    }

    pLayers->push_back(pTileLayer);
}

void LevelParser::parseTextures(TiXmlElement* pTextureRoot)
{
    TheTextureManager::Instance()->load(pTextureRoot->Attribute("value"),
        pTextureRoot->Attribute("name"), TheGame::Instance()->getRenderer());
}

void LevelParser::parseObjectLayer(TiXmlElement* pObjectElement, std::vector<Layer*> *pLayers, Level* pLevel)
{
    //create an object layer
    ObjectLayer* pObjectLayer = new ObjectLayer();

    //std::cout << pObjectElement->FirstChildElement()->Value() << std::endl;

    for (TiXmlElement* e = pObjectElement->FirstChildElement(); e != NULL;
        e = e->NextSiblingElement()) 
    {
        //std::cout << e->Value() << std::endl;
        if (e->Value() == std::string("object")) {
            int x, y;
            int width, height;
            int numFrames;
            int callbackID = 0;
            int animSpeed = 0;
            std::string textureID;
            std::string type;

            //get the initial node value type, x and y
            e->Attribute("x", &x);
            e->Attribute("y", &y);
            type = e->Attribute("type");
            GameObject* pGameObject = TheGameObjectFactory::Instance()->create(type);

            //get the property values
            for (TiXmlElement* properties = e->FirstChildElement(); properties != NULL;
                properties = properties->NextSiblingElement())
            {
                if (properties->Value() == std::string("properties"))
                {
                    for (TiXmlElement* property = properties->FirstChildElement();
                        property != NULL; property = property->NextSiblingElement())
                    {
                        //std::cout << "property value:" << property->Value() << ", attribute name:" << property->Attribute("name") << std::endl;
                        if (property->Value() == std::string("property"))
                        {
                            if (property->Attribute("name") == std::string("numFrames"))
                            {
                                property->Attribute("value", &numFrames);
                            }
                            else if (property->Attribute("name") ==  std::string("textureHeight"))
                            {
                                property->Attribute("value", &height);
                            }
                            else if (property->Attribute("name") == std::string("textureID"))
                            {
                                textureID = property->Attribute("value");
                            }
                            else if (property->Attribute("name") == std::string("textureWidth"))
                            {
                                property->Attribute("value", &width);
                            }
                            else if (property->Attribute("name") == std::string("callbackID"))
                            {
                                property->Attribute("value", &callbackID);
                            }
                            else if (e->Attribute("name") == std::string("animSpeed"))
                            {
                                property->Attribute("value", &animSpeed);
                            }
                        }
                    }
                }
            }
            pGameObject->load(std::unique_ptr<LoaderParams>(new LoaderParams(x, y, width, height, textureID, numFrames,
                callbackID, animSpeed)));
            pGameObject->setCollisionLayers(pLevel->getCollisionLayers());

            if(type == "Player")
            {
                pLevel->setPlayer(dynamic_cast<Player*>(pGameObject));
            }

            pObjectLayer->getGameObjects()->push_back(pGameObject);
        }
    }

    pLayers->push_back(pObjectLayer);
}