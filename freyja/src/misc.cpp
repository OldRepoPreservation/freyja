/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : (c) 2001-2007 Terry 'Mongoose' Hendrix II
 * Comments: Misc events for freyja3d client.
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.06.26:
 * Mongoose - Created
 ==========================================================================*/

#include "config.h"

#include <freyja/MeshABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LightABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/MeshABI.h>
#include <freyja/Mesh.h>
#include <freyja/Bone.h>

#include <mgtk/mgtk_events.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/ConfirmationDialog.h>
#include <mgtk/mgtk_lua.h>

#include "freyja_events.h"
#include "freyja3d_scenegraph.h"
#include "FreyjaControl.h"

using namespace freyja3d;


void polymap_update_question()
{
	if (mgtk::ExecuteConfirmationDialog("PolyMapDialog"))
	{
		Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());
		m->ConvertAllFacesToTexCoordPloymapping();
	}
}



////////////////////////////////////////////////////////////
// Non-object Event Handler Functions
////////////////////////////////////////////////////////////

void eNoImplementation(ResourceEvent *e)
{
	freyja_print("!'%s' : No longer implemented or disabled.",
				(e && e->getName()) ? e->getName() : "Unknown event");
}


void eMeshUnselectFaces()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if ( m )
	{
		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			m->ClearFaceFlags(i, Vertex::fSelected);
		}

		freyja_print("Reset selected flag on all faces in mesh.");
	}
}


void eMeshUnselectVertices()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if ( m )
	{
		for (uint32 i = 0, n = m->GetVertexCount(); i < n; ++i)
		{
			m->ClearVertexFlags(i, Vertex::fSelected);
		}

		freyja_print("Reset selected flag on all vertices in mesh.");
	}
}


void eSetSelectedViewport(unsigned int value)
{
	FreyjaControl::GetInstance()->SetSelectedViewport(value);
}


void eSelectedFacesGenerateNormals()
{
	Mesh *m = freyjaGetMeshClass(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		m->SelectedFacesGenerateVertexNormals();
	}
}

void eSelectedFacesFlipNormals()
{
	Mesh *m = freyjaGetMeshClass(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		m->SelectedFacesFlipVertexNormals();
	}
}


void eSelectedFacesDelete()
{
	Mesh *m = freyjaGetMeshClass(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		m->DeleteSelectedFaces();
	}
}


void eSmoothingGroupsDialog()
{
	uint32 id = ResourceEvent::GetResourceIdBySymbol("eSmoothingGroupsDialog");
	mgtk_event_dialog_visible_set(id, 1);	
}


void eSmooth(unsigned int group, unsigned int value)
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		freyja_print("Faces in group %i %s.", 
					 group+1, value ? "selected" : "unselected");

		if (value)
		{
			FreyjaControl::GetInstance()->mGroupBitmap |= (1<<group);
			m->SetGroupsFaceSelected(FreyjaControl::GetInstance()->mGroupBitmap);
		}
		else
		{
			m->ClearGroupsFaceSelected(FreyjaControl::GetInstance()->mGroupBitmap);
			FreyjaControl::GetInstance()->mGroupBitmap ^= (1<<group);
		}

		freyja_event_gl_refresh();
	}
}


void eGroupClear()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		uint32 group = FreyjaControl::GetInstance()->mGroupBitmap;

		if (group > 24)
		{
			freyja_print("Make sure only one group is toggled while assigning");
			return;
		}

		freyja_print("Selected faces removed from smoothing group (%i).",group);
		m->SelectedFacesMarkSmoothingGroup(group, false);
		freyja_event_gl_refresh();
	}
}


void eSetSelectedFacesAlpha()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		freyja_print("Selected faces alpha flag enabled.");
		m->SetFlagForSelectedFaces(Face::fAlpha);
		FreyjaControl::GetInstance()->Dirty();
	}
	else
	{
		freyja_print("Alpha flag can not be set. No selected faces.");
	}
}


