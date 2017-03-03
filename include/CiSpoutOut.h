#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "spout.h"

#include <string>

namespace cinder {

	class SpoutOut {
	public:
		SpoutOut( const std::string& name, const ivec2& size )
			: mMemorySharedMode{ false }
			, mSize{ size }
			, mName{ name }
		{
			if( mSpoutSender.CreateSender( mName.c_str(), mSize.x, mSize.y ) ) {
				mMemorySharedMode = mSpoutSender.GetMemoryShareMode();
				CI_LOG_I( "Memory share: " << mMemorySharedMode );
				mTexture = gl::Texture2d::create( mSize.x, mSize.y, gl::Texture::Format().loadTopDown() );
			}
			else {
				throw std::exception( " Failed to initialize sender." );
			}
		}

		~SpoutOut() {
			mSpoutSender.ReleaseSender();
		}

		void sendTexture( const gl::Texture2dRef& texture ) {
			if( glm::ivec2( mSize ) != texture->getSize() ) {
				mSize = texture->getSize();
				resize();
			}
			else {
				mSpoutSender.SendTexture( texture->getId(), texture->getTarget(), texture->getWidth(), texture->getHeight() );
			}
		}

		void sendViewport() {
			mSize = app::getWindowSize();
			if( glm::ivec2( mSize ) != mTexture->getSize() ) {
				resize();
			}
			else {
				mTexture->bind();
				glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, mSize.x, mSize.y );
				mTexture->unbind();
				mSpoutSender.SendTexture( mTexture->getId(), mTexture->getTarget(), mSize.x, mSize.y );
			}
		}

		glm::ivec2				getSize() const { return mSize; }
		std::string				getName() const { return mName; }
		SpoutSender&			getSpoutSender() { return mSpoutSender; }
		const SpoutSender&		getSpoutSender() const { return mSpoutSender; }
		bool					isMemoryShareMode() const { return mMemorySharedMode; }
	private:
		bool resize()
		{
			if( mTexture && mSize == glm::uvec2( mTexture->getSize() ) )
				return false;

			mSpoutSender.UpdateSender( mName.c_str(), mSize.x, mSize.y );
			
			mTexture = gl::Texture2d::create( mSize.x, mSize.y, gl::Texture::Format().loadTopDown() );
			CI_LOG_I( "Recreated texture with size: " << mSize );
			return true;
		}

		bool				mMemorySharedMode;
		char				mSenderName[256]; // sender name
		std::string			mName;
		SpoutSender			mSpoutSender;	// Create a Spout receiver object
		gl::Texture2dRef	mTexture;
		glm::uvec2			mSize;
	};

} // end namespace ci

