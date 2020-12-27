#pragma once

#include "d3dclass.h"
#include "cameraclass.h"

class GameManagerClass
{
public:
	GameManagerClass();
	GameManagerClass(const GameManagerClass&);
	~GameManagerClass();

	void GoForward(float);
	void GoLeft(float);
	void GoBack(float);
	void GoRight(float);

	D3DXVECTOR3 GetCamPosition();
	D3DXVECTOR3 GetCamRotation();

	void SetCamView(CameraClass& m_Camera, int mouseX, int mouseY);

private:
	D3DXVECTOR3 m_CamPos, m_CamRot;
	float m_PreX, m_PreY, m_CamSpeed;
};

