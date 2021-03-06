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
#include <freyja/PixelBuffer.h>
#include <freyja/LightABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/MeshABI.h>
#include <freyja/Mesh.h>
#include <freyja/Bone.h>
#include <freyja/Scene.h>

#include <mgtk/mgtk_events.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/ConfirmationDialog.h>
#include <mgtk/mgtk_lua.h>

#include "freyja_events.h"
#include "freyja3d_scenegraph.h"
#include "Control.h"

using namespace freyja3d;
using namespace freyja;


void polymap_update_question()
{
	if (mgtk::ExecuteConfirmationDialog("PolyMapDialog"))
	{
		//FIXME Mesh *m = gScene->GetSelectedMesh();
		//FIXME m->ConvertAllFacesToTexCoordPloymapping();
	}
}


int freyja3d_misc_vec_compare_func(const void* a, const void* b)
{
	vec_t aa = *(vec_t*)a;
	vec_t bb = *(vec_t*)b;

	return ( aa < bb ) ? -1 : ( aa == bb ) ? 0 : 1;
}


////////////////////////////////////////////////////////////
// Non-object Event Handler Functions
////////////////////////////////////////////////////////////

void eOpenFile( )
{
	mstl::String path =freyja_rc_map_string( "/snippets/" ); 

	char *filename =
	mgtk_filechooser_blocking("freyja - Open Model...", 
							  //mRecentModel.GetPath(), 
							  path.c_str(), 0,
							  "Freyja Modeler (*.freyja)", "*.freyja");
		
	if ( filename )
	{
		if ( gScene && gScene->Unserialize( filename ) )
		{
			freyja3d_print( "Imported '%s'.", filename );
		}
		else
		{
			freyja3d_print( "Failed to import '%s'.", filename );
		}
	}
	
	mgtk_filechooser_blocking_free( filename );
}


void eExportKeyAsMesh()
{
	Mesh* mesh = gScene->GetCurrentMesh();

	if ( mesh )
	{	
		//FIXME Mesh* copy = mesh->CopyWithBlendedVertices();
		//FIXME copy->AddToPool();
		freyja3d_print( "Converted keyframe to mesh." );
	}
}


void eConvertSkelToMeshAnim()
{
#if FIXME
	// FIXME: It might be good to allow an option to pad out times 
	//        to a set interval instead of just unique frames.

	uint32 track_id = FreyjaControl::GetInstance()->GetSelectedAnimation();

	Vector<vec_t> times;

	for ( uint32 bone_id = 0, bone_count = freyjaGetBoneCount(); bone_id < bone_count; ++bone_id )
	{
		/* Gather up a list of keyframe times. */
		Bone* bone = Bone::GetBone( bone_id );
		BoneTrack& track = bone->GetTrack( track_id );
		//vec_t rate_inverse = 1.0f / track.GetRate();

		for ( uint32 i = 0; i < track.GetRotKeyframeCount(); ++i )
		{
			vec_t time = track.GetRotKeyframeTime( i );
			//vec3_t rot;
			//track.GetRotKeyframe( i, rot );
			//vec_t time = (vec_t)i * rateInverse;
			times.push_back( time );
		}

		for ( uint32 i = 0; i < track.GetLocKeyframeCount(); ++i )
		{
			vec_t time = track.GetLocKeyframeTime( i );
			times.push_back( time );
		}
	}


	{
		/* Remove repeated time markers in a not so efficent manner. */
		Vector<vec_t> unique_times;

		uint32 i, j;
		foreach( times, i )
		{
			bool found = false;

			foreach( unique_times, j )
			{
				if ( unique_times[ j ] + 0.001f > times[ i ] &&
					 unique_times[ j ] - 0.001f < times[ i ] )
					found = true;
			}

			if ( !found )
			{
				unique_times.push_back( times[ i ] );
			}
		}

		freyja_print( "%s: Frame reduction %i -> %i", __func__, times.size(), unique_times.size() );

		/* Sort the frame times for the correct order. */
		for (uint32 i = 0, n = unique_times.size(); i < n; ++i)
		{
			for (uint32 j = i, n = unique_times.size(); j < n; ++j)
			{
				if ( unique_times[i] > unique_times[j] )
					unique_times.swap( i, j );
			}
		}

#if 0 // Log the frames written for animator debugging use.
		foreach( unique_times, i )
		{
			freyja_print( "%i. %f", i, unique_times[i] );
		}	
#endif

		/* Generate blended geometry for times, and store them as vertex morph frames. */ 
		index_t mesh_id = FreyjaControl::GetInstance()->GetSelectedMesh();

		Mesh* mesh = Mesh::GetMesh( mesh_id );

		if ( mesh )
		{
			Mesh* clone = new Mesh( *mesh );
			index_t clone_id = clone->AddToPool();

			foreach( unique_times, i )
			{
				freyjaMeshUpdateBlendVertices( clone_id, track_id, unique_times[i] );
				uint32 k = clone->CreateVertexKeyframeFromBlended( 0, unique_times[i] );
				freyja_print( "%i. %i", i, k );
			}

			for ( uint32 i = 0, n = clone->GetVertexCount(); i < n; ++i )
			{
				clone->RemoveWeight( i );
			}

			freyja_print( "%s: Converted frames %i -> %i", 
						  __func__, unique_times.size(), clone->GetVertexAnimKeyframeCount(0) );
		}
	}
#endif
}