void eClearSelectedFacesAlpha()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		freyja_print("Selected faces alpha flag enabled.");
		m->ClearFlagForSelectedFaces(Face::fAlpha);
		FreyjaControl::GetInstance()->Dirty();
	}
	else
	{
		freyja_print("Alpha flag can not be cleared. No selected faces.");
	}
}


void eGroupAssign()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		uint32 group = FreyjaControl::GetInstance()->mGroupBitmap;

		if (group > 24)
		{
			freyja_print("Make sure only one group is toggled while assigning");
			return;
		}

		freyja_print("Selected faces assigned to smoothing group (%i).", group);
		m->SelectedFacesMarkSmoothingGroup(group, true);

		// Go ahead and update the vertex normals here automatically for now
		//m->GroupedFacesGenerateVertexNormals(group);
		freyja_event_gl_refresh();
	}
}


vec_t gWeight = 1.0f;
void eWeight(vec_t w)
{
	gWeight = w;
	freyja_print("Weight set to %f", gWeight);
}

void eAssignWeight()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
		m->SetWeightSelectedVertices(bone, gWeight);
		freyja_print("Selected vertices set to bone %i weighting to %f.", bone, gWeight);
	}
}

void eClearWeight()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
		m->RemoveWeightSelectedVertices(bone);
		freyja_print("Selected vertices removing bone %i weighting...", bone);
	}
}


void eBoneRefreshBindPose()
{
	index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
	Bone *b = Bone::GetBone( bone );
	if (b) b->UpdateBindPose();
}


void eMirrorMeshX()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(-1.0f, 1.0f, 1.0f));
		freyja_print("Mirroring mesh over X...");
		m->TransformVertices(mat);
	}
}

void eMirrorMeshY()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, -1.0f, 1.0f));
		freyja_print("Mirroring mesh over Y...");
		m->TransformVertices(mat);
	}
}


void eMirrorMeshZ()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, 1.0f, -1.0f));
		freyja_print("Mirroring mesh over Z...");
		m->TransformVertices(mat);
	}
}


void eMirrorFacesX()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(-1.0f, 1.0f, 1.0f));
		freyja_print("Mirroring selected faces over X...");
		//m->TransformFacesWithFlag(Face::fSelected, mat);
		m->TransformVertices(mat);
	}
}


void eMirrorFacesY()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, -1.0f, 1.0f));
		freyja_print("Mirroring selected faces over Y...");
		m->TransformFacesWithFlag(Face::fSelected, mat);
		m->TransformVertices(mat);
	}
}


void eMirrorFacesZ()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, 1.0f, -1.0f));
		freyja_print("Mirroring selected faces over Z...");
		m->TransformFacesWithFlag(Face::fSelected, mat);
		m->TransformVertices(mat);
	}
}


vec_t gSnapWeldVertsDist = 0.001f;
void eSnapWeldVertsDist(vec_t d)
{
	gSnapWeldVertsDist = d;
}


void eSnapWeldVerts()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		m->WeldVerticesByDistance(gSnapWeldVertsDist);
	}
}

void eCleanupVertices()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		m->VertexCleanup();
	}
}



void eGenerateCone()
{
	FREYJA_INFOMSG(0, "FIXME: Replace obsolete mesh generator.");
	const char *dialog = "GenerateTubeDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		//int count = mgtk_get_query_dialog_int(dialog, "count");
		int wedges = mgtk_get_query_dialog_int(dialog, "segments");
		vec_t height = mgtk_get_query_dialog_float(dialog, "height");
		vec_t radius = mgtk_get_query_dialog_float(dialog, "radius");
	
		hel::Vec3 v;
		//freyjaGenerateConeMesh(v.mVec, height, count);
		freyjaMeshCreateCone(v.mVec, height, radius, wedges);
		FreyjaControl::GetInstance()->Dirty();
	}
}


