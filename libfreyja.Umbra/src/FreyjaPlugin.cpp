/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : Plugin
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the plugin system with C exported API.
 *
 * 
 *-- Test Defines -------------------------------------------
 *           
 * TEST_PLUGIN  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2001.02.24:
 * Mongoose - Created, based on python test code
 *
 ==========================================================================*/

#include <dlfcn.h> 
#include <sys/types.h>
#include <dirent.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <hel/math.h>
#include <hel/Quaternion.h>
#include <hel/Vector3d.h>
#include <hel/BoundingVolume.h>

#include "FreyjaFileReader.h"
#include "FreyjaPlugin.h"


FreyjaPlugin *FreyjaPlugin::mPlugin = 0x0;


FreyjaPlugin::FreyjaPlugin(FreyjaScene *scene, char *plugin_dir)
{
	mPrinter = 0x0;

	/* Backend, data model */
	mOptions = 0;
	mScene = scene;

	/* State machine's 3d model object pointers */
	mModel = 0x0;
	mMesh = 0x0;
	mVertexFrame = 0x0;
	mPolygon = 0x0;
	mUVMap = 0x0;
	mMaterial = 0x0;
	mSkeleton = 0x0;
	mBone = 0x0;
	mAnimation = 0x0;

	mPoint[0] = mPoint[1] = mPoint[2] = 0.0f;

	/* Plugin directory setup */
	if (plugin_dir && plugin_dir[0])
	{
		long len = strlen(plugin_dir);

		mPluginDir = new char[len + 2];
		strncpy(mPluginDir, plugin_dir, len);
		mPluginDir[len+1] = 0;
	}
	else
	{
		mPluginDir = new char[64];
		strcpy(mPluginDir, "/usr/local/lib/freyja/modules/model/");    
	}

	addPluginDirectory("/usr/local/lib/freyja/modules/model");
	addPluginDirectory("/usr/share/freyja/plugins");

	FreyjaPlugin::mPlugin = this;
}


