#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2\SDL.h>
#include <SDL2\SDL_render.h>
#include <SDL2\SDL_image.h>

int main()
{
	SDL_SetMainReady();

	//store flags
	int renderFlags = SDL_RENDERER_ACCELERATED;
	int windowsFlags = 0;

	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	//initialize SDL windows
	SDL_Window* window = SDL_CreateWindow("My 2D Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowsFlags);
	if (!window)
	{
		printf("Failed to open window: %s", SDL_GetError());
		exit(1);
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	//setup renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, renderFlags);
	if (!renderer)
	{
		printf("Failed to create renderer: %s", SDL_GetError());
		exit(1);
	}

	//load character
	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
	{
		printf("DEBUG %s", SDL_GetError());
		exit(1);
	}
	SDL_Texture* texture;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
		SDL_LOG_PRIORITY_INFO,
		"Loading character sprite");
	texture = IMG_LoadTexture(renderer, "randy.png");

	//control flags for player
	bool movingUp = false;
	bool movingDown = false;
	bool movingRight = false;
	bool movingLeft = false;
	int playerCurrentPositionX = 100;
	int playerCurrentPositionY = 100;
	int speedModifier = 1;

	//main loop
	while (true)
	{
		//update scene
		SDL_SetRenderDrawColor(renderer, 180, 60, 20, 255);
		SDL_RenderClear(renderer);

		//handle input
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					exit(0);
					break;
				case SDL_KEYDOWN:
					if (event.key.repeat == 0)
					{
						if (event.key.keysym.scancode == SDL_SCANCODE_UP)
						{
							movingUp = true;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
						{
							movingDown = true;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
						{
							movingRight = true;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
						{
							movingLeft = true;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
						{
							speedModifier = 2;
						}
					}
					break;
				case SDL_KEYUP:
					if (event.key.repeat == 0)
					{
						if (event.key.keysym.scancode == SDL_SCANCODE_UP)
						{
							movingUp = false;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
						{
							movingDown = false;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
						{
							movingRight = false;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
						{
							movingLeft = false;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
						{
							speedModifier = 1;
						}
					}
					break;
				default:
					break;
			}
		}

		//draw textures
		SDL_Rect dest;
		if (movingUp)
		{
			playerCurrentPositionY -= 1 * speedModifier;
		}
		if (movingDown)
		{
			playerCurrentPositionY += 1 * speedModifier;
		}
		if (movingRight)
		{
			playerCurrentPositionX += 1 * speedModifier;
		}
		if (movingLeft)
		{
			playerCurrentPositionX -= 1 * speedModifier;
		}

		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

		//restrict player to bounds of screen
		if (playerCurrentPositionX + dest.w >= 1280)
		{
			playerCurrentPositionX = 1280 - dest.w;
		}
		if (playerCurrentPositionX < 0)
		{
			playerCurrentPositionX = 0;
		}
		if (playerCurrentPositionY + dest.h >= 720)
		{
			playerCurrentPositionY = 720 - dest.h;
		}
		if (playerCurrentPositionY < 0)
		{
			playerCurrentPositionY = 0;
		}

		dest.x = playerCurrentPositionX;
		dest.y = playerCurrentPositionY;

		SDL_RenderCopy(renderer, texture, NULL, &dest);

		//render scene
		SDL_RenderPresent(renderer);
	}
}
