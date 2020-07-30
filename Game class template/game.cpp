// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <stdlib.h>
#include <time.h>

const int thickness = 15;
const float paddleH = 150.0f;
const int WIDTH = 1024;
const int HEIGHT = 768;
int NBALLS = 3;
int NBALLSLEFT = NBALLS;
bool oneTime = true;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddleDirP1(0)
,mPaddleDirP2(0)
{
	
}

bool Game::Initialize()
{
    // Random numbers
    srand( (unsigned) time(NULL));
    
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1) + Homework", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		WIDTH,	// Width of window
		HEIGHT,	// Height of window
		0  // Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	/// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	// Player 1
	mPaddlePosP1.x = 10.0f;
	mPaddlePosP1.y = (float) HEIGHT/2.0f;
    
    // Player 2
    mPaddlePosP2.x = (float) WIDTH - (10.0f + (float) thickness);
    mPaddlePosP2.y = (float) HEIGHT/2.0f;
    
    // Ball
    for (int i = 0; i < NBALLS; ++i) {
        int y = rand() % (HEIGHT - thickness) + thickness;
        int right = rand() % 10 + 1; // 1 - 10
        int up = rand() % 10 + 1;    // 1 - 10
        
        Vector2 pos;
        Vector2 vel;
        Ball ball;
        pos.x = (float)HEIGHT/2.0f;
        pos.y = y;
        vel.x = (right % 2 == 0) ? -200.0f : 200.0f;
        vel.y = (up % 2 == 0) ? -235.0f : 235.0f;
        ball.pos = pos;
        ball.vel = vel;
        balls.emplace_back(ball);
    }
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle direction based on W/S keys
	mPaddleDirP1 = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDirP1 -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDirP1 += 1;
	}
    
    // Update paddle direction based on I/K keys
    mPaddleDirP2 = 0;
    if (state[SDL_SCANCODE_I])
    {
        mPaddleDirP2 -= 1;
    }
    if (state[SDL_SCANCODE_K])
    {
        mPaddleDirP2 += 1;
    }
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	// Update paddle position based on direction Player 1
	if (mPaddleDirP1 != 0)
	{
		mPaddlePosP1.y += mPaddleDirP1 * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePosP1.y < (paddleH/2.0f + thickness))
		{
			mPaddlePosP1.y = paddleH/2.0f + thickness;
		}
		else if (mPaddlePosP1.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddlePosP1.y = 768.0f - paddleH/2.0f - thickness;
		}
	}
    
    // Update paddle position based on direction Player 2
    if (mPaddleDirP2 != 0)
    {
        mPaddlePosP2.y += mPaddleDirP2 * 300.0f * deltaTime;
        // Make sure paddle doesn't move off screen!
        if (mPaddlePosP2.y < (paddleH/2.0f + thickness))
        {
            mPaddlePosP2.y = paddleH/2.0f + thickness;
        }
        else if (mPaddlePosP2.y > (768.0f - paddleH/2.0f - thickness))
        {
            mPaddlePosP2.y = 768.0f - paddleH/2.0f - thickness;
        }
    }
    
    for (auto& i : balls) {
        // Update ball position based on ball velocity
        i.pos.x += i.vel.x * deltaTime;
        i.pos.y += i.vel.y * deltaTime;
        
        // Bounce if needed
        // Did we intersect with the paddle? Player 1
        float diff = mPaddlePosP1.y - i.pos.y;
        // Take absolute value of difference
        diff = (diff > 0.0f) ? diff : -diff;
        if (
            // Our y-difference is small enough
            diff <= paddleH / 2.0f &&
            // We are in the correct x-position
            i.pos.x <= 25.0f && i.pos.x >= 20.0f &&
            // The ball is moving to the left
            i.vel.x < 0.0f)
        {
            i.vel.x *= -1.0f;
            i.vel.x += 80;
            i.vel.y += 80;
        }
        
        // Did we intersect with the paddle? Player 2
        diff = mPaddlePosP2.y - i.pos.y;
        // Take absolute value of difference
        diff = (diff > 0.0f) ? diff : -diff;
        if (
            // Our y-difference is small enough
            diff <= paddleH / 2.0f &&
            // We are in the correct x-position
            i.pos.x >= WIDTH - (thickness + 10) && i.pos.x <= WIDTH - (thickness + 5) &&
            // The ball is moving to the right
            i.vel.x > 0.0f)
        {
            i.vel.x *= -1.0f;
            i.vel.x -= 80;
            i.vel.y -= 80;
        }
        
        // Did the ball go off the screen? Player 2 scores
        else if (i.pos.x <= 0.0f)
        {
            i.wasP2 = true;
            i.available = false;
        }
        // Did the ball go off the screen? Player 1 scores
        else if (i.pos.x >= (1024.0f - thickness) && i.vel.x > 0.0f)
        {
            i.wasP1 = true;
            i.available = false;
        }
        
        // Did the ball collide with the top wall?
        if (i.pos.y <= thickness && i.vel.y < 0.0f)
        {
            i.vel.y *= -1;
        }
        // Did the ball collide with the bottom wall?
        else if (i.pos.y >= (768 - thickness) &&
            i.vel.y > 0.0f)
        {
            i.vel.y *= -1;
        }
        
        if (!ballsAvailables()) {
            if (oneTime) {
                showWinner();
            }
            oneTime = false;
            mIsRunning = false;
        }
    }
}

void Game::GenerateOutput()
{
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
	
    /*
	// Draw right wall
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);
	*/
    
	// Draw paddle Player 1
	SDL_Rect paddleP1{
		static_cast<int>(mPaddlePosP1.x),
		static_cast<int>(mPaddlePosP1.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddleP1);
    
    // Draw paddle Player 2
    SDL_Rect paddleP2{
        static_cast<int>(mPaddlePosP2.x),
        static_cast<int>(mPaddlePosP2.y - paddleH/2),
        thickness,
        static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(mRenderer, &paddleP2);
	
	// Draw ball
    
    for (auto& i : balls) {
        if (i.available) {
            SDL_Rect ball{
                static_cast<int>(i.pos.x - thickness/2),
                static_cast<int>(i.pos.y - thickness/2),
                thickness,
                thickness
            };
            SDL_RenderFillRect(mRenderer, &ball);
        }
    }
    
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::showWinner () {
    int P1points = 0;
    int P2points = 0;
    
    for (auto& i : balls) {
        if (i.wasP1)
            P1points++;
        if (i.wasP2)
            P2points++;
    }
    
    if (P1points > P2points) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "The game is finished",
                                 "Player 1 won",
                                 mWindow);
    } else if (P1points < P2points){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "The game is finished",
                                 "Player 2 won",
                                 mWindow);
    } else {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
        "The game is finished",
        "Empate",
        mWindow);
    }
}

bool Game::ballsAvailables() {
    for (auto& i : balls) {
        if (i.available)
            return true;
    }
    return false;
}
