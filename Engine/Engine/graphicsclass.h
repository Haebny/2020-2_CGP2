#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"

// Globals
const bool FULL_SCREEN = false;			// 풀스크린 
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;		// 스크린 깊이
const float SCREEN_NEAR = 0.1f;

// Exercise 1-2: FULL_SCREEN = true;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	TextureShaderClass* m_TextureShader;
};

#endif