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
 *           template generator script.  <stu7440@westga.edu>
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

#ifndef GUARD__MONGOOSE_FREYJA_FREYJARENDER_H_
#define GUARD__MONGOOSE_FREYJA_FREYJARENDER_H_

#include <hel/ViewVolume.h>
#include <hel/Ray.h>
#include <hel/Quat.h>
#include <mstl/SystemIO.h>
#include <mgtk/ResourceEventDelegate.h>
#include <freyja/Scene.h>
#include <freyja/RenderableStrategy.h>
#include "OpenGLPrinter.h"
#include "Control.h"
#include "freyja_events.h"

namespace freyja3d {

enum view_mode {                   /* View|Edit mode */     
	VIEWMODE_MODEL_EDIT       = 1,
	VIEWMODE_TEXTURE_EDIT     = 2,
	VIEWMODE_MAP_VIEW         = 3,
	VIEWMODE_MODEL_VIEW       = 4,
	VIEWMODE_MATERIAL_EDIT    = 5
};

enum rotate_flags {
	X_F = 1,
	Y_F = 2,
	Z_F = 4
};

typedef enum { 
	
	PLANE_FRONT = 0, // XY
	PLANE_LEFT  = 1, // ZY
	PLANE_TOP   = 2, // XZ
	PLANE_FREE,
	PLANE_BACK, 
	PLANE_BOTTOM, 
	PLANE_RIGHT,

	DRAW_CAMERA,
	DRAW_UV,
	DRAW_MATERIAL,
	DRAW_CURVE
	
} freyja_plane_t;


class Viewport
{
public:
	Viewport() 
		: camera(0)
	{ }

	~Viewport() 
	{ }

	long x, y, w, h;

	freyja_plane_t plane;

	long camera;