FreyjaPlugin::~FreyjaPlugin()
{
	clear();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


long FreyjaPlugin::getBoneMeshIndex(long item)
{
	
	printError("FreyjaPlugin::getBoneMesh is not implemented");
	return 0;
}


int FreyjaPlugin::getTexture(unsigned int index,
							 unsigned int *w, unsigned int *h, 
							 unsigned int *numBytes,  unsigned char **image)
{
	printError("FreyjaPlugin::getTexture is not implemented");
	return -1;
}


unsigned int FreyjaPlugin::getCount(freyja_object_t type)
{
	switch (type)
	{
	case FREYJA_MODEL:
		return mScene->getModelCount();
		break;

	case FREYJA_VERTEX:
		if (mMesh)
		{
			return mMesh->getVertexCount();
		}
		return 0;
		break;

	case FREYJA_TEXCOORD:
		if (mMesh)
		{
			return mMesh->getTexCoordCount();
		}
		return 0;
		break;

	case FREYJA_MESH:
		if (mModel)
		{
			return mModel->getMeshCount();
		}
		return 0;
		break;

	case FREYJA_VERTEX_FRAME:
		if (mMesh)
		{
			return mMesh->getVertexFrameCount();
		}
		return 0;
		break;


	case FREYJA_VERTEX_GROUP:
		return mScene->getVertexGroupCount();
		break;


	case FREYJA_POLYGON:
		if (mMesh)
		{
			return mMesh->getPolygonCount();
		}
		return 0;
		break;

	case FREYJA_BONE:
		if (mSkeleton)
		{
			return mSkeleton->getBoneCount();
		}
		return 0;
		break;

	case FREYJA_SKELETON:
		return mScene->getSkeletonCount();
		break;

	case FREYJA_ANIMATION:
		return mScene->getAnimationCount();
		break;

	case FREYJA_MATERIAL:
		return mScene->getMaterialCount();
		break;

	case FREYJA_TEXTURE:
		return mScene->getTextureCount();
		break;

	case FREYJA_MESHTREE_FRAME:
	case FREYJA_MESHTREE_TAG:
		return 0;
		break;
	}

	return 0;
}


long FreyjaPlugin::freyjaIterator(freyja_object_t type, int item)
{
	switch (type)
	{
	case FREYJA_VERTEX:
		if (!mMesh)
			return FREYJA_PLUGIN_ERROR;

		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexVertex = mMesh->vertices.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexVertex;
			break;
		default:
			mIndexVertex = item;
		}

		if (mIndexVertex < mMesh->vertices.end())
		{
			return mIndexVertex;
		}
		break;


	case FREYJA_TEXCOORD:
		if (!mMesh)
			return FREYJA_PLUGIN_ERROR;

		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexTexCoord = mMesh->texcoords.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexTexCoord;
			break;
		default:
			mIndexTexCoord = item;
		}

		if (mIndexTexCoord < mMesh->texcoords.end())
		{
			return mIndexTexCoord;
		}
		break;

	case FREYJA_MODEL:
		if (!mScene)
			return FREYJA_PLUGIN_ERROR;

		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexModel = mScene->models.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexModel;
			break;
		default:
			mIndexModel = item;
		}

		if (mIndexModel < mScene->models.end())
		{
			mModel = mScene->models[mIndexModel];
			return mIndexModel;
		}
		break;


	case FREYJA_MESH:
		if (!mScene)
			return FREYJA_PLUGIN_ERROR;

		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexMesh = mScene->meshes.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexMesh;
			break;
		default:
			mIndexMesh = item;
		}

		if (mIndexMesh < mScene->meshes.end())
		{
			mMesh = mScene->meshes[mIndexMesh];
			return mIndexMesh;
		}
		break;


	case FREYJA_VERTEX_FRAME:
		if (!mMesh)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexVertexFrame = mMesh->frames.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexVertexFrame;
			break;
		default:
			mIndexVertexFrame = item;
		}

		if (mIndexVertexFrame < mMesh->frames.end())
		{
			mVertexFrame = mMesh->frames[mIndexVertexFrame];
			return mIndexVertexFrame;
		}
		break;


	case FREYJA_POLYGON:    
		if (!mMesh)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexPolygon = mMesh->polygons.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexPolygon;
			break;
		default:
			mIndexPolygon = item;
		}

		if (mIndexPolygon < mMesh->polygons.end())
		{
			mPolygon = mMesh->polygons[mIndexPolygon];
			return mIndexPolygon;
		}
		break;


	case FREYJA_BONE:
		if (!mSkeleton)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexBone = mSkeleton->mBones.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexBone;
			break;
		default:
			mIndexBone = item;
		}

		if (mIndexBone < mSkeleton->mBones.end())
		{
			mBone = mSkeleton->mBones[mIndexBone];
			return mIndexBone;
		}
		break;


	case FREYJA_SKELETON:
		if (!mScene)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexSkeleton = mScene->skeletons.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexSkeleton;
			break;
		default:
			mIndexSkeleton = item;
		}

		if (mIndexSkeleton < mScene->skeletons.end())
		{
			mSkeleton = mScene->skeletons[mIndexSkeleton];
			return mIndexSkeleton;
		}
		break;


	case FREYJA_ANIMATION:
		if (!mScene)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexAnimation = mScene->animations.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexAnimation;
			break;
		default:
			mIndexAnimation = item;
		}

		if (mIndexAnimation < mScene->animations.end())
		{
			mAnimation = mScene->animations[mIndexAnimation];
			return mIndexAnimation;
		}
		break;


	case FREYJA_VERTEX_GROUP:
		if (!mScene)
			return FREYJA_PLUGIN_ERROR;

		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexVertexGroup = mScene->vertexgroups.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexVertexGroup;
			break;
		default:
			mIndexVertexGroup = item;
		}

		if (mIndexVertexGroup < mScene->vertexgroups.end())
		{
			mVertexGroup = mScene->vertexgroups[mIndexVertexGroup];
			return mIndexVertexGroup;
		}
		break;


	case FREYJA_MATERIAL:
		if (!mScene)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexMaterial = mScene->materials.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexMaterial;
			break;
		default:
			mIndexMaterial = item;
		}

		if (mIndexMaterial < mScene->materials.end())
		{
			mMaterial = mScene->materials[mIndexMaterial];
			return mIndexMaterial;
		}
		break;


	case FREYJA_TEXTURE:
		if (!mScene)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexTexture = mScene->textures.begin();
			break;
		case FREYJA_LIST_NEXT:
			++mIndexTexture;
			break;
		default:
			mIndexTexture = item;
		}

		if (mIndexTexture < mScene->textures.end())
		{
			mTexture = mScene->textures[mIndexTexture];
			return mIndexTexture;
		}
		break;


	case FREYJA_MESHTREE_FRAME:
	case FREYJA_MESHTREE_TAG:
		break;
	}

	return FREYJA_PLUGIN_ERROR;
}


FreyjaBone *FreyjaPlugin::getBone(unsigned int index)
{
	if (!mSkeleton || mSkeleton->mBones.empty() || 
		index > mSkeleton->mBones.size()-1)
		return 0x0;

	return mSkeleton->mBones[index];
}


