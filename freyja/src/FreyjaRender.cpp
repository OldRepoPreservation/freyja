/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaRender
 * License : No use w/o permission (C)2001 Mongoose
 * Comments: GL context rendering backend for Freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.07.28:
 * Mongoose - Update documentation and prune obsoloete methods
 *
 * 2000.08.25:
 * Mongoose - Created, based on GooseEgg view prototype
 *
 ==========================================================================*/

#include "config.h"

#warning FIXME This class should not be using OpenGL directly.
#include "opengl_config.h" // Includes windows.h, so avoid header interaction

#include "FreyjaOpenGL.h"

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include <freyja/CameraABI.h>
#include <freyja/LightABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/Bone.h>
#include <freyja/Mesh.h>
#include <freyja/MeshABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/Metadata.h>

#include "freyja_events.h"
#include "Plugins.h"
#include "Control.h"
#include "Cursor.h"
#include "MaterialControl.h"
#include "OpenGLRenderableStrategy.h"

#include "FreyjaRender.h"



using namespace freyja;
using namespace freyja3d;

FreyjaRender *FreyjaRender::mInstance = NULL;

hel::Ray FreyjaRender::mTestRay;
OpenGLPrinter FreyjaRender::mPrinter;

vec4_t FreyjaRender::mColors[16];
vec4_t FreyjaRender::mColorBackground;
vec4_t FreyjaRender::mColorText;
vec4_t FreyjaRender::mColorWireframe;
vec4_t FreyjaRender::mColorWireframeHighlight;
vec4_t FreyjaRender::mColorBone;
vec4_t FreyjaRender::mColorBoneHighlight;
vec4_t FreyjaRender::mColorJoint;
vec4_t FreyjaRender::mColorJointHighlight;
vec4_t FreyjaRender::mColorBoundingBox;
vec4_t FreyjaRender::mColorGridSeperator;
vec4_t FreyjaRender::mColorGridLine;
vec4_t FreyjaRender::mColorVertex;
vec4_t FreyjaRender::mColorVertexHighlight;
vec4_t FreyjaRender::mColorAxisX;
vec4_t FreyjaRender::mColorAxisY;
vec4_t FreyjaRender::mColorAxisZ;
//unsigned int  FreyjaRender::mRenderMode = 0;
unsigned int FreyjaRender::mSelectedBone = 0;
unsigned int FreyjaRender::mBoneRenderType = 2;
unsigned char FreyjaRender::mJointRenderType = 1;
int FreyjaRender::mPatchDisplayList = -1;

vec_t FreyjaRender::mBoneLineWidth = 3.0f;        /* Custom artifact size */
vec_t FreyjaRender::mBonePointSize = 5.0f;
vec_t FreyjaRender::mDefaultPointSize = 3.5f;
vec_t FreyjaRender::mDefaultLineWidth = 1.0f;
vec_t FreyjaRender::mVertexPointSize = 3.5f;

int FreyjaRender::EvModeAutoKeyframeId = -1;

matrix_t gModelViewMatrix;

freyja3d::Cursor gCursor;
freyja3d::Cursor& GetCursor()
{ return gCursor; }

index_t GetSelectedCamera()
{ return 0; }

