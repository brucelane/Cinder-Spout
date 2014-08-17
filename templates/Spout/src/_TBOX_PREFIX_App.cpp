/*
                
        Basic Spout sender for Cinder

        Search for "SPOUT" to see what is required
        Uses the Spout dll

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

*/

#include "cinder/app/AppNative.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include <vector>

// spout
#include "spout.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class _TBOX_PREFIX_App : public AppNative {
public:
    void prepareSettings(Settings *settings);
	void setup();
	void update();
	void draw();
    void shutdown();
    
private:
    // spout
    // still using the cam vars from the cinder demo
    CameraPersp mCam;
    gl::Texture cubeTexture;
    Matrix44f   mCubeRotation;

    // -------- SPOUT -------------
    SpoutSender spoutsender;                    // Create a Spout sender object
    bool bInitialized;                          // true if a sender initializes OK
    bool bMemoryMode;                           // tells us if texture share compatible
    unsigned int g_Width, g_Height;             // size of the texture being sent out
    char SenderName[256];                       // sender name 
    gl::Texture spoutTexture;                   // Local Cinder texture used for sharing
    // ----------------------------

};

// -------- SPOUT -------------
void _TBOX_PREFIX_App::prepareSettings(Settings *settings)
{
        g_Width  = 640;
        g_Height = 512;
        settings->setWindowSize( g_Width, g_Height );
        settings->setFullScreen( false );
        settings->setResizable( false ); // keep the screen size constant for a sender
        settings->setFrameRate( 60.0f );
}
// ----------------------------

void _TBOX_PREFIX_App::setup()
{
    // load an image to texture the demo cube with
    cubeTexture = loadImage( "../assets/SpoutLogoMarble3.jpg" );
    
    mCam.lookAt( Vec3f( 3, 2, -3 ), Vec3f::zero() );
    mCubeRotation.setToIdentity();
    glEnable( GL_TEXTURE_2D );
    gl::enableDepthRead();
    gl::enableDepthWrite(); 

    // -------- SPOUT -------------
    // Set up the texture we will use to send out
    // We grab the screen so it has to be the same size
    spoutTexture =  gl::Texture(g_Width, g_Height);
    strcpy_s(SenderName, "CINDER Spout SDK Sender"); // we have to set a sender name first
    // Optionally test for texture share compatibility
    // bMemoryMode informs us whether Spout initialized for texture share or memory share
    bMemoryMode = spoutsender.GetMemoryShareMode();
    // Initialize a sender
    bInitialized = spoutsender.CreateSender(SenderName, g_Width, g_Height);
    // ----------------------------
}

void _TBOX_PREFIX_App::update()
{
    // Rotate the cube by .015 radians around an arbitrary axis
    mCubeRotation.rotate( Vec3f( 1, 1, 1 ), 0.015f );

    mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
    gl::setMatrices( mCam );
}

void _TBOX_PREFIX_App::draw()
{
    gl::clear( Color( 0.39f, 0.025f, 0.0f ) ); // red/brown to be different
    
    if( ! cubeTexture )
        return;

    cubeTexture.bind();
    glPushMatrix();
        gl::multModelView( mCubeRotation );
        gl::drawCube( Vec3f::zero(), Vec3f( 2.5f, 2.5f, 2.5f ) );
    glPopMatrix();
    cubeTexture.unbind();


    // -------- SPOUT -------------
    if(bInitialized) {

        // Grab the screen (current read buffer) into the local spout texture
        spoutTexture.bind();
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_Width, g_Height);
        spoutTexture.unbind();

        // Send the texture for all receivers to use
        // NOTE : if SendTexture is called with a framebuffer object bound, that binding will be lost
        // and has to be restored afterwards because Spout uses an fbo for intermediate rendering
        spoutsender.SendTexture(spoutTexture.getId(), spoutTexture.getTarget(), g_Width, g_Height);

    }

    // Show the user what it is sending
    char txt[256];
    sprintf_s(txt, "Sending as [%s]", SenderName);
    gl::setMatricesWindow( getWindowSize() );
    gl::enableAlphaBlending();
    gl::drawString( txt, Vec2f( toPixels( 20 ), toPixels( 20 ) ), Color( 1, 1, 1 ), Font( "Verdana", toPixels( 24 ) ) );
    sprintf_s(txt, "fps : %2.2d", (int)getAverageFps());
    gl::drawString( txt, Vec2f(getWindowWidth() - toPixels( 100 ), toPixels( 20 ) ), Color( 1, 1, 1 ), Font( "Verdana", toPixels( 24 ) ) );
    gl::disableAlphaBlending();
    // ----------------------------
}
// -------- SPOUT -------------
void _TBOX_PREFIX_App::shutdown()
{
    spoutsender.ReleaseSender();
}
// This line tells Cinder to actually create the application
CINDER_APP_NATIVE( _TBOX_PREFIX_App, RendererGl )
