/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : SDL/GLUT template
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: This is the common event system interface
 *           for freyja
 *
 *           It's independent of the widget set, etc
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.06.26:
 * Mongoose - Created
 ==========================================================================*/

#include <mgtk/mgtk_events.h>
#include "FreyjaModel.h"
#include "FreyjaRender.h"
#include "FreyjaControl.h"
#include "MaterialManager.h"
#include "freyja_events.h"


arg_list_t *freyja_rc_color(arg_list_t *args);


Resource gResource;
FreyjaControl *gFreyjaControl = 0x0;
int gSkelTreeWidgetIndex;


void setColor(vec4_t dest, vec4_t color)
{
	dest[0] = color[0];	
	dest[1] = color[1];	
	dest[2] = color[2];	
	dest[3] = color[3];
}


///////////////////////////////////////////////////////////////////////
// MGtk wrappers
///////////////////////////////////////////////////////////////////////

void mgtk_update_tree(unsigned int id, mgtk_tree_t *tree)
{
	freyja_print("FIXME: mgtk_update_tree() not in libmgtk yet");
}

void mgtk_handle_application_window_close()
{
	gFreyjaControl->event(eShutdown);
}


void mgtk_handle_color(int id, float r, float g, float b, float a)
{
	vec4_t color;

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	switch (id)
	{
	case eColorMaterialAmbient: 
		gMaterialManager->setColor(MaterialManager::eAmbient, color);
		freyja_refresh_material_interface(); // FIXME HACK
		break;

	case eColorMaterialDiffuse:
		gMaterialManager->setColor(MaterialManager::eDiffuse, color);
		freyja_refresh_material_interface(); // FIXME HACK
		break;

	case eColorMaterialSpecular:
		gMaterialManager->setColor(MaterialManager::eSpecular, color);
		freyja_refresh_material_interface(); // FIXME HACK
		break;

	case eColorMaterialEmissive:
		gMaterialManager->setColor(MaterialManager::eEmissive, color);
		freyja_refresh_material_interface(); // FIXME HACK
		break;

#ifdef BACKPORTED_LIGHTS
	case eColorLightAmbient: 
		setColor(OpenGLFreyjaScene::mMaterialLight.mAmbient, color);
		freyja_event_set_color(eColorLightAmbient, r, g, b, a);
		break;

	case eColorLightDiffuse:
		setColor(OpenGLFreyjaScene::mMaterialLight.mDiffuse, color);
		freyja_event_set_color(eColorLightDiffuse, r, g, b, a);
		break;

	case eColorLightSpecular:
		setColor(OpenGLFreyjaScene::mMaterialLight.mSpecular, color);
		freyja_event_set_color(eColorLightSpecular, r, g, b, a);
		break;
#else
	case eColorLightAmbient: 
	case eColorLightDiffuse:
	case eColorLightSpecular:
		freyja_print("Colored lights not backported");
		break;
#endif

	case eColorBackground:
		setColor(FreyjaRender::mColorBackground, color);
		freyja_event_set_color(eColorBackground, r, g, b, a);
		break;

	case eColorGrid:
		setColor(FreyjaRender::mColorGridSeperator, color);
		setColor(FreyjaRender::mColorGridLine, color);
		freyja_event_set_color(eColorGrid, r, g, b, a);
		break;

	case eColorVertex:
		setColor(FreyjaRender::mColorVertex, color);
		freyja_event_set_color(eColorVertex, r, g, b, a);
		break;

	case eColorVertexHighlight:
		setColor(FreyjaRender::mColorVertexHighlight, color);
		freyja_event_set_color(eColorVertexHighlight, r, g, b, a);
		break;

	case eColorMesh:
		setColor(FreyjaRender::mColorWireframe, color);
		freyja_event_set_color(eColorMesh, r, g, b, a);
		break;

	case eColorMeshHighlight:
		setColor(FreyjaRender::mColorWireframeHighlight, color);
		freyja_event_set_color(eColorMeshHighlight, r, g, b, a);
		break;

	default:
		return;
	}

	mgtk_event_gl_refresh();
}


void mgtk_handle_command(int command)
{
	gFreyjaControl->event(command);
}