void FreyjaPlugin::getVertex(unsigned int index, vec3_t xyz)
{
	if (mMesh)
	{
		mMesh->getVertex(index, xyz);
	}
}


void FreyjaPlugin::getNormal(unsigned int index, vec3_t xyz)
{
	if (mMesh)
	{
		mMesh->getNormal(index, xyz);
	}
}


void FreyjaPlugin::getTexCoord(unsigned int index, vec2_t uv)
{
	if (mMesh)
	{
		mMesh->getTexCoord(index, uv);
	}
}


int FreyjaPlugin::getTextureFilename(unsigned int index, char **filename)
{
	*filename = 0x0;
	printError("FreyjaPlugin::freyjaTextureStoreFilename not implemented");
	return -1;
}


int FreyjaPlugin::getPolygon(freyja_object_t obj, int item, unsigned int *value)
{
	FreyjaPolygon *polygon;

  
	if (!mMesh)
		return -1;

	// FIXME: Temp cache to check old cached polygon
	polygon = mMesh->polygons[mIndexPolygon];

	if (!polygon || polygon != mPolygon)
		return -1; 

	switch (obj)
	{
	case FREYJA_VERTEX:
		if (item > (int)polygon->vertices.end())
			return -1;

		*value = polygon->vertices[item];
		return *value;
		break;


	case FREYJA_TEXCOORD:
		if (item > (int)polygon->texcoords.end())
			return -1;

		*value = polygon->texcoords[item];
		return *value;
		break;

	default:
		return -1;
	}

	return -1;
}


int FreyjaPlugin::getPolygon(freyja_object_t obj, int item, vec_t *value)
{
	FreyjaPolygon *polygon;
	unsigned int index;

  
	if (!mMesh)
		return -1;

	// FIXME: Temp cache to check old cached polygon
	polygon = mMesh->polygons[mIndexPolygon];

	if (!polygon || polygon != mPolygon)
		return -1; 

	switch (obj)
	{
	case FREYJA_VERTEX:
		if (item > (int)polygon->vertices.end() || polygon->vertices.empty())
			return -1;

		index = polygon->vertices[item];
		mMesh->getVertex(index, value);
		return index;
		break;


	case FREYJA_TEXCOORD:
		if (item > (int)polygon->texcoords.end() || polygon->texcoords.empty())
			return -1;

		index = polygon->texcoords[item];
		mMesh->getTexCoord(index, value);
		return index;
		break;

	default:
		return -1;
	}

	return -1;
}


long FreyjaPlugin::getCurrentIndex(freyja_object_t type)
{
	switch (type)
	{
	case FREYJA_MODEL:
		return mIndexModel;
		break;

	case FREYJA_MESH:
		return mIndexMesh;
		break;

	case FREYJA_TEXTURE:
		return mIndexTexture;
		break;

	case FREYJA_MATERIAL:
		return mIndexMaterial;
		break;

	case FREYJA_VERTEX_FRAME:
		return mIndexVertexFrame;
		break;

	case FREYJA_VERTEX:
		return mIndexVertex;
		break;

	case FREYJA_VERTEX_GROUP:
		return mIndexVertexGroup;
		break;

	case FREYJA_TEXCOORD:
		return mIndexTexCoord;
		break;

	case FREYJA_POLYGON:
		return mIndexPolygon;
		break;

	case FREYJA_BONE:
		return mIndexBone;
		break;

	case FREYJA_SKELETON:
		return mIndexSkeleton;
		break;

	case FREYJA_ANIMATION:
		return mIndexAnimation;
		break;


	case FREYJA_MESHTREE_FRAME:
	case FREYJA_MESHTREE_TAG:
		break;
	}

	return FREYJA_PLUGIN_ERROR;
}

	
void FreyjaPlugin::printError(char *format, ...)
{
	va_list args;
	
	va_start(args, format);

	if (mPrinter)
	{
		mPrinter->errorArgs(format, &args);
	}
	else
	{
		vfprintf(stderr, format, args);
		fprintf(stderr, "\n");
	}
	va_end(args);
}


void FreyjaPlugin::printErrorArgs(char *format, va_list *args)
{
	if (mPrinter)
	{
		mPrinter->errorArgs(format, args);
	}
	else
	{
		vfprintf(stderr, format, *args);
		fprintf(stderr, "\n");
	}
}


