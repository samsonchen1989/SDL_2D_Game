#include "StateParser.h"
#include "TextureManager.h"
#include "Game.h"
#include "GameObjectFactory.h"

bool StateParser::parseState(const char* stateFile, std::string stateID,
    std::vector<GameObject*> *pObjects,
    std::vector<std::string> *pTextureIDs)
{
    //create the XML document
    TiXmlDocument xmlDoc;

    if (xmlDoc.LoadFile(stateFile) == false) {
        std::cerr << xmlDoc.ErrorDesc() << "\n";
    }

    //get the root element
    TiXmlElement* pRoot = xmlDoc.RootElement();

    //pre declare the states root node
    TiXmlElement* pStateRoot = 0;
    //get the states root node and assign it to pStateRoot
    for (TiXmlElement* e = pRoot->FirstChildElement(); e != NULL;
        e = e->NextSiblingElement())
    {
        if (e->Value() == stateID) {
            pStateRoot = e;
        }
    }

    //pre declare the texture root
    TiXmlElement* pTextureRoot = 0;
    //get the root of the texture elements
    for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL;
        e = e->NextSiblingElement())
    {
        if (e->Value() == std::string("TEXTURES")) {
            pTextureRoot = e;
        }
    }

    //now parse the textures
    parsetextures(pTextureRoot, pTextureIDs);

    //pre declare the object root node
    TiXmlElement* pObjectRoot = 0;
    //get the root node and assign it to pObjectRoot
    for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL;
        e = e->NextSiblingElement())
    {
        if (e->Value() == std::string("OBJECTS")) {
            pObjectRoot = e;
        }
    }

    //now parse the objects
    parseObjects(pObjectRoot, pObjects);

    return true;
}

void StateParser::parsetextures(TiXmlElement* pSTateRoot,
    std::vector<std::string> *pTextureIDs)
{
    for (TiXmlElement* e = pSTateRoot->FirstChildElement(); e != NULL;
        e = e->NextSiblingElement())
    {
        std::string filenameAttribute = e->Attribute("filename");
        std::string idAttribute = e->Attribute("ID");
        pTextureIDs->push_back(idAttribute);

        TheTextureManager::Instance()->load(filenameAttribute, idAttribute,
            TheGame::Instance()->getRenderer());
    }
}

void StateParser::parseObjects(TiXmlElement* pStateRoot,
    std::vector<GameObject*> *pObjects)
{
    for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL;
        e = e->NextSiblingElement())
    {
        int x, y;
        int width, height;
        int numFrame;
        int callbackID;
        int animSpeed;
        std::string textureID;

        e->Attribute("x", &x);
        e->Attribute("y", &y);
        e->Attribute("width", &width);
        e->Attribute("height", &height);
        e->Attribute("numFrames", &numFrame);
        e->Attribute("callbackID", &callbackID);
        e->Attribute("animSpeed", &animSpeed);
        //std::cout << "StateParser animSpeed:" << animSpeed << std::endl;

        textureID = e->Attribute("textureID");
        //std::cout << "Attribute(type):" << e->Attribute("type") << std::endl;
        GameObject* pGameObject = TheGameObjectFactory::Instance()->create(e->Attribute("type"));
        pGameObject->load(std::unique_ptr<LoaderParams>(new LoaderParams(x, y, width, height, textureID, numFrame, callbackID, animSpeed)));
        pObjects->push_back(pGameObject);
    }
}