freyja::Material* GetSelectedMaterial()
{ return MaterialControl::GetInstance()->GetCurrentMaterial(); }

 
FreyjaRender::FreyjaRender() :
	mTimer(),
	mViewMode( VIEWMODE_MODEL_EDIT ),
	mRenderMode(fBoundingVolSelection | 
				fBonesNoZbuffer | 
				fBoundingVolumes |
				fFPSCap |
				fGroupColors),
	mSelectedView( PLANE_FREE ),
	mSelectedViewport( 0 ),
	mZoom(1.0f),
	mScroll( 0.0f, -18.0f, 0.0f ),
	mMaterialYAngle( 0.0f )
{
	mAngles[0] = 18.0f;
	mAngles[1] = 42.0f;
	mAngles[2] = 0.0f;


	for (long i = 0; i < 3; ++i)
	{
		mColorAxisX[i] = RED[i] * 0.75f;
		mColorAxisY[i] = GREEN[i] * 0.75f;
		mColorAxisZ[i] = BLUE[i] * 0.75f;
	}

	mViewports[0].plane = PLANE_FRONT;
	mViewports[1].plane = PLANE_LEFT;
	mViewports[2].plane = PLANE_TOP;
	mViewports[3].plane = PLANE_FREE;

	// To avoid branching in renderer by using a little more memory.
	for (uint32 i = 0; i < 3; ++i)
	{
		mColors[ 0][i] = WHITE[i];
		mColors[ 1][i] = NEXT_PURPLE2[i];
		mColors[ 2][i] = PINK[i];
		mColors[ 3][i] = GREEN[i];
		mColors[ 4][i] = YELLOW[i];
		mColors[ 5][i] = CYAN[i];
		mColors[ 6][i] = DARK_YELLOW[i];
		mColors[ 7][i] = DARK_BLUE[i];
		mColors[ 8][i] = ORANGE[i];  // FIXME, needs more colors
		mColors[ 9][i] = YELLOW[i]*0.25f;
		mColors[10][i] = YELLOW[i]*0.50f;
		mColors[11][i] = GREEN[i]*0.25f;
		mColors[12][i] = GREEN[i]*0.50f;
		mColors[13][i] = YELLOW[i]*0.75f;
		mColors[14][i] = GREEN[i]*0.75f;
		mColors[15][i] = PINK[i]*0.50f;
	}
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


/* Mongoose 2004.03.26, 
 * You have to be kidding me 
 * Replace with quarternion/matrix and change API  */
void FreyjaRender::Rotate(float n, int axis)
{
	mAngles[axis] += n;

	if (mAngles[axis] >= 360.0)
	{
		mAngles[axis] = 0.0;
	}
	else if (mAngles[axis] < 0.0)
	{
		mAngles[axis] = 360.0;
	}
}

void FreyjaRender::Rotate(int flags, float n)
{
	if (flags & X_F)
		Rotate(n, 0);

	if (flags & Y_F)
		Rotate(n, 1);

	if (flags & Z_F)
		Rotate(n, 2);
}


void FreyjaRender::DrawScene( freyja::Scene* scene )
{
	if ( scene )
	{
		glPushAttrib( GL_ENABLE_BIT );
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glPointSize(mVertexPointSize);

		glDisable(GL_TEXTURE_2D);
		glColor3fv( WHITE );

		freyja::Material* last_material = NULL;

		for ( RenderableIterator it = scene->GetRenderListIterator( ), end = it.end();
			  it != end; it++ )
		{
			glPushMatrix( );

			/* Translate. */
			{
				const hel::Vec3& pos = (*it)->GetWorldPosition( );
				glTranslatef( pos.mX, pos.mY, pos.mZ );
			}

			/* Rotate. */
			{
				const hel::Quat& q = (*it)->GetWorldOrientation( );
				vec_t theta;
				hel::Vec3 v;
				q.GetAxisAngles( theta, v );
				glRotatef( theta, v.mX, v.mY, v.mZ );
			}

			/* Scale. */
			{
				hel::Vec3 v = (*it)->GetScale( );
				glScalef( v.mX, v.mY, v.mZ );
			}

			if ( (*it)->GetMaterial( ) != last_material )
			{
				// FIXME: Set new material / shader here or use a renderable to set it inline for sorted renderlist.
				OpenGLRenderableStrategy::ApplyMaterial( (*it)->GetMaterial( ) );
				last_material = (*it)->GetMaterial( );
			}

			OpenGLRenderableStrategy strategeryizeification;
			(*it)->Draw( &strategeryizeification );

			// FIXME: Stop gap fix to lack of pre/post processing currently.
			glDisable( GL_BLEND );
			freyja3d::OpenGL::BindFragmentGLSL( 0 );

			glPopMatrix( );
		}

		glPopAttrib( );
	}
}


void FreyjaRender::DrawCamWindow()
{
	hel::Vec3 pos, target;
	hel::Vec3 up(0.0f, 1.0f, 0.0f);

#warning FIXME Update to 10.x API.
#if FIXME
	// FIXME: Use viewport's selected camera.
	freyja_ptr camera = GetSelectedCamera( );
	freyjaNodeGetPosition(camera, pos.mVec);
	freyjaCameraGetTarget3fv(camera, target.mVec);
	freyjaCameraGetUp3fv(camera, up.mVec);
#endif
	
	//glLoadIdentity();

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(90.0f, OpenGL::GetInstance()->GetAspectRatio(), 4.0f, 9000.0f);

	gluLookAt(pos.mX, pos.mY, pos.mZ,
			  target.mX, target.mY,target.mZ,
			  up.mX, up.mY, up.mZ);
	
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	if (mRenderMode & fSolidPlane)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glColor3fv(WHITE);
		OpenGLRenderableStrategy::ApplyMaterial( GetSelectedMaterial() );
		mglDrawPlane(50.0f, 2.0f, 1.0f);
		glPopAttrib();
	}
	else if (mRenderMode & fGrid)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		glLineWidth(1.25f);
		mglDrawGrid(mColorGridLine, 50.0f, 2.0f, 1.0f);

		glColor3fv(mColorGridLine);
		glLineWidth(4.0f);
		glBegin(GL_LINES);
		//glColor3fv(RED);
		glVertex3f(-50.0f, 0.0f, 0.0f);	
		glVertex3f(50.0f, 0.0f, 0.0f);
		//glColor3fv(BLUE);
		glVertex3f(0.0f, 0.0f, -50.0f);	
		glVertex3f(0.0f, 0.0f, 50.0f);
		glEnd();

#if 1
		//glColor3fv(mColorGridLine);
		glLineWidth(1.75f);
		glBegin(GL_LINES);

		for (float x = -50.0f; x < 60.0f; x += 10.f)
		{
			if (x < 10.0f && x > -10.0f) 
				continue;

			glVertex3f(x, 0.0f, -50.0f);	
			glVertex3f(x, 0.0f, 50.0f);
			glVertex3f(-50.0f, 0.0f, x);	
			glVertex3f(50.0f, 0.0f, x);
		}

		glEnd();
#endif

		glPopAttrib();
	}


	ApplyLights();

	glScalef(mZoom, mZoom, mZoom);

	mglGetOpenGLModelviewMatrix16fv(gModelViewMatrix);

	if (mRenderMode & fDrawPickRay)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		hel::Vec3 rayEnd = mTestRay.GetOrigin() + mTestRay.GetDir() * 1000;
		glPointSize(2.0);
		glBegin(GL_POINTS);	
		glColor3fv(GREEN);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glBegin(GL_LINES);	
		glColor3fv(YELLOW);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glColor3fv(DARK_YELLOW);	
		glVertex3fv(rayEnd.mVec);
		glEnd();



		glBegin(GL_POINTS);	
		glColor3fv(BLUE);	
		glVertex3fv(pos.mVec);
		//glVertex3fv(target.mVec);
		glEnd();
		glBegin(GL_LINES);	
		glColor3fv(RED);	
		glVertex3fv(pos.mVec);
		glColor3fv(DARK_RED);	
		glVertex3fv(target.mVec);
		glEnd();
		glPointSize(mDefaultPointSize);

		glPopAttrib();
	}

	DrawScene( gScene );

	DrawIcons();

	glPopMatrix();


	// FIXME: Use camera data to fix projections and status...

#if PLANE_NOTIFY_WITH_AXIS
	{
		glPushMatrix();
   
		float scaleEnv = OpenGL::GetInstance()->GetNearHeight();
		glTranslatef(-scaleEnv + 2.5f, -scaleEnv + 2.5f, 10.0);
		//glRotatef(mAngles[0], 1.0, 0.0, 0.0);
		//glRotatef(mAngles[1], 0.0, 1.0, 0.0);
		//glRotatef(mAngles[2], 0.0, 0.0, 1.0);

		glClear(GL_DEPTH_BUFFER_BIT);
		mglDrawEditorAxis();
		glPopMatrix();
	}
#endif

	{
		float height = OpenGL::GetInstance()->GetContextHeight();
		float width = OpenGL::GetInstance()->GetContextWidth();
		OpenGL::GetInstance()->ResizeContext(width, height); // reset for other windows
	}

	// OpenGLPrinter test
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor3fv(WHITE);
	//mPrinter.Print2d(-mScaleEnv, mScaleEnv - 1.5f, 0.06f, freyjaGetCameraNameString(camera) );
	glPopAttrib();
}


