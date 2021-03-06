/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongoose@icculus.org
 * Object  : Nod
 * License : GPL
 * Comments: NOD module for freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2001.07.04:
 * Mongoose - Created, based on viewer code from anonymous ( file structure )
 ==========================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <mstl/SystemIO.h>

#include "Nod.h"


using namespace mstl;

Nod::Nod()
{
	materials = 0x0;
	bones = 0x0;
	meshes = 0x0;
	vertices = 0x0;
	mesh_groups = 0x0;
	faces = 0x0;
}


Nod::~Nod()
{
	if (materials)
		delete [] materials;

	if (bones)
		delete [] bones;

	if (meshes)
		delete [] meshes;

	if (vertices)
		delete [] vertices;

	if (mesh_groups)
		delete [] mesh_groups;

	if (faces)
		delete [] faces;
}


void Nod::GetEulerAngles2(const float m[3][4], float angles[3])
{
	float cy;

	// order of angles x,y,z

	cy = (float) sqrt(m[0][0] * m[0][0] + m[0][1] * m[0][1]); // AX^2 + BX^2

	if (cy > 16 * 0.001)
	{
		angles[0] = (float) atan2(m[1][2], m[2][2]); // CY, CZ
		angles[1] = (float) atan2(-m[0][2], cy); // -CX, cy
		angles[2] = (float) atan2(m[0][1], m[0][0]); // BX, AX
	}
	else
	{
		angles[0] = (float) atan2(-m[2][1], m[1][1]); //-BZ, BY
		angles[1] = (float) atan2(-m[0][2], cy);	// -CX, cy
		angles[2] = 0;
	}
}

void Nod::print()
{
	long i, j, k;


	printf("Version %i\n", header1.Version);
	printf("NumMaterials %i\n", header1.NumMaterials);

	for (i = 0; i < header1.NumMaterials; ++i)
	{
		printf("materials[%li].MaterialName = '%s'\n",
			   i, materials[i].MaterialName);
	}

	printf("header2.NumBones = %i\n", header2.NumBones);
	printf("header2.NumMeshs = %i\n", header2.NumMeshs);
	printf("header2.NumVertices = %i\n", header2.NumVertices);
	printf("header2.NumFaces = %i\n", header2.NumFaces);
	printf("header2.NumGroups = %i\n", header2.NumGroups);
	printf("header2.ModelFlags = {\n");

	if (!header2.ModelFlags)
		printf("NONE ");
			  
	if (header2.ModelFlags & NOD_MF_HASLOD)
	{
		printf("NOD_MF_HASLOD ");
	}
			  
	if (header2.ModelFlags & NOD_MF_INLINE)
	{
		printf("NOD_MF_INLINE ");
	}
			  
	if (header2.ModelFlags & NOD_MF_STATIC)
	{
		printf("NOD_MF_STATIC ");
	}
			  
	if (header2.ModelFlags & NOD_MF_RESERVED1)
	{
		printf("NOD_MF_RESERVED1 ");
	}
			  
	if (header2.ModelFlags & NOD_MF_RESERVED1)
	{
		printf("NOD_MF_RESERVED1 ");
	}

	printf("}\n");

	printf("header2.Bounds[] = {");

	for (i = 0; i < 6; ++i)
	{
		printf("%.4f ", header2.Bounds[i]);
	}

	printf("}\n");

	for (i = 0; i < header2.NumBones; ++i)
	{
		printf("bone[%li].translate = %.4f %.4f %.4f ", i, 
			   bones[i].RestTranslate[0],
			   bones[i].RestTranslate[1],
			   bones[i].RestTranslate[2]);

		printf("bone[%li].inverse = {\n", i);
		for (j = 0; j < 3; ++j)
		{
			for (k = 0; k < 4; ++k)
			{
				printf("%.4f ", bones[i].RestMatrixInverse[j][k]);
			}
			printf("\n");
		}
		printf("\n");

		printf("bone[%li].sibling = %i\n", i, bones[i].SiblingID);
		printf("bone[%li].child = %i\n", i, bones[i].ChildID);
		printf("bone[%li].parent = %i\n", i, bones[i].ParentID);
	}


	for (i = 0; i < header2.NumMeshs; ++i)
	{
		printf("meshes[%li].MeshName = '%s'\n",
			   i, meshes[i].MeshName);
		
	}

	for (i = 0; i < header2.NumVertices; ++i)
	{
		printf("vertices[%li].Pos = %f %f %f\n", i,
			   vertices[i].Pos[0], vertices[i].Pos[1], vertices[i].Pos[2]);
		printf("vertices[%li].Norm = %f %f %f\n", i,
			   vertices[i].Norm[0], vertices[i].Norm[1], vertices[i].Norm[2]);
		printf("vertices[%li].UV = %f %f\n", i,
			   vertices[i].UV[0], vertices[i].UV[1]);
		printf("vertices[%li].Weight = %f\n", i,
			   vertices[i].Weight);
		printf("vertices[%li].BoneNum = %i\n", i,
			   vertices[i].BoneNum);
	}

	for (i = 0; i < header2.NumFaces; ++i)
	{
		printf("face[%li] = { %i %i %i}\n", i,
			   faces[i].indices[0], faces[i].indices[1], faces[i].indices[2]);
	}

	for (i = 0; i < header2.NumGroups; ++i)
	{
		printf("mesh_groups[%li].MaterialID = %i", i, mesh_groups[i].MaterialID);
		printf("mesh_groups[%li].RESERVED = ?", i);
		printf("mesh_groups[%li].NumFace = %is", i, mesh_groups[i].NumFaces);
		printf("mesh_groups[%li].NumVertices = %i", i, mesh_groups[i].NumVertices);
		printf("mesh_groups[%li].MinVertices = %i", i, mesh_groups[i].MinVertices);
		printf("mesh_groups[%li].dummy = %i", i, mesh_groups[i].dummy);
		printf("mesh_groups[%li].GroupFlags = 0x%x", i, mesh_groups[i].GroupFlags);
		printf("mesh_groups[%li].BoneNum = %i", i, mesh_groups[i].BoneNum);
		printf("mesh_groups[%li].MeshNum = %i", i, mesh_groups[i].MeshNum);
	}
}


