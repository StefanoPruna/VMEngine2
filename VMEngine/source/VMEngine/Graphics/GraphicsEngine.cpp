#include "VMEngine/Graphics/GraphicsEngine.h"
#include "GLEW/glew.h"
#include "VMEngine/Graphics/VertexArrayObject.h"
#include "VMEngine/CoreMinimal.h"
#include "VMEngine/Graphics/ShaderProgram.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

GraphicsEngine::GraphicsEngine()
{
	SdlWindow = nullptr;
	SdlGLContext = NULL;
}

GraphicsEngine::~GraphicsEngine()
{
	//Delete the SDL window from memory
	SDL_DestroyWindow(SdlWindow);
	//destroy the GL 
	SDL_GL_DeleteContext(SdlGLContext);
	//close the SDL framework
	SDL_Quit();

	cout << "Destroy the Graphic Engine..." << "\n";
}

bool GraphicsEngine::InitGE(const char* WTitle, bool bFullscreen, int WWidth, int WHeight)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL failed: " << SDL_GetError() << "\n";
		return false;
	}

	//Use OpenGL 4.6 compatibility and set default
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);

	//Set the fullscreen
	int FullscreenFlag = 0;

	if (bFullscreen)
	{
		FullscreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
	}
	else
	{
		FullscreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	}

	//Create the SDL2 window
	SdlWindow = SDL_CreateWindow(
		WTitle,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WWidth, WHeight,
		FullscreenFlag
	);

	if (SdlWindow == nullptr)
	{
		cout << "SDL window failed: " << SDL_GetError() << "\n";
		return false;
	}

	//add the GL attributes to the window
	SdlGLContext = SDL_GL_CreateContext(SdlWindow);

	if (SdlGLContext == NULL)
	{
		cout << "SDL GL context failed: " << SDL_GetError() << "\n";
		return false;
	}

	//To make 
	glewExperimental = GL_TRUE;

	const GLenum InitGLEW = glewInit();

	if (InitGLEW != GLEW_OK)
	{
		cout << "GLEW failed: " << glewGetErrorString(InitGLEW) << "\n";
		return false;
	}

	return true;
}

void GraphicsEngine::PresentGraphics()
{
	//Present the new graphics using OpenGL
	SDL_GL_SwapWindow(SdlWindow);
}

void GraphicsEngine::ClearGraphics()
{
	//R G B A for background
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	//Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
}

void GraphicsEngine::Draw()
{
	ClearGraphics();

	HandleWireFrameMode(false);

	vmuint index = 0;
	//foreach loop
	for (VAOPtr VAO : VAOs)
	{
		Shader->RunShader();

		//move the object
		glm::mat4 transform = glm::mat4(1.0f);

		if (index == 0)//Triangle
		{
			//move in the x, y or z direction based on the amount added
			transform = glm::translate(transform, glm::vec3(-0.5f, 0.0, 0.0f));
			//radians is rotation amount
			//vec3 is the direction to rotate in
			transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::scale(transform, glm::vec3(0.75f, 0.75f, 1.0f));
		}		
		else if (index == 1)//Square
		{
			transform = glm::translate(transform, glm::vec3(0.5f, 0.0, 0.0f));
			//x and y will work for our 2D shapes
			//z must be larger than 0 to see the object
			transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));
		}
		else if(index == 2) //Circle
		{
			//move in the x, y or z direction based on the amount added
			transform = glm::translate(transform, glm::vec3(-0.25f, 0.5f, 0.0f));
			//radians is rotation amount
			//vec3 is the direction to rotate in
			transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));
		}
		else if (index == 3)//Arrow
		{
			transform = glm::translate(transform, glm::vec3(0.5f, 0.70, 0.0f));
			//x and y will work for our 2D shapes
			//z must be larger than 0 to see the object
			transform = glm::rotate(transform, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::scale(transform, glm::vec3(0.25f, 0.25f, 1.0f));
		}

		Shader->SetMat4("transform", transform);

		//Draw each VAO
		VAO->Draw();

		index++;
	}

	index = 0;

	PresentGraphics();
}

SDL_Window* GraphicsEngine::GetWindow() const
{
	return SdlWindow;
}

void GraphicsEngine::CreateVAO(GeometricShapes Shape)
{
	//Create a new VAO as a shared pointer
	VAOPtr NewVAO = make_shared<VAO>(Shape);
	//Assign value/object to the stack
	VAOs.push_back(NewVAO);
}

void GraphicsEngine::CreateShader(VFShaderParams ShaderFilePaths)
{
	ShaderPtr NewShader = make_shared<ShaderProgram>();

	NewShader->InitVFShader(ShaderFilePaths);

	Shader = NewShader;
}

void GraphicsEngine::HandleWireFrameMode(bool bShowWireFrameMode)
{
	//if wireframe mode is set, change it and viceversa
	if (bShowWireFrameMode != bWireFrameMode)
	{
		//change how OpenGL renders between the vertices
		bWireFrameMode = bShowWireFrameMode;
		if (bWireFrameMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		cout << "WireFrame mode updated..." << endl;
	}
}