void FreyjaRender::DrawFreeWindow()
{
	glPushMatrix();

	// TODO: Replace with Mat44 transform
	glTranslatef(mScroll[0], mScroll[1]/*+8.0f*/, mScroll[2]);
	glRotatef(mAngles[0], 1.0, 0.0, 0.0);
	glRotatef(mAngles[1], 0.0, 1.0, 0.0);
	glRotatef(mAngles[2], 0.0, 0.0, 1.0);

	if (mRenderMode & fSolidPlane)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glColor3fv(WHITE);
		OpenGLRenderableStrategy::ApplyMaterial(GetSelectedMaterial());
		mglDrawPlane(50.0f, 2.0f, 1.0f);
		glPopAttrib();
	}
	else if (mRenderMode & fGrid)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		glLineWidth(1.25f);
		mglDrawGrid(mColorGridLine, 50.0f, 2.0f, 1.0f);

		glColor3fv(mColorGridLine);
		glLineWidth(4.0f);
		glBegin(GL_LINES);
		//glColor3fv(RED);
		glVertex3f(-50.0f, 0.0f, 0.0f);	
		glVertex3f(50.0f, 0.0f, 0.0f);
		//glColor3fv(BLUE);
		glVertex3f(0.0f, 0.0f, -50.0f);	
		glVertex3f(0.0f, 0.0f, 50.0f);
		glEnd();

#if 1
		//glColor3fv(mColorGridLine);
		glLineWidth(1.75f);
		glBegin(GL_LINES);

		for (float x = -50.0f; x < 60.0f; x += 10.f)
		{
			if (x < 10.0f && x > -10.0f) 
				continue;

			glVertex3f(x, 0.0f, -50.0f);	
			glVertex3f(x, 0.0f, 50.0f);
			glVertex3f(-50.0f, 0.0f, x);	
			glVertex3f(50.0f, 0.0f, x);
		}

		glEnd();
#endif

		glPopAttrib();
	}


	//glPushMatrix();

	ApplyLights();

	glScalef(mZoom, mZoom, mZoom);

	mglGetOpenGLModelviewMatrix16fv(gModelViewMatrix);

	if (mRenderMode & fDrawPickRay)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		hel::Vec3 rayEnd = mTestRay.GetOrigin() + mTestRay.GetDir() * 1000;
		glPointSize(2.0);
		glBegin(GL_POINTS);	
		glColor3fv(GREEN);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glBegin(GL_LINES);	
		glColor3fv(YELLOW);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glColor3fv(DARK_YELLOW);	
		glVertex3fv(rayEnd.mVec);
		glEnd();
		//glPointSize(mDefaultPointSize);

		{	
			hel::Vec3 pos, target;

#warning FIXME Update to 10.x API.
#if FIXME
			index_t camera = GetSelectedCamera();
			freyjaGetCameraPos3fv(camera, pos.mVec);
			freyjaCameraGetTarget3fv(camera, target.mVec);
#endif
			//glBegin(GL_POINTS);	
			glColor3fv(BLUE);	
			//glVertex3fv(pos.mVec);
			glPushMatrix();
			glTranslatef(pos.mX, pos.mY, pos.mZ);
			mglDrawControlPoint();
			glPopMatrix();

			//glVertex3fv(target.mVec);
			glEnd();
			glBegin(GL_LINES);	
			glColor3fv(RED);	
			glVertex3fv(pos.mVec);
			glColor3fv(DARK_RED);	
			glVertex3fv(target.mVec);
			glEnd();
			glPointSize(mDefaultPointSize);
		}

		glPopAttrib();
	}

	DrawScene ( gScene );

	//glPopMatrix(); // Remove scaling

	DrawIcons();

	glPopMatrix();


#if PLANE_NOTIFY_WITH_AXIS
	{   
		float scaleEnv = OpenGL::GetInstance()->GetNearHeight();
		
		glPushMatrix();
		glTranslatef(-scaleEnv + 2.5f, -scaleEnv + 2.5f, 10.0);
		glRotatef(mAngles[0], 1.0, 0.0, 0.0);
		glRotatef(mAngles[1], 0.0, 1.0, 0.0);
		glRotatef(mAngles[2], 0.0, 0.0, 1.0);

		glClear(GL_DEPTH_BUFFER_BIT);
		mglDrawEditorAxis();
		glPopMatrix();
	}
#endif

	// OpenGLPrinter test
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor3fv(WHITE);
	mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Orbit");
	glPopAttrib();
}


void FreyjaRender::DrawMetadataRenderables()
{
#if FIXME
	/* Metadata objects */
	glPushAttrib(GL_LIGHTING_BIT);  
	glDisable(GL_LIGHTING);

	BindColorTexture();
	glColor3fv(ORANGE);

	for (uint32 i = 0, n = Metadata::GetObjectCount(); i < n; ++i)
	{
		Metadata* metadata = Metadata::GetObjectByUid( i );
		
		// This method will encounter any 'gaps' ( NULL pointers ) in the container.
		if ( metadata )
		{
			glPushMatrix();

			const Vec3 &pos = metadata->GetPos();
			glTranslatef(pos.mX, pos.mY, pos.mZ);
			mglDrawControlPoint();

			glPopMatrix();
		}
	}

	glPopAttrib();



	glPushAttrib(GL_LIGHTING_BIT);  
	glDisable(GL_LIGHTING);

	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);//GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (uint32 i = 0, n = Metadata::GetObjectCount(); i < n; ++i)
	{
		Metadata* metadata = Metadata::GetObjectByUid( i );
		
		// This method will encounter any 'gaps' ( NULL pointers ) in the container.
		if ( metadata )
		{
			Renderable* renderable = metadata->GetRenderable(); 

			if ( renderable )
			{
				BindColorTexture();

				{
					glPushMatrix();
					const Vec3 &pos = metadata->GetPos();
					glTranslatef(pos.mX, pos.mY, pos.mZ);

					Vec3 rot;
					metadata->GetRot().GetEulerAngles(rot.mVec);
					rot *= HEL_180_OVER_PI;
					glRotatef(rot.mX, 1,0,0);
					glRotatef(rot.mY, 0,1,0);
					glRotatef(rot.mZ, 0,0,1);

					glPushMatrix();
					const Vec3& scale = metadata->GetScale();
					glScalef(scale.mX, scale.mY, scale.mZ);
				}

				#warning "FIXME: Add support for metadata:renderable material."
#if 1
		glPushAttrib(GL_POLYGON_BIT);
		glPushAttrib(GL_LINE_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_BACK, GL_LINE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glDisable(GL_DEPTH_TEST);

		glLineWidth(3.0f);

		glColor3fv(BLACK);
#endif

				glVertexPointer(3, GL_FLOAT, 0, renderable->mVertices);
				glNormalPointer(GL_FLOAT, 0, renderable->mNormals);
				glTexCoordPointer(3, GL_FLOAT, 0, renderable->mTexcoords);
				glDrawElements(GL_TRIANGLES, renderable->mFaceCount*3,
				               GL_UNSIGNED_SHORT, renderable->mIndices);

#if 1
		glPopAttrib(); // GL_LINE_BIT | GL_DEPTH_BUFFER_BIT

		glPolygonMode(GL_FRONT, GL_FILL);
		glCullFace(GL_BACK);

		glPopAttrib(); // GL_POLYGON_BIT

				glColor3fv(WHITE);

				glVertexPointer(3, GL_FLOAT, 0, renderable->mVertices);
				glNormalPointer(GL_FLOAT, 0, renderable->mNormals);
				glTexCoordPointer(3, GL_FLOAT, 0, renderable->mTexcoords);
				glDrawElements(GL_TRIANGLES, 
				               (renderable->mFaceCount<<1)+renderable->mFaceCount,
				               GL_UNSIGNED_SHORT, renderable->mIndices);
#endif

				glPopMatrix();
				glPopMatrix();
			}
		}
	}

	glPopAttrib();

	glPopClientAttrib();

	/* End metadata objects */

#endif // FIXME
}