	long mode;
};


class FreyjaRender :
	public Control
{
public:

	typedef enum {                 /* Rendering modes */
		fWireframe        = 1,
		fShadowVolume     = 2,
		fFace             = 4,
		fGrid             = 8,
		fLighting         = 16,    /* Render with GL lighting */
		fParticles        = 32,
		fBones            = 64,
		fMaterial         = 128,
		fPoints           = 256,   
		fDrawPickRay      = 512,   /* Render the pick ray */
		fBoundingVolumes  = 1024,  /* Render geometry bounding volumes */ 
		fNormals          = 2048,  /* Render vectors for normals */
		fViewports        = 4096, 
		fBoundingVolSelection      = 8192,
		fSkeletalVertexBlending    = 16384,
		fBonesNoZbuffer            = 32768,
		fRenderGridClearedZBuffer  = 65536,
		fKeyFrameAnimation         = 131072,
		fSolidPlane                = 262144,
		fFPSCap                    = 524288,
		fGroupColors               = (1<<20),
		fBones2                    = (1<<21),
		fBoneName                  = (1<<22),
		fBones3                    = (1<<23)
		
	} flags_t;


	static FreyjaRender* GetInstance( ) 
	{ return ( mInstance ? mInstance : ( mInstance = new FreyjaRender( ) ) ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns instance of FreyjaRender singleton.
	 *
	 ------------------------------------------------------*/

	static void Destroy( )
	{ 
		delete mInstance;
		mInstance = NULL; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////
	// Events, once all these are 'sorted' decouple.
	////////////////////////////////////////////////////////////

	typedef void (FreyjaRender::*MethodPtr)();
	typedef void (FreyjaRender::*MethodPtr1u)(unsigned int);


	static void AttachMethodListeners();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Hooks up MethodDelegates to the event system.
	 ------------------------------------------------------*/

	static int CreateListener(const char *name, MethodPtr ptr)
	{
		MethodDelegate *d = 
		new MethodDelegateArg0<FreyjaRender>(mInstance, ptr);
		ResourceEventDelegate::add(name, d);
		return GetEventIdByName(name);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Bind a MethodDelegate Callback to an external
	 *        langauge symbol.
	 *
	 ------------------------------------------------------*/

	static int CreateListener(const char *name, MethodPtr1u ptr)
	{
		MethodDelegate *d = 
		new MethodDelegateArg1<FreyjaRender, unsigned int>(mInstance, ptr);
		ResourceEventDelegate::add(name, d);
		return GetEventIdByName(name);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Bind a MethodDelegate Callback to an external
	 *        langauge symbol.
	 *
	 ------------------------------------------------------*/

	void DrawIcons();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 3d icons grouped together for better rendering.
	 *
	 ------------------------------------------------------*/

	void DrawMetadataRenderables();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renderables grouped together for better rendering.
	 *
	 ------------------------------------------------------*/

	void EvScreenShot();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eViewports(uint32 value) 
	{ SetRenderFlag(fViewports, value, "Four window view"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderBoneName(uint32 value)
	{ SetRenderFlag(fBoneName, value, "Bone name rendering"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderSkeleton3(uint32 value)
	{ SetRenderFlag(fBones3, value, "Skeleton3 rendering"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderSkeleton2(uint32 value)
	{ SetRenderFlag(fBones2, value, "Skeleton2 rendering"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderSkeleton(uint32 value)
	{ SetRenderFlag(fBones, value, "Skeleton rendering"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderVertex(uint32 value)
	{ SetRenderFlag(fPoints, value, "Vertex Rendering"); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/}

	void eRenderWireframe(uint32 value)
	{ SetRenderFlag(fWireframe, value, "Wireframe"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderNormals(uint32 value)
	{ SetRenderFlag(fNormals, value, "Normal visualisation"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderLighting(uint32 value)
	{ SetRenderFlag(fLighting, value, "OpenGL lighting"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderMaterial(uint32 value)
	{ SetRenderFlag(fMaterial, value, "Material usage"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderGrid(uint32 value)
	{ SetRenderFlag(fGrid, value, "Grid"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderSolidGround(uint32 value)
	{ SetRenderFlag(fSolidPlane, value, "Solid ground"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderFace(uint32 value)
	{ SetRenderFlag(fFace, value, "Face rendering"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void EvRenderShadowVolume(uint32 value)
	{ SetRenderFlag(fShadowVolume, value, "Shadow volume rendering."); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderPickRay(uint32 value)
	{ SetRenderFlag(fDrawPickRay, value, "Pick ray visibility"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderBbox(uint32 value)
	{ SetRenderFlag(fBoundingVolumes, value, "Bounding volume rendering"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eGroupColors(uint32 value)
	{ SetRenderFlag(fGroupColors, value, "Smoothing group color coding"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderBoneZClear(uint32 value)
	{ SetRenderFlag(fBonesNoZbuffer, value, "Skeleton with cleared Z buffer"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderGridZClear(uint32 value)
	{ SetRenderFlag(fRenderGridClearedZBuffer, value, "Grid depth clear"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eRenderSkeletalDeform(uint32 value)
	{ SetRenderFlag(fSkeletalVertexBlending, value, 
					"Animation with skeletal vertex blending"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eFPSCap(uint32 value)
	{ SetRenderFlag(fFPSCap, value, "FPS cap"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void eLineBone(uint32 value)
	{
		if (value)
		{
			uint32 id = ResourceEvent::GetResourceIdBySymbol("ePolyMeshBone");
			mgtk_toggle_value_set(id, 0);
			FreyjaRender::mBoneRenderType = 1;
		}
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void ePolyMeshBone(uint32 value)
	{
		if (value)
		{
			uint32 id = ResourceEvent::GetResourceIdBySymbol("eLineBone");
			mgtk_toggle_value_set(id, 0);
			FreyjaRender::mBoneRenderType = 2;
		}
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int GetFlags() 
	{ return mRenderMode; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns options flags for view
	 *
	 ------------------------------------------------------*/

	unsigned int GetMode() 
	{ return mRenderMode; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	unsigned int GetViewMode()
	{ return mRenderMode; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const vec_t &GetZoom() { return mZoom; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Current zoom factor for the scene
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void ClearFlag(flags_t flag);
	/*------------------------------------------------------
	 * Pre  : Flag is valid
	 * Post : Clears control flag for model
	 *
	 ------------------------------------------------------*/

	void Display();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders a frame to GL context
	 *
	 ------------------------------------------------------*/

	void Flag(flags_t flag, bool t);
	/*------------------------------------------------------
	 * Pre  : <flag> is valid
	 * Post : Sets control flag on or off
	 *
	 ------------------------------------------------------*/

	void SetFlag(flags_t flag);
	/*------------------------------------------------------
	 * Pre  : Flag is valid
	 * Post : Sets control flag for model
	 *
	 ------------------------------------------------------*/

	void SetRenderFlag(flags_t flag, bool t, const char *s)
	{
		Flag(flag, t);
		freyja3d_print("%s is [%s]", s, t ? "ON" : "OFF");
		mgtk_event_gl_refresh();
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void HandleMotion( mgtk_mouse_event_t* event );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	freyja_plane_t GetSelectedView( ) const
	{ return mSelectedView; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetSelectedView( freyja_plane_t view )
	{ mSelectedView = view; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetViewMode(int mode)
	{ mViewMode = mode; }
	/*------------------------------------------------------
	 * Pre  : Mode are valid view_mode(s)
	 * Post : Sets viewing options
	 *
	 ------------------------------------------------------*/

	void SetViewportWindow(long idx, freyja_plane_t win)
	{ if (idx < (long)mViewportsCount) mViewports[idx].plane = win; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetZoom(vec_t zoom) 
	{ mZoom = zoom; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set current zoom factor for the scene
	 *
	 ------------------------------------------------------*/

	// FIXME to be replaced by Arcball most likely
	void Rotate(int flags, float n);
	/*------------------------------------------------------
	 * Pre  : Flags are valid XYZ flags
	 * Post : Rotate on axis n degrees
	 *
	 ------------------------------------------------------*/

	void SetViewport( freyja_plane_t mode )
	{
		SetSelectedView( mode );
		if ( GetFlags( ) & FreyjaRender::fViewports )
		{
			mViewports[mSelectedViewport].plane = mode;
			mViewports[mSelectedViewport].camera = 0;//GetSelectedCamera();
		}
		RefreshContext( );
	}

	void EvViewportBack( )
	{
		Print("Back view");
		SetViewport(PLANE_BACK);
	}

	void EvViewportBottom( )
	{
		Print("Bottom view");  
		SetViewport(PLANE_BOTTOM);
	}

	void EvViewportRight()
	{
		Print("Right view");
		SetViewport(PLANE_RIGHT);
	}

	void EvViewportFront()
	{
		Print("Front view");
		SetViewport(PLANE_FRONT);
	}

	void EvViewportTop()
	{
		Print("Top view");  
		SetViewport(PLANE_TOP);
	}

	void EvViewportLeft()
	{
		Print("Left view");
		SetViewport(PLANE_LEFT);
	}

	void EvViewportOrbit()
	{
		Print("Orbital view");
		SetViewport(PLANE_FREE);
	}

	void EvViewportUV()
	{
		Print("UV Editor view");
		SetViewport(DRAW_UV);
	}

	void EvViewportCurve()
	{
		Print("Curve editor view");
		SetViewport(DRAW_CURVE);
	}

	void EvViewportCamera()
	{
		Print("Curve editor view");
		SetViewport(DRAW_CURVE);
	}

	void EvViewportMaterial()
	{
		Print("Material preview");
		SetSelectedView(DRAW_MATERIAL);
	}

	void EvModeAutoKeyframe(unsigned int value)
	{
		if (value)
		{
			SetViewMode(VIEWMODE_MODEL_EDIT);
			SetFlag(FreyjaRender::fKeyFrameAnimation);
			//		SetControlScheme(eScheme_Animation);
			Print("Animation Scheme selected.");
			RefreshContext();
		}
		else
		{
			// If you disable auto keyframe you must set a new major mode
			SetViewMode(VIEWMODE_MODEL_EDIT);
			ClearFlag(FreyjaRender::fKeyFrameAnimation);
			//	SetControlScheme(eScheme_Model);
			Print("Model Editor Scheme selected.");
			RefreshContext();
		}
	}


	void EvModeUV()
	{
		SetViewMode(VIEWMODE_TEXTURE_EDIT);
		RefreshContext();
		Print("UV Editor Scheme");
		//SetControlScheme(eScheme_UV);
	}


	void EvModeModel()
	{
		// Radio button like func for multiple widgets on same event
		mgtk_toggle_value_set(EvModeAutoKeyframeId, 0);

		SetViewMode(VIEWMODE_MODEL_EDIT);
		RefreshContext();
		Print("Model Editor Scheme");
		//SetControlScheme(eScheme_Model);
	}


	void EvModeMaterial()
	{
		SetViewMode(VIEWMODE_MATERIAL_EDIT);
		RefreshContext();
		Print("Material Editor Scheme");
		//SetControlScheme(eScheme_Material);
	}


	static vec4_t mColorBackground;
	static vec4_t mColorGridLine;
	static vec4_t mColorVertex;
	static vec4_t mColorVertexHighlight;
	static vec4_t mColorWireframeHighlight;
	static vec4_t mColorWireframe;
	static vec4_t mColorBoneHighlight;
	static vec4_t mColorBone;
	static vec4_t mColorJointHighlight;
	static vec4_t mColorJoint;
	static vec4_t mColorText;
	static vec4_t mColorBoundingBox;
	static vec4_t mColorGridSeperator;
	static vec4_t mColorAxisX;
	static vec4_t mColorAxisY;
	static vec4_t mColorAxisZ;
	static unsigned int mSelectedBone;
	static unsigned int mBoneRenderType;
	static unsigned char mJointRenderType;
	static int mPatchDisplayList;

	static vec_t mBoneLineWidth;               /* Custom artifact size */
	static vec_t mBonePointSize;
	static vec_t mDefaultPointSize;
	static vec_t mDefaultLineWidth;
	static vec_t mVertexPointSize;

	static hel::Ray mTestRay;

	static vec4_t mColors[16];

	static OpenGLPrinter mPrinter;

	uint32 mViewportsCount;                    /* How many viewports are used */

	Viewport mViewports[4];                    /* Viewports information */

protected:

	FreyjaRender();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FreyjaRender is constructed
	 *
	 ------------------------------------------------------*/

	~FreyjaRender()
	{  }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/

	static FreyjaRender* mInstance;


private:    

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	//void BindColorTexture();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Binds a generated white texture used for colors,
	 *        which helps avoid overuse of altering texture state. 
	 *
	 ------------------------------------------------------*/

	void BindTexture(uint32 texture);
	/*------------------------------------------------------
	 * Pre  : texture is valid texture id
	 * Post : Binds GL texture 
	 *
	 ------------------------------------------------------*/

	void Draw( freyja::Renderable* renderable );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void DrawGrid(freyja_plane_t plane, int w, int h, int size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders a grid orthogal to gl context
	 *        WxH at size spacing and adjusts for scroll
	 *
	 ------------------------------------------------------*/

	void DrawCurveWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders curve editing interface in viewport
	 *
	 ------------------------------------------------------*/

	void DrawCamWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders camera view in a viewport.
	 *
	 ------------------------------------------------------*/

	void DrawFreeWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders oribital model editor interface in viewport
	 *
	 ------------------------------------------------------*/

	void DrawMaterialEditWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders material editing interface in viewport
	 *
	 ------------------------------------------------------*/

	void DrawScene( freyja::Scene* scene );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void DrawShadowCasters(bool depthMapPass);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Draw the objects in the scene which cast shadows
	 *        During depth map pass turn off a lot of stuff ;)
	 *
	 ------------------------------------------------------*/

	void DrawShadowRecievers();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void DrawUVWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders UV editing interface in viewport
	 *
	 * Mongoose 2004.03.30, 
	 * Dependences of this method:
	 * x GL context width and height
	 * - Current mesh to skin
	 * - OpenGL 2d view helper function
	 * - Quad to render skin helper function
	 ------------------------------------------------------*/

	void DrawWindow(freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders given editor interface in viewport
	 *
	 ------------------------------------------------------*/

	void RenderMesh(index_t mesh);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders mesh
	 *
	 ------------------------------------------------------*/

	void RenderModel(index_t model);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders model
	 *
	 ------------------------------------------------------*/

	void RenderSkeleton(index_t skeleton, uint32 bone, vec_t scale);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders skeleton
	 *
	 ------------------------------------------------------*/

	void ApplyLights();
	/*------------------------------------------------------
	 * Pre  : Called from viewport interface renderer
	 * Post : Does lighting setup
	 *
	 ------------------------------------------------------*/

	void Rotate(float n, int axis);
	/*------------------------------------------------------
	 * Pre  : Axis is 0 = x, 1 = y, 2 = z
	 * Post : Rotate on axis n degrees
	 *
	 ------------------------------------------------------*/

	void ShadowedSceneSetup();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Light coordinate system conversion, etc
	 *
	 ------------------------------------------------------*/

	mstl::Timer mTimer;

	uint32 mViewMode;                          /* View mode */

	uint32 mRenderMode;                        /* Rendering mode */

	freyja_plane_t mSelectedView;

	uint32 mSelectedViewport;

	vec_t mZoom;                               /* Used to cache zoom */

	uint32 mTextureId;                         /* Currently bound texture */

	bool mInitContext;                         /* OpenGL context started? */

	hel::Vec3 mScroll;                         /* Scene translation. */

	vec3_t mAngles;                            /* Scene Used to rotate the scene */

	vec2_t mUpperLeftText;

	bool mScrollingUV_X;

	static int EvModeAutoKeyframeId;

	freyja::Material* mLastMaterial;

	vec_t mMaterialYAngle;
};

} /* End namespace freyja3d */

#endif