void mgtk_handle_command2i(int event, int command)
{
	freyja_event2i(event, command);
}


void mgtk_handle_event1u(int event, unsigned int value)
{
	if (!gFreyjaControl->event(event, value))
	{
		if (freyja_event2i(EVENT_MISC, event) == -1)
			freyja_print("  mgtk_handle_event1u spawned previous unhandled event %i:%i", EVENT_MISC, event);
	}
}


void mgtk_handle_event1f(int event, float value)
{
	if (!gFreyjaControl->event(event, value))
	{
		if (freyja_event2i(EVENT_MISC, event) == -1)
			freyja_print("   mgtk_handle_event1f spawned previous unhandled event %i:%i", EVENT_MISC, event);
	}
}


void mgtk_handle_file_dialog_selection(char *filename)
{
	gFreyjaControl->handleFilename(filename);
}


void mgtk_handle_gldisplay()
{
	if (gFreyjaControl)
		gFreyjaControl->updateDisplay();
}


void mgtk_handle_glresize(unsigned int width, unsigned int height)
{
	if (gFreyjaControl)
		gFreyjaControl->resizeDisplay(width, height);
}


void mgtk_handle_key_press(int key, int mod)
{
	freyja_print("mgtk_handle_key_press(%d, %d) not handled", key, mod);
}