void FreyjaRender::DrawIcons()
{
	DrawMetadataRenderables();

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);

	OpenGL::BindColorTexture();

	if (mRenderMode & fLighting)
	{
		vec4_t pos;
		freyjaGetLightPosition4v(0, pos);
		
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);

		glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
		mglDrawControlPoint();

		glPopMatrix();
	}

	GetCursor().Display();

#if FIXME
	/* Special edit modes. */
	switch ( GetObjectMode() )
	{
	case FreyjaControl::tCamera:
		for (uint32 i = 0, n = freyjaGetCameraCount(); i < n; ++i)
		{
			hel::Vec3 target, pos;
			freyjaGetCameraTarget3fv(i, target.mVec);
			freyjaGetCameraPos3fv(i, pos.mVec);
			
			glBegin(GL_LINES);
			glColor3fv(RED);
			glVertex3fv(pos.mVec);
			glColor3fv(DARK_RED);
			glVertex3fv(target.mVec);
			glEnd();

			glPushMatrix();
			glTranslatef(pos.mX, pos.mY, pos.mZ);
			glColor3fv(RED);
			mglDrawControlPoint();
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(target.mX, target.mY, target.mZ);
			glColor3fv(DARK_RED);
			mglDrawControlPoint();
			glPopMatrix();
		}
		break;

	default:
		;
	}
#endif

	// Draw currently enabled plugin 'icons'.
	// PluginManager.Draw();
	freyja3d_plugin_draw_loop();

	glPopAttrib();
}


void FreyjaRender::AttachMethodListeners()
{
	CreateListener("eScreenShot", &FreyjaRender::EvScreenShot);

	CreateListener("eShadowVolume", &FreyjaRender::EvRenderShadowVolume);
	CreateListener("eViewports", &FreyjaRender::eViewports);
	CreateListener("eRenderBoneName", &FreyjaRender::eRenderBoneName);
	CreateListener("eRenderSkeleton", &FreyjaRender::eRenderSkeleton);
	CreateListener("eRenderSkeleton2", &FreyjaRender::eRenderSkeleton2);
	CreateListener("eRenderSkeleton3", &FreyjaRender::eRenderSkeleton3);
	CreateListener("eRenderVertex", &FreyjaRender::eRenderVertex);
	CreateListener("eRenderWireframe", &FreyjaRender::eRenderWireframe);
	CreateListener("eRenderNormals", &FreyjaRender::eRenderNormals);
	CreateListener("eRenderLighting", &FreyjaRender::eRenderLighting);
	CreateListener("eRenderMaterial", &FreyjaRender::eRenderMaterial);
	CreateListener("eRenderGrid", &FreyjaRender::eRenderGrid);
	CreateListener("eRenderSolidGround", &FreyjaRender::eRenderSolidGround);
	CreateListener("eRenderFace", &FreyjaRender::eRenderFace);
	CreateListener("eRenderPickRay", &FreyjaRender::eRenderPickRay);
	CreateListener("eRenderBbox", &FreyjaRender::eRenderBbox);
	CreateListener("eGroupColors", &FreyjaRender::eGroupColors);
	CreateListener("eRenderBoneZClear", &FreyjaRender::eRenderBoneZClear);
	CreateListener("eRenderGridZClear", &FreyjaRender::eRenderGridZClear);
	CreateListener("eSkeletalDeform", &FreyjaRender::eRenderSkeletalDeform);
	CreateListener("eFPSCap", &FreyjaRender::eFPSCap);
	CreateListener("ePolyMeshBone", &FreyjaRender::ePolyMeshBone);
	CreateListener("eLineBone", &FreyjaRender::eLineBone);


	CreateListener("eViewportBack", &FreyjaRender::EvViewportBack);
	CreateListener("eViewportBottom", &FreyjaRender::EvViewportBottom);
	CreateListener("eViewportRight", &FreyjaRender::EvViewportRight);
	CreateListener("eViewportFront", &FreyjaRender::EvViewportFront);
	CreateListener("eViewportTop", &FreyjaRender::EvViewportTop);
	CreateListener("eViewportLeft", &FreyjaRender::EvViewportLeft);
	CreateListener("eViewportOrbit", &FreyjaRender::EvViewportOrbit);
	CreateListener("eViewportUV", &FreyjaRender::EvViewportUV);
	CreateListener("eViewportCurve", &FreyjaRender::EvViewportCurve);
	CreateListener("eViewportCamera", &FreyjaRender::EvViewportCamera);
	CreateListener("eViewportMaterial", &FreyjaRender::EvViewportMaterial);


	// Mode events
	EvModeAutoKeyframeId = CreateListener("eModeAnim", &FreyjaRender::EvModeAutoKeyframe);
	CreateListener("eModeUV", &FreyjaRender::EvModeUV);
	CreateListener("eModeModel", &FreyjaRender::EvModeModel);
	CreateListener("eModeMaterial", &FreyjaRender::EvModeMaterial);
}

////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaRender::ClearFlag(flags_t flag)
{
	mRenderMode &= ~flag;
}


