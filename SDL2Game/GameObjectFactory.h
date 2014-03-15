#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include <string>
#include <map>
#include <iostream>

#include "GameObject.h"

class BaseCreator
{
public:
    virtual GameObject* createGameObject() const = 0;
    virtual ~BaseCreator() {}
};

class GameObjectFactory
{
public:
    static GameObjectFactory* Instance()
    {
        if (m_pFactory == 0) {
            m_pFactory = new GameObjectFactory();
        }

        return m_pFactory;
    }

    bool registerType(std::string typeID, BaseCreator* pCreator)
    {
        std::map<std::string, BaseCreator*>::iterator it = m_creators.find(typeID);

        //if the type is already registered, do nothing
        if (it != m_creators.end()) {
            delete pCreator;
            return false;
        }

        m_creators[typeID] = pCreator;

        return true;
    }

    GameObject* create(std::string typeID)
    {
        std::map<std::string, BaseCreator*>::iterator it = m_creators.find(typeID);
        std::cout << "create:" << typeID << std::endl;
        if (it == m_creators.end()) {
            std::cout << "could not find type:" << typeID << std::endl;
            return NULL;
        }
        std::cout << "find it:" << typeID << std::endl;
        BaseCreator* pCreator = (*it).second;
        return pCreator->createGameObject();
    }

private:
    GameObjectFactory() {}
    virtual ~GameObjectFactory() {}

    static GameObjectFactory* m_pFactory;
    std::map<std::string, BaseCreator*> m_creators;
};

typedef GameObjectFactory TheGameObjectFactory;

#endif