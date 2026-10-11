/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main ()
{

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 320, "Melon Run ");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture G1 = LoadTexture("GFork.png");
	Texture G2 = LoadTexture("GCoffee.png");
	Texture F1 = LoadTexture("FlyWhip.png");
	Texture F2 = LoadTexture("FlyKnife.png");
	Texture player = LoadTexture("walk1.png");
	Texture walk2 = LoadTexture("walk2.PNG");
	Texture background = LoadTexture("BG.PNG");
	Texture slide = LoadTexture("Slide.PNG");

	// OBSTACLE VARIABLES

	float obstacleX = 1280;
	float obstacleY = 0;

	float obstacleSpeed = 270.0f;

	bool obstacleActive = false;

	int obstacleType = 0;    // 0 = ground, 1 = fly
	int obstacleImage = 0;   // 0 = image 1, 1 = image 2

	float spawnTimer = 1.5f;

	float minGap = 1.5f;
	float maxGap = 2.5f;

	//Score
	float score = 0.0f;

	//Animation melon
	float animationTimer = 0.0f;
	int currentFrame = 0;

	// background scrolling
	float bgX1 = 0;
	float bgX2 = 1280;
	float bgSpeed = 200;

	// player and obstacle rectangles for collision detection
	Rectangle playerRec;
	Rectangle obstacleRec;
	Texture currentObstacle;
	bool gameOver = false;

	// PLAYER MOVEMENT
	float playerX = 100;
	float playerY = 200;

	float velocityY = 0.0f;
	float gravity = 1200.0f;
	float jumpForce = -500.0f;

	bool isJumping = false;
	bool isSliding = false;
	bool gameStarted = false;

	// Initialize the audio device
	InitAudioDevice();

	TraceLog(LOG_INFO, "Audio ready: %d", IsAudioDeviceReady());
	TraceLog(LOG_INFO, "Music file exists: %d", FileExists("music.mp3"));

	Music music = LoadMusicStream("music.mp3");
	music.looping = true;
	PlayMusicStream(music);

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float deltaTime = GetFrameTime();
		UpdateMusicStream(music);
		// START / RESTART GAME
		if (IsKeyPressed(KEY_R))
		{
    		gameStarted = true;
    		gameOver = false;

    		score = 0;

    		playerY = 200;
    		velocityY = 0;
			isJumping = false;
			isSliding = false;

			obstacleActive = false;
			obstacleX = 1280;
			spawnTimer = 1.5f;		

			bgX1 = 0;
			bgX2 = 1280;

			currentFrame = 0;
			animationTimer = 0;
		}

		if (gameStarted && !gameOver)
		{

			// JUMP
			if (IsKeyPressed(KEY_SPACE) && !isJumping)
{
    velocityY = jumpForce;
    isJumping = true;
}

			// SLIDE
			if (IsKeyDown(KEY_DOWN) && !isJumping)
			{
				isSliding = true;
			}
			else
			{
				isSliding = false;
			}


			// Gravity
			velocityY += gravity * deltaTime;
			playerY += velocityY * deltaTime;

			// Player lands on ground
			if (playerY >= 200)
		{
   			playerY = 200;
    		velocityY = 0;
    		isJumping = false;
		}			
		
		// update the score
		score += deltaTime * 5;

		//Background Loop
		bgX1 -= bgSpeed * deltaTime;
		bgX2 -= bgSpeed * deltaTime;

		if (bgX1 <= -1280)
{
    	bgX1 = bgX2 + 1280;
}

		if (bgX2 <= -1280)
		{
    		bgX2 = bgX1 + 1280;
		}

		// Melon Animaton Loop
		animationTimer += deltaTime ;

		if (animationTimer >= 0.2f)
		{
    	currentFrame = 1 - currentFrame;
    	animationTimer = 0.0f;
		}

		// RANDOM OBSTACLE

		if (!obstacleActive)
		{
    		spawnTimer -= deltaTime;

    		if (spawnTimer <= 0.0f)
    	{
        	obstacleActive = true;

        // Start the obstacle off the right side of the screen
        	obstacleX = 1280;

        // Randomly choose between ground and fly
        	obstacleType = GetRandomValue(0, 1);

        // Randomly choose 1 or 2
        	obstacleImage = GetRandomValue(0, 1);

        // GROUND
       
        	if (obstacleType == 0)
       		{
            if (obstacleImage == 0)
            {
                // Ground 1
                obstacleY = 220;

                minGap = 1.0f;
                maxGap = 1.6f;
            }
            else
            {
                // Ground 2
                obstacleY = 220;

                minGap = 1.2f;
                maxGap = 1.7f;
            }
        	}

        // FLY
        	else
        	{
        	    if (obstacleImage == 0)
            {
                // FLY 1
                obstacleY = 150;

                minGap = 0.8f;
                maxGap = 2.1f;
            }
            else
            {
                // FLY 2
                obstacleY = 140;

                minGap = 0.9f;
                maxGap = 2.0f;
            }
    		}
		
    }
}


// MOVE OBSTACLE

if (obstacleActive)
{
    obstacleX -= obstacleSpeed * deltaTime;
}

// WHEN OBSTACLE LEAVES SCREEN

if (obstacleActive && obstacleX < -200)
{
    obstacleActive = false;

    // Wait for the next obstacle
    spawnTimer =
        GetRandomValue(
            (int)(minGap * 100),
            (int)(maxGap * 100)
        ) / 100.0f;
}
// Set the current obstacle texture based on the type and image
	if (obstacleType == 0)
	{
    if (obstacleImage == 0)
        currentObstacle = G1;
    else
        currentObstacle = G2;
	}
	else
	{
    if (obstacleImage == 0)
        currentObstacle = F1;
    else
        currentObstacle = F2;
	}

	// Hitbox for the player
	if (isSliding)
	{
    playerRec = (Rectangle){
        playerX,
        playerY + 30,
        slide.width - 10,
        slide.height - 10
    };
	}
	else
	{
    playerRec = (Rectangle){
        playerX,
        playerY,
        player.width - 10,
        player.height
    };
	}


	// Hitbox for the obstacle
	obstacleRec = (Rectangle){
    obstacleX + 25,
    obstacleY + 15,
    currentObstacle.width - 50,
    currentObstacle.height - 60
	};

	// check for collision between the player and the obstacle
	if (obstacleActive && CheckCollisionRecs(playerRec, obstacleRec))
    {
		gameOver = true;
    }
		}

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw the background
		DrawTexture(background, bgX1, 0, WHITE);
		DrawTexture(background, bgX2, 0, WHITE);

		// draw the player
		if (isSliding)
		{
			DrawTexture(slide, (int)playerX, (int)playerY + 30, WHITE);
		}
		else
		{
			if (currentFrame == 0)
			DrawTexture(player, (int)playerX, (int)playerY, WHITE);
			else
			DrawTexture(walk2, (int)playerX, (int)playerY, WHITE);
		}


		// draw the score
		DrawText(
    	TextFormat("Score: %d", (int)score),
    	1000,30,35,WHITE
		);

		// draw the obstacle Enemy
		if (obstacleActive)
		{
			DrawTexture(currentObstacle, (int)obstacleX, (int)obstacleY, WHITE);
		}

		// draw some text using the default font

		if (!gameStarted)
		{
    		DrawText("PRESS R TO START", 450, 140, 40, WHITE);
		}

		if (gameOver)
		{
    		DrawText("GAME OVER", 470, 100, 60, RED);

			DrawText(
        	TextFormat("Score: %d", (int)score),
        	520,
        	180,
        	40,
        	WHITE
			);

			DrawText("PRESS R TO RESTART", 400, 240, 40, WHITE);
		}

		// Debugging Collision Boxes
		//DrawRectangleLinesEx(playerRec, 2, GREEN);

		//if (obstacleActive)
		//{
   		// DrawRectangleLinesEx(obstacleRec, 2, RED);
		//}
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(player);
	UnloadTexture(walk2);
	UnloadTexture(background);
	UnloadTexture(G1);
	UnloadTexture(G2);
	UnloadTexture(F1);
	UnloadTexture(F2);
	UnloadTexture(slide);
	UnloadMusicStream(music);
	CloseAudioDevice();

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
