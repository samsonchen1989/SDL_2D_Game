#include <iostream>
#include <SDL_image.h>
#include "TextureManager.h"

TextureManager* TextureManager::s_pInstance = 0;

bool TextureManager::load(std::string fileName, std::string id,
    SDL_Renderer* pRenderer)
{
    std::cout << "filename:" << fileName << ", id:" << id << std::endl;
    //c_str() converts string to C-like char*.
    SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

    if (pTempSurface == 0) {
        return false;
    }

    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
    SDL_FreeSurface(pTempSurface);

    //everything ok, add the texture to our list.
    if (pTexture != 0) {
        m_textureMap[id] = pTexture;
        return true;
    }

    return false;
}

void TextureManager::draw(std::string id, int x, int y, int width, int height,
    SDL_Renderer* pRenderer, SDL_RendererFlip flip)
{
    SDL_Rect srcRect;
    SDL_Rect destRect;

    destRect.w = width;
    destRect.h = height;
    destRect.x = x;
    destRect.y = y;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = destRect.w;
    srcRect.h = destRect.h;

    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

void TextureManager::drawFrame(std::string id, int x, int y, int width, int height,
    int currentRow, int currentFrame, SDL_Renderer* pRenderer,
    double angle, int alpha, SDL_RendererFlip flip)
{
    SDL_Rect srcRect;
    SDL_Rect destRect;

    destRect.x = x;
    destRect.y = y;
    destRect.w = width;
    destRect.h = height;

    srcRect.x = width * currentFrame;
    srcRect.y = height * currentRow;
    srcRect.w = destRect.w;
    srcRect.h = destRect.h;
    //std::cout << "id:" << id << std::endl;
    //set the alpha of the texture and pass in the angle
    SDL_SetTextureAlphaMod(m_textureMap[id], alpha);
    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, angle, 0, flip);
}

void TextureManager::drawTile(std::string id, int margin, int spacing, int x, int y,
    int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer)
{
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = margin + (spacing + width) * currentFrame;
    srcRect.y = margin + (spacing + height) * currentRow;
    srcRect.w = width;
    srcRect.h = height;

    destRect.x = x;
    destRect.y = y;
    destRect.w = width;
    destRect.h = height;

    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
}

void TextureManager::clearTextureMap()
{
    m_textureMap.clear();
}

void TextureManager::clearFromTextureMap(std::string id)
{
    m_textureMap.erase(id);
}

void TextureManager::printTextureMap()
{
    for (std::map<std::string, SDL_Texture*>::iterator it = m_textureMap.begin();
            it != m_textureMap.end(); ++it)
    {
        std::cout << "map:" << it->first << std::endl;
    }
}