bool Nod::load(const char *filename)
{
	SystemIO::FileReader r;
	long i, j, k;


	if (!r.Open(filename))
		return false;

	header1.Version = r.ReadLong();
	header1.NumMaterials = r.ReadLong();


	materials = new nod_material_t[header1.NumMaterials];

	for (i = 0; i < header1.NumMaterials; ++i)
	{
		r.ReadString(32, materials[i].MaterialName);
	}

	header2.NumBones = r.ReadInt16();
	header2.NumMeshs = r.ReadInt16();
	header2.NumVertices = r.ReadLong();
	header2.NumFaces = r.ReadLong();
	header2.NumGroups = r.ReadInt16();
	header2.ModelFlags = r.ReadLong();

	for (i = 0; i < 6; ++i)
	{
		header2.Bounds[i] = r.ReadFloat32();
	}


	bones = new nod_bone_t[header2.NumBones];

	for (i = 0; i < header2.NumBones; ++i)
	{
		bones[i].RestTranslate[0] = r.ReadFloat32();
		bones[i].RestTranslate[1] = r.ReadFloat32();
		bones[i].RestTranslate[2] = r.ReadFloat32();

		for (j = 0; j < 3; ++j)
		{
			for (k = 0; k < 4; ++k)
			{
				bones[i].RestMatrixInverse[j][k] = r.ReadFloat32();
			}
		}

		bones[i].SiblingID = r.ReadInt16();
		bones[i].ChildID = r.ReadInt16();
		bones[i].ParentID = r.ReadInt16();
	}


	meshes = new nod_mesh_t[header2.NumMeshs];

	for (i = 0; i < header2.NumMeshs; ++i)
	{
		r.ReadString(32, meshes[i].MeshName);
	}

	vertices = new nod_vertex_t[header2.NumVertices];

	for (i = 0; i < header2.NumVertices; ++i)
	{
		vertices[i].Pos[0] = r.ReadFloat32();
		vertices[i].Pos[1] = r.ReadFloat32();
		vertices[i].Pos[2] = r.ReadFloat32();

		vertices[i].Norm[0] = r.ReadFloat32();
		vertices[i].Norm[1] = r.ReadFloat32();
		vertices[i].Norm[2] = r.ReadFloat32();

		vertices[i].UV[0] = r.ReadFloat32();
		vertices[i].UV[1] = r.ReadFloat32();

		vertices[i].Weight = r.ReadFloat32();

		vertices[i].BoneNum = r.ReadLong();
	}


	if (header2.ModelFlags & NOD_MF_HASLOD)
	{
		for (i = 0; i < header2.NumVertices; ++i)
		{
			r.ReadInt16();  // ?
		}
	}

	faces = new nod_face_t[header2.NumFaces];

	for (i = 0; i < header2.NumFaces; ++i)
	{
		faces[i].indices[0] = r.ReadInt16();
		faces[i].indices[1] = r.ReadInt16();
		faces[i].indices[2] = r.ReadInt16();
	}

	mesh_groups = new nod_meshgroup_t[header2.NumGroups];
 
	for (i = 0; i < header2.NumGroups; ++i)
	{
		mesh_groups[i].MaterialID = r.ReadLong();
		r.ReadBuffer(12, mesh_groups[i].RESERVED);
		mesh_groups[i].NumFaces = r.ReadInt16();
		mesh_groups[i].NumVertices = r.ReadInt16();
		mesh_groups[i].MinVertices = r.ReadInt16();
		mesh_groups[i].dummy = r.ReadInt16();
		mesh_groups[i].GroupFlags = r.ReadInt16();
		mesh_groups[i].BoneNum = r.ReadInt8U();
		mesh_groups[i].MeshNum = r.ReadInt8U();
	}

	r.Close();
	
	return true;
}


