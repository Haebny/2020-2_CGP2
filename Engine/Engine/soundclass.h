#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class SoundClass
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	enum SoundType
	{
		BGM = 0,
		DAMAGE,
		CORRECT,
		SUCCESS
	};

public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool Initialize(HWND);
	void Shutdown();

	bool CheckPlaying();
	void PlayMusic(int soundType);

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayBGM();
	bool PlayDamageSound();
	bool PlayCorrectSound();
	bool PlaySuccessSound();
private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer8* m_secondaryBuffer1;
	IDirectSoundBuffer8* m_secondaryBuffer2;
	IDirectSoundBuffer8* m_secondaryBuffer3;
	IDirectSoundBuffer8* m_secondaryBuffer4;

	bool isPlaying;
	SoundType soundType;
};

#endif