void mgtk_handle_resource_init(Resource &r)
{
	/* Mongoose 2002.01.12, 
	 * Bind script functions to C/C++ functions */
	r.RegisterFunction("color", freyja_rc_color);

	/* Mongoose 2002.01.21, 
	 * Bind some script vars to matching name in C/C++ */
	r.RegisterInt("eNone", eNone);

	r.RegisterInt("eCopyAppendMode", eCopyAppendMode);

	r.RegisterInt("eRedo", eRedo);
	r.RegisterInt("eUndo", eUndo);
	r.RegisterInt("eCut", eCut);
	r.RegisterInt("eCopy", eCopy);
	r.RegisterInt("ePaste", ePaste);

	r.RegisterInt("eSplitObject", eSplitObject);
	r.RegisterInt("eSelectAll", eSelectAll);
	r.RegisterInt("eAppendFile", eAppendFile);
	r.RegisterInt("eRevertFile", eRevertFile);
	r.RegisterInt("eExportFile", eExportFile);
	r.RegisterInt("eImportFile", eImportFile);
	r.RegisterInt("eCloseFile", eCloseFile);
	r.RegisterInt("eOpenFileTexture", eOpenFileTexture);

	// dialogs
	r.RegisterInt("ePreferencesDialog", ePreferencesDialog);
	r.RegisterInt("eAboutDialog", eAboutDialog);
	r.RegisterInt("eHelpDialog", eHelpDialog);

	r.RegisterInt("eDelete", eDelete);
	r.RegisterInt("eSelect", eSelect);

	r.RegisterInt("eMaterialMultiTex", eMaterialMultiTex);
	r.RegisterInt("eMaterialTex", eMaterialTex);

	r.RegisterInt("eScreenShot", eScreenShot);
	r.RegisterInt("eShutdown", eShutdown);
	r.RegisterInt("eNewFile", eNewFile);
	r.RegisterInt("eOpenFile", eOpenFile);
	r.RegisterInt("eSaveFile", eSaveFile);
	r.RegisterInt("eSaveAsFile", eSaveAsFile);
	r.RegisterInt("eInfo", eInfo);
	r.RegisterInt("eDebug", eDebug);
	r.RegisterInt("eFullscreen", eFullscreen);

	r.RegisterInt("eGenerateNormals", eGenerateNormals);
	r.RegisterInt("eGenerateCube", eGenerateCube);
	r.RegisterInt("eGenerateTube", eGenerateTube);
	r.RegisterInt("eGenerateCircle", eGenerateCircle);
	r.RegisterInt("eGeneratePlane", eGeneratePlane);
	r.RegisterInt("eGenerateCylinder", eGenerateCylinder);
	r.RegisterInt("eGenerateSphere", eGenerateSphere);
	r.RegisterInt("eGenerateCone", eGenerateCone);

	r.RegisterInt("eMeshNew", eMeshNew);
	r.RegisterInt("eMeshDelete", eMeshDelete);
	r.RegisterInt("eMeshSelect", eMeshSelect);
	r.RegisterInt("eMeshMirror", eMeshMirror);
	//r.RegisterInt("eMeshMove", eMeshMove);
	//r.RegisterInt("eMeshRotate", eMeshRotate);
	//r.RegisterInt("eMeshScale", eMeshScale);

	r.RegisterInt("ePolygonSize", ePolygonSize);
	r.RegisterInt("eGenMeshHeight", eGenMeshHeight);
	r.RegisterInt("eGenMeshCount", eGenMeshCount);
	r.RegisterInt("eGenMeshSegements", eGenMeshSegements);

	r.RegisterInt("ePointJoint", ePointJoint);
	r.RegisterInt("eSphereJoint", eSphereJoint);
	r.RegisterInt("eAxisJoint", eAxisJoint);
	r.RegisterInt("eLineBone", eLineBone);
	r.RegisterInt("ePolyMeshBone", ePolyMeshBone);
	r.RegisterInt("eRenderBbox", eRenderBbox);

	r.RegisterInt("eSetMaterialTexture", eSetMaterialTexture);
	r.RegisterInt("eTmpUVMapOff",eTmpUVMapOff);
	r.RegisterInt("eTmpUVMapOn",eTmpUVMapOn);
	r.RegisterInt("eMirrorUV_X", eMirrorUV_X);
	r.RegisterInt("eMirrorUV_Y", eMirrorUV_Y);
	r.RegisterInt("eTranslateUV", eTranslateUV);
	r.RegisterInt("eRotateUV", eRotateUV);
	r.RegisterInt("eScaleUV", eScaleUV);
	r.RegisterInt("eSetMeshTexture", eSetMeshTexture);
	r.RegisterInt("eSetPolygonTexture", eSetPolygonTexture);
	r.RegisterInt("eTextureSlotLoad", eTextureSlotLoad);

	r.RegisterInt("eAddObject", eAddObject);
	r.RegisterInt("eMoveObject", eMoveObject);
	r.RegisterInt("eRotateObject", eRotateObject);
	r.RegisterInt("eScaleObject", eScaleObject);

	r.RegisterInt("eTransformSelectedVertices", eTransformSelectedVertices);
	r.RegisterInt("eTransformGroup", eTransformGroup);
	r.RegisterInt("eTransformScene", eTransformScene);
	r.RegisterInt("eTransformMesh", eTransformMesh);
	r.RegisterInt("eTransformMeshPivot", eTransformMeshPivot);
	r.RegisterInt("eTransformBone", eTransformBone);
	r.RegisterInt("eTransformPoint", eTransformPoint);


	r.RegisterInt("eAnimationNext", eAnimationNext);
	r.RegisterInt("eAnimationPrev", eAnimationPrev);
	r.RegisterInt("eAnimationStop", eAnimationStop);
	r.RegisterInt("eAnimationPlay", eAnimationPlay);
	r.RegisterInt("eAnimationSlider", eAnimationSlider);

	/* Widget events ( widgets hold data like spinbuttons, etc ) */
	r.RegisterInt("eScale", eScale);
	r.RegisterInt("eScale_X", eScale_X);
	r.RegisterInt("eScale_Y", eScale_Y);
	r.RegisterInt("eScale_Z", eScale_Z);
	r.RegisterInt("eMove", eMove);
	r.RegisterInt("eMove_X", eMove_X);
	r.RegisterInt("eMove_Y", eMove_Y);
	r.RegisterInt("eMove_Z", eMove_Z);
	r.RegisterInt("eRotate", eRotate);
	r.RegisterInt("eRotate_X", eRotate_X);
	r.RegisterInt("eRotate_Y", eRotate_Y);
	r.RegisterInt("eRotate_Z", eRotate_Z);
	r.RegisterInt("eZoom", eZoom);
	r.RegisterInt("eCamera", eCamera);
	r.RegisterInt("eSelectMaterial", eSelectMaterial);

	r.RegisterInt("eModelIterator", eModelIterator);
	r.RegisterInt("eMeshIterator", eMeshIterator);
	r.RegisterInt("eGroupIterator", eGroupIterator);
	r.RegisterInt("eBoneIterator", eBoneIterator);

	r.RegisterInt("CMD_MISC_TEXEL_COMBINE", CMD_MISC_TEXEL_COMBINE);
	r.RegisterInt("CMD_MISC_GEN_TEXMAP_XY", CMD_MISC_GEN_TEXMAP_XY);

	r.RegisterInt("eSetTextureNameA", eSetTextureNameA);
	r.RegisterInt("eSetTextureNameB", eSetTextureNameB);
	r.RegisterInt("eSetMaterialName", eSetMaterialName);

	r.RegisterInt("eZoomIn", eZoomIn);
	r.RegisterInt("eZoomOut", eZoomOut);
	r.RegisterInt("eSelectLight", eSelectLight);

	r.RegisterInt("eColorMaterialAmbient", eColorMaterialAmbient);
	r.RegisterInt("eColorMaterialDiffuse", eColorMaterialDiffuse);
	r.RegisterInt("eColorMaterialSpecular", eColorMaterialSpecular);
	r.RegisterInt("eColorMaterialEmissive", eColorMaterialEmissive);
	r.RegisterInt("eColorLightAmbient", eColorLightAmbient);
	r.RegisterInt("eColorLightDiffuse", eColorLightDiffuse);
	r.RegisterInt("eColorLightSpecular", eColorLightSpecular);
	r.RegisterInt("eColorBackground", eColorBackground);
	r.RegisterInt("eColorGrid", eColorGrid);
	r.RegisterInt("eColorMesh", eColorMesh);
	r.RegisterInt("eColorVertex", eColorVertex);
	r.RegisterInt("eColorVertexHighlight", eColorVertexHighlight);
	r.RegisterInt("eColorMeshHighlight", eColorMeshHighlight);


	r.RegisterInt("eMode", EVENT_MISC); // Hack to fake 2i event to eventMisc
	r.RegisterInt("eVertexNew", CMD_POINT_ADD);
	//r.RegisterInt("eVertexMove", CMD_POINT_MOVE);
	r.RegisterInt("eVertexCombine", CMD_POINT_COMBINE);
	r.RegisterInt("eVertexDelete", CMD_POINT_DELETE);
	r.RegisterInt("ePolygonNew", CMD_POLYGON_ADD);	
	r.RegisterInt("ePolygonDelete", CMD_POLYGON_DELETE);	
	r.RegisterInt("ePolygonSelect", CMD_POLYGON_SELECT);	

	/* Bone edit */
	r.RegisterInt("eSetCurrentBoneName", eSetCurrentBoneName); // textbox
	r.RegisterInt("eBoneNew", CMD_BONE_NEW);
	r.RegisterInt("eBoneSelect", CMD_BONE_SELECT);
	r.RegisterInt("eBoneLinkChild", CMD_BONE_CONNECT);
	r.RegisterInt("eBoneUnLinkChild", CMD_BONE_DISCONNECT);
	r.RegisterInt("eBoneLinkMesh", CMD_BONE_ADD_MESH);
	r.RegisterInt("eBoneUnLinkMesh", CMD_BONE_DELETE_MESH);
	r.RegisterInt("eBoneMoveJoint", CMD_BONE_MOVE_PIVOT);


	r.RegisterInt("eSelectionByBox", CMD_MISC_BBOX_SELECT);
	r.RegisterInt("eTextureMapProjection", CMD_MISC_GEN_TEXMAP_XY);
	r.RegisterInt("eModeModel", FREYJA_MODE_MODEL_EDIT);
	r.RegisterInt("eModeMaterial", FREYJA_MODE_MATERIAL_EDIT);
	r.RegisterInt("eModeSkin", FREYJA_MODE_TEXTURE_EDIT);
	r.RegisterInt("eModeAnimation", FREYJA_MODE_MODEL_VIEW);

	r.RegisterInt("eGeneratePatchMesh", eGeneratePatchMesh);

	r.RegisterInt("eRenderPatch", eRenderPatch);
	r.RegisterInt("eRenderWireframe",FREYJA_MODE_RENDER_WIREFRAME);
	r.RegisterInt("eRenderFace", FREYJA_MODE_RENDER_FACE);
	r.RegisterInt("eRenderVertex", FREYJA_MODE_RENDER_POINTS);
	r.RegisterInt("eRenderNormals", FREYJA_MODE_RENDER_NORMALS);
	r.RegisterInt("eRenderTexture", FREYJA_MODE_RENDER_TEXTURE);
	r.RegisterInt("eRenderLighting", FREYJA_MODE_RENDER_LIGHTING);
	r.RegisterInt("eRenderMaterial", FREYJA_MODE_RENDER_MATERIAL);
	r.RegisterInt("eRenderSkeleton",FREYJA_MODE_RENDER_BONETAG);
	r.RegisterInt("eRenderGrid", FREYJA_MODE_RENDER_GRID);

	r.RegisterInt("eOpenGLNormalize", eOpenGLNormalize);
	r.RegisterInt("eOpenGLBlend", eOpenGLBlend);

	r.RegisterInt("FREYJA_MODE_PLANE_XY", FREYJA_MODE_PLANE_XY);
	r.RegisterInt("FREYJA_MODE_PLANE_YZ", FREYJA_MODE_PLANE_YZ);
	r.RegisterInt("FREYJA_MODE_PLANE_XZ", FREYJA_MODE_PLANE_XZ);
}


