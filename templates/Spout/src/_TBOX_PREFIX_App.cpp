/*

        Basic Spout sender for Cinder

        Search for "SPOUT" to see what is required
        Uses the Spout SDK

        Based on the RotatingBox CINDER example without much modification
        Nothing fancy about this, just the basics.

        Search for "SPOUT" to see what is required

    ==========================================================================
    Copyright (C) 2014-2016 Lynn Jarvis.

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
    04.06.14 - updated Spout dll 04/06 - host fbo option removed
    11.07.14 - changed to Spout SDK instead of the dll
    29.09.14 - updated to revised SDK
    12.10.14 - recompiled for release
    03.01.15 - SDK recompile
    04.02.15 - SDK recompile for default DX9 (see SpoutGLDXinterop.h)
    14.02.15 - SDK recompile for default DX11 and auto compatibility detection (see SpoutGLDXinterop.cpp)
    21.05.15 - Added optional SetDX9 call
             - Recompiled for both DX9 and DX11 for new installer
    26.05.15 - Recompile for revised SpoutPanel registry write of sender name
    01.07.15 - Convert project to VS2012
             - add a window title
    30-03-16 - Rebuild for Spout 2.005 release - VS2012 /MT

*/

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include <vector>

// spout
#include "spout.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class _TBOX_PREFIX_App : public App {
public:
 	void setup();
	void update();
	void draw();
    void mouseDown(MouseEvent event);
    void shutdown();

private:
    // spout
    // still using the cam vars from the cinder demo
    CameraPersp         mCam;
    gl::TextureRef      cubeTexture;
    mat4                mCubeRotation;
    gl::BatchRef        mBatchCube;
    gl::GlslProgRef     mGlslCube;

    // -------- SPOUT -------------
    SpoutSender         spoutsender;                    // Create a Spout sender object
    bool                bSenderInitialized;             // true if a sender initializes OK
    bool                bMemoryMode;                    // tells us if texture share compatible
    unsigned int        g_Width, g_Height;              // size of the texture being sent out
    char                SenderName[256];                // sender name
    gl::TextureRef      spoutSenderTexture;             // Local Cinder texture used for sharing
    // -------- SPOUT RECEIVER ----
    bool                bReceiverInitialized;           // true if a receiver initializes OK
    gl::TextureRef      spoutReceiverTexture;           // Local Cinder texture used for sharing
    char                ReceiverSenderName[256];        // sender name from the receiver
    SpoutReceiver       spoutreceiver;                  // Create a Spout receiver object
    bool bDoneOnce;                                     // only try to initialize once
    int nSenders;
    // ----------------------------

};

// -------- SPOUT -------------

void _TBOX_PREFIX_App::setup()
{
    g_Width  = 640;
    g_Height = 360;
    setWindowSize( g_Width, g_Height );
    setFullScreen( false );
    //setResizable( false ); // keep the screen size constant for a sender
    setFrameRate( 60.0f );
    // load an image to texture the demo cube with
    cubeTexture = gl::Texture::create(loadImage(loadAsset("SpoutLogoMarble3.jpg")), gl::Texture::Format().mipmap());
    mGlslCube = gl::GlslProg::create(loadAsset("cubeshader.vert"), loadAsset("cubeshader.frag"));
    mBatchCube = gl::Batch::create(geom::Cube(), mGlslCube);

    mCam.lookAt( vec3( 3, 2, 4 ), vec3(0) );

    gl::enableDepthRead();
    gl::enableDepthWrite();

    // -------- SPOUT -------------
    // Set up the texture we will use to send out
    // We grab the screen so it has to be the same size
    spoutSenderTexture =  gl::Texture::create(g_Width, g_Height);
    strcpy_s(SenderName, "CINDER Spout SDK Sender"); // we have to set a sender name first
    // Optionally set for DirectX 9 instead of default DirectX 11 functions
    //spoutsender.SetDX9(true);

    // Initialize a sender
    bSenderInitialized = spoutsender.CreateSender(SenderName, g_Width, g_Height);

    // Optionally test for texture share compatibility
    // bMemoryMode informs us whether Spout initialized for texture share or memory share
    bMemoryMode = spoutsender.GetMemoryShareMode();

    // -------- SPOUT RECEIVER -----
    bReceiverInitialized = false;
    spoutReceiverTexture = gl::Texture::create(g_Width, g_Height);

}

void _TBOX_PREFIX_App::update()
{
    // -------- SPOUT SENDER-------------
    // Rotate the cube by 0.2 degrees around the y-axis
    mCubeRotation *= rotate(toRadians(0.2f), normalize(vec3(0, 1, 0)));

    mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
    gl::setMatrices( mCam );
}

void _TBOX_PREFIX_App::draw()
{
    unsigned int width, height;
    char txtReceiver[256];

    gl::clear();

    gl::setMatrices(mCam);

    // Save current global width and height - they will be changed
    // by receivetexture if the sender changes dimensions
    width  = g_Width;
    height = g_Height;

    gl::clear( Color( 0.39f, 0.025f, 0.0f ) ); // red/brown to be different
    gl::color( Color( 1, 1, 1 ) );

    if( ! cubeTexture )
        return;

    gl::ScopedModelMatrix modelScope;
    gl::multModelMatrix(mCubeRotation);
    cubeTexture->bind();
    mBatchCube->draw();

    // -------- SPOUT SENDER-------------
    if (bSenderInitialized) {

        // Grab the screen (current read buffer) into the local spout texture
        spoutSenderTexture->bind();
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_Width, g_Height);
        spoutSenderTexture->unbind();

        // Send the texture for all receivers to use
        // NOTE : if SendTexture is called with a framebuffer object bound,
        // include the FBO id as an argument so that the binding is restored afterwards
        // because Spout uses an fbo for intermediate rendering
        spoutsender.SendTexture(spoutSenderTexture->getId(), spoutSenderTexture->getTarget(), g_Width, g_Height);


    }
}
void _TBOX_PREFIX_App::mouseDown(MouseEvent event)
{
    if( event.isRightDown() ) { // Select a sender
        // SpoutPanel.exe must be in the executable path
        spoutreceiver.SelectSenderPanel(); // DirectX 11 by default
    }
}
// -------- SPOUT -------------
void _TBOX_PREFIX_App::shutdown()
{
    spoutsender.ReleaseSender();
    spoutreceiver.ReleaseReceiver();
}
// This line tells Cinder to actually create the application
CINDER_APP( _TBOX_PREFIX_App, RendererGl )
