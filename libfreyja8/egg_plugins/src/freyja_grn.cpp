/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Template plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2004.03.02:
 * Mongoose - Created, using my grn2ase code as a base which uses
 *            the grn system from a UO clone I patched up.
 *
 *            See GRN files for details.  This is to be replaced with
 *            my own GRN implementation later, so it'll be cleaner for
 *            library use.
 ==========================================================================*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include <freyja8/EggPlugin.h>

/* Export as C functions */
extern "C" {

	int freyja_model__grn_check(char *filename);
	int freyja_model__grn_import(char *filename);
	int freyja_model__grn_export(char *filename);
}

#include "GRN/grannyfile.h"

using namespace std;

#ifdef WIN32
#   define snprintf _snprintf
#endif

unsigned int freyja_model_grn_info()
{
	return (1 | 2 | 4);
}


int freyja_model__grn_check(char *filename)
{
	cGrannyFile grn;

	grn.load(filename, "");

	if (!grn.initialized())
	{
		fprintf(stderr, "grn_import> Failed to load file\n");
		return -1;
	}	

	return 0;
}


int freyja_model__grn_export(char *filename)
{
	return -1;
}


int freyja_model__grn_import(char *filename)
{
	const float scale = 15.0f;
	cGrannyFile grn;
	unsigned int i, j;
	string out;


	grn.load(filename, "");

	if (!grn.initialized())
	{
		fprintf(stderr, "grn_import> Failed to load file\n");
		return -1;
	}

	list<Mesh>::const_iterator iter;
	Meshes meshes = grn.getMeshes();
	Textures textures = grn.getTextures();
    Bones bones = grn.getBones();
	string texture = grn.getTextureName();


	// printf("%i meshes\n", meshes.meshes.size());

#ifdef EGGV9
	eggBegin(FREYJA_MODEL);
	eggMetaData1String(NAME, filename);
	eggMetaData3u(WORLD_TO_FREYJA, X, Z, NY);
	eggMetaData1f(SCALE_VERTEX, 15.0f);
	eggMetaData1f(SCALE_BONE, 15.0f);
#endif

	for (iter = meshes.meshes.begin(); iter != meshes.meshes.end(); iter++)
	{
		Mesh mesh = *iter;
		Map <unsigned int, unsigned int> transUV, transXYZ;
		unsigned int textureIndex = 0;

		eggPrintMessage("Importing mesh\n");

		if (texture.c_str())
		{
			string texture2 = texture + ".png";
			textureIndex = eggTextureStoreFilename((char*)texture2.c_str());
		}

		eggBegin(FREYJA_MESH);
		eggBegin(FREYJA_GROUP);  

		for (j = 0; j < mesh.points.size(); ++j)
		{
			Point p = mesh.points[j];
			i = eggVertexStore3f(p.points[0]*scale, p.points[2]*scale, -p.points[1]*scale);
			// XZ-Y

			// Generates id translator list
			transXYZ.Add(j, i);
		}
		eggEnd(); // GROUP

		for (j = 0; j < mesh.normals.size(); ++j)
		{
			Point p = mesh.normals[j];
			eggVertexNormalStore3f(transXYZ[j], 
								   p.points[0], p.points[2], -p.points[1]);
		}

		for (j = 0; j < mesh.textureMap.size(); ++j)
		{
			Point p = mesh.textureMap[j];
			i = eggTexCoordStore2f(p.points[0], p.points[1]);

			// Generates id translator list
			transUV.Add(j, i);
		}

		for (j = 0; j < mesh.polygons.size(); ++j)
		{
			gPolygon p = mesh.polygons[j];

			eggBegin(FREYJA_POLYGON);

			eggVertex1i(transXYZ[p.nodes[0]]);
			eggVertex1i(transXYZ[p.nodes[1]]);
			eggVertex1i(transXYZ[p.nodes[2]]);

			eggTexCoord1i(transUV[textures.polygons[j].nodes[1]]);
			eggTexCoord1i(transUV[textures.polygons[j].nodes[2]]);
			eggTexCoord1i(transUV[textures.polygons[j].nodes[3]]);

			eggTexture1i(textureIndex);
			eggEnd(); // FREYJA_POLYGON
		}

		eggEnd(); // MESH

		transXYZ.Clear();
		transUV.Clear();

		/* Weights */
		for (i = 0; i < mesh.weights.size(); ++i)
		{
			BoneWeight w = mesh.weights[i];

			for (j = 0; j < w.numWeights; ++j)
			{
				eggVertexWeightStore(i, w.weights[j], w.bones[j]);
			}
		}


		/* Skeleton */
		eggBegin(FREYJA_SKELETON);

		for (i = 0; i < bones.bones.size(); ++i)
		{
			Bone* bone = bones.bones[i];
			char boneName[64];

			if (!bone) // depends on bones[] type if this ever happens
			{
				eggPrintMessage("ERROR: NULL GRN bone!\n");
				eggBegin(FREYJA_BONE);
				eggTagFlags1u(0x0);
				eggTagName("ERROR: NULL!");
				eggEnd();

				continue;
			}

			BoneTies &boneTies = grn.getTies();
			dword id = boneTies.boneObjects[bone->id];
			dword anmNm = grn.findString("__ObjectName");
			dword textId = grn.getValue(id, anmNm);
			std::string boneStr = grn.findID( textId );
			

			snprintf(boneName, 64, "%s", boneStr.data());
			//snprintf(boneName, 64, "bone%04ld", bone->id);

			eggPrintMessage("bone[%i] = '%s', parent = %u\n", i, boneName, bone->parent);

			eggBegin(FREYJA_BONE);
			eggTagFlags1u(0x0);
			eggTagName(boneName);
			eggSetBoneParent(bone->parent);

			eggTagPos3f(bone->translate.points[0]*scale, 
						bone->translate.points[2]*scale, 
						-bone->translate.points[1]*scale);

			eggTagRotateQuaternion4f(bone->quaternion.points[3], // wxyz 
									 bone->quaternion.points[0], 
									 bone->quaternion.points[1],
									 bone->quaternion.points[2]);

			for (j = 0; j < bones.bones.size(); ++j)
			{
				Bone *child = bones.bones[j];

				if (child->parent == bone->id && child->id != bone->id)
				{
					eggTagAddSlave1u(child->id);
				}
			}

			eggEnd(); // FREYJA_BONE
		}

		eggEnd(); // FREYJA_SKELETON
	}

#ifdef EGGV9
	eggEnd(); // FREYJA_MODEL
#endif

	return 0;
}
