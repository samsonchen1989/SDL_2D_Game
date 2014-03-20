#include "ObjectLayer.h"

void ObjectLayer::update()
{
	for (unsigned i = 0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->update();
	}
}

void ObjectLayer::render()
{
	for (unsigned i = 0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->draw();
	}
}