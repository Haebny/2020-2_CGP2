#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
//#include "bitmapclass.h"
#include "textclass.h"
#include "skyboxclass.h"

#include <vector>
using namespace std;

// Globals
const bool FULL_SCREEN = false;			// 풀스크린 
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;		// 스크린 깊이
const float SCREEN_NEAR = 0.1f;


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
	bool Frame(int, int, int, int, float);

	void GoForward();
	void GoLeft();
	void GoBack();
	void GoRight();

	//void MovePlayer(char);
	//void TurnLeft();
	//void TurnRight();

private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	vector<Model> m_Models;
	Model dog, cat, wood, floor;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	LightClass *m_Light2, *m_Light3, *m_Light4;
	TextureShaderClass* m_TextureShader;
	//BitmapClass* m_Bitmap;
	TextClass* m_Text;
	SkyboxClass* m_Skybox;

private:
	D3DXVECTOR3 CamPos, CamRot;
	float PreX, PreY;
	float speed;
};

#endif