void FreyjaRender::Display() 
{ 
	if ( //!mInitContext ||
		 (GetMode() & fFPSCap) && mTimer.GetTicks() < 16 ) // ~60.0 fps cap
	{
		//freyja_print("%ims since last frame", mTimer.GetTicks());
		return;
	}

	mUpperLeftText[0] = -OpenGL::GetInstance()->GetNearHeight( ) - 8;
	mUpperLeftText[1] = OpenGL::GetInstance()->GetNearHeight( ) - 1.5f;

	glClearColor(mColorBackground[0], mColorBackground[1], mColorBackground[2], 1.0);
	
	// 2007.05.20 - Removed depth test toggle
	//glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);

	/* Disable lighting here until the color materials are updated. */
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	
	switch (mViewMode)
	{
	case VIEWMODE_MODEL_VIEW:
		glPushMatrix();
		DrawFreeWindow();
		glPopMatrix();
		break;

	case VIEWMODE_MODEL_EDIT:
		if ( mRenderMode & fViewports )
		{
			// Get viewport size info
			GLint vp[4];
			glGetIntegerv(GL_VIEWPORT, vp);    
			long width = vp[2] / 2;
			long height = vp[3];

			// Testing default viewports 0-3, these should move
			// to resize event later
			mViewportsCount = 4;

			// 0 ( Lower left here )
			mViewports[0].x = 0;
			mViewports[0].y = 0;
			mViewports[0].w = width;
			mViewports[0].h = height/2;
			//mViewports[0].plane = PLANE_FRONT;

			// 1 ( Lower right here )
			mViewports[1].x = width;
			mViewports[1].y = 0;
			mViewports[1].w = width;
			mViewports[1].h = height/2;
			//mViewports[1].plane = PLANE_LEFT;

			// 2 ( Upper right here )
			mViewports[2].x = width;
			mViewports[2].y = height/2;
			mViewports[2].w = width;
			mViewports[2].h = height/2;
			//mViewports[2].plane = PLANE_TOP;

			// 3 ( Upper left here )
			mViewports[3].x = 0;
			mViewports[3].y = height/2;
			mViewports[3].w = width;
			mViewports[3].h = height/2;
			//mViewports[3].plane = PLANE_FREE;

			for (uint32 i = 0; i < mViewportsCount; ++i)
			{
				glPushAttrib(GL_ENABLE_BIT);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_LIGHTING);
				glDisable(GL_BLEND);

				glColor3fv(BLACK);
				glBegin(GL_LINES);
				glVertex2f(mViewports[i].x, mViewports[i].y);
				glVertex2f(mViewports[i].x, mViewports[i].h);
				glVertex2f(mViewports[i].x, mViewports[i].y);
				glVertex2f(mViewports[i].w, mViewports[i].y);
				glEnd();

				glPopAttrib();

				glViewport(mViewports[i].x, mViewports[i].y, 
						   mViewports[i].w, mViewports[i].h);
				// Removed extra GL matrix stack frame around this call
				DrawWindow(mViewports[i].plane);

				// Clear depth after each viewport
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glClear(GL_DEPTH_BUFFER_BIT);


			}

			glViewport(vp[0], vp[1], vp[2], vp[3]);
		}
		else
		{
			glPushMatrix();
			DrawWindow( GetSelectedView( ) );
			glPopMatrix();
		}
		break;

	case VIEWMODE_TEXTURE_EDIT:
		DrawUVWindow();
		break;

	case VIEWMODE_MATERIAL_EDIT:
		DrawMaterialEditWindow();
		break;

	default:
		;
	}

	// OpenGLPrinter test
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor3fv(GREEN);
	{
		float scaleEnv = OpenGL::GetInstance()->GetNearHeight();
		mPrinter.Print2d(15.0f, scaleEnv - 1.5f, 0.04f, FREYJA_WATERMARK );
	}
	glPopAttrib();

	//CHECK_OPENGL_ERROR( glFlush() );
	glFlush();

	mTimer.Reset();
}



void FreyjaRender::SetFlag(flags_t flag)
{
	mRenderMode |= flag;
}



////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void FreyjaRender::ApplyLights()
{
	if (mRenderMode & fLighting)
	{
		vec4_t pos = { 0.0f, 0.0f, 0.0f, 1.0f};
		freyjaGetLightPosition4v(0, pos);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);

		vec4_t color;
		freyjaGetLightAmbient(freyjaGetCurrentLight(), color);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);

		freyjaLightDiffuse(freyjaGetCurrentLight(), color);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);

		freyjaGetLightSpecular(freyjaGetCurrentLight(), color);
		glLightfv(GL_LIGHT0, GL_SPECULAR, color);
	}
#if 0
	else 
	{
		glEnable(GL_LIGHTING);

		vec4_t pos = { 0.0f, 0.0f, 1.0f, 0.0f };
		vec4_t color = { 0.5f, 0.5f, 0.5f, 1.0f };

		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, color);
		//glLightfv(GL_LIGHT0, GL_DIFFUSE, color);


		pos[3] = -1.0f;
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_POSITION, pos);
		glLightfv(GL_LIGHT1, GL_AMBIENT, color);
		//glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
	}
#endif
}


void FreyjaRender::Flag(flags_t flag, bool t)
{
	t ? SetFlag(flag) : ClearFlag(flag);
}