void eGenerateCylinder()
{
	FREYJA_INFOMSG(0, "FIXME: Replace obsolete mesh generator.");
	const char *dialog = "GenerateTubeDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int count = mgtk_get_query_dialog_int(dialog, "count");
		int segments = mgtk_get_query_dialog_int(dialog, "segments");
		vec_t height = mgtk_get_query_dialog_float(dialog, "height");
		vec_t radius = mgtk_get_query_dialog_float(dialog, "radius");
	
		hel::Vec3 v;
		//freyjaGenerateCylinderMesh(v.mVec, height, count, segments);
		freyjaMeshCreateCylinder(v.mVec, height, radius, count, segments);
		FreyjaControl::GetInstance()->Dirty();
	}
}


void eGenerateTube()
{
	const char *dialog = "GenerateTubeDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int count = mgtk_get_query_dialog_int(dialog, "count");
		int segments = mgtk_get_query_dialog_int(dialog, "segments");
		vec_t height = mgtk_get_query_dialog_float(dialog, "height");
		vec_t radius = mgtk_get_query_dialog_float(dialog, "radius");

		hel::Vec3 v;
		//freyjaGenerateTubeMesh(v.mVec, height, count, segments);
		freyjaMeshCreateTube(v.mVec, height, radius, count, segments);
		FreyjaControl::GetInstance()->Dirty();		
	}
}


void eGenerateSphere()
{
	const char *dialog = "GenerateSphereDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int count = mgtk_get_query_dialog_int(dialog, "count");
		vec_t radius = mgtk_get_query_dialog_float(dialog, "radius");

		hel::Vec3 v;
		//freyjaGenerateSphereMesh(v.mVec, radius * 2.0f, count, count);
		index_t mesh = freyjaMeshCreateSphere(v.mVec, radius * 2.0f, count, count);
		FreyjaControl::GetInstance()->SetSelectedMesh(mesh);
		FreyjaControl::GetInstance()->Dirty();
	}
}


void eGenerateCube()
{
	//FREYJA_INFOMSG(0, "FIXME: Replace obsolete mesh generator.");
	const char *dialog = "GenerateCubeDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		//int rows = mgtk_get_query_dialog_int(dialog, "rows");
		//int cols = mgtk_get_query_dialog_int(dialog, "cols");
		float size = mgtk_get_query_dialog_float(dialog, "size");

		hel::Vec3 v(size * -0.5f, 0.0f, size * -0.5f);
		index_t mesh = freyjaMeshCreateCube(v.mVec, size);
		FreyjaControl::GetInstance()->SetSelectedMesh(mesh);
		FreyjaControl::GetInstance()->Dirty();

		freyja3d_scenegraph_update();
	}
}


void eGeneratePlane()
{
	const char *dialog = "GenerateSheetDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int rows = mgtk_get_query_dialog_int(dialog, "rows");
		int cols = mgtk_get_query_dialog_int(dialog, "cols");
		vec_t size = mgtk_get_query_dialog_float(dialog, "size");

		//FREYJA_ASSERTMSG(0, "%i %i %f", rows, cols, size);

		size *= 4;
		hel::Vec3 v(size * -0.5f, 0.3f, size * -0.5f);
		index_t mesh = freyjaMeshCreateSheet(v.mVec, size, rows, cols);
		FreyjaControl::GetInstance()->SetSelectedMesh(mesh);
		FreyjaControl::GetInstance()->Dirty();		
	}
}


void eGenerateCircle()
{
	const char *dialog = "GenerateCircleDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int count = mgtk_get_query_dialog_int(dialog, "count");
		vec_t r = mgtk_get_query_dialog_float(dialog, "radius");
		hel::Vec3 v(0.0f, 0.2f, 0.0f);

		index_t mesh = freyjaMeshCreateCircle(v.mVec, r, count);
		FreyjaControl::GetInstance()->SetSelectedMesh(mesh);
		FreyjaControl::GetInstance()->Dirty();		
	}
}


