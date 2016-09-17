/*
				
		Basic Spout receiver for Cinder

		Uses the Spout SDK

		Based on the RotatingBox CINDER example without much modification
		Nothing fancy about this, just the basics.

		Search for "SPOUT" to see what is required

	==========================================================================
	Copyright (C) 2014 Lynn Jarvis.

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	==========================================================================

	11.05.14 - used updated Spout Dll with host fbo option and rgba
	04.06.14 - used updated Spout Dll 04/06 with host fbo option removed
			 - added Update function
			 - moved receiver initialization from Setup to Update for sender detection
	11.07.14 - changed to Spout SDK instead of the dll
	29.09.14 - update with with SDK revision
	12.10.14 - recompiled for release
	03.01.15 - SDK recompile - SpoutPanel detected from registry install path
	04.02.15 - SDK recompile for default DX9 (see SpoutGLDXinterop.h)
	14.02.15 - SDK recompile for default DX11 and auto compatibility detection (see SpoutGLDXinterop.cpp)
	21.05.15 - Added optional SetDX9 call
			 - Recompiled for both DX9 and DX11 for new installer
	26.05.15 - Recompile for revised SpoutPanel registry write of sender name
	01.07.15 - Convert project to VS2012
			 - add a window title
	30.03.16 - Rebuild for 2.005 release - VS2012 /MT

*/

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"

#include "CiSpoutIn.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SpoutReceiverApp : public App {
public:
	void draw() override;
	void update() override;
	void mouseDown(MouseEvent event) override;

	SpoutIn	mSpoutIn;
};

void SpoutReceiverApp::update()
{
	if( mSpoutIn.getSize() != app::getWindowSize() ) {
		app::setWindowSize( mSpoutIn.getSize() );
	}
}

void SpoutReceiverApp::draw()
{
	gl::clear();

	auto tex = mSpoutIn.receiveTexture();
	if( tex ) {
		// Otherwise draw the texture and fill the screen
		gl::draw( tex, getWindowBounds() );

		// Show the user what it is receiving
		gl::ScopedBlendAlpha alpha;
		gl::enableAlphaBlending();
		gl::drawString( "Receiving from: " + mSpoutIn.getSenderName(), vec2( toPixels( 20 ), toPixels( 20 ) ), Color( 1, 1, 1 ), Font( "Verdana", toPixels( 24 ) ) );
		gl::drawString( "fps: " + std::to_string( (int)getAverageFps() ), vec2( getWindowWidth() - toPixels( 100 ), toPixels( 20 ) ), Color( 1, 1, 1 ), Font( "Verdana", toPixels( 24 ) ) );
		gl::drawString( "RH click to select a sender", vec2( toPixels( 20 ), getWindowHeight() - toPixels( 40 ) ), Color( 1, 1, 1 ), Font( "Verdana", toPixels( 24 ) ) );
	}
	else {
		gl::ScopedBlendAlpha alpha;
		gl::enableAlphaBlending();
		gl::drawString( "No sender/texture detected", vec2( toPixels( 20 ), toPixels( 20 ) ), Color( 1, 1, 1 ), Font( "Verdana", toPixels( 24 ) ) );
	}
}
void SpoutReceiverApp::mouseDown(MouseEvent event)
{
	if( event.isRightDown() ) { // Select a sender
		// SpoutPanel.exe must be in the executable path
		mSpoutIn.getSpoutReceiver().SelectSenderPanel(); // DirectX 11 by default
	}
}

void prepareSettings( App::Settings *settings )
{
	settings->setWindowSize( 320, 240 );
}

// This line tells Cinder to actually create the application
CINDER_APP( SpoutReceiverApp, RendererGl, prepareSettings )
