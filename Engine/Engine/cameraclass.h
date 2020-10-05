#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <d3dx10math.h>

// 어떻게 우리가 장면을 보는지에 대한 정보를 DirectX 11에게 전달
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;	// 관리하기가 편하기 때문에 행렬 사용
};

#endif