/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja, GooseEgg
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : mongoose@users.sourceforge.net
 * Object  : Egg
 * License : GPL, also (C) 2000-2004 Mongoose
 * Comments: This is the data model agent of Egg models
 *
 * 
 *-- Test Defines ----------------------------------------------------
 *           
 * UNIT_TEST_EGG  - Builds module as test code as a console program
 *
 *-- History --------------------------------------------------------- 
 *
 * 2004.08.12:
 * Mongoose - Eggv9 implementation hammered out, now I have some free time
 *
 * 2004.04.08:
 * Mongoose - All new API using Hel math/physics lib, endian safe
 *            file I/O, and generic methods.
 *
 *            Metadata chunk/type back
 *
 *            Removed a ton of code.
 *
 *            Renamed texels to texcoords, since they haven't been
 *            texels for several years now.
 *
 * 2004.03.15:
 * Mongoose - Replaced List collection use with Vector
 *            Some coding style changes to remove mismatch
 *            This is a bigger deal than it seems  =)
 *
 * 2002.08.31:
 * Mongoose - Introducing simple vectors to replace Lists and
 *            the Chain prototype collections.
 *
 *            Should reduce memory requirements and improve speed
 *
 * 2002.07.05:
 * Mongoose - Refactoring and rewritten to conform to new code style
 *
 *            The reason I do this IS to break the API for testing
 *            if you don't like it make an adpater
 *
 *            Removed marker system ( No longer used by anyone? )
 *
 * 2001.07.05:
 * Mongoose - New mtk3d API and other major changes started
 *
 * 2001-04-14:
 * Mongoose - Finishing rollover from EggV7 to EggV8 
 *
 * 2001-01-31:
 * Mongoose - Composite Eggv* starts, using pieces of 
 *            tracers ( prototypes ) and reformed to the
 *            EggV8 001 (20010101) spec.  
 *
 *            Also new major egg releases include major number 
 *            in classname, to ease transitions of other projects.
 *
 *            Removal of internal shaders, just id refernce now.
 *
 *            New focus on documentation, to aid other projects
 *            using this module.
 *
 * 2000-12-09:
 * Mongoose - Move to new data structures
 *
 * 2000-11-20:
 * Mongoose - Implementation of v8 design ( Freyja backend )
 *
 * 2000-10-16:
 * Mongoose - Documentation addtion and interface changes
 *
 * 2000-10-07:
 * Mongoose - Frame, mesh, and interface changes
 *
 * 2000-09-15:
 * Mongoose - API change!  Transforms have 1 new API call
 *            The 3 old methods per mesh, frame, list are gone
 ===========================================================================*/

#ifndef GUARD__MONGOOSE_GOOSEEGG_EGG_H
#define GUARD__MONGOOSE_GOOSEEGG_EGG_H

#include <mstl/Vector.h>
#include <mstl/Map.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>

#include "EggFileReader.h"
#include "EggFileWriter.h"


class EggChunk
{
public:

	/*** Public Accessors *********************************/

	virtual bool saveFile(EggFileWriter &w) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Saves a model to disk file
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.08.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual bool saveTextFile(EggFileWriter &w) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Saves a model to disk file as UTF-8 text
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	/*** Public Mutators ***********************************/

	virtual bool loadFile(EggFileReader &r) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads a model from a disk file
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.08.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual bool loadTextFile(EggFileReader &r) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads a model from a UTF-8 text disk file
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	unsigned int id;                 /* Unique identifier */

	unsigned int chunkHeader;        /* This is used to identify a chunk 
									  * in binary mode */

	unsigned int chunkFooter;        /* Used for checking chunk reads */
};


/* Likey to replace with EggImage*/
class TextureData
{
public:
	char *name;                /* Texture name */
	
	char *filename;
	
	unsigned char *image;      /* RGB(A) Texture data */
	
	unsigned int imageWidth;
	
	unsigned int imageHeight;
	
	unsigned char mipmaps;
	
	unsigned char pixelDepth; /* 3 - RGB24bit, 4 - RGBA32bit */
	
	unsigned int id;          /* OpenGL texture id use */
};


