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
#include "particleshaderclass.h"
#include "particlesystemclass.h"

#include <vector>
#include <directxmath.h>
using namespace std;

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass {
private:
	enum State
	{
		IDLE,
		TRACE
	};
	struct Model
	{
		ModelClass* model;
		char* obj_path;
		WCHAR* tex_path;
		char* name;
		char* tag;
		D3DXVECTOR3 pos, startPos;
		D3DXVECTOR3 min, max;
		bool col;
		State state;
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

	void HoldGift();

	int GetScore();
	void IncreaseScore();

	void GhostFSM(float, Model&);

	void ShowGameResult();
	void StartGame();

	int GetResult();
	bool GetDamageTrigger();
	bool GetCorrectTrigger();
	bool GetSuccessTrigger();

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
		reindeer, horns, face,			// npc
		player, horns2, eyelips, nose,	// player
		house, house2, house3, house4,	// house
		treetop, treebottom, treetop11, treebottom11,// tree
		treetop2, treebottom2, treetop12, treebottom12,
		treetop3, treebottom3, treetop13, treebottom13,
		treetop4, treebottom4, treetop14, treebottom14,
		treetop5, treebottom5, treetop15, treebottom15,
		treetop6, treebottom6, treetop16, treebottom16,
		treetop7, treebottom7, treetop17, treebottom17,
		treetop8, treebottom8, treetop18, treebottom18,
		treetop9, treebottom9, treetop19, treebottom19,
		treetop10, treebottom10, treetop20, treebottom20,
		ghost, ghost2, ghost3, ghost4, ghost5, ghost6, ghost7,	// ghost
		gift, gift2, gift3;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	LightClass *m_Light2, *m_Light3, *m_Light4;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_TitleScene; // , m_ResultScene;
	TextClass* m_Text;
	SkyboxClass* m_Skybox;

	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem;

private:
	D3DXVECTOR3 m_CamPos, m_CamRot, playerPos, prePos;
	float PreX, PreY;
	float camSpeed, ghostSpeed;

	int m_Score, m_playerLife;
	INT64 currentTime, m_startTime;
	double mSecondsPerCount;

	enum Result
	{
		Playing,
		Failure,
		Success
	};

	int m_Result, box;
	bool isCollided, isImmortal,
		holdable, isHolding, isIncrease, isDamaged,
		titleScene, resultScene;
};

#endif 