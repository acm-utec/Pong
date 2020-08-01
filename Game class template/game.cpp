//
// Created by Cesar on 29/07/2020.
//

#include "game.h"
#include <iostream>
const int thickness =20;
const int paddleH=150;
const int Width= 1024;
const int Height=768;
Game::Game(): mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true){
    mPaddlePosP1.x=10;
    mPaddlePosP1.y=180;
    mBallPos.x=Width/2;
    mBallPos.y=Height/2;
    mTicksCount=0;
    mPaddleDir=0;
}
bool Game::Initialize(){
    int	sdlResult	=	SDL_Init(SDL_INIT_VIDEO);
    if	(sdlResult	!=	0){
        SDL_Log("Unable	to	initialize	SDL:	%s",	SDL_GetError());
        return	false;
    }
    mWindow	=	SDL_CreateWindow(
            "Game	Programming	in	C++	(Chapter	1)",	//	Window	title
            100,			//	Top	left	x-coordinate	of	window
            100,			//	Top	left	y-coordinate	of	window
            Width,		//	Width	of	window
            Height,			//	Height	of	window
            0						//	Flags	(0	for	no	flags	set)
    );
    mRenderer = SDL_CreateRenderer(
        mWindow, // Window to create renderer for
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );
    if	(!mWindow){
        SDL_Log("Failed	to	create	window:	%s",	SDL_GetError());
        return	false;
    }


}
void Game::RunLoop(){
    while	(mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }

}
void Game::ProcessInput(){
    SDL_Event	event;
    //	While	there	are	still	events	in	the	queue
    while	(SDL_PollEvent(&event)){
        switch	(event.type){
            case	SDL_QUIT:
                mIsRunning	=	false;
                break;
        }

    }
    const	Uint8*	state	=	SDL_GetKeyboardState(NULL);
    //	If	escape	is	pressed,	also	end	loop
    if	(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning	=	false;
    }
    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W])
    {
    mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
    mPaddleDir += 1;
    }

    }
void Game::UpdateGame(){
    

    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;
    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    // Update tick counts (for next frame)
    mTicksCount = SDL_GetTicks();
    if (deltaTime > 0.05f){
        deltaTime = 0.05f;
    }
    if (mPaddleDir != 0){
        mPaddlePosP1.y += mPaddleDir * 300.0f * deltaTime;
        std::cout<<mPaddlePosP1.y<<std::endl;
        if (mPaddlePosP1.y < (paddleH/2.0f + thickness)){
            mPaddlePosP1.y = paddleH/2.0f + thickness;
        }
        else if (mPaddlePosP1.y > (Height - paddleH/2.0f - thickness)){
            mPaddlePosP1.y = Height - paddleH/2.0f - thickness;
        }
    }
}

void Game::GenerateOutput(){
    // Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);
	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);


    
	// Draw paddle Player 1
	SDL_Rect paddleP1{
		static_cast<int>(mPaddlePosP1.x - thickness/2),
		static_cast<int>(mPaddlePosP1.y - paddleH/2),
		30,
        paddleH
	};
	SDL_RenderFillRect(mRenderer, &paddleP1);  
    SDL_RenderPresent(mRenderer);

    

}


void Game::Shutdown(){
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}