class Material
{
public:
	Vector<unsigned int> textures;    /* Texture indices used by material */

	char *name;                /* Material name */

	vec4_t ambient;            /* Ambient color */

	vec4_t diffuse;            /* Diffuse color */

	vec4_t specular;           /* Specular color */

	vec4_t emissive;           /* Emissive color */

	vec_t shininess;           /* Specular exponent */

	unsigned int blend_src;    /* Blend source factor */

	unsigned int blend_dest;   /* Blend destination factor */

	int parent;                /* Linked material id */

	int children;              /* Linked materials count */

	bool hasAlphaChannel;      /* For depth sorting use */

private:
	unsigned int id;                 /* Unique identifier */

	static unsigned int id_counter;  /* Id system use */
};


class MetaData /* Useful for storing ASCII files as strings, textures, etc */
{
public:
	unsigned int id;                  /* Unique identifier */

	unsigned int type;
	unsigned int size;
	char *symbol;
	char *description;
	void *data;
};


class Weight
{
public:
	vec_t weight;                     /* Weight for vertex use */
	unsigned int bone;                /* Bone id */
};


class VertexWeight
{
public:
	Vector <Weight *> weights;
};


class Polygon
{
public:

	unsigned int flags;

	unsigned int id;                  /* Unique identifier */

	int material;                     /* Material id, if (mat != mesh.mat)
									   * to support multimaterial meshes */

	Vector <unsigned int> vertices;   /* Vertices composing polygon */

	Vector <unsigned int> texcoords;  /* If non-empty this polygon uses
									   * it's own texcoords in place of
									   * vertices's texcoords (polymapped) */
};


class VertexGroup
{
public:
	int id;                           /* Unique identifier */

	Vector <unsigned int> vertex;     /* Vertices composing group */
	Vector3d bboxMin;                 /* Min corner of bounding box */
	Vector3d bboxMax;                 /* Max corner of bounding box */

	Vector3d center;                  /* Center of bounding volume */

	vec_t radius;                     /* Radius of bounding sphere if used */
};


class UVMap
{
public:
	void transform();

	Vector <unsigned int> polygons;   /* Contains TexCoords composing group
									   * either polymapped or by vertex */
};


class VertexFrame
{
public:
	void transform();

	Vector<vec_t> vertices;           /* Vertex animation frame */
};


/* Vertex no longer a primative object class/type */
/* Move csg to plugin maybe child class of Mesh, CSGMesh */
class Mesh
{
public:
	void rotate(vec_t x, vec_t y, vec_t z);

	void rotateAboutPoint(vec3_t point, vec_t x, vec_t y, vec_t z);

	void scale(vec_t x, vec_t y, vec_t z);

	void transform();

	void translate(vec_t x, vec_t y, vec_t z);

	bool combineTexcoords(unsigned int a, unsigned int b)
	{
		Polygon *polygon;
		unsigned int i;


		// Make all polygons referencing A point to B
		for (i = polygons.begin(); i < polygons.end(); ++i)
		{
			polygon = polygons[i];

			if (polygon && polygon->texcoords[i] == a)
				polygon->texcoords.assign(i, b);
		}

		// Mark A as unused in the texcoord array reference
		//usedTexCoord.pushBack(a);

		return true;
	}