void eGenerateRing()
{
	const char *dialog = "GenerateRingDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int rings = mgtk_get_query_dialog_int(dialog, "rings");
		int count = mgtk_get_query_dialog_int(dialog, "count");
		vec_t r = mgtk_get_query_dialog_float(dialog, "radius");
		hel::Vec3 v(0.0f, 0.2f, 0.0f);

		index_t mesh = freyjaMeshCreateRing(v.mVec, r, count, rings+1);
		FreyjaControl::GetInstance()->SetSelectedMesh(mesh);
		FreyjaControl::GetInstance()->Dirty();		
	}
}


void eMeshTesselate()
{
	freyjaMeshTesselateTriangles(FreyjaControl::GetInstance()->GetSelectedMesh());
	FreyjaControl::GetInstance()->Dirty();
}


void eMeshTexcoordSpherical()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());
	if (m)
	{
		freyja_print("Spherical texcoord generation for selected faces...");
		m->UVMapSelectedFaces_Spherical();
		polymap_update_question();
		FreyjaControl::GetInstance()->Dirty();
	}
}


void eMeshTexcoordCylindrical()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());
	if (m)
	{
		freyja_print("Cylindrical texcoord generation for selected faces...");
		m->UVMapSelectedFaces_Cylindrical();
		polymap_update_question();
		FreyjaControl::GetInstance()->Dirty();
	}
}


void eMeshTexcoordPlaneProj()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());
	if (m)
	{
		freyja_print("Planar texcoord generation for selected faces...");
		m->UVMapSelectedFaces_Plane();
		polymap_update_question();
		FreyjaControl::GetInstance()->Dirty();
	}
}


void eMeshGenerateNormals()
{
	freyjaMeshGenerateVertexNormals(FreyjaControl::GetInstance()->GetSelectedMesh());
	freyja_event_gl_refresh();
	FreyjaControl::GetInstance()->Dirty();
}

// This should be moved back into control with a -W/Int prefix
void eMoveObject()
{
	FreyjaControl::GetInstance()->EvMoveObject(1);
}

void eRotateObject()
{
	FreyjaControl::GetInstance()->EvRotateObject(1);
}	

void eScaleObject()
{
	FreyjaControl::GetInstance()->EvScaleObject(1);
}	

void eSelect()
{
	FreyjaControl::GetInstance()->EvSelectObject(1);
}

void eUnselect()
{
	FreyjaControl::GetInstance()->EvUnselectObject(1);
}		


void eBoneSelect()
{
	FreyjaControl::GetInstance()->SetObjectMode(FreyjaControl::tBone);
	FreyjaControl::GetInstance()->SetActionMode(FreyjaControl::aSelect);
	freyja_print("Select bone...");
}


void eBoneNew()
{
	FreyjaControl::GetInstance()->SetObjectMode(FreyjaControl::tBone);
	FreyjaControl::GetInstance()->CreateObject();
	//freyja_print("Select new child bone placement directly...");
	//mEventMode = BONE_ADD_MODE;
	FreyjaControl::GetInstance()->Dirty();
}


