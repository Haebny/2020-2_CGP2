#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();
	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

	void SetKeyState(int);
	bool GetKeyState(int);

private:
	bool m_keys[256];
	bool num1, num2, num3, num4;
};

#endif