void mgtk_handle_resource_start()
{
	/* Mongoose 2002.02.02, This is the backend entry
	 *   for some damn reason it's started by the fucking
	 *   widget layer 'woo hoo'
	 *
	 *   Also needs no parms really
	 *
	 *   TODO, FIXME: Rewrite all code to focus on this file
	 *                as main entry and start interface from
	 *                here, most likey using resource for
	 *                total gui control */

	freyja_print("@Freyja started...");

	mgtk_handle_resource_init(gResource);

	gFreyjaControl = new FreyjaControl(&gResource);

	/* Build the user interface from lisp, and load user preferences */
	gFreyjaControl->loadResource();
	//gFreyjaControl->initOpenGL();
	//gFreyjaRender->Init(740, 560, true);

	freyja_refresh_material_interface();
	freyja_set_main_window_title(BUILD_NAME);

	/* Mongoose 2002.01.23, Switch to mesh mode */
	//freyja_event2i(EVENT_MISC, FREYJA_MODE_MODEL_EDIT);
	freyja_event2i(EVENT_MISC, FREYJA_MODE_MODEL_EDIT);
	mgtk_event_gl_refresh();

	freyja_print("Welcome to Freyja %s-%s, %s", VERSION, BUILD_ID, __DATE__);

	/* Mongoose 2004.10.29, 
	 * Temp test of pattern system */
	//freyja_event_fileselection_append_pattern("ASE (3ds max)", "ase");

	/* Mongoose 2002.02.23, Hook for exit() calls */
	atexit(freyja_event_shutdown);
}