	bool combineVertices(unsigned int a, unsigned int b)
	{
		Polygon *polygon;
		unsigned int i;


		// Make all polygons referencing A point to B
		for (i = polygons.begin(); i < polygons.end(); ++i)
		{
			polygon = polygons[i];

			if (polygon && polygon->vertices[i] == a)
				polygon->vertices.assign(i, b);
		}

		// Mark A as unused in the texcoord array reference
		//usedVertex.pushBack(a);

		// Don't bother touching weights, they aren't managed

		return true;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Combines object A and B in model
	 *
	 *        Destroys A and B then replaces them with new 
	 *        object with index A where: A = A + B
	 *
	 *        Returns true on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.04:
	 * Mongoose - Hard ABI back
	 *
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2000.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool combineTexcoords(unsigned int a, unsigned int b);
	bool combineVertices(unsigned int a, unsigned int b);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Combines object A and B in model
	 *
	 *        Destroys A and B then replaces them with new 
	 *        object with index A where: A = A + B
	 *
	 *        Returns true on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.04:
	 * Mongoose - Hard ABI back
	 *
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2000.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	Mesh *csgUnion(Mesh *a, Mesh *b);
	Mesh *csgIntersection(Mesh *a, Mesh *b);
	Mesh *csgDifference(Mesh *a, Mesh *b);
	//unsigned int csg(egg_type_t type, egg_csg_t operation,
	//				 unsigned int a, unsigned int b);
	/*------------------------------------------------------
	 * Pre  : Don't count on more than simple vertex culling now
	 *
	 * Post : OPERATION on TYPE object A and B in model
	 *
	 *        A and B are perserved, creates new object C 
	 *        where: C = A OPERATION B
	 *
	 *        UNION        : C = A u B
	 *                       if A eq B then copy of A is made
	 *
	 *        INTERSECTION : C = A n B
	 *                       if A eq B then copy of A is made
	 *
	 *        DIFFERENCE   : C = A - B
	 *                       B culls A, order matters
	 *                       if A eq B then undefined behavior
	 *
	 *        Returns C's index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - Created with new generic API based on mtk
	 ------------------------------------------------------*/

	Vector<Polygon *> polygons;        /* Polygons of this mesh */

	Vector<VertexFrame *> frames;       /* Vertex morph frames */

	Vector<Material *> materials;       /* Material data */

	Vector<UVMap *> uvmaps;             /* UVMaps of this mesh */

	Vector<VertexWeight *> weights;     /* Vertex weights */

	Vector <unsigned int> groups;       /* Vertex groups of this mesh */

	// Could make a class/struct to hold full references to polygons, etc
	// instead of reference counting via scoring invalid ids
	Vector <unsigned int> refVertices;  /* Vertex pool helper, tracks use */   

	Vector<vec_t> vertices;             /* Vertex array, could be replaced 
										 * with VertexFrame's vertices */

	Vector<vec_t> texcoords;            /* Texcoord array */

	Vector<vec_t> normals;              /* Normal array */

	Vector3d center;

	unsigned int id;                    /* Unique identifier */

	unsigned int texcoordDepth;         /* 1d, 2d, 3d */

	int material;                       /* Base material id */

	unsigned int currentFrame;          /* Current vertex morph frame */
};


class Bone 
{
public:
	void translate();
	void rotate();

	unsigned int id;                 /* Unique identifier */

	char name[64];                   /* Human readable identifier */

	int parent;                      /* Parent of this bone */

	Vector <unsigned int> children;  /* Children bones */

	Vector <unsigned int> meshes;    /* Meshes are bound to bone if meshtree */

	Matrix matrix;                   /* Cached matrix */
	Matrix transform;                /* Transform mesh/children */
};


class Skeleton
{
public:
	int id;                           /* Unique identifier */

	Vector<Bone *> bones;             /* Bones in this skeleton */
	Vector<Weight *> weights;         /* Weights for skeleton use */

	unsigned int root;                /* Root bone index */
	Vector3d center;                  /* Position */
};



class Animation
{
public:
	int id;                           /* Unique identifier */

	unsigned int frameRate;
	unsigned int currentFrame;
	unsigned int lastFrame;
	vec_t time;
	vec_t lastTime;

	Vector<unsigned int> frames;  /* vertexframes / skeletalframes */
};


class Model
{
public:
	Vector<Animation *> animations;   /* Animation data */

	Vector<Skeleton*> skeletons;      /* Skeletal data for this model */

	Vector<Mesh *> meshes;            /* Geometery structure */

	Vector<MetaData *> metadata;      /* Metadata for external use */

	int id;                           /* Unique identifier */
};


class Egg
{
public:

	typedef enum {
		EGG_VERSION    = 0x30302E39,  /* Library ABI Version '9.00' */
		EGG_MAGIC      = 0x20676745,  /* File header, versioning 'EGG ' */
	} egg_magic_t;

