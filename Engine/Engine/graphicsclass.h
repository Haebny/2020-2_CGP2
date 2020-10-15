#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

// Globals
const bool FULL_SCREEN = false;			// 풀스크린 
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;		// 스크린 깊이
const float SCREEN_NEAR = 0.1f;

// Exercise 1-2: FULL_SCREEN = true;

class GraphicsClass
{
public:
	struct Model
	{
		ModelClass* model;
		char* obj_path;
		WCHAR* tex_path;

		//bool isMoving;
		//D3DXVECTOR3 pos;
		//char* name;
	};
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	LightClass *m_Light1, *m_Light2, *m_Light3, *m_Light4;
};

#endif