void mgtk_handle_slider1u(int event, unsigned int value)
{
	gFreyjaControl->event(event, value);
}


void mgtk_handle_text(int event, char *text)
{
	if (gFreyjaControl)
	{
		gFreyjaControl->handleTextEvent(event, text);
	}
}


void mgtk_get_pixmap_filename(char *dest, unsigned int size, char *icon_name)
{
	freyja_get_pixmap_filename(dest, size, icon_name);
}


char *mgtk_rc_map(char *filename_or_dirname)
{
	return freyja_rc_map(filename_or_dirname);
}



///////////////////////////////////////////////////////////////////////
// Freyja wrappers
///////////////////////////////////////////////////////////////////////

void freyja_event_file_dialog(char *s)
{
	static bool on = 0;


	mgtk_event_file_dialog(s);

	if (!on)
	{
		extern void mgtk_add_menu_item(char *text, long event);
		long i, count = EggPlugin::mEggPlugin->getPluginDescCount();

		//mgtk_add_menu_item("All Files (*.*)", 9000);
		mgtk_event_fileselection_append_pattern("All Files (*.*)", "*.*");

		for (i = 0; i < count; ++i)
		{
			FreyjaPluginDesc *plugin = EggPlugin::mEggPlugin->getPluginDesc(i);
			
			if (plugin && plugin->mImportFlags)
				//mgtk_add_menu_item(plugin->mDescription, 9001+i);
				mgtk_event_fileselection_append_pattern(plugin->mDescription,
														plugin->mExtention);
		}
		
		
		on = 1;
	}
}


