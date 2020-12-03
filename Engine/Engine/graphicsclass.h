#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_ 

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"
#include "skyboxclass.h"

#include <vector>
#include <ctime>
using namespace std;

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass {
private:
	struct Model
	{
		ModelClass* model;
		char* obj_path;
		WCHAR* tex_path;
		char* name;
		char* tag;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 min, max;
	};

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, int, int, float);

	void GoForward(float);
	void GoLeft(float);
	void GoBack(float);
	void GoRight(float);

	void NevigateForward(float);
	void NevigateLeft(float);
	void NevigateBackward(float);
	void NevigateRight(float);

	int GetScore();
	void IncreaseScore();

	void EnemyFSM(float);

	void ShowGameResult();
	void RestartGame();
	void StartGame();

	int GetResult();

	// 충돌 처리
	bool CheckAABB(Model, Model);

	void CountSeconds(float);

public:
	int m_input;

private:
	bool Render(float);

private:
	CameraClass* m_Camera;
	D3DClass* m_D3D;
	vector<Model> m_Models;
	Model floor,
		reindeer, horns, face,
		player, horns2, eyelips, nose,
		treetop, treebottom,
		ghost,
		house;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	LightClass *m_Light2, *m_Light3, *m_Light4;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_TitleScene, m_ResultScene;
	TextClass* m_Text;
	SkyboxClass* m_Skybox;

private:
	D3DXVECTOR3 m_CamPos, m_CamRot, playerPos, playerRot;
	float PreX, PreY;
	float camSpeed;

	int m_Score, m_playerLife;
	INT64 currentTime, m_startTime;
	double mSecondsPerCount;

	enum Result
	{
		Playing,
		Failure,
		Success
	};

	int m_Result;
	bool isTimeOver, isCollided, isImmortal;
};

#endif 