bool Nod::save(const char *filename)
{
#ifdef FIXME
	SystemIO::FileWriter w;
	long i, j, k;


	if (!r.openFile(filename))
		return false;

	header1.Version = r.ReadLong();
	header1.NumMaterials = r.ReadLong();


	materials = new nod_material_t[header1.NumMaterials];

	for (i = 0; i < header1.NumMaterials; ++i)
	{
		r.ReadCharString(32, materials[i].MaterialName);
	}

	header2.NumBones = r.ReadInt16();
	header2.NumMeshs = r.ReadInt16();
	header2.NumVertices = r.ReadLong();
	header2.NumFaces = r.ReadLong();
	header2.NumGroups = r.ReadInt16();
	header2.ModelFlags = r.ReadLong();

	for (i = 0; i < 6; ++i)
	{
		header2.Bounds[i] = r.ReadFloat32();
	}


	bones = new nod_bone_t[header2.NumBones];

	for (i = 0; i < header2.NumBones; ++i)
	{
		bones[i].RestTranslate[0] = r.ReadFloat32();
		bones[i].RestTranslate[1] = r.ReadFloat32();
		bones[i].RestTranslate[2] = r.ReadFloat32();

		for (j = 0; j < 3; ++j)
		{
			for (k = 0; k < 4; ++k)
			{
				bones[i].RestMatrixInverse[j][k] = r.ReadFloat32();
			}
		}

		bones[i].SiblingID = r.ReadInt16();
		bones[i].ChildID = r.ReadInt16();
		bones[i].ParentID = r.ReadInt16();
	}


	meshes = new nod_mesh_t[header2.NumMeshs];

	for (i = 0; i < header2.NumMeshs; ++i)
	{
		r.ReadCharString(32, meshes[i].MeshName);
	}

	vertices = new nod_vertex_t[header2.NumVertices];

	for (i = 0; i < header2.NumVertices; ++i)
	{
		vertices[i].Pos[0] = r.ReadFloat32();
		vertices[i].Pos[1] = r.ReadFloat32();
		vertices[i].Pos[2] = r.ReadFloat32();

		vertices[i].Norm[0] = r.ReadFloat32();
		vertices[i].Norm[1] = r.ReadFloat32();
		vertices[i].Norm[2] = r.ReadFloat32();

		vertices[i].UV[0] = r.ReadFloat32();
		vertices[i].UV[1] = r.ReadFloat32();

		vertices[i].Weight = r.ReadFloat32();

		vertices[i].BoneNum = r.ReadLong();
	}


	if (header2.ModelFlags & NOD_MF_HASLOD)
	{
		for (i = 0; i < header2.NumVertices; ++i)
		{
			r.ReadInt16();  // ?
		}
	}

	faces = new nod_face_t[header2.NumFaces];

	for (i = 0; i < header2.NumFaces; ++i)
	{
		faces[i].indices[0] = r.ReadInt16();
		faces[i].indices[1] = r.ReadInt16();
		faces[i].indices[2] = r.ReadInt16();
	}

	mesh_groups = new nod_meshgroup_t[header2.NumGroups];
 
	for (i = 0; i < header2.NumGroups; ++i)
	{
		mesh_groups[i].MaterialID = r.ReadLong();
		r.ReadBufferUnsignedChar(12, mesh_groups[i].RESERVED);
		mesh_groups[i].NumFaces = r.ReadInt16();
		mesh_groups[i].NumVertices = r.ReadInt16();
		mesh_groups[i].MinVertices = r.ReadInt16();
		mesh_groups[i].dummy = r.ReadInt16();
		mesh_groups[i].GroupFlags = r.ReadInt16();
		mesh_groups[i].BoneNum = r.ReadInt8U();
		mesh_groups[i].MeshNum = r.ReadInt8U();
	}

	r.closeFile();
	
	return true;
#else
	return false;
#endif
}


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS

