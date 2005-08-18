/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the Freyja plugin ABI/API.
 * 
 *           
 *-- History ------------------------------------------------ 
 *
 * 2005.01.03: (v0.9.1)
 * Mongoose - Using Freyja versioning numbers for ABI reporting.
 *
 *            Lots of pruning and documentation updates to the ABI.  Also
 *            some new features to go with the refactoring.
 *
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  Created, FreyjaPlugin ABI interface defination refinement
 *             based on FreyjaPlugin and EggPlugin.
 *
 *             This common interface makes it possible to share ABI 
 *             compatible binary C/C++ plugins between FreyjaPlugin and
 *             EggPlugin.  ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#ifndef GUARD__MONGOOSE_FREYJAPLUGINABI_H
#define GUARD__MONGOOSE_FREYJAPLUGINABI_H

#include <stdarg.h>
#include <hel/math.h>
#include <mstl/Vector.h>

#define FREYJA_API_VERSION   "Freyja 0.9.3"

/* index_t invalid state is equal to UINT_MAX 32bit */
#define INDEX_INVALID      4294967295U

/* FSM uses index_t with extentions by these error/states */
#define FREYJA_NEXT     (INDEX_INVALID - 1)
#define FREYJA_RESET    (INDEX_INVALID - 2)
#define FREYJA_CURRENT  (INDEX_INVALID - 3)
#define FREYJA_SIZE     (INDEX_INVALID - 4)
#define FREYJA_ERROR    (INDEX_INVALID - 5)

/* Flags used to determine polygon data paking */
#define fPolygon_VertexUV      2
#define fPolygon_PolyMapped    8
#define fPolygon_PolyMappedUV  8
#define fPolygon_ColorMapped   16
#define fPolygon_Alpha         32