void freyja_application_window_move(int x, int y)
{
	mgtk_application_window_move(x, y);
}


void freyja_event_fullscreen()
{
	mgtk_application_window_fullscreen();
}


void freyja_event_unfullscreen()
{
	mgtk_application_window_unfullscreen();
}


void freyja_event_exit()
{
	mgtk_event_shutdown();
}

void freyja_event_gl_refresh()
{
	mgtk_event_gl_refresh();
}


void freyja_event_fileselection_append_pattern(char *label, char *pattern)
{
	mgtk_event_fileselection_append_pattern(label, pattern);
}


void freyja_event_info_dialog(char *icon, char *message)
{
	mgtk_create_info_dialog(icon, message);
}


void freyja_event_register_control(FreyjaControl *c)
{
	gFreyjaControl = c;
}


void mgtk_handle_motion(int x, int y)
{
	if (gFreyjaControl)
	{
		gFreyjaControl->motionEvent(x, y);
	}
}


void mgtk_handle_mouse(int button, int state, int mod, int x, int y)
{
	if (gFreyjaControl)
	{
		gFreyjaControl->mouseEvent(button, state, mod, x, y);
	}
}


void freyja_event_set_color(int colorId, float r, float g, float b, float a)
{
	mgtk_event_set_color(colorId, r, g, b, a);
}


int freyja_event2i(int event, int cmd)
{
	if (gFreyjaControl)
	{
		if (!gFreyjaControl->handleEvent(event, cmd))
		{
			freyja_print("  freyja_event2i passed previous unhandled event %i:%i",
						event, cmd);

			return -1;
		}
	}

	return 0;
}


FILE *get_log_file()
{
	static FILE *f = 0x0;
	char *s;

	if (!f)
	{
#ifdef LOG_CHECK_AND_WRITE
		unsigned int i;
		char filename[128];

		for (i = 0; i < 256; ++i)
		{
			snprintf(filename, 128, "/tmp/Freyja%i.log", i);

			f = fopen(filename, "rb");

			if (!f)
				break;
		}

		f = fopen(filename, "w");
#else
		s = freyja_rc_map("Freyja-chimera.log");
		f = fopen(s, "w");
#endif
	}

	return f;
}


void freyja_swap_buffers()
{
	mgtk_event_swap_gl_buffers();
}


void close_log_file()
{
	if (get_log_file())
		fclose(get_log_file());
}


void freyja_print_args(char *format, va_list *args)
{
	FILE *f = get_log_file();
	char buffer[1024];
	unsigned int l;


	// Strip message of an trailing carrage return 
	//  and print to stdout and the status bar
	vsnprintf(buffer, 1024, format, *args);
	
	l = strlen(buffer);
  
	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

#ifdef DEBUG_EVENT_PRINT
	fprintf(stdout, "DEBUG> %s\n", buffer);
#endif

	if (buffer[0] == '!')
	{
		buffer[0] = ' ';
		fprintf(stderr, "%s\n", buffer);
	}

	mgtk_event_notify_statusbar(buffer);

	if (f)
	{
		fprintf(f, "> ");
		vfprintf(f, format, *args);
		fprintf(f, "\n");
		fflush(f);
	}
}


void mgtk_print(char *format, ...)
{
	va_list args;

	va_start(args, format);
	freyja_print_args(format, &args);
	va_end(args);
}


void freyja_print(char *format, ...)
{
	va_list args;

	va_start(args, format);
	freyja_print_args(format, &args);
	va_end(args);
}


