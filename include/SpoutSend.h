//
//					Spout.h
// 
#pragma once

#ifndef __SpoutSend__ // traditional as well
#define __SpoutSend__

#include <windows.h>
#include <gl/GL.h>

namespace SpoutSend {
	
	// exported functions
	extern "C" _declspec(dllexport)
	bool InitSender(char *name, unsigned int width, unsigned int height, bool& bTextureShare, bool bMemoryShare = false);

	extern "C" _declspec(dllexport)
	bool ReleaseSender();

	extern "C" _declspec(dllexport)
	bool SendTexture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert = false, GLuint HostFBO = 0);

	extern "C" _declspec(dllexport)
	bool TextureShareCompatible();

}

#endif