	typedef enum {
		VERTEX     = 0x54524556,
		TEXCOORD   = 0x54455843,
		NORMAL     = 0x4D524F4E,
		WEIGHT     = 0x54484757,
		POLYGON    = 0x504F4C59,
		GROUP      = 0x56475250,
		MESH       = 0x4D455348,
		SKELETON   = 0x534B454C,
		BONE       = 0x424F4E45,
		ANIMATION  = 0x414E494D,
		METADATA   = 0x4D455441
	} egg_type_t;


	typedef enum {
		
		SCALE               = 0, 
		ROTATE              = 1, 
		TRANSLATE           = 2, 
		ROTATE_ABOUT_POINT  = 3,
		SCALE_ABOUT_POINT   = 4
	} egg_transform_t;
	

	typedef enum {
		UNION = 1,
        INTERSECTION,
		DIFFERENCE
	} egg_csg_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Egg();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an Egg object
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~Egg();
	/*------------------------------------------------------
	 * Pre  : This object is allocated
	 * Post : Deconstructs the Egg object
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	
	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void erase();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :  All data is reset and cleared
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.08.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setDebugLevel(unsigned int level);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Debugging is set 0 ( Off ) to 5 ( Highest )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.05:
	 * Mongoose - Now supports multiple levels 
	 *
	 * 2001.01.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

#ifdef FIXME

	unsigned int csg(egg_type_t type, egg_csg_t operation,
					 unsigned int a, unsigned int b);
	/*------------------------------------------------------
	 * Pre  : Don't count on more than simple vertex culling now
	 *
	 * Post : OPERATION on TYPE object A and B in model
	 *
	 *        A and B are perserved, creates new object C 
	 *        where: C = A OPERATION B
	 *
	 *        UNION        : C = A u B
	 *                       if A eq B then copy of A is made
	 *
	 *        INTERSECTION : C = A n B
	 *                       if A eq B then copy of A is made
	 *
	 *        DIFFERENCE   : C = A - B
	 *                       B culls A, order matters
	 *                       if A eq B then undefined behavior
	 *
	 *        Returns C's index
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - Created with new generic API based on mtk
	 ------------------------------------------------------*/