void freyja_event_shutdown()
{
	if (gFreyjaControl)
	{
		delete gFreyjaControl;
	}

	freyja_print("!Thanks for using %s", PROGRAM_NAME);
	freyja_print("!   Build date: %s @ %s", __DATE__, __TIME__);
	freyja_print("!   Build host: %s", BUILD_HOST);
	freyja_print("!   Email addr: %s", EMAIL_ADDRESS);
	freyja_print("!   Web site  : %s", PROJECT_URL);

	close_log_file();
}


char *freyja_rc_map(char *s)
{
	char *rc = NULL;
	char *path = "freyja";
	char *env;
	unsigned int len;
	
	
	if (!s || !s[0])
	{
		return NULL;
	}

	len = strlen(s) + strlen(path);
  
#ifdef unix
	env = getenv("HOME");

	if (!env || !env[0])
	{
		printf("ERROR: Bad HOME envronment\n");
		return NULL;
	}

	len += strlen(env) + 8;

   rc = new char[len + 1];
   snprintf(rc, len, "%s/.%s/%s", env, path, s);
#else
	len += 8;

   rc = new char[len + 1];
   snprintf(rc, len, "C:/%s/%s", path, s);
#endif
	
	return rc;
}


void freyja_event_notify_observer1f(event_subject_id e, float value)
{
	mgtk_event_notify_observer1f(e, value);
}


void freyja_event_set_float(int event, float value)
{
	mgtk_spinbutton_value_set(event, value);
}


int freyja_create_confirm_dialog(char *dialog_icon,
								 char *information_message, 
								 char *question_message,
								 char *cancel_icon, char *cancel_text,
								 char *accept_icon, char *accept_text)
{
	return mgtk_create_confirm_dialog(dialog_icon,
									  information_message, 
									  question_message,
									  cancel_icon, cancel_text,
									  accept_icon, accept_text);
}