void eMeshUnselectFaces()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if ( m )
	{
		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			m->ClearFaceFlags(i, Vertex::fSelected);
		}

		freyja_print("Reset selected flag on all faces in mesh.");
	}
#endif
}


void eMeshUnselectVertices()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if ( m )
	{
		for (uint32 i = 0, n = m->GetVertexCount(); i < n; ++i)
		{
			m->ClearVertexFlags(i, Vertex::fSelected);
		}

		freyja_print("Reset selected flag on all vertices in mesh.");
	}
#endif
}


void eSetSelectedViewport(unsigned int value)
{
	//FreyjaControl::GetInstance()->SetSelectedViewport(value);
}


void eSelectedFacesGenerateNormals()
{
#if FIXME
	Mesh *m = gScene->GetSelectedMesh();

	if (m)
	{
		m->SelectedFacesGenerateVertexNormals();
	}
#endif
}


void eSelectedFacesFlipNormals()
{
#if FIXME
	Mesh *m = gScene->GetSelectedMesh();

	if (m)
	{
		m->SelectedFacesFlipVertexNormals();
	}
#endif
}


void eSelectedFacesDelete()
{
#if FIXME
	Mesh *m = gScene->GetSelectedMesh();

	if (m)
	{
		m->DeleteSelectedFaces();
	}
#endif
}


void eSmoothingGroupsDialog()
{
#if FIXME
	uint32 id = ResourceEvent::GetResourceIdBySymbol("eSmoothingGroupsDialog");
	mgtk_event_dialog_visible_set(id, 1);	
#endif
}


void eSmooth(unsigned int group, unsigned int value)
{
#if FIXME
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
#endif
}


void eGroupClear()
{
#if FIXME
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
#endif
}


void eSetSelectedFacesAlpha()
{
#if FIXME
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
#endif
}


void eClearSelectedFacesAlpha()
{
#if FIXME
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
#endif
}


void eGroupAssign()
{
#if FIXME
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
#endif
}


vec_t gWeight = 1.0f;
void eWeight(vec_t w)
{
	gWeight = w;
	freyja3d_print("Weight set to %f", gWeight);
}

void eAssignWeight()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
		m->SetWeightSelectedVertices(bone, gWeight);
		freyja_print("Selected vertices set to bone %i weighting to %f.", bone, gWeight);
	}
#endif
}

void eClearWeight()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
		m->RemoveWeightSelectedVertices(bone);
		freyja_print("Selected vertices removing bone %i weighting...", bone);
	}
#endif
}


void eBoneRefreshBindPose()
{
#if FIXME
	index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
	Bone *b = Bone::GetBone( bone );
	if (b) b->UpdateBindPose();
#endif
}


void eMirrorMeshX()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(-1.0f, 1.0f, 1.0f));
		freyja_print("Mirroring mesh over X...");
		m->TransformVertices(mat);
	}
#endif
}

void eMirrorMeshY()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, -1.0f, 1.0f));
		freyja_print("Mirroring mesh over Y...");
		m->TransformVertices(mat);
	}
#endif
}


void eMirrorMeshZ()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, 1.0f, -1.0f));
		freyja_print("Mirroring mesh over Z...");
		m->TransformVertices(mat);
	}
#endif
}


void eMirrorFacesX()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(-1.0f, 1.0f, 1.0f));
		freyja_print("Mirroring selected faces over X...");
		//m->TransformFacesWithFlag(Face::fSelected, mat);
		m->TransformVertices(mat);
	}
#endif
}


void eMirrorFacesY()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, -1.0f, 1.0f));
		freyja_print("Mirroring selected faces over Y...");
		m->TransformFacesWithFlag(Face::fSelected, mat);
		m->TransformVertices(mat);
	}
#endif
}


void eMirrorFacesZ()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, 1.0f, -1.0f));
		freyja_print("Mirroring selected faces over Z...");
		m->TransformFacesWithFlag(Face::fSelected, mat);
		m->TransformVertices(mat);
	}
#endif
}


vec_t gSnapWeldVertsDist = 0.001f;
void eSnapWeldVertsDist(vec_t d)
{
	gSnapWeldVertsDist = d;
}


void eSnapWeldVerts()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		m->WeldVerticesByDistance(gSnapWeldVertsDist);
	}