void FreyjaPlugin::printMessage(char *format, ...)
{
	va_list args;
	
	va_start(args, format);

	if (mPrinter)
	{
		mPrinter->messageArgs(format, &args);
	}
	else
	{
		vfprintf(stdout, format, args);
		printf("\n");
	}
	va_end(args);
}


void FreyjaPlugin::printMessageArgs(char *format, va_list *args)
{
	if (mPrinter)
	{
		mPrinter->messageArgs(format, args);
	}
	else
	{
		vfprintf(stdout, format, *args);
		printf("\n");
	}
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void externFunctionBegin(char *func_symbol_name)
{
}


void externFunctionArg(char *arg_symbol_name, float default_value)
{
}


void externFunctionArg(char *arg_symbol_name, int default_value)
{
}


void externFunctionEnd()
{
}


void FreyjaPlugin::setPrinter(FreyjaPrinter *printer)
{
	mPrinter = printer;
}


void FreyjaPlugin::addPluginDirectory(const char *dir)
{
	unsigned int l;
	char *dir2;


	if (!dir || !dir[0])
		return;

	l = strlen(dir);

	dir2 = new char[l+1];
	strncpy(dir2, dir, l);
	dir2[l] = 0;

	mPluginDirectories.pushBack(dir2);
}


int FreyjaPlugin::setTextureFilename(const char *filename)
{
	printError("FreyjaPlugin::freyjaTextureStoreFilename not implemented");
	return -1;
}


void FreyjaPlugin::clear()
{
	mModel = 0x0;
	mMesh = 0x0;
	mVertexFrame = 0x0;
	mPolygon = 0x0;
	mUVMap = 0x0;
	mMaterial = 0x0;
	mSkeleton = 0x0;
	mBone = 0x0;
	mAnimation = 0x0;

	mPoint[0] = mPoint[1] = mPoint[2] = 0.0f;

	mScene->erase();
}


void FreyjaPlugin::transform(freyja_transform_t object,
							 freyja_transform_action_t action,
							 vec_t x, vec_t y, vec_t z)
{
	Matrix m;
	unsigned int i, count;


	m.setIdentity();

	/* Mongoose 2004.10.30, 
	 * Rotate/Scale about point needs:
	 * 1. Translate -point.x -point.y point.-z,
	 * 2. Rotate x, y, z
	 * 3. Then translate point.x, point.y, point.z */

	switch (action)
	{
	case fTranslate:
		m.translate(x, y, z);
		break;

	case fRotateAboutPoint:
		m.translate(-mPoint[0], -mPoint[1], -mPoint[2]);
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.rotate(x, y, z);
		m.translate(mPoint[0], mPoint[1], mPoint[2]);
		break;

	case fScaleAboutPoint:
		m.translate(-mPoint[0], -mPoint[1], -mPoint[2]);
		m.scale(x, y, z);
		m.translate(mPoint[0], mPoint[1], mPoint[2]);
		break;

	case fRotate:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.rotate(x, y, z);
		break;

	case fScale:
		m.scale(x, y, z);
		break;

	default:
		printError("FreyjaPlugin::transform> Unknown action");
		return;
	}


	switch (object)
	{
	case fTransformScene:
		printError("FreyjaPlugin::transform> Unimplemented object");
		break;
	case fTransformModel:
		printError("FreyjaPlugin::transform> Unimplemented object");
		break;
	case fTransformMesh:
		if (mMesh)
		{
			vec_t *array = mMesh->vertices.getVectorArray();

			for (count = mMesh->vertices.end()/3, i = 0; i < count; ++i)
			{
				/* Mongoose 2004.10.30, 
				 * This transforms the vertex and copies result back to it */
				m.multiply3v(array+i*3, array+i*3);
			}
		}
		break;
	case fTransformVertexFrame:
		printError("FreyjaPlugin::transform> Unimplemented object");
		break;
	case fTransformSkeleton:
		printError("FreyjaPlugin::transform> Unimplemented object");
		break;
	case fTransformBone:
		printError("FreyjaPlugin::transform> Unimplemented object");
		break;
	case fTransformUVMap:
		printError("FreyjaPlugin::transform> Unimplemented object");
		break;
	case fTransformVertexGroup:
		printError("FreyjaPlugin::transform> Unimplemented object");
		break;
	case fTransformVertex:
		printError("FreyjaPlugin::transform> Unimplemented object");
		break;
	case fTransformTexCoord:
		printError("FreyjaPlugin::transform> Unimplemented object");
		break;
	default:
		printError("FreyjaPlugin::transform> Unknown object type");
		return;
	}
	
	printError("FreyjaPlugin::transform() not fully implemented"); 

}


void FreyjaPlugin::importPluginDirectory(const char *dir)
{
}


void FreyjaPlugin::importPlugins()
{
#ifdef FREYJA_PLUGINS
	FreyjaFileReader reader;
	int (*import)(char *filename);
	//int (*check)(char *filename);
	bool done = false;
	char *module_filename;
	void *handle;
	char *error;
	unsigned int i;
	//unsigned char *image = 0x0;
	//unsigned int i, width = 0, height = 0;
	//char type = 0;


	printMessage("[FreyjaImage plugin system invoked]");

	for (i = mPluginDirectories.begin(); i < mPluginDirectories.end(); ++i)
	{
		if (!reader.openDirectory(mPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access image plugin directory");
			continue;
		}

		while (!done && (module_filename = reader.getNextDirectoryListing()))
		{
			if (reader.isDirectory(module_filename))
				continue;

			if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
			{
				printError("In module '%s'.", module_filename);
				
				if ((error = dlerror()) != NULL)  
				{
					printError("%s", error);
				}

				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				printMessage("Module '%s' opened.", module_filename);
				
				import = (int (*)(char *filename))dlsym(handle, "import_model");

				if ((error = dlerror()) != NULL)  
				{
					printError("%s", error);
					dlclose(handle);
					continue;
				}

				/* Mongoose 2004.10.30, 
				 * FIXME: Cache any functional controls / plugins */
				//done = !(*import)((char*)filename);
				
				if ((error = dlerror()) != NULL) 
				{
					printError("%s", error);
					dlclose(handle);
					continue;
				}
				
				dlclose(handle);
			}
		}

		reader.closeDirectory();

		if (done)
			break;
	}

	printMessage("[FreyjaPlugin module loader sleeps now]");
#else
	printMessage("FreyjaPlugin: This build was compiled w/o plugin support");
#endif
}


int FreyjaPlugin::importModel(const char *filename)
{
#ifdef FREYJA_PLUGINS
	FreyjaFileReader reader;
	bool loaded = false, done = false;
	char *module_filename;
	int (*import)(char *filename);
	void *handle;
	char *error;
	unsigned int i;


	freyjaPrintMessage("[FreyjaPlugin module loader invoked]");

	if (!reader.doesFileExist(filename))
	{
		freyjaPrintError("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	for (i = mPluginDirectories.begin(); i < mPluginDirectories.end(); ++i)
	{
		if (!reader.openDirectory(mPluginDirectories[i]))
		{
			printError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while (!done && (module_filename = reader.getNextDirectoryListing()))
		{
			if (reader.isDirectory(module_filename))
				continue;

			freyjaPrintMessage("Module '%s' invoked.", module_filename);

			if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
			{
				printError("In module '%s'.", module_filename);
				
				if ((error = dlerror()) != NULL)  
				{
					printError("%s", error);
				}

				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				freyjaPrintMessage("Module '%s' opened.", module_filename);

#ifdef NO_DUPE_DL_SYM_HACK
				/* Mongoose 2004.11.01, 
				 * temp fix? */
				unsigned int l = strlen(module_filename);
				char tmp[128];
				module_filename[l-3] = 0;
				snprintf(tmp, 64, "%s_import_model", basename(module_filename));
				freyjaPrintMessage("Symbol '%s' import...", tmp);
				import = (int (*)(char *filename))dlsym(handle, tmp);
#else
				import = (int (*)(char *filename))dlsym(handle, "import_model");
#endif

				if ((error = dlerror()) != NULL)  
				{
					printError("%s", error);
					dlclose(handle);
					continue;
				}
				
				done = !(*import)((char*)filename);
				
				if ((error = dlerror()) != NULL) 
				{
					printError("%s", error);
					dlclose(handle);
					continue;
				}
				
				dlclose(handle);
			}
		}

		reader.closeDirectory();

		if (done)
			break;
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (loaded)
		return 0; // sucess
#endif
	return -1;
}


int FreyjaPlugin::exportModel(const char *filename, char *type)
{
#ifdef FREYJA_PLUGINS
	FreyjaFileReader reader;
	bool saved = false;
	char module_filename[128];
	char module_export[128];
	char *name;
	int (*export_mdl)(char *filename);
	void *handle;
	char *error;
	unsigned long i;


	if (!type || !filename)
		return -100;

	if (strcmp(type, "native") == 0)
	{
		return mScene->exportScene(filename);
	}

	freyjaPrintMessage("[FreyjaPlugin module loader invoked]\n");

	name = type;

	for (i = mPluginDirectories.begin(); i < mPluginDirectories.end(); ++i)
	{
		if (!reader.openDirectory(mPluginDirectories[i]))
		{
			freyjaPrintError("Couldn't access plugin directory");
			continue;
		}

		sprintf(module_filename, "%s/%s.so", mPluginDirectories[i], name);
		sprintf(module_export, "freyja_model__%s_export", name);  // use 'model_export'?

		if (!(handle = dlopen(module_filename, RTLD_NOW)))
		{
			freyjaPrintError("\tERROR: In module '%s'.\n", module_filename);

			if ((error = dlerror()) != NULL)  
			{
				freyjaPrintError("\tERROR: %s\n", error);
			}
		}
		else
		{
			freyjaPrintMessage("\tModule '%s' opened.\n", module_filename);
    
			export_mdl = (int (*)(char * filename))dlsym(handle, module_export);

			if ((error = dlerror()) != NULL)  
			{
				freyjaPrintError("\tERROR: %s\n", error);
				dlclose(handle);
			}

			saved = (!(*export_mdl)((char*)filename));

			if ((error = dlerror()) != NULL) 
			{
				dlclose(handle);
			}

			dlclose(handle);
		}

		if (saved)
			break;
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (saved)
		return 0; // sucess
#else
	if (!type || !filename)
		return -100;

	if (strcmp(type, "freyja") == 0)
		return mFreyja->saveFile(filename);
#endif
	return -1;
}


void FreyjaPlugin::freyjaBegin(freyja_object_t type)
{
	switch (type)
	{
	case FREYJA_MATERIAL:
		mStack.push(FREYJA_MATERIAL);
		mMaterial = new FreyjaMaterial();
		mScene->materials.pushBack(mMaterial);
		mMaterial->id = mScene->materials.end() - 1;
		mIndexMaterial = mMaterial->id;
		break;


	case FREYJA_TEXTURE:
		mStack.push(FREYJA_TEXTURE);
		mTexture = new FreyjaTexture();
		mScene->textures.pushBack(mTexture);
		mTexture->id = mScene->textures.end() - 1;
		mIndexTexture = mTexture->id;
		break;

	case FREYJA_MODEL:
		mStack.push(FREYJA_MODEL);
		mModel = new FreyjaModel();
		mScene->models.pushBack(mModel);
		mModel->id = mScene->models.end() - 1;
		mIndexModel = mModel->id;
		break;


	case FREYJA_MESH:
		mStack.push(FREYJA_MESH);
		mMesh = new FreyjaMesh();
		mScene->meshes.pushBack(mMesh);
		mMesh->id = mScene->meshes.end() - 1;
		mModel->meshes.pushBack(mMesh->id);
		mIndexMesh = mMesh->id;
		break;


	case FREYJA_POLYGON:
		mStack.push(FREYJA_POLYGON);
		mPolygon = new FreyjaPolygon();
		mPolygon->id = mMesh->polygons.end() - 1;
		mMesh->polygons.pushBack(mPolygon);
		mIndexPolygon = mPolygon->id;
		break;


	case FREYJA_VERTEX_FRAME:
		mStack.push(FREYJA_VERTEX_FRAME);
		mVertexFrame = new FreyjaVertexFrame();
		mVertexFrame->id = mMesh->frames.end() - 1;
		mMesh->frames.pushBack(mVertexFrame);
		mIndexVertexFrame = mVertexFrame->id;
		break;


	case FREYJA_VERTEX_GROUP:
		mStack.push(FREYJA_VERTEX_GROUP);
		mVertexGroup = new FreyjaVertexGroup();
		mVertexGroup->id = mScene->vertexgroups.end() - 1;
		mScene->vertexgroups.pushBack(mVertexGroup);
		mIndexVertexGroup = mVertexGroup->id;
		break;


	case FREYJA_BONE:
		mStack.push(FREYJA_BONE);
		mBone = new FreyjaBone();
		mSkeleton->mBones.pushBack(mBone);
		mBone->id = mSkeleton->mBones.end() - 1;
		mIndexBone = mBone->id;
		break;


	case FREYJA_SKELETON:
		mStack.push(FREYJA_SKELETON);
		mSkeleton = new FreyjaSkeleton();
		mScene->skeletons.pushBack(mSkeleton);
		mSkeleton->mId = mScene->skeletons.end() - 1;
		mModel->skeletons.pushBack(mSkeleton->mId);
		mIndexSkeleton = mSkeleton->mId;
		break;


	case FREYJA_ANIMATION:
		mStack.push(FREYJA_ANIMATION);
		mAnimation = new FreyjaAnimation();
		mScene->animations.pushBack(mAnimation);
		mAnimation->id = mScene->animations.end() - 1;
		mModel->animations.pushBack(mAnimation->id);
		mIndexAnimation = mAnimation->id;
		break;


	case FREYJA_VERTEX:
		break;
	case FREYJA_TEXCOORD:
		break;
	case FREYJA_MESHTREE_FRAME:
		break;
	case FREYJA_MESHTREE_TAG:
		break;
	}
}


void FreyjaPlugin::freyjaEnd()
{
	switch (mStack.pop())
	{
	case FREYJA_POLYGON:
		break;

	default: // FIXME
		;
	}
}


unsigned int FreyjaPlugin::freyjaNormal3fv(vec3_t xyz)
{
	return mMesh->addNormal(xyz);
}


unsigned int FreyjaPlugin::freyjaTexCoord2fv(vec2_t uv)
{
	if (uv[0] < 0.0f)
		uv[0] = 0.0f;
	else if (uv[0] > 1.0f)
		uv[0] = 1.0f;
	
	if (uv[1] < 0.0f)
		uv[1] = 0.0f;
	else if (uv[1] > 1.0f)
		uv[1] = 1.0f;

	return mMesh->addTexCoord(uv);
}


unsigned int FreyjaPlugin::freyjaVertex3fv(vec3_t xyz)
{
	unsigned int v = mMesh->addVertex(xyz);

	if (mStack.peek() == FREYJA_VERTEX_FRAME)
	{
		mVertexFrame->vertices.pushBack(v);
	}

	return v;
}


void FreyjaPlugin::freyjaSetNormal3fv(unsigned int index, vec3_t xyz)
{
	return mMesh->setNormal(index, xyz);
}


void FreyjaPlugin::freyjaSetTexCoord2fv(unsigned int index, vec2_t uv)
{
	if (uv[0] < 0.0f)
		uv[0] = 0.0f;
	else if (uv[0] > 1.0f)
		uv[0] = 1.0f;
	
	if (uv[1] < 0.0f)
		uv[1] = 0.0f;
	else if (uv[1] > 1.0f)
		uv[1] = 1.0f;

	return mMesh->setTexCoord(index, uv);
}


void FreyjaPlugin::freyjaSetVertex3fv(unsigned int index, vec3_t xyz)
{
	return mMesh->setVertex(index, xyz);
}


void FreyjaPlugin::freyjaVertexWeightStore(unsigned int index, 
										   vec_t weight, unsigned int bone)
{
	mMesh->addVertexWeight(index, weight, bone);
}


void FreyjaPlugin::freyjaVertex1i(unsigned int index)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		//mVertexList.pushBack(index);
		mPolygon->vertices.pushBack(index);
		mMesh->refPolygons[index]->polygons.pushBack(mPolygon->id);
	}
	else
	{
		freyjaPrintError("FreyjaPlugin::freyjaVertex1i> Vertex defined outside POLYGON!\n");
	}
}


void FreyjaPlugin::freyjaTexCoord1i(unsigned int index)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		//mTexCoordList.pushBack(freyja_id);
		mPolygon->texcoords.pushBack(index);
	}
	else
	{
		freyjaPrintError("FreyjaPlugin::freyjaTexCoord1i> Texel defined outside POLYGON!\n");
	}
}


void FreyjaPlugin::freyjaMaterial1i(int id)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mPolygon->material = id;
	}
	else if (mStack.peek() == FREYJA_MESH)
	{
		mMesh->material = id;
	}
	else
	{
		freyjaPrintError("FreyjaPlugin::freyjaMaterial1i> Material defined outside POLYGON or MESH!\n");
	}
}


unsigned int FreyjaPlugin::freyjaFlags()
{
	return mOptions;
}


void FreyjaPlugin::freyjaFlags(unsigned int set)
{
	mOptions = set;
}


void FreyjaPlugin::freyjaBoneName(char *name)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mBone)
		{
			freyjaPrintError("FreyjaPlugin::freyjaTagPos> BONEMTAG isn't allocated!\n");
			return;
		}
		else 
		{
			strncpy(mBone->name, name, 64);
			mBone->name[63] = 0;
		}
	}
	else
	{
		freyjaPrintError("FreyjaPlugin::freyjaTagPos> Pos defined outside BONEMTAG!\n");
		return;
	}
}