void FreyjaRender::DrawCurveWindow()
{
#if FIXME
	const unsigned int width = GetWindowWidth();
	const unsigned int height = GetWindowHeight();
	const unsigned int end = 175;
	const vec_t end_v = 1.0f / end;
	const vec_t inc_v = 5;

	glPushMatrix();

	mgl2dProjection( width, height );

	/* Background Quad */
	{
		const vec_t x = 0.0f;
		const vec_t y = 0.0f;

		glColor3fv( mColorBackground );
		BindColorTexture();
		glColor3fv( mColorBackground );

		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
		glEnd();
	}


	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glLineWidth(mDefaultLineWidth);
	glPointSize(mVertexPointSize);



#if 0
#else
	/* Skeletal animation, if any... */
	uint32 bone = FreyjaControl::GetInstance()->GetSelectedBone();
	Bone *b = Bone::GetBone(bone);

	// FIXME: Use cursor mode to determine which to 'modify and render here'
	if ( b )
	{
		const vec_t shift = 0.0f;
		uint32 a = FreyjaControl::GetInstance()->GetSelectedAnimation();
		BoneTrack &track = b->GetTrack(a);
		vec_t rateInverse = 1.0f / track.GetRate();
		hel::Vec3 v;

		vec_t yT2 = 20.0f, x = 0.0f, s = 1.0f;

		//glLineWidth(1.5);

		for (uint32 j = 0; j < 3; ++j)
		{
			switch (j)
			{
			case 0:
				glColor3fv(DARK_RED);
				break;

			case 1:
				glColor3fv(DARK_GREEN);
				break;

			default:
				glColor3fv(DARK_BLUE);
				break;
			}

			glBegin(GL_LINES);
			glVertex2f(x, yT2);

			for (uint32 i = 0; i < track.GetRotKeyframeCount(); ++i)
			{
				vec_t time = (vec_t)i * rateInverse;
				hel::Vec3 p = track.GetRot(time) * 57.295779513082323f;
				v.mVec[0] = x + i*s;
				v.mVec[1] = yT2 + p.mVec[j];
				glVertex2fv(v.mVec);
				glVertex2fv(v.mVec);
			}

#if 1
			glVertex2f(width, yT2);
			glEnd();

			switch (j)
			{
			case 0:
				glColor3fv(RED);
				break;

			case 1:
				glColor3fv(GREEN);
				break;

			default:
				glColor3fv(BLUE);
				break;
			}

			glBegin(GL_LINES);
			glVertex2f(x, yT2+shift);

			for (uint32 i = 0; i < track.GetRotKeyframeCount(); ++i)
			{
				vec_t time = (vec_t)i * rateInverse;
				hel::Vec3 p = track.GetLoc(time) * 2.0f;;
				v.mVec[0] = x + i*s;
				v.mVec[1] = yT2+shift + p.mVec[j];
				glVertex2fv(v.mVec);
				glVertex2fv(v.mVec);
			}

			glVertex2f(width, yT2+shift);
			glEnd();
		}

		/* Display keyframes by time markers. */
		glColor3fv( YELLOW );
		glBegin(GL_LINES);

		for (uint32 i = 0; i < track.GetRotKeyframeCount(); ++i)
		{
			vec_t time = track.GetRotKeyframeTime( i );
			vec_t x = (float)i * end_v * width;
			
			//y = height - y * height;
			glVertex2f( x, 0.0f );
			glVertex2f( x, height );
		}

		glEnd();
#endif
	}
#endif

	/* Lines */
	glColor3fv(mColorWireframe);
	glBegin(GL_LINES);

	for (unsigned int i = 0; i < end; i+=inc_v)
	{
		vec_t x = (float)i * end_v;
		x *= width;

		//y = height - y * height;
		glVertex2f( x, 0.0f );
		glVertex2f( x, height );
	}

	glEnd();


	/* Time marker. */
	{
		glColor3fv(mColorWireframeHighlight);
		glBegin(GL_LINES);
		vec_t x = FreyjaControl::GetInstance()->GetSelectedKeyFrame();

		// 'full coverage' but this is not used for events.
		// x = (x/500)*width;

		// 'matching' which ensures 1:1 mapping of the same value.
		x = x * end_v * width;

		glVertex2f( x, 0.0f );
		glVertex2f( x, height );
		glEnd();
	}

	glPopAttrib();


	/* Text */
	{
		glPushAttrib(GL_ENABLE_BIT);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glColor3fv(WHITE);

		const float text_height = height - height * 0.5f;
		String s;
		vec_t x;

		for (unsigned int i = inc_v; i < end; i+=inc_v)
		{			
			x = (float)i * end_v;
			x *= width;	
			s.Set("%i", i);
			mPrinter.Print2d( x, text_height, 0.33f, s.c_str() );
		}

		glPopAttrib();
	}

	ResizeContext(width, height);
	glPopMatrix();
#endif
}


void FreyjaRender::DrawUVWindow()
{
	unsigned int width = OpenGL::GetInstance()->GetContextWidth();
	unsigned int height = OpenGL::GetInstance()->GetContextHeight();

	glPushMatrix();

	mgl2dProjection(width, height);

	OpenGL::GetInstance()->DrawQuad(0.0f, 0.0f, width, height);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glLineWidth(mDefaultLineWidth);
	glPointSize(mVertexPointSize);

#if FIXME
	Mesh *m = freyjaGetMeshClass(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		/* Render UVs as points */
		// FIXME: This will do many points for each point, opt later
		//        when we can't have mixed polymaps
		glBegin(GL_POINTS);
		glColor3fv(mColorVertexHighlight);
		index_t selected = freyjaGetCurrentMaterial();//FreyjaControl::GetInstance()->GetSelectedTexture();

		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);

			if (!f || f->mMaterial != selected)
				continue;

			mstl::Vector<index_t> &indices = 
				(f->mFlags & Face::fPolyMappedTexCoords && f->mTexCoordIndices.size()) ? 
					f->mTexCoordIndices : f->mIndices;

			for (uint32 j = 0, jn = indices.size(); j < jn; ++j)
			{
				hel::Vec3 v;
				m->GetTexCoord(indices[j], v.mVec);

				v.mX *= width;
				v.mY = height - v.mY * height;

				glVertex2fv(v.mVec);
			}
		}

		glEnd();


		// Render wireframe faces	
		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			Face *f = m->GetFace(i);
			hel::Vec3 v;
			
			// 0.9.5.13 - We want to always see the wireframe faces for selection!
			if (!f ) //|| f->mMaterial != selected) 
				continue;
			
			/* Render face as wireframe */
			glBegin(GL_LINE_LOOP);

			if (f->mFlags & Face::fSelected)
				glColor3fv(mColorWireframeHighlight);
			else
				glColor3fv(mColorWireframe);
			
			mstl::Vector<index_t> &indices = 
				(f->mFlags & Face::fPolyMappedTexCoords && f->mTexCoordIndices.size()) ? 
					f->mTexCoordIndices : f->mIndices;

			for (uint32 j = 0, jn = indices.size(); j < jn; ++j)
			{
				hel::Vec3 v;
				m->GetTexCoord(indices[j], v.mVec);

				v.mX *= width;
				v.mY = height - v.mY * height;

				glVertex2fv(v.mVec);
			}
			
			glEnd();
		}
	}

#endif // FIXME

	glPopAttrib();

	OpenGL::GetInstance()->ResizeContext(width, height);
	glPopMatrix();
}