#endif
}

void eCleanupVertices()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());

	if (m)
	{
		m->VertexCleanup();
	}
#endif
}



void eGenerateCone()
{
#if FIXME
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
#endif
}


void eGenerateCylinder()
{
#if FIXME
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
#endif
}


void eGenerateTube()
{
#if FIXME
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
#endif
}


void eGenerateSphere()
{
#if FIXME
	const char *dialog = "GenerateSphereDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int count = mgtk_get_query_dialog_int(dialog, "count");
		vec_t radius = mgtk_get_query_dialog_float(dialog, "radius");

		hel::Vec3 v;
		//freyjaGenerateSphereMesh(v.mVec, radius * 2.0f, count, count);
		freyja::Mesh* mesh = freyjaMeshCreateSphere(v.mVec, radius * 2.0f, count, count);
		//FreyjaControl::GetInstance()->SetSelectedMesh(mesh);
		FreyjaControl::GetInstance()->Dirty();
	}
#endif
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
		freyja::Mesh* mesh = (freyja::Mesh*)freyjaMeshCreateCube( v.mVec, size );
		gScene->Add( mesh );
		gScene->SetCurrentMesh( mesh );
		//FreyjaControl::GetInstance()->Dirty();

		freyja3d_scenegraph_update(); // FIXME: Should be updated via observer model.
	}
}


void eGeneratePlane()
{
#if FIXME
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
#endif
}


void eGenerateCircle()
{
#if FIXME
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
#endif
}


void eGenerateRing()
{
#if FIXME
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
#endif
}


void eMeshTesselate()
{
#if FIXME
	freyjaMeshTesselateTriangles(FreyjaControl::GetInstance()->GetSelectedMesh());
	FreyjaControl::GetInstance()->Dirty();
#endif
}


void eMeshTexcoordSpherical()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());
	if (m)
	{
		freyja_print("Spherical texcoord generation for selected faces...");
		m->UVMapSelectedFaces_Spherical();
		polymap_update_question();
		FreyjaControl::GetInstance()->Dirty();
	}
#endif
}


void eMeshTexcoordCylindrical()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());
	if (m)
	{
		freyja_print("Cylindrical texcoord generation for selected faces...");
		m->UVMapSelectedFaces_Cylindrical();
		polymap_update_question();
		FreyjaControl::GetInstance()->Dirty();
	}
#endif
}


void eMeshTexcoordPlaneProj()
{
#if FIXME
	Mesh *m = Mesh::GetMesh(FreyjaControl::GetInstance()->GetSelectedMesh());
	if (m)
	{
		freyja_print("Planar texcoord generation for selected faces...");
		m->UVMapSelectedFaces_Plane();
		polymap_update_question();
		FreyjaControl::GetInstance()->Dirty();
	}
#endif
}


void eMeshGenerateNormals()
{
#if FIXME
	freyjaMeshGenerateVertexNormals(FreyjaControl::GetInstance()->GetSelectedMesh());
	freyja_event_gl_refresh();
	FreyjaControl::GetInstance()->Dirty();
#endif
}

// This should be moved back into control with a -W/Int prefix
void eMoveObject()
{
	//FreyjaControl::GetInstance()->EvMoveObject(1);
}

void eRotateObject()
{
	//FreyjaControl::GetInstance()->EvRotateObject(1);
}	

void eScaleObject()
{
	//FreyjaControl::GetInstance()->EvScaleObject(1);
}	

void eSelect()
{
	//FreyjaControl::GetInstance()->EvSelectObject(1);
}

void eUnselect()
{
	//FreyjaControl::GetInstance()->EvUnselectObject(1);
}		


void eBoneSelect()
{
	//FreyjaControl::GetInstance()->SetObjectMode(FreyjaControl::tBone);
	//FreyjaControl::GetInstance()->SetActionMode(FreyjaControl::aSelect);
	freyja3d_print("Select bone...");
}


void eBoneNew()
{
	//FreyjaControl::GetInstance()->SetObjectMode(FreyjaControl::tBone);
	//FreyjaControl::GetInstance()->CreateObject();
	//freyja_print("Select new child bone placement directly...");
	//mEventMode = BONE_ADD_MODE;
	//FreyjaControl::GetInstance()->Dirty();
}


void eExtrude()
{
#if FIXME
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
#endif
}

void eMeshFlipNormals()
{
#if FIXME
	freyjaMeshNormalFlip(FreyjaControl::GetInstance()->GetSelectedMesh());
	freyja_print("Flipping normals for mesh[%i]", 
				 FreyjaControl::GetInstance()->GetSelectedMesh());
	FreyjaControl::GetInstance()->Dirty();
#endif
}


void eHelpDialog()
{
	freyja_set_dialog_visible("eHelpDialog");
}


void ePreferencesDialog()
{
	freyja_set_dialog_visible( "ePreferencesDialog" );
}


