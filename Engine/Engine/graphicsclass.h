#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"

#include <vector>

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
		const char* obj_path;
		const WCHAR* tex_path;
		const char* name;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, char);

	void MovePlayer(char);
	void GoFoward();
	void GoBack();
	void TurnLeft();
	void TurnRight();

private:
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	vector<Model> m_Models;
	Model dog, cat, wood, floor;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	LightClass *m_Light2, *m_Light3, *m_Light4;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	Model m_Player;

	//int m_switch;
	char m_key;
	float PreX, PreY;
	D3DXVECTOR3 CamPos, CamRot;
	float speed;
};

#endif