void FreyjaPlugin::freyjaBonePos(float x, float y, float z)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mBone)
		{
			freyjaPrintError("FreyjaPlugin::freyjaTagPos> BONEMTAG isn't allocated!\n");
			return;
		}
		else 
		{
			mBone->position[0] = x;
			mBone->position[1] = y;
			mBone->position[2] = z;
		}
	}
	else if (mBone) // HACK
	{
		mBone->position[0] = x;
		mBone->position[1] = y;
		mBone->position[2] = z;
	}
	else
	{
		freyjaPrintError("FreyjaPlugin::freyjaTagPos> Pos defined outside BONEMTAG!\n");
		return;
	}
}


void FreyjaPlugin::freyjaBoneFlags(unsigned int flags)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mBone)
		{
			freyjaPrintError("FreyjaPlugin::freyjaTagFlags> BONEMTAG isn't allocated!\n");
			return;
		}
		else 
		{
			mBone->flags = flags;
		}
	}
	else if (mBone) // HACK
	{
		mBone->flags = flags;
	}
	else
	{
		freyjaPrintError("FreyjaPlugin::freyjaTagFlags> Flag defined outside BONEMTAG!\n");
		return;
	}
}


void FreyjaPlugin::freyjaBoneAddMesh(unsigned int mesh)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mBone)
		{
			freyjaPrintError( 
					"FreyjaPlugin::freyjaTagAddMesh> BONEMTAG isn't allocated!\n");
			return;
		}
		else 
		{
			mBone->meshes.pushBack(mesh);
		}
	}
	else if (mBone) // HACK
	{
		mBone->meshes.pushBack(mesh);
	}
	else
	{
		freyjaPrintError("FreyjaPlugin::freyjaTagAddMesh> Mesh defined outside BONEMTAG!\n");
		return;
	}
}


