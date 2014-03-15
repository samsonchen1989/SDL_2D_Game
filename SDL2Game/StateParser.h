#ifndef STATE_PARSER_H
#define STATE_PARSER_H

#include <iostream>
#include <vector>
#include "tinyxml/tinyxml.h"
#include "GameObject.h"

class StateParser
{
public:
    bool parseState(const char* stateFile, std::string stateID,
        std::vector<GameObject*> *pObjects,
        std::vector<std::string> *pTextureIDs);

private:
    void parseObjects(TiXmlElement* pStateRoot, std::vector<GameObject*> *pObjects);
    void parsetextures(TiXmlElement* pSTateRoot, std::vector<std::string> *pTextureIDs);
};

#endif