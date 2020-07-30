// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <iostream>
#include "SDL/SDL.h"

using namespace std;

// Vector2 struct just stores x/y coordinates
// (for now)
struct Vector2
{
	float x;
	float y;
};

// Ball struct for multiball game
struct Ball {
    Vector2 pos;
    Vector2 vel;
    bool available = true;
    bool wasP1 = false;
    bool wasP2 = false;
};

// Game class
class Game
{
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
    void showWinner();
    bool ballsAvailables();

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;
	
	// Pong specific
    // Player 1
	// Direction of paddle
	int mPaddleDirP1;
	// Position of paddle
	Vector2 mPaddlePosP1;
    
    // Player 2
    // Direction of paddle
    int mPaddleDirP2;
    // Position of paddle
    Vector2 mPaddlePosP2;
    
    // Ball
    vector<Ball> balls;
};
