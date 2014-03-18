#include "Game.h"

const int FPS = 60;
const int DELAY_TIME = static_cast<int>(1000.0f / FPS);

int main(int argc, char** grgs)
{
    Uint32 frameStart;
    Uint32 frameTime;

    GAME_STATUS_TAG res;
    res = TheGame::Instance()->init("SDL_Game", 100, 100, 640, 480, false);
    if (res == GAME_INIT_SUCCESS) {
        while (TheGame::Instance()->running()) {
            frameStart = SDL_GetTicks();

            TheGame::Instance()->handleEvents();
            TheGame::Instance()->update();
            TheGame::Instance()->render();

            frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < DELAY_TIME) {
                SDL_Delay(static_cast<int>(DELAY_TIME - frameTime));
            }
        }
    } else {
        return -1;
    }

    TheGame::Instance()->clean();

    return 0;
}