void eAboutDialog()
{
	freyja_set_dialog_visible("eAboutDialog");
}


void eAnimationNext()
{
	//FreyjaControl::GetInstance()->SetSelectedAnimation(FreyjaControl::GetInstance()->GetSelectedAnimation() + 1);
	freyja3d_print("Animation Track[%i].", 0);//			 FreyjaControl::GetInstance()->GetSelectedAnimation());
}

void eAnimationPrev()
{
#if 0
	if (FreyjaControl::GetInstance()->GetSelectedAnimation())
		FreyjaControl::GetInstance()->SetSelectedAnimation(FreyjaControl::GetInstance()->GetSelectedAnimation() - 1);
	else
		FreyjaControl::GetInstance()->SetSelectedAnimation(0);
	
	freyja_print("Animation Track[%i].", 
				 FreyjaControl::GetInstance()->GetSelectedAnimation());
#endif
}


void eLightPos(uint32 i, vec_t value)
{
	uint32 light = 0;//FreyjaControl::GetInstance()->GetSelectedLight();
	vec3_t pos;
	freyjaGetLightPosition4v(light, pos);
	pos[i] = value;
	freyjaLightPosition4v(light, pos);
	//FreyjaControl::GetInstance()->Dirty();	
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
	//FreyjaControl::GetInstance()->SetFaceEdgeCount(value);
	//freyja_print("Polygons creation using %i sides", 
	//			 FreyjaControl::GetInstance()->GetFaceEdgeCount());
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
#if FIXME
	index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
	Bone *b = Bone::GetBone( bone );
	if (text && b)
	{
		b->mMetaData = text;
	}
#endif
}


void eBoneMetadata()
{
#if FIXME
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
#endif
}


void eBoneKeyFrameMetadataText(char *text)
{
#if FIXME
	index_t bone = FreyjaControl::GetInstance()->GetSelectedBone();
	Bone *b = Bone::GetBone( bone );
	if (text && b)
	{
		BoneTrack &track = b->GetTrack(FreyjaControl::GetInstance()->GetSelectedAnimation());
		uint32 key = FreyjaControl::GetInstance()->GetSelectedKeyFrame();
		track.SetMetadata(key, text);
	}
#endif
}


void eBoneKeyFrameMetadata()
{
#if FIXME
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
#endif
}


#include <freyja/Metadata.h>
void eMetadataIterator(unsigned int id)
{
#if FIXME
	FreyjaControl::GetInstance()->SetSelectedMetadata( id );

	// Test, Do we want to assume the user always wants to edit from this event?
	Metadata* metadata = Metadata::GetObjectByUid( id );

	if ( metadata )
	{
		const char* dialog = "MetadataPropertyDialog";
		mgtk_set_query_dialog_string_default( dialog, "name", metadata->GetName() );
		mgtk_set_query_dialog_string_default( dialog, "type", metadata->GetType() );
		mgtk_set_query_dialog_string_default( dialog, "metadata", metadata->GetMetadata() );
		mgtk_set_query_dialog_string_default( dialog, "model", metadata->GetModel() );
		mgtk_set_query_dialog_string_default( dialog, "material", metadata->GetMaterial() );
		
		int update = mgtk_execute_query_dialog( dialog );
		
		if ( update )
		{
			const char* s = mgtk_get_query_dialog_string(dialog, "name");
			if (s) metadata->SetName( s );
			
			s = mgtk_get_query_dialog_string(dialog, "type");
			if (s) metadata->SetType( s );
			
			s = mgtk_get_query_dialog_string(dialog, "metadata");
			if (s) metadata->SetMetadata( s );
			
			s = mgtk_get_query_dialog_string(dialog, "model");
			if (s) metadata->SetModel( s );
			
			s = mgtk_get_query_dialog_string(dialog, "material");
			if (s) metadata->SetMaterial( s );
			
			FreyjaControl::GetInstance()->Dirty();
		}
	}

#endif
}


void eCameraIterator(unsigned int id)
{
	//FreyjaControl::GetInstance()->SetSelectedCamera( id );
	freyja3d_print( "camera%i selected.", id );
}