void eExtrude()
{
	// FIXME: Give user the option of pick ray and face normal control
	hel::Vec3 v = FreyjaRender::mTestRay.GetDir();
	v *= -8.0f;

	index_t mesh = FreyjaControl::GetInstance()->GetSelectedMesh();
	index_t face = FreyjaControl::GetInstance()->GetSelectedFace();

	Mesh *m = Mesh::GetMesh( mesh );
					
	if (m)
	{			 
		Face *f = m->GetFace( face );
		v = f ? f->mNormal : v;
		
	}

	freyjaMeshPolygonExtrudeQuad1f(FreyjaControl::GetInstance()->GetSelectedMesh(), 
								   FreyjaControl::GetInstance()->GetSelectedFace(),
								   v.mVec);
#if 0
	// Currently we don't support this 
	// ( it marks all the new face vertices as selected )
	if (freyjaGetPolygonVertexCount(GetSelectedFace()))
	{
		long polygonIndex = GetSelectedFace();
		long i, idx, count = freyjaGetPolygonVertexCount(polygonIndex);
		Vector<unsigned int> list = GetVertexSelectionList();
		
		list.clear();
		
		for (i = 0; i < count; ++i)
		{
			idx = freyjaGetPolygonVertexIndex(polygonIndex, i);
			list.pushBack(idx);
		}
	}
#endif

	FreyjaControl::GetInstance()->Dirty();
}

void eMeshFlipNormals()
{
	freyjaMeshNormalFlip(FreyjaControl::GetInstance()->GetSelectedMesh());
	freyja_print("Flipping normals for mesh[%i]", 
				 FreyjaControl::GetInstance()->GetSelectedMesh());
	FreyjaControl::GetInstance()->Dirty();
}


void eHelpDialog()
{
	freyja_set_dialog_visible("eHelpDialog");
}

void ePreferencesDialog()
{
	freyja_set_dialog_visible("ePreferencesDialog");
}

void eAboutDialog()
{
	freyja_set_dialog_visible("eAboutDialog");
}


void eAnimationNext()
{
	FreyjaControl::GetInstance()->SetSelectedAnimation(FreyjaControl::GetInstance()->GetSelectedAnimation() + 1);
	freyja_print("Animation Track[%i].", 
				 FreyjaControl::GetInstance()->GetSelectedAnimation());
}

void eAnimationPrev()
{
	if (FreyjaControl::GetInstance()->GetSelectedAnimation())
		FreyjaControl::GetInstance()->SetSelectedAnimation(FreyjaControl::GetInstance()->GetSelectedAnimation() - 1);
	else
		FreyjaControl::GetInstance()->SetSelectedAnimation(0);
	
	freyja_print("Animation Track[%i].", 
				 FreyjaControl::GetInstance()->GetSelectedAnimation());
}


void eLightPos(uint32 i, vec_t value)
{
	uint32 light = FreyjaControl::GetInstance()->GetSelectedLight();
	vec3_t pos;
	freyjaGetLightPosition4v(light, pos);
	pos[i] = value;
	freyjaLightPosition4v(light, pos);
	FreyjaControl::GetInstance()->Dirty();	
}


void eLightPosX(vec_t value)
{
	eLightPos(0, value);
}


void eLightPosY(vec_t value)
{
	eLightPos(1, value);
}


void eLightPosZ(vec_t value)
{
	eLightPos(2, value);
}


void ePolygonSize(uint32 value)
{
	FreyjaControl::GetInstance()->SetFaceEdgeCount(value);
	freyja_print("Polygons creation using %i sides", 
				 FreyjaControl::GetInstance()->GetFaceEdgeCount());
}


