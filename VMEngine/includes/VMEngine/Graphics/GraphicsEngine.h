#pragma once
#include "VMEngine/CoreMinimal.h"
#include "SDL2//SDL.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	//Initialised Graphic Engine/ the Window
	bool InitGE(const char* WTitle, bool bFullscreen, int WWidth, int WHeight);

	//Show the newest frame graphic
	void PresentGraphics();
	//Clear the old frame
	void ClearGraphics();

	//Handle the rendering of our graphics
	//Return the sdl window
	void Draw();

	SDL_Window* GetWindow() const;

	//Add a new VAO to the VAOStack using a shape
	void CreateVAO(GeometricShapes Shape);

	void CreateShader(VFShaderParams ShaderFilePahts);

private:
	SDL_Window* SdlWindow;
	SDL_GLContext SdlGLContext;
	//Stack all the VAOs
	VAOStack VAOs;
	//Handle wireframe mode
	void HandleWireFrameMode(bool bShowWireFrameMode);
	//set wireframe
	bool bWireFrameMode;

	//single shader
	ShaderPtr Shader;
};