#include <stdio.h>
#include <string.h>
#include <freyja/freyja.h>
#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/Vector.h>

using namespace mstl;
#include <hel/Mat44.h>
#include "Nod.h"

using namespace hel;

extern "C" {

	int freyja_model__nod_check(char *filename);
	int freyja_model__nod_import(char *filename);
	int freyja_model__nod_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginName1s("nod");
	freyjaPluginDescription1s("Vampire Model (*.nod)");
	freyjaPluginAddExtention1s("*.nod");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}

int import_model(char *filename)
{
	if (!freyja_model__nod_check(filename))
		return freyja_model__nod_import(filename);

	return -1;
}

#define NO_BONES  // Try to transform vertices for boneless import
#define TRANS     // Translator mapping enabled ( to load full models )

int freyja_model__nod_check(char *filename)
{
  FILE *f;
  int id;


  f = fopen(filename, "rb");
  
  if (!f)
  {
    perror("mod_check> fopen failed");
    return -1;
  }

  fread(&id, 4, 1, f);      
  fclose(f);
  
  if (id == 7)
    return 0;

  return -2;
}


int freyja_model__nod_import(char *filename)
{
	const vec_t scale = 0.25;
	Vector<long> vertices;
	Nod nod;
	long num_verts = 0, num_faces = 0;
	int i, j, b;
	char name[64];
	Mat44 matrix;
	vec3_t pos, rot;
	index_t skeleton, bone;


	if (!nod.load(filename))
		return -1;

	//unsigned int matMap[nod.header1.NumMaterials];

	index_t model = freyjaModelCreate();
				  
	for (i = 0; i < nod.header1.NumMaterials; i++)
	{
		index_t materialIndex = freyjaMaterialCreate();
		freyjaMaterialName(materialIndex, nod.materials[i].MaterialName);

		// Texture ID will be overwritten if exists and loads
		freyjaMaterialTexture(materialIndex, i);
		freyjaMaterialTextureName(materialIndex, nod.materials[i].MaterialName);
	}

	skeleton = freyjaSkeletonCreate();
	freyjaModelAddSkeleton(model, skeleton);

	for (b = 0; b < nod.header2.NumBones; ++b)
	{
		snprintf(name, 64, "bone[%i]", b);
		nod.GetEulerAngles2(nod.bones[b].RestMatrixInverse, rot);

		rot[0] = helDegToRad(rot[0]);
		rot[1] = helDegToRad(rot[2]);
		rot[2] = helDegToRad(rot[1]);

		bone = freyjaBoneCreate(skeleton); // transb[]
		freyjaSkeletonAddBone(skeleton, bone);
		freyjaBoneFlags(bone, 0x0);
		freyjaBoneParent(bone, nod.bones[b].ParentID);
		freyjaBoneName(bone, name);
		freyjaBoneTranslate3f(bone, 
							  nod.bones[b].RestTranslate[0]*scale,
							  nod.bones[b].RestTranslate[2]*scale,
							  nod.bones[b].RestTranslate[1]*scale);
		freyjaBoneRotateEuler3fv(bone, rot);

#if 0		
		if (b == 0)
		{
			freyjaBoneTranslate3f(bone,
								  nod.bones[b].RestTranslate[0]*scale,
								  nod.bones[b].RestTranslate[2]*scale,
								  nod.bones[b].RestTranslate[1]*scale);
			freyjaBoneRotateEuler3f(bone, 
			                       rot[0] - HEL_DEG_TO_RAD(90), rot[2] + HEL_DEG_TO_RAD(180), rot[1]);
		}
#endif

		freyjaPrintMessage("bone[%i].rotate = %f %f %f", b, 
						   rot[0], rot[1], rot[2]);
		freyjaPrintMessage("bone[%i].translate = %f %f %f", b, 
						   nod.bones[b].RestTranslate[0],
						   nod.bones[b].RestTranslate[2],
						   nod.bones[b].RestTranslate[1]);
		freyjaPrintMessage("bone[%i].parent = %i", b, nod.bones[b].ParentID);
		freyjaPrintMessage("bone[%i].child = %i", b, nod.bones[b].ChildID);
		freyjaPrintMessage("bone[%i].sibling = %i\n", b, nod.bones[b].SiblingID);

		for (i = 0; i < nod.header2.NumBones; ++i)
		{
			if (nod.bones[i].ParentID == b)
				freyjaBoneAddChild(bone, i);
		}
	}

	// Create mesh
	index_t mesh = freyjaMeshCreate();
	freyjaModelAddMesh(model, mesh);

	for (i = 0; i < nod.header2.NumGroups; ++i)
	{  
		for (j = 0; j < nod.mesh_groups[i].NumVertices; j++)
		{
			bone = nod.vertices[num_verts + j].BoneNum;

			// Store vertices and texels in group
#if 0
			pos[0] = nod.vertices[num_verts + j].Pos[0];
			pos[1] = nod.vertices[num_verts + j].Pos[1];
			pos[2] = nod.vertices[num_verts + j].Pos[2];

			nod.GetEulerAngles2(nod.bones[b].RestMatrixInverse, rot);
			matrix.setIdentity();
			matrix.translate(nod.bones[b].RestTranslate[0],
							 nod.bones[b].RestTranslate[1],
							 nod.bones[b].RestTranslate[2]);
			matrix.rotate(rot[0], rot[1], rot[2]);
			//matrix.multiply3v(pos, pos);

			index_t vertex = freyjaVertexCreate3f(pos[0]*scale, pos[2]*scale, pos[1]*scale);	
			freyjaVertexNormal3fv(vertex, nod.vertices[num_verts + j].Norm);
			freyjaVertexTexcoord2f(vertex,
								   nod.vertices[num_verts + j].UV[0],
								   1.0 - nod.vertices[num_verts + j].UV[1]);
#else
			Vec3 p(nod.vertices[num_verts + j].Pos[0],
				   nod.vertices[num_verts + j].Pos[2],
				   nod.vertices[num_verts + j].Pos[1]);
			p *= scale;
			index_t vertex = freyjaMeshVertexCreate3fv(mesh, p.mVec);

			float w = nod.vertices[num_verts + j].Weight;
			freyjaMeshVertexWeight(mesh, vertex, bone, w);

			if (w < 1.0f)
			{
				index_t parent = nod.bones[bone].ParentID;
				freyjaMeshVertexWeight(mesh, vertex, parent, 1.0f-w);
			}

			freyjaMeshVertexNormal3fv(mesh, vertex, nod.vertices[num_verts + j].Norm);
			p = Vec3(nod.vertices[num_verts + j].UV[0],
					1.0 - nod.vertices[num_verts + j].UV[1], 0.0f);
			freyjaMeshVertexTexCoord3fv(mesh, vertex, p.mVec);
#endif

			vertices.push_back(vertex);
		}

		for (j = 0; j < nod.mesh_groups[i].NumFaces; j++)
		{
			// Start a new polygon
			index_t face = freyjaMeshPolygonCreate(mesh);
			freyjaMeshPolygonMaterial(mesh, face, i);
			freyjaMeshPolygonGroup1u(mesh, face, i+1);

			for (uint32 k = 0; k < 3; ++k)
			{
				freyjaMeshPolygonAddVertex1i(mesh, face, vertices[nod.faces[num_faces+j].indices[k]]);
			}
		}

		vertices.clear();

		num_verts += nod.mesh_groups[i].NumVertices;
		num_faces += nod.mesh_groups[i].NumFaces;
	}

	return 0;
}


