#include "GameManagerClass.h"


GameManagerClass::GameManagerClass()
{
	m_PreX = 0.0f;
	m_PreY = 0.0f;
	m_CamSpeed = 0.1f;

	m_CamPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CamRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

GameManagerClass::GameManagerClass(const GameManagerClass &)
{
}

GameManagerClass::~GameManagerClass()
{
}

/// Camera Movements
void GameManagerClass::GoForward(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * m_CamSpeed * frameTime;
}

void GameManagerClass::GoLeft(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(-1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * m_CamSpeed * frameTime;
}

void GameManagerClass::GoBack(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(0.0f, 0.0f, -1.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * m_CamSpeed * frameTime;
}

void GameManagerClass::GoRight(float frameTime)
{
	D3DXMATRIX Dir;
	D3DXMatrixIdentity(&Dir);
	D3DXVECTOR3 Direction(1.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&Dir, m_CamRot.y * 0.0174532925f, m_CamRot.x * 0.0174532925f, m_CamRot.z * 0.0174532925f);
	D3DXVec3TransformCoord(&Direction, &Direction, &Dir);

	m_CamPos += Direction * m_CamSpeed * frameTime;
}

D3DXVECTOR3 GameManagerClass::GetCamPosition()
{
	return m_CamPos;
}

D3DXVECTOR3 GameManagerClass::GetCamRotation()
{
	return m_CamRot;
}

void GameManagerClass::SetCamView(CameraClass& m_Camera, int mouseX, int mouseY)
{
	// Set the camera movement.
	m_CamRot.y -= (m_PreX - mouseX) * 0.1f;
	m_CamRot.x -= (m_PreY - mouseY) * 0.1f;

	m_PreX = (float)mouseX;
	m_PreY = (float)mouseY;

	m_Camera.SetPosition(m_CamPos.x, m_CamPos.y, m_CamPos.z);
	m_Camera.SetRotation(m_CamRot.x, m_CamRot.y, m_CamRot.z);
}