void freyja_refresh_material_interface()
{
	vec4_t ambient, diffuse, specular, emissive;
	unsigned int src, dest, flags;
	vec_t shininess;


	gMaterialManager->getColor(MaterialManager::eAmbient, ambient);
	gMaterialManager->getColor(MaterialManager::eDiffuse, diffuse);
	gMaterialManager->getColor(MaterialManager::eSpecular, specular);
	gMaterialManager->getColor(MaterialManager::eEmissive, emissive);
	shininess = gMaterialManager->getShininess();
	src = gMaterialManager->getBlendSourceIndex();
	dest = gMaterialManager->getBlendDestIndex();
	flags = gMaterialManager->getFlags();

	freyja_event_set_color(eColorMaterialAmbient, 
						   ambient[0], ambient[1], ambient[2], ambient[3]);
	freyja_event_set_color(eColorMaterialDiffuse, 
						   diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	freyja_event_set_color(eColorMaterialSpecular, 
						   specular[0], specular[1], specular[2], specular[3]);
	freyja_event_set_color(eColorMaterialEmissive, 
						   emissive[0], emissive[1], emissive[2], emissive[3]);

#ifdef NOT_BACKPORTED
	freyja_event_set_color(eColorLightAmbient, 
						   OpenGLFreyjaScene::mMaterialLight.mAmbient[0],
						   OpenGLFreyjaScene::mMaterialLight.mAmbient[1],
						   OpenGLFreyjaScene::mMaterialLight.mAmbient[2],
						   OpenGLFreyjaScene::mMaterialLight.mAmbient[3]);
	freyja_event_set_color(eColorLightDiffuse, 
						   OpenGLFreyjaScene::mMaterialLight.mDiffuse[0],
						   OpenGLFreyjaScene::mMaterialLight.mDiffuse[1],
						   OpenGLFreyjaScene::mMaterialLight.mDiffuse[2],
						   OpenGLFreyjaScene::mMaterialLight.mDiffuse[3]);
	freyja_event_set_color(eColorLightSpecular,
						   OpenGLFreyjaScene::mMaterialLight.mSpecular[0],
						   OpenGLFreyjaScene::mMaterialLight.mSpecular[1],
						   OpenGLFreyjaScene::mMaterialLight.mSpecular[2],
						   OpenGLFreyjaScene::mMaterialLight.mSpecular[3]);
#endif

	mgtk_spinbutton_value_set(700, ambient[0]);	
	mgtk_spinbutton_value_set(701, ambient[1]);
	mgtk_spinbutton_value_set(702, ambient[2]);
	mgtk_spinbutton_value_set(703, ambient[3]);
	mgtk_spinbutton_value_set(704, diffuse[0]);
	mgtk_spinbutton_value_set(705, diffuse[1]);
	mgtk_spinbutton_value_set(706, diffuse[2]);
	mgtk_spinbutton_value_set(707, diffuse[3]);
	mgtk_spinbutton_value_set(708, specular[0]);
	mgtk_spinbutton_value_set(709, specular[1]);
	mgtk_spinbutton_value_set(710, specular[2]);
	mgtk_spinbutton_value_set(711, specular[3]);
	mgtk_spinbutton_value_set(712, emissive[0]);
	mgtk_spinbutton_value_set(713, emissive[1]);
	mgtk_spinbutton_value_set(714, emissive[2]);
	mgtk_spinbutton_value_set(715, emissive[3]);
	mgtk_spinbutton_value_set(716, shininess);
	mgtk_option_menu_value_set(780, src);
	mgtk_option_menu_value_set(781, dest);

	if (flags & Material::fEnable_Blending)
	{
		mgtk_togglebutton_value_set(790, true);
	}
	else
	{
		mgtk_togglebutton_value_set(790, false);
	}

	mgtk_textentry_value_set(799, gMaterialManager->getName());

	freyja_print("refresh_material_interface> FIXME %s:%i", __FILE__, __LINE__);
}


arg_list_t *freyja_rc_color(arg_list_t *args)
{
	arg_list_t *sym, *r, *g, *b, *a;


	symbol_enforce_type(&sym, INT);
	symbol_enforce_type(&r, FLOAT);
	symbol_enforce_type(&g, FLOAT);
	symbol_enforce_type(&b, FLOAT);
	symbol_enforce_type(&a, FLOAT);

	// Call event func here - simulated with printf in tests
	if (sym && r && g && b && a)
	{
#ifdef DEBUG_RESOURCE_COLOR
		freyja_print("extern \"C\" { color(%s, %f, %f, %f); }\n",
					 get_int(sym),//get_string(sym),
					 get_float(r), get_float(g), get_float(b));
#endif

		mgtk_handle_color(get_int(sym),
						  get_float(r), get_float(g), get_float(b),
						  get_float(a));
	}
	else
	{
		printf("color> Failed to extract strict typed data from script\n");
	}

	delete_arg(&sym);
	delete_arg(&r);
	delete_arg(&g);
	delete_arg(&b);
	delete_arg(&a);

	return NULL;
}


int main(int argc, char *argv[])
{
	mgtk_init(argc, argv);

	/* Mongoose 2002.02.23, 
	 * Load file passed by command line args */
	if (argc > 1)
	{
		mgtk_event_fileselection_set_dir(argv[1]);
		mgtk_handle_file_dialog_selection(argv[1]);
	}

	mgtk_start();

	return 0;
}


////////////////////////////////////////////////
// OLD CODE
////////////////////////////////////////////////

void event_register_control(FreyjaControl *c)
{
	gFreyjaControl = c;
}


void freyja_event_file_dialog_notify(char *filename)
{
	if (gFreyjaControl)
		gFreyjaControl->handleFilename(filename);
}


void mgtk_event_gldisplay()
{
	if (gFreyjaControl)
		gFreyjaControl->updateDisplay();
}


void mgtk_event_glresize(unsigned int width, unsigned int height)
{
	if (gFreyjaControl)
		gFreyjaControl->resizeDisplay(width, height);
}


void freyja_get_pixmap_filename(char *dest, unsigned int size, char *icon_name)
{
	if (size < 1)
		return;

#ifdef unix
	snprintf(dest, size, "%s/.freyja/icons/%s",
			 (char *)getenv("HOME"), icon_name);
#else
	strcpy(dest, "data/icons/%s", icon_name);
#endif

	dest[size-1] = 0;
}


void freyja_set_main_window_title(char *title)
{
	mgtk_application_window_title(title);
}