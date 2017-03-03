/*

Basic Spout sender for Cinder

Search for "SPOUT" to see what is required
Uses the Spout SDK

Based on the RotatingBox CINDER example without much modification
Nothing fancy about this, just the basics.

Search for "SPOUT" to see what is required

==========================================================================
Copyright (C) 2014-2017 Lynn Jarvis.

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
03-03-17 - Spout 2.006 release


*/
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/GeomIo.h"
#include "cinder/CameraUi.h"
#include "cinder/Camera.h"

#include "CiSpoutOut.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class _TBOX_PREFIX_App : public App {
public:
	_TBOX_PREFIX_App();
	void mouseDrag(MouseEvent event) override;
	void mouseDown(MouseEvent event) override;
	void mouseUp(MouseEvent event) override;
	void update() override;
	void draw() override;
	void resize() override;

	gl::BatchRef mBatch;

	CameraPersp		mCamera;
	CameraUi		mCameraUi;

	SpoutOut mSpoutOut;
};

_TBOX_PREFIX_App::_TBOX_PREFIX_App()
	: mCameraUi(&mCamera)
	, mSpoutOut("cispout", app::getWindowSize())
{
	auto teapot = geom::Teapot() >> geom::Scale(vec3(10));
	mBatch = gl::Batch::create(teapot, gl::getStockShader(gl::ShaderDef().lambert()));

	gl::enableDepth();
}

void _TBOX_PREFIX_App::mouseDrag(MouseEvent event)
{
	mCameraUi.mouseDrag(event);
}

void _TBOX_PREFIX_App::mouseDown(MouseEvent event)
{
	mCameraUi.mouseDown(event);
}

void _TBOX_PREFIX_App::mouseUp(MouseEvent event)
{
	mCameraUi.mouseUp(event);
}

void _TBOX_PREFIX_App::update()
{
}

void _TBOX_PREFIX_App::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::setMatrices(mCamera);
	mBatch->draw();

	mSpoutOut.sendViewport();
}

void _TBOX_PREFIX_App::resize()
{
	mCamera.setAspectRatio(app::getWindowAspectRatio());
}

CINDER_APP(_TBOX_PREFIX_App, RendererGl{ RendererGl::Options().msaa(16) })