void FreyjaRender::DrawGrid(freyja_plane_t plane, int w, int h, int size)
{
	int x, y;

	//glPushMatrix();

	if (mRenderMode & fRenderGridClearedZBuffer)
		glClear(GL_DEPTH_BUFFER_BIT);

	glLineWidth(2.0);

	switch (plane)
	{
	case PLANE_FRONT:
	case PLANE_BACK:
		x = (int)mScroll[0];
		y = (int)mScroll[1];

		glBegin(GL_LINES); 
		glColor3fv(FreyjaRender::mColorAxisX);
		glVertex2i(-w, y);
		glVertex2i(w, y);
    
		glColor3fv(FreyjaRender::mColorAxisY);
		glVertex2i(x, -h);
		glVertex2i(x, h);
		glEnd();
		break;


	case PLANE_TOP:
	case PLANE_BOTTOM:
		x = (int)mScroll[0];
		y = (int)mScroll[2];

		glBegin(GL_LINES); 
		glColor3fv(FreyjaRender::mColorAxisX);
		glVertex2i(-w, y);
		glVertex2i(w, y);
    
		glColor3fv(FreyjaRender::mColorAxisZ);
		glVertex2i(x, -h);
		glVertex2i(x, h);
		glEnd();
		break;


	case PLANE_LEFT:
	case PLANE_RIGHT:
	   x = (int)mScroll[2];
	   y = (int)mScroll[1];

		glBegin(GL_LINES); 
		glColor3fv(FreyjaRender::mColorAxisZ);
		glVertex2i(-w, y);
		glVertex2i(w, y);
    
		glColor3fv(FreyjaRender::mColorAxisY);
		glVertex2i(x, -h);
		glVertex2i(x, h);
		glEnd(); 
		break;

	default:
		return;
   }

	//if (mRenderMode & fGrid)
   {
		int offset_x = (x % size) - w;
		int offset_y = (y % size) - h;

		glLineWidth(1.0);
		glColor3fv(mColorGridLine);

		glBegin(GL_LINES);
		for (y = offset_y; y < w; y++)
		{
			glVertex2i(-w, y);
			glVertex2i(w, y);
		}

		for (x = offset_x; x < h; x++)
		{
			glVertex2i(x, -h);
			glVertex2i(x, h);
		}
		glEnd();

		glLineWidth(1.75);
		glColor3fv(mColorGridSeperator);
		glBegin(GL_LINES);
		for (y = offset_y; y < w; y+=10)
		{
			glVertex2i(-w, y);
			glVertex2i(w, y);
		}

		for (x = offset_x; x < h; x+=10)
		{
			glVertex2i(x, -h);
			glVertex2i(x, h);
		}
		glEnd();
   }

   //glPopAttrib();
   //glPopMatrix();
}


void FreyjaRender::DrawWindow( freyja_plane_t plane )
{
	switch (plane)
	{
	case DRAW_CAMERA:
		DrawCamWindow();
		return;
		
	case PLANE_FREE:
		DrawFreeWindow();
		return;
	   
	case DRAW_UV:
		DrawUVWindow();
		return;
		
	case DRAW_MATERIAL:
		DrawMaterialEditWindow();
		return;

	case DRAW_CURVE:
		DrawCurveWindow();
		return;

	default:
		;
	}

	glPushMatrix(); // New for 2d caption, etc

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	//glDisable(GL_TEXTURE);
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0); // color

	if (mRenderMode & fGrid)
		DrawGrid(plane,
				 OpenGL::GetInstance()->GetContextWidth(), 
				 OpenGL::GetInstance()->GetContextHeight(), 
				 10);

#if PLANE_NOTIFY_WITH_AXIS
	glPushMatrix();

	{
		float scaleEnv = OpenGL::GetInstance()->GetNearHeight();
		glTranslatef(-scaleEnv + 2.5f, -scaleEnv + 2.5f, 10.0);
	}

	switch (plane)
	{
	case PLANE_FRONT: // XY
		break;

	case PLANE_BACK: // XY
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		break;

	case PLANE_TOP: // XZ
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		break;

	case PLANE_BOTTOM: // XZ
		glRotatef(-90.0f, 1.0f, 0.0, 0.0);
		break;

	case PLANE_LEFT: // ZY
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		break;

	case PLANE_RIGHT: // ZY
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		break;

	default:
		;
	}

	glLineWidth(2.0);
	mglDrawEditorAxis();

	glPopMatrix();
#endif

	glPopAttrib();

	switch (plane)
	{
	case PLANE_FRONT: // XY
		glTranslatef(mScroll[0], mScroll[1], 0.0);
		break;

	case PLANE_BACK: // XY
		glTranslatef(mScroll[0], mScroll[1], 0.0);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		break;

	case PLANE_TOP: // XZ
		glTranslatef(mScroll[0], mScroll[2], 0.0);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		break;

	case PLANE_BOTTOM: // XZ
		glTranslatef(mScroll[0], mScroll[2], 0.0);
		glRotatef(-90.0f, 1.0f, 0.0, 0.0);
		break;

	case PLANE_LEFT: // ZY
		glTranslatef(mScroll[2], mScroll[1], 0.0);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		break;

	case PLANE_RIGHT: // ZY
		glTranslatef(mScroll[2], mScroll[1], 0.0);
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		break;

	default:
		;
	}

	glPushMatrix();

	ApplyLights();

	glScalef(mZoom, mZoom, mZoom);

	mglGetOpenGLModelviewMatrix16fv(gModelViewMatrix);

	if (mRenderMode & fDrawPickRay)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		hel::Vec3 rayEnd = mTestRay.GetOrigin() + mTestRay.GetDir() * 1000;
		glPointSize(2.0);
		glBegin(GL_POINTS);	
		glColor3fv(GREEN);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glBegin(GL_LINES);	
		glColor3fv(YELLOW);	
		glVertex3fv(mTestRay.GetOrigin().mVec);
		glColor3fv(DARK_YELLOW);	
		glVertex3fv(rayEnd.mVec);
		glEnd();
		glPointSize(mDefaultPointSize);

		glPopAttrib();
	}

	/* Draw model geometry, metadata visualizations, and all that good stuff. */
	DrawScene ( gScene );

	DrawIcons();

	glPopMatrix();

	glPopMatrix(); // New for 2d caption, etc

	// OpenGLPrinter test
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor3fv(WHITE);	

	switch (plane)
	{
	case PLANE_FRONT:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Front");
		break;

	case PLANE_BACK:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Back");
		break;

	case PLANE_TOP:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Top");
		break;

	case PLANE_BOTTOM:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Bottom");
		break;

	case PLANE_LEFT:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Left");
		break;

	case PLANE_RIGHT:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Right");
		break;

	default:
		mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Unknown");
	}

	glPopAttrib();
}


void draw_material_texture_square( int id, float x, float y, float z, float w, float h, float s )
{
	/* Texture square. */
	glPushMatrix();
	glColor3fv(BLACK);
	OpenGL::BindTexture( GL_TEXTURE0, 0 );
	glBegin(GL_LINE_LOOP);
	glVertex3f(x*s, y*s, z);
	glVertex3f(x*s, (y+h)*s, z);
	glVertex3f((x+w)*s, (y+h)*s, z);
	glVertex3f((x+w)*s, y*s, z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x*s, y*s+h/2, z);
	glVertex3f( 8.0f, 0.0f, z );
	glEnd();

	glColor3fv(WHITE);
	OpenGL::BindTexture( GL_TEXTURE0, id );
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x, y, z);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x, y+h, z);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x+w, y+h, z);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(x+w, y, z);
	glEnd();	
	glPopMatrix();
	/* End texture square. */
}