mstl::String gTestTextView("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
void eTestTextViewText(char *text)
{
	gTestTextView = text;
}


void eTestTextView()
{
	uint32 e = ResourceEvent::GetResourceIdBySymbol("eTestTextViewText");

	mgtk_create_query_dialog_text("gtk-dialog-question", 
								  "TextView editor test.", 
								  e, gTestTextView.c_str());
}


void eTestAssertMsgBox()
{
	void *ptr = NULL;

	FREYJA_ASSERTMSG(ptr != NULL, 
					 "Dummy NULL pointer assertion test.\n\tptr = %p", 
					 ptr);
}


void eBoneMetadataText(char *text)
{
	index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
	Bone *b = Bone::GetBone( bone );
	if (text && b)
	{
		b->mMetaData = text;
	}
}


void eBoneMetadata()
{
	index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
	Bone *b = Bone::GetBone( bone );
	if (b)
	{
		mstl::String s;
		s.Set("Bone[%u] '%s' metadata.", 
			  b->GetUID(), b->GetName());
		uint32 e = ResourceEvent::GetResourceIdBySymbol("eBoneMetadataText");
		mgtk_create_query_dialog_text("gtk-dialog-question", 
									  s.c_str(), e, b->mMetaData.c_str());
	}
}


void eBoneKeyFrameMetadataText(char *text)
{
	index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
	Bone *b = Bone::GetBone( bone );
	if (text && b)
	{
		BoneTrack &track = b->GetTrack(FreyjaControl::GetInstance()->GetSelectedAnimation());
		uint32 key = FreyjaControl::GetInstance()->GetSelectedKeyFrame();
		track.SetMetadata(key, text);
	}
}


void eBoneKeyFrameMetadata()
{
	FreyjaControl *cntrl = FreyjaControl::GetInstance();

	index_t bone = cntrl->GetSelectedBone();
	Bone *b = Bone::GetBone( bone );
	if (b)
	{
		BoneTrack &track = b->GetTrack(cntrl->GetSelectedAnimation());
		uint32 k = cntrl->GetSelectedKeyFrame();
		vec_t time = (vec_t)k / track.GetRate(); 
		index_t id = track.NewRotKeyframe(time); // returns old if found

		mstl::String s;
		s.Set("Bone[%u] '%s' keyframe %i metadata.", 
			  b->GetUID(), b->GetName(), id);
		uint32 e = ResourceEvent::GetResourceIdBySymbol("eBoneKeyFrameMetadataText");
		mgtk_create_query_dialog_text("gtk-dialog-question", 
									  s.c_str(), e, track.GetMetadata(id) );
	}
}


void eResetColorsToDefault()
{
	freyja_handle_color(eColorBackground, 0.5f, 0.5f, 0.5f, 1.0f);
	freyja_handle_color(eColorGrid,	0.4f, 0.4f, 0.4f, 0.9f);
	freyja_handle_color(eColorMesh,	0.0f, 0.0f, 0.0f, 1.0f);
	freyja_handle_color(eColorMeshHighlight, 0.2f, 0.2f, 1.0f, 1.0f);
	freyja_handle_color(eColorVertex, 0.2f, 1.0f, 1.0f, 1.0f);
	freyja_handle_color(eColorVertexHighlight, 1.0f, 0.0f, 0.86f, 1.0f);
	freyja_handle_color(eColorBone,	0.882352941f, 0.654901961f, 0.219607843f, 1.0f);
	freyja_handle_color(eColorBoneHighlight, 0.956862745f, 0.415686275f, 0.2f, 1.0f);
	freyja_handle_color(eColorJoint, 1.0f, 1.0f, 0.0f, 1.0f);
	freyja_handle_color(eColorJointHighlight, 1.0f, 0.75f, 0.75f, 1.0f);
	freyja_handle_color(eColorLightAmbient,	0.8f, 0.8f, 0.8f, 1.0f);
	freyja_handle_color(eColorLightDiffuse, 0.8f, 0.8f, 0.8f, 1.0f);
	freyja_handle_color(eColorLightSpecular, 0.8f, 0.8f, 0.8f, 1.0f);
}


void freyja3d_misc_events_attach()
{
	// Empty menu events
	ResourceEventCallback2::add("eImportFile", &eNoImplementation);
	ResourceEventCallback2::add("eExportFile", &eNoImplementation);

	// Test events
	ResourceEventCallback::add("eTestAssertMsgBox", &eTestAssertMsgBox);
	ResourceEventCallback::add("eTestTextView", &eTestTextView);
	ResourceEventCallbackString::add("eTestTextViewText", &eTestTextViewText);

	// Misc events
	ResourceEventCallback::add("eResetColorsToDefault", &eResetColorsToDefault);
	ResourceEventCallbackString::add("eBoneKeyFrameMetadataText", &eBoneKeyFrameMetadataText);
	ResourceEventCallback::add("eBoneKeyFrameMetaData", &eBoneKeyFrameMetadata);
	ResourceEventCallback::add("eBoneRefreshBindPose", &eBoneRefreshBindPose);
	ResourceEventCallbackString::add("eBoneMetadataText", &eBoneMetadataText);
	ResourceEventCallback::add("eBoneMetaData", &eBoneMetadata);
	ResourceEventCallback::add("eBoneNew", &eBoneNew);
	ResourceEventCallback::add("eBoneSelect", &eBoneSelect);

	ResourceEventCallback::add("eMoveObject", &eMoveObject);
	ResourceEventCallback::add("eRotateObject", &eRotateObject);
	ResourceEventCallback::add("eScaleObject", &eScaleObject);
	ResourceEventCallback::add("eSelect", &eSelect);
	ResourceEventCallback::add("eUnselect", &eUnselect);

	ResourceEventCallback::add("eGenerateRing", &eGenerateRing);
	ResourceEventCallback::add("eGenerateCircle", &eGenerateCircle);
	ResourceEventCallback::add("eGeneratePlane", &eGeneratePlane);
	ResourceEventCallback::add("eGenerateCube", &eGenerateCube);
	ResourceEventCallback::add("eGenerateSphere", &eGenerateSphere);
	ResourceEventCallback::add("eGenerateTube", &eGenerateTube);
	ResourceEventCallback::add("eGenerateCylinder", &eGenerateCylinder);
	ResourceEventCallback::add("eGenerateCone", &eGenerateCone);

	ResourceEventCallbackVec::add("eSnapWeldVertsDist", &eSnapWeldVertsDist);

	ResourceEventCallback::add("eSetSelectedFacesAlpha", &eSetSelectedFacesAlpha);
	ResourceEventCallback::add("eClearSelectedFacesAlpha", &eClearSelectedFacesAlpha);

	ResourceEventCallback::add("eAssignWeight", &eAssignWeight);
	ResourceEventCallback::add("eClearWeight", &eClearWeight);
	ResourceEventCallbackVec::add("eWeight", &eWeight);

	ResourceEventCallback::add("eMirrorFacesX", &eMirrorFacesX);
	ResourceEventCallback::add("eMirrorFacesY", &eMirrorFacesY);
	ResourceEventCallback::add("eMirrorFacesZ", &eMirrorFacesZ);

	ResourceEventCallback::add("eMeshTesselate", &eMeshTesselate);
	ResourceEventCallback::add("eMeshTexcoordSpherical", &eMeshTexcoordSpherical);
	ResourceEventCallback::add("eMeshTexcoordCylindrical", &eMeshTexcoordCylindrical);
	ResourceEventCallback::add("eMeshGenerateNormals", &eMeshGenerateNormals);
	ResourceEventCallback::add("eMeshTexcoordPlaneProj", &eMeshTexcoordPlaneProj);
	ResourceEventCallback::add("eMirrorMeshX", &eMirrorMeshX);
	ResourceEventCallback::add("eMirrorMeshY", &eMirrorMeshY);
	ResourceEventCallback::add("eMirrorMeshZ", &eMirrorMeshZ);

	ResourceEventCallback::add("eCleanupVertices", eCleanupVertices);
	ResourceEventCallback::add("eSnapWeldVerts", &eSnapWeldVerts);

	ResourceEventCallback::add("eGroupClear", &eGroupClear);
	ResourceEventCallback::add("eGroupAssign", &eGroupAssign);

	ResourceEventCallback::add("eSmoothingGroupsDialog", eSmoothingGroupsDialog);
	ResourceEventCallbackUInt2::add("eSmooth", &eSmooth);

	ResourceEventCallback::add("eSelectedFacesFlipNormals", &eSelectedFacesFlipNormals);
	ResourceEventCallback::add("eSelectedFacesGenerateNormals", &eSelectedFacesGenerateNormals);
	ResourceEventCallback::add("eSelectedFacesDelete", &eSelectedFacesDelete);
	ResourceEventCallback::add("eMeshUnselectFaces", &eMeshUnselectFaces);
	ResourceEventCallback::add("eMeshUnselectVertices", &eMeshUnselectVertices);

	ResourceEventCallbackUInt::add("eSetSelectedViewport", &eSetSelectedViewport);
	ResourceEventCallbackUInt::add("ePolygonSize", &ePolygonSize);

	// FIXME, These light events don't appear to trigger
	ResourceEventCallbackVec::add("eLightPosX", &eLightPosX);
	ResourceEventCallbackVec::add("eLightPosY", &eLightPosY);
	ResourceEventCallbackVec::add("eLightPosZ", &eLightPosZ);

	ResourceEventCallback::add("eAnimationNext", &eAnimationNext);
	ResourceEventCallback::add("eAnimationPrev", &eAnimationPrev);

	ResourceEventCallback::add("eMeshFlipNormals", &eMeshFlipNormals);
	ResourceEventCallback::add("eHelpDialog", &eHelpDialog);
	ResourceEventCallback::add("ePreferencesDialog", &ePreferencesDialog);
	ResourceEventCallback::add("eAboutDialog", &eAboutDialog);

	ResourceEventCallback::add("eExtrude", &eExtrude);


	// Not implemented or removed misc events
	ResourceEventCallback2::add("eBezierPolygonPatch", &eNoImplementation);
	ResourceEventCallback2::add("eShaderSlotLoad", &eNoImplementation);
	ResourceEventCallback2::add("eOpenFileTextureB", &eNoImplementation);
	ResourceEventCallback2::add("eCollapseFace", &eNoImplementation);
	ResourceEventCallback2::add("eSetMaterialTextureB", &eNoImplementation);
	ResourceEventCallback2::add("eEnableMaterialFragment", &eNoImplementation);
	ResourceEventCallback2::add("eUVPickRadius", &eNoImplementation);
	ResourceEventCallback2::add("eVertexPickRadius", &eNoImplementation);
	ResourceEventCallback2::add("eAnimationStop", &eNoImplementation);
	ResourceEventCallback2::add("eAnimationPlay", &eNoImplementation);
	ResourceEventCallback2::add("eBoneLinkChild", &eNoImplementation);
	ResourceEventCallback2::add("eBoneUnLinkChild", &eNoImplementation);
	ResourceEventCallback2::add("eAppendFile", &eNoImplementation);
	ResourceEventCallback2::add("eBoneDelete", &eNoImplementation);

	ResourceEventCallback2::add("eVertexNew", &eNoImplementation);
	ResourceEventCallback2::add("eVertexDelete", &eNoImplementation);

	ResourceEventCallback2::add("ePolygonNew", &eNoImplementation);
	ResourceEventCallback2::add("ePolygonDelete", &eNoImplementation);
	ResourceEventCallback2::add("ePolygonSelect", &eNoImplementation);

	ResourceEventCallback2::add("eRenderShadow", &eNoImplementation);
	ResourceEventCallback2::add("ePolyMapTexturePolygon", &eNoImplementation);
	ResourceEventCallback2::add("eUVMapCreate", &eNoImplementation);
	ResourceEventCallback2::add("eUVMapDelete", &eNoImplementation);
	ResourceEventCallback2::add("eMirrorUV_X", &eNoImplementation);
	ResourceEventCallback2::add("eMirrorUV_Y", &eNoImplementation);
	ResourceEventCallback2::add("eTranslateUV", &eNoImplementation);
	ResourceEventCallback2::add("eRotateUV", &eNoImplementation);
	ResourceEventCallback2::add("eScaleUV", &eNoImplementation);
}


///////////////////////////////////////////////////////////////////////