void eLightIterator(unsigned int id)
{
	//FreyjaControl::GetInstance()->SetSelectedLight( id );
	freyja3d_print( "light%i selected.", id );
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


void freyja3d_misc_attach_listeners()
{
	mgtk_attach_listener( "eOpenFile", &eOpenFile );

	// Test events
	mgtk_attach_listener( "eTestAssertMsgBox", &eTestAssertMsgBox);
	mgtk_attach_listener( "eTestTextView", &eTestTextView);
	mgtk_attach_listener1s( "eTestTextViewText", &eTestTextViewText);

	// Misc Iterators
	mgtk_attach_listener1u(  "eMetadataIterator", &eMetadataIterator);
	mgtk_attach_listener1u(  "eCameraIterator", &eCameraIterator);
	mgtk_attach_listener1u(  "eLightIterator", &eLightIterator);

	// Misc events
	mgtk_attach_listener( "eResetColorsToDefault", &eResetColorsToDefault);
	mgtk_attach_listener1s( "eBoneKeyFrameMetadataText", &eBoneKeyFrameMetadataText);
	mgtk_attach_listener( "eBoneKeyFrameMetaData", &eBoneKeyFrameMetadata);
	mgtk_attach_listener( "eBoneRefreshBindPose", &eBoneRefreshBindPose);
	mgtk_attach_listener1s( "eBoneMetadataText", &eBoneMetadataText);
	mgtk_attach_listener( "eBoneMetaData", &eBoneMetadata);
	mgtk_attach_listener( "eBoneNew", &eBoneNew);
	mgtk_attach_listener( "eBoneSelect", &eBoneSelect);

	mgtk_attach_listener( "eMoveObject", &eMoveObject);
	mgtk_attach_listener( "eRotateObject", &eRotateObject);
	mgtk_attach_listener( "eScaleObject", &eScaleObject);
	mgtk_attach_listener( "eSelect", &eSelect);
	mgtk_attach_listener( "eUnselect", &eUnselect);

	mgtk_attach_listener( "eGenerateRing", &eGenerateRing);
	mgtk_attach_listener( "eGenerateCircle", &eGenerateCircle);
	mgtk_attach_listener( "eGeneratePlane", &eGeneratePlane);
	mgtk_attach_listener( "eGenerateCube", &eGenerateCube);
	mgtk_attach_listener( "eGenerateSphere", &eGenerateSphere);
	mgtk_attach_listener( "eGenerateTube", &eGenerateTube);
	mgtk_attach_listener( "eGenerateCylinder", &eGenerateCylinder);
	mgtk_attach_listener( "eGenerateCone", &eGenerateCone);

	mgtk_attach_listener1f(  "eSnapWeldVertsDist", &eSnapWeldVertsDist);

	mgtk_attach_listener( "eSetSelectedFacesAlpha", &eSetSelectedFacesAlpha);
	mgtk_attach_listener( "eClearSelectedFacesAlpha", &eClearSelectedFacesAlpha);

	mgtk_attach_listener( "eAssignWeight", &eAssignWeight);
	mgtk_attach_listener( "eClearWeight", &eClearWeight);
	mgtk_attach_listener1f(  "eWeight", &eWeight);

	mgtk_attach_listener( "eMirrorFacesX", &eMirrorFacesX);
	mgtk_attach_listener( "eMirrorFacesY", &eMirrorFacesY);
	mgtk_attach_listener( "eMirrorFacesZ", &eMirrorFacesZ);

	mgtk_attach_listener( "eMeshTesselate", &eMeshTesselate);
	mgtk_attach_listener( "eMeshTexcoordSpherical", &eMeshTexcoordSpherical);
	mgtk_attach_listener( "eMeshTexcoordCylindrical", &eMeshTexcoordCylindrical);
	mgtk_attach_listener( "eMeshGenerateNormals", &eMeshGenerateNormals);
	mgtk_attach_listener( "eMeshTexcoordPlaneProj", &eMeshTexcoordPlaneProj);
	mgtk_attach_listener( "eMirrorMeshX", &eMirrorMeshX);
	mgtk_attach_listener( "eMirrorMeshY", &eMirrorMeshY);
	mgtk_attach_listener( "eMirrorMeshZ", &eMirrorMeshZ);

	mgtk_attach_listener( "eCleanupVertices", eCleanupVertices);
	mgtk_attach_listener( "eSnapWeldVerts", &eSnapWeldVerts);

	mgtk_attach_listener( "eGroupClear", &eGroupClear);
	mgtk_attach_listener( "eGroupAssign", &eGroupAssign);

	mgtk_attach_listener( "eSmoothingGroupsDialog", eSmoothingGroupsDialog);
	mgtk_attach_listener2u( "eSmooth", &eSmooth);

	mgtk_attach_listener( "eSelectedFacesFlipNormals", &eSelectedFacesFlipNormals);
	mgtk_attach_listener( "eSelectedFacesGenerateNormals", &eSelectedFacesGenerateNormals);
	mgtk_attach_listener( "eSelectedFacesDelete", &eSelectedFacesDelete);
	mgtk_attach_listener( "eMeshUnselectFaces", &eMeshUnselectFaces);
	mgtk_attach_listener( "eMeshUnselectVertices", &eMeshUnselectVertices);

	mgtk_attach_listener1u(  "eSetSelectedViewport", &eSetSelectedViewport);
	mgtk_attach_listener1u(  "ePolygonSize", &ePolygonSize);

	// FIXME, These light events don't appear to trigger
	mgtk_attach_listener1f(  "eLightPosX", &eLightPosX);
	mgtk_attach_listener1f(  "eLightPosY", &eLightPosY);
	mgtk_attach_listener1f(  "eLightPosZ", &eLightPosZ);

	mgtk_attach_listener( "eAnimationNext", &eAnimationNext);
	mgtk_attach_listener( "eAnimationPrev", &eAnimationPrev);

	mgtk_attach_listener( "eMeshFlipNormals", &eMeshFlipNormals);
	mgtk_attach_listener( "eHelpDialog", &eHelpDialog);
	mgtk_attach_listener( "ePreferencesDialog", &ePreferencesDialog);
	mgtk_attach_listener( "eAboutDialog", &eAboutDialog);

	mgtk_attach_listener( "eExtrude", &eExtrude);

	mgtk_attach_listener(  "eExportKeyAsMesh", &eExportKeyAsMesh );
	mgtk_attach_listener(  "eConvertSkelToMeshAnim", &eConvertSkelToMeshAnim );

	/* Not implemented, removed, or 'dummy' events. */
	mgtk_attach_listener_nop( "eBezierPolygonPatch" );
	mgtk_attach_listener_nop( "eShaderSlotLoad" );
	mgtk_attach_listener_nop( "eOpenFileTextureB" );
	mgtk_attach_listener_nop( "eCollapseFace" );
	mgtk_attach_listener_nop( "eSetMaterialTextureB" );
	mgtk_attach_listener_nop( "eEnableMaterialFragment" );
	mgtk_attach_listener_nop( "eUVPickRadius" );
	mgtk_attach_listener_nop( "eVertexPickRadius" );
	mgtk_attach_listener_nop( "eAnimationStop" );
	mgtk_attach_listener_nop( "eAnimationPlay" );
	mgtk_attach_listener_nop( "eBoneLinkChild" );
	mgtk_attach_listener_nop( "eBoneUnLinkChild" );
	mgtk_attach_listener_nop( "eAppendFile" );
	mgtk_attach_listener_nop( "eBoneDelete" );

	mgtk_attach_listener_nop( "eVertexNew" );
	mgtk_attach_listener_nop( "eVertexDelete" );

	mgtk_attach_listener_nop( "ePolygonNew" );
	mgtk_attach_listener_nop( "ePolygonDelete" );
	mgtk_attach_listener_nop( "ePolygonSelect" );

	mgtk_attach_listener_nop( "eRenderShadow" );
	mgtk_attach_listener_nop( "ePolyMapTexturePolygon" );
	mgtk_attach_listener_nop( "eUVMapCreate" );
	mgtk_attach_listener_nop( "eUVMapDelete" );
	mgtk_attach_listener_nop( "eMirrorUV_X" );
	mgtk_attach_listener_nop( "eMirrorUV_Y" );
	mgtk_attach_listener_nop( "eTranslateUV" );
	mgtk_attach_listener_nop( "eRotateUV" );
	mgtk_attach_listener_nop( "eScaleUV" );

	/* Empty 'dummy' menu events. */
	mgtk_attach_listener_nop( "eImportFile" );
	mgtk_attach_listener_nop( "eExportFile" );

	/* Unimplemented events from 9.5 rollover. */
	mgtk_attach_listener_nop( "eScale" );
	mgtk_attach_listener_nop( "eMove" );
	mgtk_attach_listener_nop( "eRotate" );


#if 0
	mMaterial.AttachMethodListeners();

	//Float events
	EvMoveXId = CreateListener1f("eMove_X", &FreyjaControl::EvFloatNop);
	EvMoveYId = CreateListener1f("eMove_Y", &FreyjaControl::EvFloatNop);
	EvMoveZId = CreateListener1f("eMove_Z", &FreyjaControl::EvFloatNop);
	EvRotateXId = CreateListener1f("eRotate_X", &FreyjaControl::EvFloatNop);
	EvRotateYId = CreateListener1f("eRotate_Y", &FreyjaControl::EvFloatNop);
	EvRotateZId = CreateListener1f("eRotate_Z", &FreyjaControl::EvFloatNop);
	EvScaleXId = CreateListener1f("eScale_X", &FreyjaControl::EvScaleX);
	EvScaleYId = CreateListener1f("eScale_Y", &FreyjaControl::EvScaleY);
	EvScaleZId = CreateListener1f("eScale_Z", &FreyjaControl::EvScaleZ);
	CreateListener1f("eZoom", &FreyjaControl::SetZoom);

	// Mode events
	EvModeAutoKeyframeId =
	CreateListener1u("eModeAnim", &FreyjaControl::EvModeAutoKeyframe);
	CreateListener("eModeUV", &FreyjaControl::EvModeUV);
	CreateListener("eModeModel", &FreyjaControl::EvModeModel);
	CreateListener("eModeMaterial", &FreyjaControl::EvModeMaterial);

	// Iterator/Integer events
	CreateListener1u("ePolygonIterator", &FreyjaControl::EvPolygonIterator);
	CreateListener1u("eMeshIterator", &FreyjaControl::EvMeshIterator);
	EvBoneIteratorId = 
	CreateListener1u("eBoneIterator", &FreyjaControl::EvBoneIterator);
	CreateListener1u("eAnimationSlider", &FreyjaControl::EvAnimationSlider);

	eRotateObjectId = 
	CreateListener1u("eRotateObject", &FreyjaControl::EvRotateObject);

	eScaleObjectId = 
	CreateListener1u("eScaleObject", &FreyjaControl::EvScaleObject);

	eMoveObjectId = 
	CreateListener1u("eMoveObject", &FreyjaControl::EvMoveObject);

	eUnselectObjectId = 
	CreateListener1u("eUnselect", &FreyjaControl::EvUnselectObject);

	eSelectObjectId = 
	CreateListener1u("eSelect", &FreyjaControl::EvSelectObject);

	eInfoObjectId = 
	CreateListener1u("eInfoObject", &FreyjaControl::EvInfoObject);

	ePaintObjectId = 
	CreateListener1u("ePaintObject", &FreyjaControl::EvPaintObject);

	eBoxSelectObjectId = 
	CreateListener1u("eSelectionByBox", &FreyjaControl::EvBoxSelectObject);

	eAxisJointId = 
	CreateListener1u("eAxisJoint", &FreyjaControl::EvAxisJoint);

	eSphereJointId = 
	CreateListener1u("eSphereJoint", &FreyjaControl::EvSphereJoint);

	ePointJointId = 
	CreateListener1u("ePointJoint", &FreyjaControl::EvPointJoint);

	// Text events
	CreateListener1s("eSkeletonName", &FreyjaControl::EvSkeletonName);
	EvSetBoneNameId = 
	CreateListener1s("eSetBoneName", &FreyjaControl::EvSetBoneName);
	CreateListener1s("eOpenModel", &FreyjaControl::EvOpenModel);

	// Misc
	//CreateListener2s("eSaveModel", &FreyjaControl::EvSaveModel);
	CreateListener("eSaveModel", &FreyjaControl::SaveAsFileModel);

	CreateListener("ePolygonSplit", &FreyjaControl::EvPolygonSplit);
	CreateListener("eSetMeshTexture", &FreyjaControl::EvSetMeshTexture);
	CreateListener("eSetFacesMaterial", &FreyjaControl::EvSetFacesMaterial);
	CreateListener("eSetPolygonTexture", &FreyjaControl::EvSetPolygonTexture);

	CreateListener("eInfo", &FreyjaControl::PrintInfo);
	CreateListener("eFullscreen", &FreyjaControl::Fullscreen);

	CreateListener("eVertexCombine", &FreyjaControl::VertexCombine);
	CreateListener("eTexcoordCombine", &FreyjaControl::TexcoordCombine);
	CreateListener("eSetKeyFrame", &FreyjaControl::SetKeyFrame);

	CreateListener("eCloseFile", &FreyjaControl::CloseFile);
	CreateListener("eNewFile", &FreyjaControl::NewFile);
	CreateListener("eSaveFile", &FreyjaControl::SaveFile);
	CreateListener("eOpenFile", &FreyjaControl::OpenFile);
	CreateListener("eSaveFileModel", &FreyjaControl::SaveFileModel);
	CreateListener("eOpenFileModel", &FreyjaControl::OpenFileModel);
	CreateListener("eRevertFile", &FreyjaControl::RevertFile);

	CreateListener("eUndo", &FreyjaControl::Undo);
	CreateListener("eRedo", &FreyjaControl::Redo);

	CreateListener("eShutdown", &FreyjaControl::Shutdown);

	CreateListener("eDelete", &FreyjaControl::DeleteSelectedObject);
	CreateListener("eCreate", &FreyjaControl::CreateObject);
	CreateListener("eDupeObject", &FreyjaControl::DuplicateSelectedObject);
	CreateListener("eMergeObject", &FreyjaControl::MergeSelectedObjects);
	CreateListener("eSplitObject", &FreyjaControl::SplitSelectedObject);
	CreateListener("ePaste", &FreyjaControl::PasteSelectedObject);
	CreateListener("eCopy", &FreyjaControl::CopySelectedObject);
	CreateListener("eCut", &FreyjaControl::Cut);

	CreateListener("eMeshNew", &FreyjaControl::EvMeshNew);
	CreateListener("eMeshDelete", &FreyjaControl::EvMeshDelete);
	CreateListener("eMeshSelect", &FreyjaControl::EvMeshSelect);


	CreateListener("eSelectVerticesByFaces", &FreyjaControl::EvSelectVerticesByFaces);

	CreateListener("eViewportBack", &FreyjaControl::EvViewportBack);
	CreateListener("eViewportBottom", &FreyjaControl::EvViewportBottom);
	CreateListener("eViewportRight", &FreyjaControl::EvViewportRight);
	CreateListener("eViewportFront", &FreyjaControl::EvViewportFront);
	CreateListener("eViewportTop", &FreyjaControl::EvViewportTop);
	CreateListener("eViewportLeft", &FreyjaControl::EvViewportLeft);
	CreateListener("eViewportOrbit", &FreyjaControl::EvViewportOrbit);
	CreateListener("eViewportUV", &FreyjaControl::EvViewportUV);
	CreateListener("eViewportCurve", &FreyjaControl::EvViewportCurve);
	CreateListener("eViewportCamera", &FreyjaControl::EvViewportCamera);
	CreateListener("eViewportMaterial", &FreyjaControl::EvViewportMaterial);

	CreateListener("eTransformScene", &FreyjaControl::EvTransformScene);
	CreateListener("eTransformVertices", &FreyjaControl::EvTransformVertices);
	CreateListener("eTransformVertex", &FreyjaControl::EvTransformVertex);
	CreateListener("eTransformMeshes", &FreyjaControl::EvTransformMeshes);
	CreateListener("eTransformMesh", &FreyjaControl::EvTransformMesh);
	CreateListener("eTransformMetadata", &FreyjaControl::EvTransformMetadata);
	CreateListener("eTransformFaces", &FreyjaControl::EvTransformFaces);
	CreateListener("eTransformFace", &FreyjaControl::EvTransformFace);
	CreateListener("eTransformModel", &FreyjaControl::EvTransformModel);
	CreateListener("eTransformSkeleton", &FreyjaControl::EvTransformSkeleton);
	CreateListener("eTransformBone", &FreyjaControl::EvTransformBone);
	CreateListener("eTransformLight", &FreyjaControl::EvTransformLight);
	CreateListener("eTransformCamera", &FreyjaControl::EvTransformCamera);


	CreateListener("eMetaToXML", &FreyjaControl::EvSerializeMetadata);
	CreateListener("eXMLToMeta", &FreyjaControl::EvUnserializeMetadata);

	CreateListener("eMeshToXML", &FreyjaControl::EvSerializeMesh);
	CreateListener("eXMLToMesh", &FreyjaControl::EvUnserializeMesh);

	CreateListener("eBonesToXML", &FreyjaControl::EvSerializeBones);
	CreateListener("eXMLToBones", &FreyjaControl::EvUnserializeBones);

	CreateListener("eMeshRepack", &FreyjaControl::EvMeshRepack);

	CreateListener1u("eRecentFiles", &FreyjaControl::EvRecentFiles);
	CreateListener1u("eRecentMeshXML", &FreyjaControl::EvRecentMeshXML);
	CreateListener1u("eRecentMetadataXML", &FreyjaControl::EvRecentMetadataXML);
	CreateListener1u("eRecentSkeletonXML", &FreyjaControl::EvRecentSkeletonXML);
	CreateListener1u("eRecentKeyframe", &FreyjaControl::EvRecentKeyframe);
	CreateListener1u("eRecentLua", &FreyjaControl::EvRecentLua);
	CreateListener1u("eRecentPython", &FreyjaControl::EvRecentPython);


	CreateListener("ePaintWeight", &FreyjaControl::EvPaintWeight);
	CreateListener("ePaintUnweight", &FreyjaControl::EvPaintUnweight);
	CreateListener("ePaintSelect", &FreyjaControl::EvPaintSelect);
	CreateListener("ePaintUnselect", &FreyjaControl::EvPaintUnselect);
	CreateListener("ePaintMaterial", &FreyjaControl::EvPaintMaterial);
	CreateListener("ePaintHeight", &FreyjaControl::EvPaintHeight);
	CreateListener("ePaintDmap", &FreyjaControl::EvPaintDmap);

	CreateListener("eLoadLuaScript", &FreyjaControl::EvLoadLuaScript);
	CreateListener("eLoadPythonScript", &FreyjaControl::EvLoadPythonScript);

	CreateListener("eLaunchBugTracker", &FreyjaControl::EvLaunchBugs);
	CreateListener("eLaunchOnlineHelp", &FreyjaControl::EvLaunchFourms);
	CreateListener("eLaunchUpdate", &FreyjaControl::EvLaunchUpdate);

	CreateListener("eMeshSubDivLoop", &FreyjaControl::EvMeshSubDiv);

	CreateListener("eSelectAll", &FreyjaControl::EvSelectAll);
	CreateListener("eUnSelectAll", &FreyjaControl::EvUnselectAll);

	CreateListener("eMeshKeyframeCopy", &FreyjaControl::EvMeshFromKeyframe);
#endif
}


///////////////////////////////////////////////////////////////////////

