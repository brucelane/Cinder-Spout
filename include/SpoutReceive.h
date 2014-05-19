//
//					SpoutReceive.h
#pragma once

#ifndef __SpoutReceive__
#define __SpoutReceive__

#include <windows.h>
#include <gl/GL.h>
#include <set>
#include <string>

namespace SpoutReceive {

	// exported functions
	extern "C" _declspec(dllexport)
		bool InitReceiver(char *name, unsigned int& width, unsigned int& height, bool& bTextureShare, bool bMemoryShare = false);

	extern "C" _declspec(dllexport)
		bool ReleaseReceiver();

	extern "C" _declspec(dllexport)
		bool ReceiveTexture(char *name, GLuint TextureID, GLuint TextureTarget, unsigned int &width, unsigned int &height, GLuint HostFBO = 0);

	// extern "C" _declspec(dllexport)
	// bool TextureShareCompatible();

	extern "C" _declspec(dllexport)
		bool SelectSenderDialog();

	extern "C" _declspec(dllexport)
		bool SelectSenderPanel();

	extern "C" _declspec(dllexport)
		bool GetSenderNames(std::set<std::string> *sendernames);

	extern "C" _declspec(dllexport)
		int GetSenderCount();

	extern "C" _declspec(dllexport)
		bool GetSenderNameInfo(int index, char* sendername, int sendernameMaxSize, unsigned int &width, unsigned int &height, HANDLE &dxShareHandle);

	extern "C" _declspec(dllexport)
		bool GetSenderNameMap(char *sendermap);

	extern "C" _declspec(dllexport)
		bool GetSenderInfo(char* sendername, unsigned int &width, unsigned int &height, HANDLE &dxShareHandle);

}

#endif