void FreyjaPlugin::freyjaBoneAddChild(unsigned int bone)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mBone)
		{
			freyjaPrintError("FreyjaPlugin::freyjaTagAddSlave> BONEMTAG isn't allocated!\n");
      
			return;
		}
		else 
		{
			FreyjaBone *child = mSkeleton->mBones[bone];

			if (child)
			{
				unsigned int i;

				child->parent = freyjaGetCurrent(FREYJA_BONE);// mIndexBone;

				for (i = mBone->children.begin(); 
					 i < mBone->children.end(); ++i)
				{
					/* Avoid dupes */
					if (mBone->children[i] == bone)
						return;
				}

				mBone->children.pushBack(bone);
			}
		}
	}
	else
	{
		freyjaPrintError("FreyjaPlugin::freyjaTagAddSlave> Slave defined outside BONEMTAG!\n");

		return;
	}
}


void FreyjaPlugin::freyjaBoneRotate(vec3_t xyz)
{
	if (mBone)
	{
		mBone->rotation = Quaternion(xyz);
	}
}


void FreyjaPlugin::freyjaBoneRotate(Quaternion q)
{
	if (mBone)
	{
		mBone->rotation = q;
	}
}


void FreyjaPlugin::setTransformPoint(vec3_t p)
{
	mPoint[0] = p[0];
	mPoint[1] = p[1];
	mPoint[2] = p[2];
}


