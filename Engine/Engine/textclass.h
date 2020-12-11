#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

#define MAX_SENTENCES 20

#include "fontclass.h"
#include "fontshaderclass.h"
#include <vector>

class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);

	bool SetMousePosition(int, int, ID3D11DeviceContext*);

	bool SetScore(int, ID3D11DeviceContext*);
	bool SetResult(int, ID3D11DeviceContext*);
	bool SetGameUI(int, int, ID3D11DeviceContext*);
	bool SetCollision(bool, ID3D11DeviceContext*);
	bool SetState(bool, ID3D11DeviceContext*);
	bool SetTitle(int, ID3D11DeviceContext*);

//#ifdef DEBUG
	bool SetPos(float, float, ID3D11DeviceContext*);
//#endif

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;

	SentenceType* m_sentences[MAX_SENTENCES];
};
#endif