extern "C" {

	typedef uint32 index_t;

	typedef enum {
		INDEXED_8 = 1, 
		RGB_24, 
		RGBA_32

	} freyja_colormode_t;


	typedef enum {
		FREYJA_MODEL = 1,
		FREYJA_MESH,
		FREYJA_POLYGON,
		FREYJA_BONE,
		FREYJA_SKELETON,
		FREYJA_VERTEX_FRAME,
		FREYJA_VERTEX_GROUP,
		FREYJA_VERTEX,
		FREYJA_TEXCOORD,
		FREYJA_MATERIAL,
		FREYJA_TEXTURE,
		FREYJA_SKEL_ANIMATION,
		FREYJA_SKEL_KEYFRAME
	
	} freyja_object_t;


	typedef enum {
		fTransformScene = 1,
		fTransformModel,
		fTransformMesh,
		fTransformVertexFrame,
		fTransformSkeleton,
		fTransformBone,
		fTransformUVMap,
		fTransformVertexGroup,
		fTransformVertex,
		fTransformTexCoord
	
	} freyja_transform_t;


	typedef enum {
		fTranslate = 1,
		fRotate,
		fScale,
		fRotateAboutPoint,
		fScaleAboutPoint
	
	} freyja_transform_action_t;


	enum freyja_material_flags {
		fFreyjaMaterial_Blending = 1,
		fFreyjaMaterial_Texture = 2,
		fFreyjaMaterial_DetailTexture = 4,
		fFreyjaMaterial_Normalize = 8
	};


	///////////////////////////////////////////////////////////////////////
	// Freyja Iterator functions
	//
	///////////////////////////////////////////////////////////////////////

	void freyjaBegin(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : A new object of type is constructed.
	 ------------------------------------------------------*/

	void freyjaEnd();
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(...) is called to start the state
	 * Post : The object is finialized and construction ends
	 ------------------------------------------------------*/

	index_t freyjaIterator(freyja_object_t type, index_t item);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 *        <item> is a FREYJA_LIST_... command or item index
	 *
	 * Post : Sets current item in internal iterator to
	 *        do operations on util a new construction or
	 *        iterator operation of the same type is done.
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaCriticalSectionLock();
	void freyjaCriticalSectionUnlock(); // for null locks ( fake )
	void freyjaCriticalSectionUnLock(index_t lockId);
	/*------------------------------------------------------
	 * Pre  : FreyjaPlugin singleton exists
	 *
	 * Post : If FreyjaPlugin is locked this will block
	 *        in the future. For now it's just to test if
	 *        the singleton is up and running.
	 ------------------------------------------------------*/

	uint32 freyjaGetCount(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns total number of objects of type in 
	 *        parent frame type eg [model]'s meshes
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaGetCurrent(freyja_object_t type);
	/*------------------------------------------------------
	 * Pre  : <type> is valid
	 * Post : Returns index of current internal complex type
	 *        ( Last constructed or iterated )
	 *
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a vertexgroup
	 * Post : Sets vertexgroups[FREYJA_LIST_CURRENT]'s
	 *        pivot / center
	 ------------------------------------------------------*/

	void freyjaPolygonVertex1i(index_t index);
	void freyjaPolygonTexCoord1i(index_t index);
	void freyjaPolygonMaterial1i(index_t id);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a polygon
	 *
	 * Post : Appends/sets polygon[FREYJA_LIST_CURRENT]'s
	 *        vertices, texcoord, or material
	 ------------------------------------------------------*/

	void freyjaGetVertexTexCoord2fv(vec2_t uv);
	void freyjaGetVertexNormal3fv(vec3_t xyz);
	void freyjaGetVertex3fv(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaIterator has initialized a vertex
	 *
	 * Post : Gets vertex[FREYJA_LIST_CURRENT]'s
	 *        position, normal, or texcoord
	 ------------------------------------------------------*/

	index_t freyjaVertexCreate3f(vec_t x, vec_t y, vec_t z);
	index_t freyjaVertexCreate3fv(vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : freyjaBegin(FREYJA_GROUP);
	 *        x,y,z are valid 3space coors
	 * Post : A new vertex created in the model
	 *        Returns the native index of that vertex
	 ------------------------------------------------------*/

	typedef enum {
		
		FREYJA_PLUGIN_NONE            = 0,
		FREYJA_PLUGIN_MESH            = 1,
		FREYJA_PLUGIN_SKELETON        = 2,
		FREYJA_PLUGIN_VERTEX_MORPHING = 4,
		FREYJA_PLUGIN_VERTEX_BLENDING = 8

	} freyja_plugin_options_t;


	void freyjaPluginBegin();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginDescription1s(const char *info_line);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginAddExtention1s(const char *ext);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginImport1i(int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExport1i(int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginArg1i(const char *name, int32 defaults);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginArg1f(const char *name, float defaults);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginArg1s(const char *name, const char *defaults);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginEnd();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginRegisterMenuCallback(int (*)(int menuId, const char *label, int (*)(int event)));
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	//  File I/O
	///////////////////////////////////////////////////////////////////////

	int32 freyjaCheckModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Checks if model is in native format
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/

	int32 freyjaLoadModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads model in native format
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/

	int32 freyjaSaveModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Saves model in native format
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/

	int32 freyjaExportModel(const char *filename, const char *type);
	/*------------------------------------------------------
	 * Pre  : type is module basename eg 'md5.so' -> 'md5'
	 * Post : Exports model using plugins
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/

	int32 freyjaImportModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Imports model using plugins
	 *        Returns 0 on sucess
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Freyja ABI 0.9.3 
	///////////////////////////////////////////////////////////////////////

	void freyjaSpawn();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Starts freyja backend, also does needed allocations
	 ------------------------------------------------------*/

	void freyjaFree();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Stops freyja backend, also frees memory used
	 ------------------------------------------------------*/

	void freyjaPrintError(const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stdout
	 ------------------------------------------------------*/

	void freyjaPrintMessage(const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stdout
	 ------------------------------------------------------*/

	void *freyjaModuleImportFunction(void *handle, const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : System facade for shared object function import
	 ------------------------------------------------------*/

	void *freyjaModuleLoad(const char *module);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : System facade for shared object loading
	 ------------------------------------------------------*/

	void freyjaModuleUnload(void *handle);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : System facade for shared object unloading
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Models
	//
	////////////////////////////////////////////////////////////////

	index_t freyjaGetCurrentModelIndex();

	void freyjaModelGenerateVertexNormals(index_t modelIndex);

	uint32 freyjaGetModelFlags(index_t modelIndex);

	index_t freyjaGetModelMeshIndex(index_t modelIndex, uint32 element);

	uint32 freyjaGetModelMeshCount(index_t modelIndex);

	void freyjaModelTransform(index_t modelIndex,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);

	char freyjaGetModelAppendMeshMode(index_t modelIndex);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's mesh buffer mode is returned or -1 on error
	 *         enabled when returning  1
	 *         disabled when returning  0 
	 ------------------------------------------------------*/

	void freyjaModelClear(index_t modelIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : All data in model is reset/cleared
	 ------------------------------------------------------*/

	void freyjaModelTransformTexCoord(index_t modelIndex,
                                      index_t texCoordIndex,
                                      freyja_transform_action_t action,
                                      vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaModelClampTexCoords(index_t modelIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clamps UVs and TexCoords to 0.0 to 1.0
	 ------------------------------------------------------*/

	void freyjaModelAppendMeshMode(index_t modelIndex, char on);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's mesh buffer mode is 
	 *         enabled when <on> = 1
	 *         disabled when <on> = 0 
	 ------------------------------------------------------*/

	char freyjaModelCopyMesh(index_t modelIndex, index_t mesh, index_t frame);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's copy buffer is modified to include new mesh duplicate
	 *        using the *local <mesh> index and vertex morph <frame>
	 ------------------------------------------------------*/

	char freyjaModelPasteMesh(index_t modelIndex);
	/*------------------------------------------------------
	 * Pre  : Model <modelIndex> exists and has an active copy buffer
	 * Post : Model's copy buffer is added to freyja as world object
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Texcoords
	//
	////////////////////////////////////////////////////////////////

	char freyjaIsTexCoordAllocated(index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns 1 if texcoord exists
	 ------------------------------------------------------*/

	index_t freyjaTexCoordCreate2f(vec_t u, vec_t v);
	index_t freyjaTexCoordCreate2fv(const vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : u, v are clamped/scaled to 0.0f to 1.0f
	 * Post : A new texcoord is created
	 *        Returns the native index of that texcoord
	 ------------------------------------------------------*/

	index_t freyjaGetTexCoordPolygonRefIndex(index_t texcoordIndex,
                                             uint32 element);
	/*------------------------------------------------------
	 * Pre  : texcoordIndex exists
	 * Post : Gets index of polygon referencing this texcoord
	 ------------------------------------------------------*/

	uint32 freyjaGetTexCoordPolygonRefCount(index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : texcoordIndex exists
	 * Post : Gets number of polygons referencing this texcoord
	 ------------------------------------------------------*/

	void freyjaGetTexCoord2fv(index_t texcoordIndex, vec2_t uv);
	vec2_t *freyjaGetTexCoordUV(index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : texcoordIndex exists
	 * Post : Sets passed float array to texcoord u, v
	 ------------------------------------------------------*/

	void freyjaTexCoordCombine(uint32 A, uint32 B);
	/*------------------------------------------------------
	 * Pre  : texcoord indices A and B exist
	 * Post : replaces all references to B with A
	 ------------------------------------------------------*/

	void freyjaTexCoord2f(index_t texcoordIndex, vec_t u, vec_t v);
	void freyjaTexCoord2fv(index_t texcoordIndex, const vec2_t uv);
	/*------------------------------------------------------
	 * Pre  : texcoordIndex exists
	 * Post : Alters the texcoord's values
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Light
	//
	////////////////////////////////////////////////////////////////

	index_t freyjaLightCreate();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index of light spawned
	 ------------------------------------------------------*/

	index_t freyjaGetCurrentLight();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index of light spawned
	 ------------------------------------------------------*/

	void freyjaLightDelete(index_t lightIndex);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Light is removed from light pool
	 ------------------------------------------------------*/

	// Light mutators /////////////////

	void freyjaLightPosition4v(index_t lightIndex, vec4_t position);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <position> 
	 ------------------------------------------------------*/

	void freyjaLightAmbient(index_t lightIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <ambient> color
	 ------------------------------------------------------*/

	void freyjaLightDiffuse(index_t lightIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <diffuse> color
	 ------------------------------------------------------*/

	void freyjaLightSpecular(index_t lightIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Sets <specular> color
	 ------------------------------------------------------*/

	// Light accessors /////////////////

	void freyjaGetLightPosition4v(index_t lightIndex, vec4_t position);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <position> 
	 ------------------------------------------------------*/

	void freyjaGetLightAmbient(index_t lightIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <ambient> color
	 ------------------------------------------------------*/

	void freyjaGetLightDiffuse(index_t lightIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <diffuse> color
	 ------------------------------------------------------*/

	void freyjaGetLightSpecular(index_t lightIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Light <lightIndex> exists
	 * Post : Returns <specular> color
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Vertices
	//
	////////////////////////////////////////////////////////////////

	//USING FSM FIXME index_t freyjaVertexCreate3fv(vec3_t xyz);

	index_t freyjaVertexCombine(index_t vertexIndexA, index_t vertexIndexB);

	void freyjaVertexDelete(index_t vertexIndex);

	index_t freyjaGetCurrentVertex();

	void freyjaCurrentVertex(index_t vertexIndex);

	char freyjaIsVertexAllocated(index_t vertexIndex);

	void freyjaVertexFrame3f(index_t vertexIndex, vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Appends a new mesh animation frame to this vertex
	 ------------------------------------------------------*/

	void freyjaVertexNormalFlip(index_t vertexIndex);

	void freyjaVertexTexcoord2f(index_t vertexIndex, vec_t u, vec_t v);

	void freyjaVertexTexCoord2fv(index_t vertexIndex, vec2_t uv);

	void freyjaVertexNormal3f(index_t vertexIndex, vec_t nx, vec_t ny, vec_t nz);

	void freyjaVertexNormal3fv(index_t vertexIndex, vec3_t nxyz);

	void freyjaVertexPosition3fv(index_t vertexIndex, vec3_t xyz);

	void freyjaVertexWeight(index_t index, vec_t weight, index_t bone);
	/*------------------------------------------------------
	 * Pre  : <weight> of influence of <bone> on vertex[<index>]
	 *
	 * Post : Vertex <index> in the model gets weight added	
	 *        to influence list, if there is a weight for the
	 *        corresponding bone it is replaced
	 *
	 *        <weight> <= 0.0 removes weight	
	 *
	 *        All weights for the vertex combined must be 1.0
	 ------------------------------------------------------*/

	index_t freyjaGetVertexPolygonRefIndex(index_t vertexIndex, uint32 element);

	uint32 freyjaGetVertexPolygonRefCount(index_t vertexIndex);

	void freyjaGetVertexTexcoord2fv(index_t vertexIndex, vec2_t uv);

	void freyjaGetVertexNormalXYZ3fv(index_t vertexIndex, vec3_t nxyz);

	void freyjaGetVertexXYZ3fv(index_t vertexIndex, vec3_t xyz);

	void freyjaGetVertexFrame(index_t vertexIndex, uint32 element,
							  index_t *frameIndex, vec3_t xyz);

	uint32 freyjaGetVertexFrameCount(index_t vertexIndex);

	void freyjaGetVertexWeight(index_t vertexIndex, uint32 element,
							   index_t *bone, vec_t *weight);

	uint32 freyjaGetVertexWeightCount(index_t vertexIndex);

	uint32 freyjaGetVertexFlags(index_t vertexIndex);

	vec3_t *freyjaGetVertexXYZ(index_t vertexIndex);

	vec2_t *freyjaGetVertexUV(index_t vertexIndex);


	////////////////////////////////////////////////////////////////
	// Skeleton
	//
	////////////////////////////////////////////////////////////////

	index_t freyjaSkeletonCreate();

	void freyjaSkeletonDelete(index_t skeletonIndex);

	index_t freyjaGetCurrentSkeletonIndex();

	// Skeleton accessors /////////////////

	uint32 freyjaGetSkeletonBoneCount(index_t skeletonIndex);

	index_t freyjaGetSkeletonBoneIndex(index_t skeletonIndex, int32 element);

	uint32 freyjaGetSkeletonRootIndex(index_t skeletonIndex);


	////////////////////////////////////////////////////////////////
	// Bone
	//
	////////////////////////////////////////////////////////////////

	index_t freyjaBoneCreate(index_t skeletonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new rest bone for skeleton
	 *        Returns valid Index or -1 on Error
	 ------------------------------------------------------*/

	void freyjaIsBoneDelete(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	char freyjaIsBoneAllocated(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	// Bone mutators /////////////////

	void freyjaBoneTransform(index_t boneIndex, 
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaBoneAddKeyFrame(index_t boneIndex, index_t frameIndex, 
								vec_t time, vec3_t translate, vec3_t rotate);
	/*------------------------------------------------------
	 * Pre  : frameIndex - used for virtual grouping 
	 *        time - time to next frame
	 *        translate, rotate - transform change from rest frame
	 *
	 * Post : Appends a simple keyframe transition to bone
	 *        This call isn't API 10.0 compatible
	 ------------------------------------------------------*/

	void freyjaBoneAddVertex(index_t boneIndex, index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets a strong weight in vertex for this bone
	 ------------------------------------------------------*/

	void freyjaBoneRemoveVertex(index_t boneIndex, index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Removes weights in vertex for this bone
	 ------------------------------------------------------*/

	void freyjaBoneFlags1i(index_t boneIndex, uint32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone flags
	 ------------------------------------------------------*/

	void freyjaBoneParent1i(index_t boneIndex, index_t parentIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone parent	
	 *
	 *        This doesn't affect skeleton, a follow up
	 *        call to freyjaBoneAddChild1i is needed after
	 *        all bones in skeleton are allocated
	 ------------------------------------------------------*/

	void freyjaBoneName1s(index_t boneIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set human readable bone name
	 ------------------------------------------------------*/

	void freyjaBoneRemoveMesh1i(index_t boneIndex, index_t meshIndex);
	void freyjaBoneAddMesh1i(index_t boneIndex, index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Mesh is added to Bone's child list
	 *
	 *        Either makes mesh tree connection or
	 *        simulates by vertex weights and pivots, so
	 *        if you want to use skinning don't use these
	 ------------------------------------------------------*/

	void freyjaBoneRemoveChild1i(index_t boneIndex, index_t childIndex);
	void freyjaBoneAddChild1i(index_t boneIndex, index_t childIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Child is added to Bone's child list
	 *
	 ------------------------------------------------------*/

	void freyjaBoneTranslate3f(index_t boneIndex, vec_t x, vec_t y, vec_t z);
	void freyjaBoneTranslate3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone relative position to parent
	 ------------------------------------------------------*/

	void freyjaBoneRotateEulerXYZ3f(index_t boneIndex,
									vec_t x, vec_t y, vec_t z);
	void freyjaBoneRotateEulerXYZ3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone orientation
	 ------------------------------------------------------*/

	void freyjaBoneRotateQuatWXYZ4f(index_t boneIndex,
									vec_t w, vec_t x, vec_t y, vec_t z);
	void freyjaBoneRotateQuatWXYZ4fv(index_t boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone orientation
	 ------------------------------------------------------*/

	void freyjaBonePosition3fv(index_t boneIndex, vec3_t xyz); // Not Implemented
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set bone absolute position using IK solver 
	 ------------------------------------------------------*/


	// Bone accessors /////////////////

	const char *freyjaGetBoneName1s(index_t boneIndex);
	void freyjaGetBoneName(index_t boneIndex, uint32 size, char *name);
	/*------------------------------------------------------
	 * Pre  : <name> must be allocated to <size> width
	 *        A <size> of 64 is recommended
	 *
	 * Post : Gets bone[index]'s name as '\0' terminated string
	 *        Returns FREYJA_PLUGIN_ERROR on error
	 ------------------------------------------------------*/

	index_t freyjaGetBoneParent(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns bone[index]'s parent id
	 *        Returns INDEX_INVALID if there is no parent
	 ------------------------------------------------------*/

	void freyjaGetBoneRotationQuatWXYZ4fv(index_t boneIndex, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation as a Quaternion
	 ------------------------------------------------------*/

	void freyjaGetBoneRotationEulerXYZ3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s orientation in Euler angles
	 ------------------------------------------------------*/

	void freyjaGetBoneTranslation3fv(index_t boneIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets bone[index]'s relative position
	 ------------------------------------------------------*/

	index_t freyjaGetBoneSkeletalBoneIndex(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns local skeletal index of this bone
	 ------------------------------------------------------*/

	index_t freyjaGetBoneChild(index_t boneIndex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Iterator for bone children
	 ------------------------------------------------------*/

	uint32 freyjaGetBoneChildCount(index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Count of bone children
	 ------------------------------------------------------*/

	void freyjaGetBone16fv(index_t boneIndex, matrix_t mat); // Not Implemented
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get bone's absolute position and rotation 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Mesh
	//
	////////////////////////////////////////////////////////////////

	index_t freyjaMeshCreate();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index of mesh spawned
	 ------------------------------------------------------*/

	index_t freyjaGetCurrentMesh();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns index using meshIndex state 
	 ------------------------------------------------------*/

	void freyjaMeshDelete(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : Mesh <meshIndex> exists
	 * Post : Mesh is removed from mesh pool
	 ------------------------------------------------------*/

	void freyjaMeshTransform(index_t meshIndex, uint32 frame,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);

	void freyjaMeshFrameTransform(index_t meshIndex, uint32 frame,
									freyja_transform_action_t action, 
									vec_t x, vec_t y, vec_t z);

	void freyjaMeshClampTexCoords(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Makes sure all UVs are inside 0,0 - 1,1
	 ------------------------------------------------------*/

	void freyjaMeshMaterial(index_t meshIndex, index_t materialIndex);

	void freyjaMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz);

	void freyjaGetMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz);

	void freyjaMeshPromoteTexcoordsToPloymapping(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Takes vertex UVs and promotes them to polymapped
	 *        texcoords ( stored in polygon ala texture polygons )
	 ------------------------------------------------------*/

	void freyjaMeshAddPolygon(index_t meshIndex, index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaMeshRemovePolygon(index_t meshIndex, index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Removes references to polygonIndex in mesh, but
	 *        it doesn't free the allocated polygon
	 ------------------------------------------------------*/

	void freyjaMeshPolygonSplitTexCoords(index_t meshIndex,
                                         index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : All TexCoords are duplicated, then references to old
	 *        TexCoords are removed -- this is useful for making a single
	 *        polymapped texture polygon when all others will remain
	 *        in a UV Mesh
	 ------------------------------------------------------*/

	void freyjaMeshUVMapPlanar(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Planar mapping algorithm
	 ------------------------------------------------------*/

	void freyjaMeshUVMapSpherical(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Spherical mapping algorithm
	 ------------------------------------------------------*/

	void freyjaMeshUVMapCylindrical(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Texcoords computed with Cylindrical mapping algorithm
	 ------------------------------------------------------*/

	void freyjaMeshTesselateTriangles(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Divides all polygons in mesh into triangles
	 ------------------------------------------------------*/

	void freyjaMeshNormalFlip(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Flips all vertex normals in mesh
	 ------------------------------------------------------*/

	void freyjaMeshGenerateVertexNormals(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : meshIndex references a valid mesh
	 * Post : Recalculates all vertex normals in mesh
	 ------------------------------------------------------*/

	void freyjaMeshPosition(index_t meshIndex, vec3_t xyz);

	void freyjaMeshName1s(index_t meshIndex, const char *name);

	void freyjaMeshFlags1u(uint32 flags);
	/*------------------------------------------------------
	 * Pre  : Pass valid freyja_mesh_flags_t's bitmap
	 * Post : Sets flags for current mesh
	 ------------------------------------------------------*/



	// Mesh accessors //////////////

	char freyjaIsMeshAllocated(index_t meshIndex);

	void freyjaGetMeshFrameBoundingBox(index_t meshIndex, 
										uint32 frame, vec3_t min, vec3_t max);

	uint32 freyjaGetMeshFlags(index_t meshIndex);

	void freyjaGetMeshPosition(index_t meshIndex, vec3_t xyz);

	const char *freyjaGetMeshNameString(index_t meshIndex);

	void freyjaGetMeshName1s(index_t meshIndex, uint32 lenght, char *name);

	uint32 freyjaGetMeshTexCoordCount(index_t meshIndex);

	index_t freyjaGetMeshTexCoordIndex(index_t meshIndex, index_t element);

	index_t freyjaGetMeshVertexIndex(index_t meshIndex, index_t element);
	/*------------------------------------------------------
	 * Pre  : freyjaGetMeshVertexCount must be called before
	 *        this is valid while using the Egg backend
	 *
	 * Post : Returns the index of the local vertex <element>
	 *        for mesh <meshIndex> or -1 on error
	 ------------------------------------------------------*/

	uint32 freyjaGetMeshVertexCount(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of local vertices
	 *        for mesh <meshIndex> or 0 on error
	 *
	 ------------------------------------------------------*/

	index_t freyjaGetMeshPolygonVertexIndex(index_t meshIndex,
                                            index_t faceVertexIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the index of local vertices
	 *        for mesh <meshIndex> with local
	 *        mapping from polygon to mesh ids or -1 on error
	 *
	 ------------------------------------------------------*/

	// 'free vertices' outside of polygons as well as 'grouped' vertices
	void freyjaMeshVertexGroupAppendGobalVertex(index_t meshIndex, uint32 element, 
												index_t vertexIndex);
	uint32 freyjaGetMeshVertexGroupVertexCount(index_t meshIndex, uint32 element);
	index_t freyjaGetMeshVertexGroupVertexIndex(index_t meshIndex, uint32 element,
											  uint32 vertexElement);

	index_t freyjaGetMeshPolygonIndex(index_t meshIndex, uint32 element);

	uint32 freyjaGetMeshPolygonCount(index_t meshIndex);

	index_t freyjaGetMeshVertexGroupIndex(index_t meshIndex, uint32 element);

	uint32 freyjaGetMeshVertexGroupCount(index_t meshIndex);

	index_t freyjaGetMeshVertexFrameIndex(index_t meshIndex, uint32 element); // Not Implemented
	uint32 freyjaGetMeshVertexFrameCount(index_t meshIndex); // Not Implemented


	///////////////////////////////////////////////////////////////////////
	// Polygon
	///////////////////////////////////////////////////////////////////////

	index_t freyjaPolygonCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Makes invalid/empty polygon to be filled, returns index
	 ------------------------------------------------------*/

	void freyjaPolygonDelete(index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Removes polygon 
	 ------------------------------------------------------*/

	char freyjaIsPolygonAllocated(index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns 1 if exists
	 ------------------------------------------------------*/

	void freyjaPolygonSplit(index_t meshIndex, index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *
	 * Post : Polygon is split into 2 smaller polygons 
	 ------------------------------------------------------*/

	void freyjaPolygonTexCoordPurge(index_t polygonIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *
	 * Post : All polymapped texcoords are dereferenced
	 ------------------------------------------------------*/

	void freyjaPolygonExtrudeQuad1f(index_t polygonIndex, vec_t dist);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by face normal scaled by dist
	 ------------------------------------------------------*/

	void freyjaPolygonExtrudeQuad(index_t polygonIndex, vec3_t normal);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 *        the 'normal' is the vector you wish to follow with extrude
	 *
	 * Post : Adds a quad where every edge is on this face by
	 *        extruding by 'normal'
	 ------------------------------------------------------*/

	void freyjaPolygonAddVertex1i(index_t polygonIndex, index_t vertexIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a vertex to a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonAddTexCoord1i(index_t polygonIndex, index_t texcoordIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Adds a texcoord to a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonSetMaterial1i(index_t polygonIndex, index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Polygon polygonIndex exists
	 * Post : Sets material for a polygon
	 ------------------------------------------------------*/

	void freyjaPolygonSmoothingGroup(index_t smoothingGroupIndex); // NOT IMPLEMENTED
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assigns face to smoothing group
	 ------------------------------------------------------*/

	index_t freyjaGetPolygonMaterial(index_t polygonIndex);

	uint32 freyjaGetPolygonFlags(index_t polygonIndex);

	uint32 freyjaGetPolygonEdgeCount(index_t polygonIndex);

	uint32 freyjaGetPolygonVertexCount(index_t polygonIndex);

	uint32 freyjaGetPolygonTexCoordCount(index_t polygonIndex);

	index_t freyjaGetPolygonVertexIndex(index_t polygonIndex, uint32 element);

	index_t freyjaGetPolygonTexCoordIndex(index_t polygonIndex, uint32 element);


	///////////////////////////////////////////////////////////////////////
	// Animation ( 0.9.3 ABI, Can't be used with freyjaIterators )
	///////////////////////////////////////////////////////////////////////

	index_t freyjaAnimationCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new Freyja Animation object
	 *        Returns the new Animation's index or -1 on error
	 *
	 * NOTES: The new skeletal animation system allows for
	 *        sharing (subset) animations across multiple
	 *        skeletons using name matching to assure the
	 *        skeletons match. 
	 ------------------------------------------------------*/

	index_t freyjaAnimationBoneCreate(index_t animationIndex,
								      const char *name, index_t boneIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Creates a new Freyja Animation Bone object
	 *        Returns the new Bone's index or -1 on error
	 ------------------------------------------------------*/

	index_t freyjaAnimationBoneKeyFrameCreate(index_t animationIndex,
										   index_t boneIndex,
										   vec_t time, vec3_t xyz, vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 *
	 * Post : Returns the keyframe's local Animation's Bone's
	 *        Keyframe element index or -1 on error
	 ------------------------------------------------------*/


	/* Animation Accessors */

	uint32 freyjaGetAnimationCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of Animations being managed
	 ------------------------------------------------------*/

	uint32 freyjaGetAnimationFrameCount(index_t animationIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Returns the number of frames or -1 on error
	 ------------------------------------------------------*/

	// FIXME: This is still using fused Animation/Skeleton
	uint32 freyjaGetAnimationBoneCount(int32 skeletonIndex);
	/*------------------------------------------------------
	 * Pre  : Skeleton <skeletonIndex> exists
	 * Post : Returns the number of bones or -1 on error
	 ------------------------------------------------------*/

	uint32 freyjaGetAnimationBoneKeyFrameCount(index_t animationIndex, 
											 int32 boneIndex);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 *
	 * Post : Returns the number of keyframes or -1 on error
	 ------------------------------------------------------*/

	// Finish me


	/* Animation Mutators */

	void freyjaAnimationName(index_t animationIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : Sets human readable animation name
	 ------------------------------------------------------*/

	void freyjaAnimationBoneName(index_t animationIndex, int32 boneIndex,
								 const char *name);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 *        Animation Bone <boneIndex> exists
	 * Post : Sets human readable animation name
	 ------------------------------------------------------*/

	void freyjaAnimationFrameRate(index_t animationIndex, vec_t frameRate);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaAnimationTime(index_t animationIndex, vec_t time);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaAnimationSubsetRoot(index_t animationIndex, int32 startBone);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaAnimationKeyFrameTime(index_t animationIndex, int32 boneIndex,
									 index_t keyframeIndex, vec_t time);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets time for <keyFrameIndex> keyframe in animation
	 ------------------------------------------------------*/

	void freyjaAnimationKeyFramePosition(index_t animationIndex, int32 boneIndex, 
										 index_t keyframeIndex, vec3_t position);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's position in animation
	 ------------------------------------------------------*/

	void freyjaAnimationKeyFrameOrientationXYZ(index_t animationIndex,
											   int32 boneIndex, 
											   index_t keyframeIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's orienation in animation
	 *        by Euler angles
	 ------------------------------------------------------*/

	void freyjaAnimationKeyFrameOrientationWXYZ(index_t animationIndex,
												int32 boneIndex,
												index_t keyframeIndex,vec4_t wxyz);
	/*------------------------------------------------------
	 * Pre  : Animation <animationIndex> and <keyFrameindex> exist
	 * Post : Sets <keyFrameIndex> keyframe's orienation in animation
	 *        by Quaternion ( note the w, xyz order )
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Texture ( 0.9.3 ABI, Can't be used with freyjaIterators )
	///////////////////////////////////////////////////////////////////////

	index_t freyjaTextureCreateFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new index or INDEX_INVALID
	 ------------------------------------------------------*/

	index_t freyjaTextureCreateBuffer(byte *image, uint32 byteDepth,
                                      uint32 width, uint32 height,
                                      freyja_colormode_t type);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new index or INDEX_INVALID
	 ------------------------------------------------------*/

	void freyjaTextureDelete(index_t textureIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deletes texture given any valid index 
	 ------------------------------------------------------*/

	void freyjaGetTextureImage(index_t textureIndex,
                               uint32 *w, uint32 *h, uint32 *bitDepth,  
                               uint32 *type, byte **image);
	/*------------------------------------------------------
	 * Pre  : Do not attempt to alter <image> on return
	 *
	 * Post : Gets pointers to texture[index]'s data members
	 *        Returns 0 on success.
	 ------------------------------------------------------*/

	uint32 freyjaGetTexturePoolCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of texture slots
	 ------------------------------------------------------*/

	uint32 freyjaGetTextureCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of texture slots used
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Material ( 0.9.3 ABI, Can't be used with freyjaIterators )
	///////////////////////////////////////////////////////////////////////

	index_t freyjaMaterialCreate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new Freyja Material object
	 *        Returns the new Material's index or -1 on error
	 ------------------------------------------------------*/

	index_t freyjaGetCurrentMaterial();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaCurrentMaterial(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	/* Material Accessors */

	uint32 freyjaGetMaterialCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of Materials being managed
	 ------------------------------------------------------*/

	index_t freyjaGetMaterialIndex(index_t materialIndex, uint32 element);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns the gobal object index ( from the local
	 *        Material element index ) or -1 on error
	 ------------------------------------------------------*/

	const char *freyjaGetMaterialName(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 *        Don't alter the returned string
	 *
	 * Post : Returns a pointer to NULL terminated name string
	 *        Returns 0x0 on error
	 ------------------------------------------------------*/

	uint32 freyjaGetMaterialFlags(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns flags or -1 on error
	 ------------------------------------------------------*/

	index_t freyjaGetMaterialTexture(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns texture index or -1 for error or no texture
	 ------------------------------------------------------*/

	const char *freyjaGetMaterialTextureName(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaGetMaterialAmbient(index_t materialIndex, vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <ambient> color
	 ------------------------------------------------------*/

	void freyjaGetMaterialDiffuse(index_t materialIndex, vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <diffuse> color
	 ------------------------------------------------------*/

	void freyjaGetMaterialSpecular(index_t materialIndex, vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <specular> color
	 ------------------------------------------------------*/

	void freyjaGetMaterialEmissive(index_t materialIndex, vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns <emissive> color
	 ------------------------------------------------------*/

	vec_t freyjaGetMaterialShininess(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns specular exponent or -1.0 on error
	 ------------------------------------------------------*/

	vec_t freyjaGetMaterialTransparency(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns transparency or -1.0 on error
	 ------------------------------------------------------*/

	int32 freyjaGetMaterialBlendSource(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns blend source factor or -1 on error
	 ------------------------------------------------------*/

	int32 freyjaGetMaterialBlendDestination(index_t materialIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Returns blend destination factor or -1 on error
	 ------------------------------------------------------*/


	/* Material Mutators */

	int32 freyjaLoadMaterialASCII(index_t materialIndex, const char *filename);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material is reset by loaded disk file
	 ------------------------------------------------------*/

	void freyjaMaterialName(index_t materialIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <name> id is set
	 ------------------------------------------------------*/

	void freyjaMaterialClearFlag(index_t materialIndex, uint32 flag);
	void freyjaMaterialSetFlag(index_t materialIndex, uint32 flag);
	void freyjaMaterialFlags(index_t materialIndex, uint32 flags);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's bit <flags> are set
	 ------------------------------------------------------*/

	void freyjaMaterialTextureName(index_t materialIndex, const char *name);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material textures's <name> id is set
	 ------------------------------------------------------*/

	void freyjaMaterialTexture(index_t materialIndex, index_t textureIndex);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <texture> index is set
	 ------------------------------------------------------*/

	void freyjaMaterialAmbient(index_t materialIndex, const vec4_t ambient);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <ambient> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialDiffuse(index_t materialIndex, const vec4_t diffuse);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <diffuse> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialSpecular(index_t materialIndex, const vec4_t specular);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <specular> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialEmissive(index_t materialIndex, const vec4_t emissive);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's <emissive> color is set
	 ------------------------------------------------------*/

	void freyjaMaterialShininess(index_t materialIndex, vec_t exponent);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's Specular <exponent> is set
	 ------------------------------------------------------*/

	void freyjaMaterialTransparency(index_t materialIndex, vec_t transparency);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 *        <transparency> is a value from 0.0 to 1.0
	 *
	 * Post : Material's <transparency> is set
	 ------------------------------------------------------*/

	void freyjaMaterialBlendSource(index_t materialIndex, uint32 factor);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's blend source <factor> is set
	 ------------------------------------------------------*/

	void freyjaMaterialBlendDestination(index_t materialIndex, uint32 factor);
	/*------------------------------------------------------
	 * Pre  : Material <materialIndex> exists
	 * Post : Material's blend destination <factor> is set
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// Freyja plugin subsystem
	//
	///////////////////////////////////////////////////////////////////////

	void freyjaPluginDirectoriesInit();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets default plugin directories.
	 ------------------------------------------------------*/

	void freyjaPluginsInit();
	/*------------------------------------------------------
	 * Pre  : freyjaPluginDirectoriesInit() is called, or you've
	 *        set some custom paths with freyjaPluginAddDirectory(...)
	 * Post : Scans plugin directories and sets up newly found plugins.
	 ------------------------------------------------------*/

	void freyjaPluginAddDirectory(const char *dir);
	/*------------------------------------------------------
	 * Pre  : <dir> is valid string representing valid directory
	 * Post : Directory is added to runtime linked library
	 *        search path list.
	 ------------------------------------------------------*/

	void freyjaPluginDescription(uint32 pluginIndex, const char *info_line);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginImportFlags(uint32 pluginIndex, int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExportFlags(uint32 pluginIndex, int32 flags);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void freyjaPluginExtention(uint32 pluginIndex, const char *ext);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 freyjaGetPluginCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
	

	///////////////////////////////////////////////////////////////////////
	//  Plugin import/export iteraction
	///////////////////////////////////////////////////////////////////////

	int32 freyjaGetPluginId();

	int freyjaGetPluginArg1f(int32 pluginId, const char *name, float *arg);

	int freyjaGetPluginArg1i(int32 pluginId, const char *name, int32 *arg);

	int freyjaGetPluginArg1s(int32 pluginId, const char *name, char **arg);

	int freyjaGetPluginArgString(int32 pluginId, const char *name, 
								 int32 len, char *arg);


	///////////////////////////////////////////////////////////////////////
	//  Pak VFS 
	///////////////////////////////////////////////////////////////////////

	index_t freyjaPakBegin(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Starts a new VFS from a 'pak file'
	 *        Returns vfs index
	 ------------------------------------------------------*/

	void freyjaPakAddDecoderFunction2s(const char *module, const char *symbol);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Used to decrypt or uncompress files in a pak
	 *        using an external module and one of it's
	 *        C accessable functions using freyjaPak ABI.
	 *
	 *        NOT IMPLEMENTED!
	 ------------------------------------------------------*/

	void freyjaPakAddFullPathFile(index_t pakIndex,
								  const char *vfsFilename,
								  int32 offset, int32 size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Adds a new entry to VFS mapping a chunk from
	 *        offset to offset+size as a file named vfsFilename
	 *
	 *        Returns 1 if it is successfully 'loaded'
	 ------------------------------------------------------*/

	void freyjaPakEnd(index_t pakIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Finalizes VFS for pakIndex
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////////
	// Util
	//
	////////////////////////////////////////////////////////////////

	void freyjaGenerateQuadPlaneMesh(vec3_t origin, vec_t side);

	void freyjaGenerateQuadCubeMesh(vec3_t origin, vec_t side);

	void freyjaGenerateCircleMesh(vec3_t origin, int32 count); // radius

	void freyjaGenerateConeMesh(vec3_t origin, vec_t height, int32 count); // radius

	void freyjaGenerateCylinderMesh(vec3_t origin, vec_t height, 
								int32 count, int32 segments); // radius

	void freyjaGenerateSphereMesh(vec3_t origin, vec_t radius, 
							  int32 count, int32 segments);

	void freyjaGenerateTubeMesh(vec3_t origin, vec_t height, 
							int32 count, int32 segments); // radius


	void freyjaGenerateUVFromXYZ(vec3_t xyz, vec_t *u, vec_t *v);


	///////////////////////////////////////////////////////////////////////
	// Internal ABI calls
	//
	//   If used externally you'll likely get a lot of breakage or
	//   slower and possibly incorrect object states.
	//
	//   Only use this if you're a core developer writing
	//   special test plugins, or internal code.
	///////////////////////////////////////////////////////////////////////

	void freyja__MeshUpdateMappings(index_t meshIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates Egg backend egg_mesh_t to simulate
	 *        FreyjaMesh local vertex mappings
	 ------------------------------------------------------*/
}


/* Mongoose 2004.12.19, 
 * C++ fun */
void freyjaVertexListTransform(Vector<uint32> &list,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z);

void freyjaModelMirrorTexCoord(uint32 modelIndex, uint32 texCoordIndex,
								Vector<int32> uvMap, bool x, bool y);

char freyjaModelCopyVertexList(index_t modelIndex, 
							   Vector<unsigned int> &list,
							   int mesh, int frame);
/*------------------------------------------------------
 * Pre  : Model <modelIndex> exists and has an active copy buffer
 * Post : Model's copy buffer is modified to include new mesh duplicate
 *        using the *local <mesh> index and vertex morph <frame>
 ------------------------------------------------------*/

int32 freyjaFindPolygonByVertices(Vector<uint32> vertices);

Vector<unsigned int> *freyjaFindVerticesByBox(vec3_t bbox[2]);

void freyjaGetVertexPolygonRef1i(index_t vertexIndex, Vector<long> &polygons);
void freyjaGetVertexPolygonRef(Vector<long> &polygons);
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns a list of indices of polygons that
 *        use current vertex in iterator
 *
 *        List can be empty
 *
 *-- History ------------------------------------------
 *
 * 2004.12.17:
 * Mongoose - Created, wrapper for old Egg style
 *            reverse reference system ( very handy )
 ------------------------------------------------------*/


#endif
