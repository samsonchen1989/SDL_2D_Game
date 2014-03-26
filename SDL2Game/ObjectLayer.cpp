#include "ObjectLayer.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "Game.h"
#include "Camera.h"
#include "Level.h"
#include <iostream>

ObjectLayer::~ObjectLayer()
{
    for(std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)// < m_gameObjects.size(); i++)
    {
        delete (*it);
    }
    m_gameObjects.clear();
}

void ObjectLayer::update(Level* pLevel)
{
    //std::cout << "check collision\n";
/*
    m_collisionManager.checkPlayerEnemyBulletCollision(pLevel->getPlayer());
    m_collisionManager.checkEnemyPlayerBulletCollision((const std::vector<GameObject*>&)m_gameObjects);
    m_collisionManager.checkPlayerEnemyCollision(pLevel->getPlayer(), (const std::vector<GameObject*>&)m_gameObjects);

    if(pLevel->getPlayer()->getPosition().getX() + pLevel->getPlayer()->getWidth() < TheGame::Instance()->getGameWidth())
    {
        //std::cout << "checkPlayerTileCollision." << std::endl;
        m_collisionManager.checkPlayerTileCollision(pLevel->getPlayer(), pLevel->getCollidableLayers());
    }
*/
    m_collisionManager.checkPlayerEnemyCollision(pLevel->getPlayer(), (const std::vector<GameObject*>&)m_gameObjects);
    //m_collisionManager.checkPlayerTileCollision(pLevel->getPlayer(), pLevel->getCollidableLayers());

    // iterate through the objects
    if(!m_gameObjects.empty())
    {
        for(std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end();)// < m_gameObjects.size(); i++)
        {
            if((*it)->getPosition().getX() <= TheCamera::Instance()->getPosition().m_x + TheGame::Instance()->getGameWidth())
            {
                (*it)->setUpdating(true);
                (*it)->update();
            }
            else
            {
                if((*it)->type() != std::string("Player"))
                {
                    (*it)->setUpdating(false);
                }
                else
                {
                    (*it)->update();
                }
            }

            // check if dead or off screen
            if((*it)->dead() || (*it)->getPosition().m_y > TheGame::Instance()->getGameHeight())
            {
                std::cout << "deleting";
                delete * it;
                it = m_gameObjects.erase(it); // erase from vector and get new iterator
            }
            else
            {
                ++it; // increment if all ok
            }

        }
    }
}

void ObjectLayer::render()
{
    for(int i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->draw();
    }
}