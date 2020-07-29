#ifndef GAME_H
#define GAME_H

#include"libs/lib.h"

class Game{
  private:

      void ProcessInput();
      void UpdateGame();
      void GenerateOutput();

      SDL_Window* mWindow;
      SDL_Renderer* mRenderer;
      Uint32 mTicksCount;
      bool mIsRunning;

  public:
      Game();
      bool Initialize();
      void RunLoop();
      void Shutdown();

};

#endif