int FreyjaPlugin::setTexture(unsigned char *image, unsigned int numBytes,
							 unsigned int width, unsigned int height)
{
	FreyjaTexture *t;
	unsigned int size;


	if (!image || numBytes < 3 || !width || !height)
		return FREYJA_PLUGIN_ERROR;

	size = width*height*numBytes;

	t = new FreyjaTexture();
	t->pixelDepth = numBytes;
 	t->imageWidth = width;
	t->imageHeight = height;
	t->image = new unsigned char[size]; 
	memcpy(t->image, image, size);

	mScene->textures.pushBack(t);
	return mScene->textures.end() - 1;
}


void FreyjaPlugin::addModule(char *name)
{
	if (name && name[0])
		mModules.pushBack(name);
}


void FreyjaPlugin::setMeshPolygonsMaterial(unsigned int index, int material)
{
	FreyjaMesh *mesh = mScene->meshes[index];
	FreyjaPolygon *poly;
	unsigned int i;


	if (!mesh)
		return;

	for (i = mesh->polygons.begin(); i < mesh->polygons.end(); ++i)
	{  
		poly = mesh->polygons[i];

		if (!poly)
			continue;

		poly->material = material;
	}

	mesh->material = material;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJAPLUGIN

int runFreyjaPluginUnitTest(int argc, char *argv[])
{
	FreyjaPlugin ep;

	return 0;
}


int main(int argv, char *argc[])
{
 	printf("[FreyjaPlugin class test]\n");

	return runFreyjaPluginUnitTest(argc, argv); 
  
	return 0;
}

#endif
