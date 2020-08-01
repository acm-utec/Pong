#ifndef GAME_H
#define GAME_H
#include "SDL2/SDL.h"


struct Vector2
{
float x;
float y;
};

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
    // Player 1
	
	// Position of paddle
    Vector2 mPaddlePosP1;
	int mPaddleDir;

    // Position of the ball
    Vector2 mBallPos;    

};

#endif
