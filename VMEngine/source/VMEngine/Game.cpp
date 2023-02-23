#include "VMEngine/Game.h"

Game& Game::GetGameInstance()
{
	//Create the game only one time, because it's static
	static Game* GameInstance = new Game();
	return *GameInstance;
}

void Game::DestroyGameInstance()
{
	static Game* GameInstance = &GetGameInstance();

	delete GameInstance;
}

void Game::Start(const char* WTitle, bool bFullscreen, int WWidth, int WHeight)
{
	Graphics = make_shared<GraphicsEngine>();

	if (!Graphics->InitGE(WTitle, bFullscreen, WWidth, WHeight))
	{
		bIsGameOver = true;
	}

	Run();
}

Game::Game()
{
	cout << "Game Initialised" << "\n";

	Graphics = nullptr;

	bIsGameOver = false;
}

Game::~Game()
{
	//Destroy the graphics from memory with share pointer
	Graphics = nullptr;
	cout << "Game Over..." << endl;
}

void Game::Run()
{
	if (!bIsGameOver)
	{
		Graphics->CreateShader({
			L"Game/Shaders/SimpleShader/SimpleShader.svert",
			L"Game/Shaders/SimpleShader/SimpleShader.sfrag"
			});
		//Create the vertex
		Graphics->CreateVAO(GeometricShapes::Triangle);
		Graphics->CreateVAO(GeometricShapes::Polygon);
		Graphics->CreateVAO(GeometricShapes::Circle);
		Graphics->CreateVAO(GeometricShapes::Arrow);
	}

	while (!bIsGameOver)
	{
		ProcessInput();

		Update();

		Draw();
	}

	CloseGame();
}

void Game::ProcessInput()
{
	SDL_Event PollEvent;

	//Using ampisand
	while (SDL_PollEvent(&PollEvent))
	{
		switch (PollEvent.type)
		{
		case SDL_QUIT:
			bIsGameOver = true;
			break;
		default:
			break;
		}
	}
}

void Game::Update()
{
	//TO DO logic
}

void Game::Draw()
{
	Graphics->Draw();
}

void Game::CloseGame()
{
	//TO DO end the game
}