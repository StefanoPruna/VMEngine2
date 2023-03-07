#include "VMEngine/Game.h"
#include "VMEngine/Graphics/GraphicsEngine.h"
<<<<<<< Updated upstream
#include "VMEngine/Graphics/ShaderProgram.h"
#include "VMEngine/Graphics/Texture.h"

=======
#include "VMEngine/Graphics/Mesh.h"
>>>>>>> Stashed changes

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
	//DefaultTexture = nullptr;
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
		ShaderPtr TextureShader = Graphics->CreateShader({
			/*L"Game/Shaders/VertexShader/VertexShader.svert",
			L"Game/Shaders/VertexShader/VertexShader.sfrag"*/
			L"Game/Shaders/TextureShader/TextureShader.svert",
			L"Game/Shaders/TextureShader/TextureShader.sfrag"
			});

<<<<<<< Updated upstream
		/*DefaultTexture = make_shared<Texture>();

		if (!DefaultTexture->CreateTextureFromFilePath("Game/Textures/brick_pavement.jpg"))
			DefaultTexture = nullptr;*/

		Graphics->CreateTexture("Game/Textures/brick_pavement.jpg");
=======
		TexturePtr TConcrete = Graphics->CreateTexture("Game/Textures/brick_pavement.jpg");
		TexturePtr TGrid = Graphics->CreateTexture("Game/Textures/RectStones.jpg");
		TexturePtr TStones = Graphics->CreateTexture("Game/Textures/SquareStones.jpg");
		TexturePtr TBricks = Graphics->CreateTexture("Game/Textures/SquareBrown.jpg");
>>>>>>> Stashed changes

		//Create the vertex
		Poly = Graphics->CreateSimpleMeshShape(GeometricShapes::Polygon, TextureShader, { TConcrete });
		Tri = Graphics->CreateSimpleMeshShape(GeometricShapes::Triangle, TextureShader, { TGrid });
		//Graphics->CreateSimpleMeshShape(GeometricShapes::Circle);
		//Graphics->CreateSimpleMeshShape(GeometricShapes::Arrow);

		Poly->Transform.Location.x = 0.5f;
		Tri->Transform.Location.x = -0.5f;
		Poly->Transform.Scale = Vector3(0.5f);
		Tri->Transform.Scale = Vector3(0.7f);
		Poly->Transform.Rotation.z = 45.0f;
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
	//set delta time first always
	static double LastFrameTime = 0.0;
	//set current time since the game has passed
	double CurrentFrameTime = static_cast<double>(SDL_GetTicks64());
	//find the time difference between the last and current frame
	double NewDeltaTime = CurrentFrameTime - LastFrameTime;
	//set delta time as seconds
	DeltaTime = NewDeltaTime / 1000.0;
	//update the last frame time for the next update
	LastFrameTime = CurrentFrameTime;

	Poly->Transform.Rotation.z += 25.0f * GetFDeltaTime();

	static int MoveUp = 1.0f;

	if (Tri->Transform.Location.y > 0.5f)
		MoveUp = -1.0f;

	else if (Tri->Transform.Location.y < -0.5f)
		MoveUp = 1.0f;

	Tri->Transform.Location.y += (0.9f * MoveUp) * GetFDeltaTime();
}

void Game::Draw()
{
	Graphics->Draw();
}

void Game::CloseGame()
{
	//TO DO end the game
}