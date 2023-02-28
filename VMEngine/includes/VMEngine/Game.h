#pragma once
#include "VMEngine/Graphics/GraphicsEngine.h"
#include "CoreMinimal.h"

class Game
{
public:
	//Get the game instance or create one if it doesn't exit
	static Game& GetGameInstance();

	//Destroy the game running the destructor
	//Void because we don't want to return any values
	static void DestroyGameInstance();

	void Start(const char* WTitle, bool bFullscreen, int WWidth, int WHeight);

private:
	Game();
	~Game();

	void Run();

	//Handle the Input
	void ProcessInput();

	//Does the logic/Output
	void Update();

	//Drawing the graphics to the screen
	void Draw();

	//Anything that needs to be deleted at the end of the game
	void CloseGame();

	//boolean that ends the game
	bool bIsGameOver;

	GraphicsEnginePtr Graphics;

	//Load a default texture
	/*TexturePtr DefaultTexture;*/
};