void FreyjaRender::DrawMaterialEditWindow()
{
	glPushMatrix();
	glTranslatef(-8.0f, 0.0f, 0.0f);

	/* Setup lighting */	
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		float position[4] = {128.0, 128.0, 128.0, 1.0};
		glLightfv( GL_LIGHT0, GL_POSITION, position );

		vec4_t ambient = { 0.2, 0.2, 0.2, 1.0f };
		vec4_t diffuse = { 0.8, 0.8, 0.8, 1.0f };
		vec4_t specular = { 0.2, 0.2, 0.2, 1.0f };
		glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
		glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	}

	glPushMatrix();
	/* Rotate mesh. */
	glRotatef( mMaterialYAngle, 0, 1, 0 );

	/* Apply material to mesh. */
	glEnable( GL_TEXTURE_2D );
	OpenGLRenderableStrategy::ApplyMaterial( GetSelectedMaterial() );

	// FIXME: Add support for swapping out material preview mesh.
#if USE_TORUS_TEST
	mglDrawTorus(3.0, 10.0);
#else
	mglDrawSphere(128, 128, 13.0f);
#endif

	glPopMatrix();

	/* Disable material and lighting. */
	OpenGLRenderableStrategy::ApplyMaterial( NULL );
	glDisable(GL_LIGHTING);

#if USING_OPENGL_EXT 
	if (h_glActiveTextureARB)
	{
		glDisable(GL_TEXTURE_2D);
		//h_glActiveTextureARB(GL_TEXTURE1);
		//h_glActiveTextureARB(GL_TEXTURE2);
		//h_glActiveTextureARB(GL_TEXTURE3);
		//h_glActiveTextureARB(GL_TEXTURE4);

		glEnable(GL_TEXTURE_2D);
	}
#endif

	const float x = 21.0f, z = 0.0f, w = 16.0f, h = 16.0f, s = 1.001f, hh = 16.5f;
	float y = -23.0f;

	if ( GetSelectedMaterial() )
	{
		if ( GetSelectedMaterial()->GetTexture0Id() != -1 )
		{
			draw_material_texture_square( GetSelectedMaterial()->GetTexture0Id(),
										  x, y, z, w, h, s );
			y += hh;
		}
		
		if ( GetSelectedMaterial()->GetTexture1Id() != -1 )
		{
			draw_material_texture_square( GetSelectedMaterial()->GetTexture1Id(),
										  x, y, z, w, h, s );
			y += hh;
		}

		if ( GetSelectedMaterial()->GetTexture2Id() != -1 )
		{
			draw_material_texture_square( GetSelectedMaterial()->GetTexture2Id(),
										  x, y, z, w, h, s );
			y += hh;
		}

		if ( GetSelectedMaterial()->GetTexture3Id() != -1 )
		{
			draw_material_texture_square( GetSelectedMaterial()->GetTexture3Id(),
										  x, y, z, w, h, s );
			y += hh;
		}

		if ( GetSelectedMaterial()->GetTexture4Id() != -1 )
		{
			draw_material_texture_square( GetSelectedMaterial()->GetTexture4Id(),
										  x, y, z, w, h, s );
			y += hh;
		}
	}
	else
	{
		//draw_material_texture_square( -1, x, y, z, w, h, s );
	}

	glPopMatrix(); // translate -8,0,0

	/* Print text. */
	//OpenGLRenderableStrategy::ApplyMaterial( NULL );
	glPushAttrib(GL_ENABLE_BIT);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	glColor3fv(WHITE);
	mPrinter.Print2d(mUpperLeftText[0], mUpperLeftText[1], 0.05f, "Material");
	//mPrinter.Print2d(x*s-8, y*s+h+1, 0.04f, "Texture");
	glPopAttrib();
}


void FreyjaRender::EvScreenShot()
{
	if ( !OpenGL::GetInstance()->TakeScreenShot( "Freyja", "png" ) )
		OpenGL::GetInstance()->TakeScreenShot( "Freyja", "tga" );
}


void FreyjaRender::HandleMotion( mgtk_mouse_event_t* event )
{
	switch (event->button)
	{
	case MOUSE_BTN_MIDDLE:
		{
			const float treshold = 1.0f;
			hel::Vec3 xyz;
			bool xd = true;
			bool yd = true;

			if ( event->x_delta > treshold )
			{
				xyz[0] = 1.0f;
			}
			else if ( event->x_delta < -treshold )
			{
				xyz[0] = -1.0f;
			}
			else
			{
				xd = false;
			}

			if ( event->y_delta > treshold )
			{
				xyz[1] = -1.0f;
			}
			else if ( event->y_delta < -treshold )
			{
				xyz[1] = 1.0f;
			}
			else
			{
				yd = false;
			}

			if ( !xd && !yd )
			{
				return;
			}

			switch ( GetSelectedView( ) )
			{
			case PLANE_BACK:
				mScroll += hel::Vec3(xyz[0], xyz[1], xyz[2]);
				break;
				
			case PLANE_BOTTOM:
				mScroll += hel::Vec3(xyz[0], xyz[2], xyz[1]);
				break;
				
			case PLANE_RIGHT:
				mScroll += hel::Vec3(xyz[2], xyz[1], xyz[0]);
				break;
				
			case PLANE_FRONT: // front, xy
				mScroll += hel::Vec3(xyz[0], xyz[1], xyz[2]);
				break;
				
			case PLANE_TOP: // top, xz
				mScroll += hel::Vec3(xyz[0], xyz[2], xyz[1]);
				break;
				
			case PLANE_LEFT: // left, zy
				mScroll += hel::Vec3(xyz[2], xyz[1], xyz[0]);
				break;
				
			default:
				;
			}
		}
		break;


	case MOUSE_BTN_LEFT:
		if ( mViewMode == VIEWMODE_MATERIAL_EDIT )
		{
			const float treshold = 1.0f, inc = 2.0f;
			if ( event->x_delta > treshold )
				mMaterialYAngle += inc;
			else if (event->x_delta < -treshold)
				mMaterialYAngle -= inc;

			if ( mMaterialYAngle > 360.0f ||
				 mMaterialYAngle < -360.0f )
				mMaterialYAngle = 0.0f;
		}
		break;


	case MOUSE_BTN_RIGHT: 

		if ( GetSelectedView() == PLANE_FREE )
		{
			const float treshold = 1.0f;

			if (event->x_delta > treshold)
				Rotate(Y_F, -1.0f);
			else if (event->x_delta < -treshold)
				Rotate(Y_F, 1.0f);
			
			if ( event->y_delta > treshold)
				Rotate(X_F, -1.0f);
			else if (event->y_delta < -treshold)
				Rotate(X_F, 1.0f);
		}
		break;

	default:
		;
	}
}