int freyja_model__nod_export(char *filename)
{
	freyjaPrintMessage("Nod Export: Not implemented, %s:%i\n", 
	                   __FILE__, __LINE__);
	return -1;
}

#endif


#ifdef UNIT_TEST_NOD
int main(int argc, char *argv[])
{
	int i, j, num_verts, num_faces;
	Nod nod;
	nod_header1_t *header1;
	nod_material_t *materials;
	nod_header2_t *header2;
	nod_bone_t *bones;
	nod_mesh_t *meshes;
	nod_vertex_t *vertices;
	nod_meshgroup_t *mesh_groups;
	nod_face_t *faces;
	

	printf("[NOD class test]\n");
	
	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
		  if (nod.load(argv[2]))
		  {
			  printf("main: Nod::Load(%s) reports success.\n", argv[2]);
			  
			  header1 = &nod.header1;
			  materials = nod.materials;
			  header2 = &nod.header2;
			  bones = nod.bones;
			  meshes = nod.meshes;
			  vertices = nod.vertices;
			  mesh_groups = nod.mesh_groups;
			  faces = nod.faces;
			  
			  printf("\n\n%s\n", argv[1]);
			  
			  printf("version = %i\n", header1->Version);
			  printf("num_materials = %i\n", header1->NumMaterials);
			  printf("num_bones = %i\n", header2->NumBones);
			  printf("num_meshes = %i\n", header2->NumMeshs);
			  printf("num_vertices = %i\n", header2->NumVertices);
			  printf("num_faces = %i\n", header2->NumFaces);
			  printf("num_groups = %i\n", header2->NumGroups);
			  printf("flags = { ");
			  
			  if (!header2->ModelFlags)
				  printf("NONE ");
			  
			  if (header2->ModelFlags & NOD_MF_HASLOD)
			  {
				  printf("NOD_MF_HASLOD ");
			  }
			  
			  if (header2->ModelFlags & NOD_MF_INLINE)
			  {
				  printf("NOD_MF_INLINE ");
			  }
			  
			  if (header2->ModelFlags & NOD_MF_STATIC)
			  {
				  printf("NOD_MF_STATIC ");
			  }
			  
			  if (header2->ModelFlags & NOD_MF_RESERVED1)
			  {
				  printf("NOD_MF_RESERVED1 ");
			  }
			  
			  if (header2->ModelFlags & NOD_MF_RESERVED1)
			  {
				  printf("NOD_MF_RESERVED1 ");
			  }
			  
			  printf("}\n");
			  
			  printf("bbox = (%.3f, %.3f, %.3f)\n       (%.3f, %.3f, %.3f)\n",
						header2->Bounds[0], header2->Bounds[1], header2->Bounds[2],
						header2->Bounds[3], header2->Bounds[4], header2->Bounds[5]);
		  
			  for (i = 0; i < header1->NumMaterials; i++)
			  {
				  printf("materials[%i].name = '%s'\n", 
							i, materials[i].MaterialName);
			  }

			  for (i = 0; i < header2->NumBones; i++)
			  {
				  printf("bones[%i].RestTranslate = <%.3f %.3f %.3f>\n", 
							i, 
							bones[i].RestTranslate[0],
							bones[i].RestTranslate[1],
							bones[i].RestTranslate[2]);

			  }
			  
			  num_verts = num_faces = 0;
			  
			  for (i = 0; i < header2->NumGroups; i++)
			  {  
				  printf("group[%02i].mesh = '%s'\n",
							i, meshes[mesh_groups[i].MeshNum].MeshName);

				  printf("group[%02i].flags = {", i);

				  if (!mesh_groups[i].GroupFlags)
					  printf("NONE ");

				  if (mesh_groups[i].GroupFlags & NOD_GF_HASLOD)
					  printf("NOD_GF_HASLOD ");

				  if (mesh_groups[i].GroupFlags & NOD_GF_NOWEIGHTS)
					  printf("NOD_GF_NOWEIGHTS ");

				  if (mesh_groups[i].GroupFlags & NOD_GF_NOSKINNING)
					  printf("NOD_GF_NOSKINNING ");

				  if (mesh_groups[i].GroupFlags & NOD_GF_MULTITEXTURE)
					  printf("NOD_GF_MULTITEXTURE ");

				  printf("}\n");

				  for (j = 0; j < mesh_groups[i].NumVertices; j++)
				  {
					  printf("group[%02i].vertex[%04i] { pos %.3f %.3f %.3f; \tuv %.3f %.3f }\n",
								i, j, 
								vertices[num_verts + j].Pos[0],
								vertices[num_verts + j].Pos[1],
								vertices[num_verts + j].Pos[2],
								vertices[num_verts + j].UV[0],
								1.0 - vertices[num_verts + j].UV[1]);
				  }

				  for (j = 0; j < mesh_groups[i].NumFaces; j++)
				  {
					  printf("group[%02i].tris[%04i] { %u, %u, %u }\n",
								i, j, 
								faces[num_faces + j].indices[0],
								faces[num_faces + j].indices[1],
								faces[num_faces + j].indices[2]);
				  }

				  num_verts += mesh_groups[i].NumVertices;
				  num_faces += mesh_groups[i].NumFaces;
			  }
		  }

		  printf("num_verts = %i, num_faces = %i\n", num_verts, num_faces);
	  }
	  else
		  printf("\n\n%s [ load ] filename.nod\n", argv[0]);
  }
  else
  {
	  printf("\n\n%s [ load ] filename.nod\n", argv[0]);
  }
}
#endif