	void remove(egg_type_t type, unsigned int index);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Remove TYPE object[index] from model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2000.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void transform(egg_type_t type, egg_transform_t transform, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.08:
	 * Mongoose - New generic API that supports all types 
	 *            in one method
	 *
	 * 2000.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int loadFile(char *filename);
	/*------------------------------------------------------
	 * Pre  : File is valid format and exists
	 * Post : Loads model from disk file
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.08.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	// Mongoose 2002.07.05, Reorder from here down later... to be broken up

	void addMetaData(unsigned int id, unsigned int type, unsigned int size, 
					 unsigned char *metadata);

	egg_texcoord_t *getTexCoord(unsigned int id);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return texcoord by id
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int addTexCoord(vec_t u, vec_t v, vec_t w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Add a new texcoord, return id
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void delTexCoord(egg_texcoord_t *t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Delete texcoord by data value
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.11.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_vertex_t *getVertex(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex with id
	 -----------------------------------------*/

	egg_vertex_t *addVertex(vec_t x, vec_t y, vec_t z,
							vec_t nx, vec_t ny, vec_t nz,
							vec_t u, vec_t v, vec_t w);
	/*------------------------------------------------------
	 * Pre  : x, y, z is position in 3 space 
	 *        nx, ny, nz is normal vector
	 *        u, v, w is the texcoord coord
	 *
	 * Post : Generates a new vertex and returns a 
	 *        pointer to it
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.07.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	egg_vertex_t *addVertex(vec_t x, vec_t y, vec_t z);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Add a new vertex, return id
	 -----------------------------------------*/

	void delVertex(egg_vertex_t *v);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete vertex by data
	 -----------------------------------------*/

	egg_polygon_t *getPolygon(unsigned int);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return polygon with id
	 -----------------------------------------*/

	unsigned int addPolygon(Vector<unsigned int> &vertex, 
							Vector<unsigned int> &texcoord, 
							int shader);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  : vertex and texcoord lists are valid
	 *        shader/texture id is valid
	 * Post : Add a new polygon, return id
	 -----------------------------------------*/

	unsigned int selectPolygon(Vector<unsigned int> &list);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete polygon by matching 
	 *        vertex list
	 -----------------------------------------*/

	void delPolygon(Vector<unsigned int> &list);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete polygon by matching 
	 *        vertex list
	 -----------------------------------------*/

	void delPolygon(egg_polygon_t *polygon);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete polygon by data
	 -----------------------------------------*/

	void delPolygon(unsigned int polygon);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Delete polygon by id
	 -----------------------------------------*/


	/// GROUPS ////////////////////////////////////

	egg_group_t *getGroup(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : 
	 * Post : Return vertex grouping with id
	 -----------------------------------------*/

	egg_group_t *newGroup();
	/*-----------------------------------------
	 * Created  : 2000-08-15, Mongoose
	 * Modified : 
	 * Comments : This makes a new initialized
	 *            egg_group_t, without linking
	 *            it to the main _group list
	 *            ( id is set to error value )
	 -----------------------------------------*/

	void addGroup(egg_group_t *group);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * Comments : This takes a group and 
	 *            registers it with the egg list
	 -----------------------------------------*/

	void delGroup(unsigned int group);
	void delGroup(egg_group_t *group);

	/// MESHES ////////////////////////////////////

	egg_mesh_t *newMesh();
	/*-----------------------------------------
	 * Created  : 2000-08-15, Mongoose
	 * Modified : 
	 * Comments : This makes a new initialized
	 *            egg_mesh_t, without linking
	 *            it to the main _mesh list
	 *            ( id is set to error value )
	 -----------------------------------------*/ 
  
	void addMesh(egg_mesh_t *mesh);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * Comments : This takes a mesh and 
	 *            registers it with the egg list
	 -----------------------------------------*/

	void delMesh(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : id is a valid mesh id
	 * Post : Removes mesh from model
	 -----------------------------------------*/

	void delMesh(egg_mesh_t *mesh);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : mesh is a valid mesh, that's
	 *        in the list
	 * Post : Removes mesh from model
	 -----------------------------------------*/

	egg_mesh_t *getMesh(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 *
	 * Pre  : id is a valid mesh id
	 * Post : Returns mesh with id from model
	 -----------------------------------------*/


    




	/// TAGS //////////////////////////////////////



	void delTag(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : id is a valid tag id
	 * Post : tag is removed
	 -----------------------------------------*/  

	void delTag(egg_bone_t *bone);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : tag is valid
	 * Post : tag is removed
	 -----------------------------------------*/

	egg_bone_t *getTag(unsigned int id);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : id is a valid tag id
	 * Post : Returns tag
	 -----------------------------------------*/

	void connectTag(unsigned int master, unsigned int slave);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : master and slave are valid tag ids
	 * Post : Connects slave to master
	 -----------------------------------------*/
 
	void disconnectTag(unsigned int master, unsigned int slave);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : master and slave are valid tag ids
	 * Post : Disconnects slave from master
	 -----------------------------------------*/

	egg_bone_t *addTag(vec_t x, vec_t y, vec_t z, char flag);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : x,y,z are valid coors
	 *        flag is valid flag
	 *        
	 * Post : Returns newly allocated tag
	 *        with pivot set at x,y,z with flag,
	 *        and transform is set to I
	 -----------------------------------------*/

	void addTag(egg_bone_t *tag);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : tag is a valid bone
	 * Post : tag is added to the bone list
	 -----------------------------------------*/
  
	void rotateTagAboutOrigin(unsigned int tag, vec_t rx, vec_t ry, vec_t rz);
	/*--------------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid bone id
	 *        frame is valid frame id
	 *        r(x, y, z) are in degrees
	 *
	 * Post : Rotates 'banded' bone on a pivot,
	 *        which in turn rotates meshes bound
	 *        to this bone and slave bones
	 --------------------------------------------*/

	void rotateTagAboutPoint(unsigned int bone, vec3_t p,
							 vec_t rx, vec_t ry, vec_t rz);
	/*--------------------------------------------
	 * Created  : ????-??-??, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid bone id
	 *        frame is valid frame id
	 *        p is a valid mtk point
	 *        r(x, y, z) are in degrees
	 *
	 * Post : Rotates 'banded' bone on point p,
	 *        which in turn rotates meshes bound
	 *        to this bone and slave bones
	 --------------------------------------------*/

	void rotateTag(int bone, int frame, vec_t rx, vec_t ry, vec_t rz);
	/*-----------------------------------------
	 * Created  : ????-??-??, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is a valid bone id
	 *        frame is a valid frame id
	 *        rx, ry, rz are in degrees
	 * Post : Rotates bone by rx,ry,rz in frame
	 -----------------------------------------*/

	void addTagMesh(egg_bone_t *bone, unsigned int mesh);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid
	 *        mesh is valid 
	 * Post : mesh is added to the bone's mesh list
	 -----------------------------------------*/

	void delTagMesh(egg_bone_t *bone, unsigned int mesh);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : bone is valid
	 *        mesh is valid
	 * Post : mesh is removed to the bone's mesh list
	 -----------------------------------------*/
  

	/// BoneFrame ///////////////////

	void addBoneFrame(egg_skeleton_t *boneframe);

	unsigned int addBoneFrame(vec_t x, vec_t y, vec_t z);
	/*-----------------------------------------
	 * Created  : 2001-01-31, Mongoose
	 * Modified : 
	 *
	 * Pre  : x,y,z are valid coors
	 * Post : Adds a new skeletal animation 
	 *        frame
	 -----------------------------------------*/

	egg_skeleton_t *getBoneFrame(unsigned int frame);
	/*-----------------------------------------
	 * Created  : Mongoose, ??
	 * Modified : 
	 *
	 * Pre  : frame is valid boneframe id
	 * Post : Returns boneframe
	 -----------------------------------------*/
    

	// Animations ///////////////////

	void addAnimation(egg_animation_t *a);
	/*-----------------------------------------
	 * Created  : Mongoose, ??
	 * Modified : 
	 *
	 * Pre  : 
	 * Post : Creates new animframe
	 -----------------------------------------*/

	egg_animation_t *getAnimation(unsigned int frame);
	/*-----------------------------------------
	 * Created  : Mongoose, ??
	 * Modified : 
	 *
	 * Pre  : frame is valid animframe id
	 * Post : Returns boneframe
	 -----------------------------------------*/





	////////////////////////////////////////////////////////////
	// Public, but should be protected Mutators  ;)
	////////////////////////////////////////////////////////////

	Vector <egg_texcoord_t *> *getTexCoordList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return texcoord list
	 -----------------------------------------*/

	Vector <egg_vertex_t *> *getVertexList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex list
	 -----------------------------------------*/

	Vector<egg_polygon_t *> *getPolygonList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex list
	 -----------------------------------------*/

	Vector<egg_group_t *> *getGroupList();
	/*-----------------------------------------
	 * Created  : 2000-11-30, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return vertex list
	 -----------------------------------------*/

	Vector<egg_mesh_t *> *getMeshList();
	/*-----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return mesh list
	 -----------------------------------------*/

	Vector<egg_bone_t *> *getTagList();
	/*-----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return bone list
	 -----------------------------------------*/

	Vector<egg_skeleton_t *> *getBoneFrameList();
	/*-----------------------------------------
	 * Created  : 1999-08-01, Mongoose
	 * Modified : 
	 * 
	 * Pre  :
	 * Post : Return BoneFrame list
	 -----------------------------------------*/
#endif

private:
	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	bool isDebugLevel(unsigned int level);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if debug level is greater or equal
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.06:
	 * Mongoose - Created, split from printDebug
	 ------------------------------------------------------*/

	virtual void print(char *s, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stdout
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.06:
	 * Mongoose - Removed internal level check for speed
	 *            and lower overhead, new ABI was printDebug
	 *
	 * 2002.07.05: 
	 * Mongoose - Debug level
	 *
	 * 2001.01.31: 
	 * Mongoose - Debug toggle
	 *
	 * 1999.07.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void printError(char *s, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report an error to stderr
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	Vector<Model *> mModels;         /* Vertex containers */

	unsigned int mDebugLevel;        /* Set debug output at runtime */
};

#endif