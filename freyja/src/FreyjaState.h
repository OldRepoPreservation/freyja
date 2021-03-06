/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Action classes to implement undo feature more cleanly
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: This is a simple action event stack for undo/redo
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJASTATE - Builds FreyjaState class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.03.05:
 * Mongoose - Created, based on test
 ==========================================================================*/

#ifndef GUARD__FREYJA3D_FREYJASTATE_H_
#define GUARD__FREYJA3D_FREYJASTATE_H_

#include <hel/Mat44.h>
#include <hel/Vec3.h>
#include <hel/Quat.h>
#include <freyja/freyja.h>
#include <freyja/BoneABI.h>
#include <freyja/MeshABI.h>
#include <freyja/Mesh.h>
#include <mstl/Action.h>

using namespace freyja;

#if FIXME
class ActionMeshTranslateExt : 
	public mstl::Action
{
 public:
	ActionMeshTranslateExt(freyja::Mesh* mesh, hel::Vec3 xyz, hel::Vec3 &v) :
		Action(),
		mCursorXYZ(v),
		mMesh(mesh),
		mXYZ(xyz)
	{ }

	virtual bool Redo() 
	{ return false; }

	virtual bool Undo() 
	{
		if (mMesh)
		{
			// Adjust relative translation to absolote position
			hel::Vec3 u = mXYZ - mMesh->GetPosition();
			mMesh->SetPosition(mXYZ);
			mMesh->Translate( u );
		}

		mCursorXYZ = mXYZ;
		return true;
	}

	hel::Vec3 &mCursorXYZ;
	freyja::Mesh* mMesh;                /* mesh */
	hel::Vec3 mXYZ;                     /* Storage for 3d transform event */
};


class ActionModelTransform : public Action
{
 public:
	ActionModelTransform(index_t model, freyja_transform_action_t a, hel::Vec3 v) :
		Action(),
		mModel(model),
		mAction(a),
		mXYZ(v)
	{}

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		// FIXME: WRONG!
		for (uint32 i = 0, n = freyjaGetMeshCount(); i < n; ++i)
			freyjaMeshTransform3fv(i, mAction, mXYZ.mVec);

		return true;
	}

	index_t mModel;                     /* Which model? */
	freyja_transform_action_t mAction;  /* Type of transform */
	hel::Vec3 mXYZ;                      /* Storage for 3d transform event */
};


class ActionMeshTransform : public Action
{
 public:
	ActionMeshTransform(index_t mesh, freyja_transform_action_t a, hel::Vec3 v) :
		Action(),
		mMesh(mesh),
		mAction(a),
		mXYZ(v)
	{}

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		freyjaMeshTransform3fv(mMesh, mAction, mXYZ.mVec);
		return true;
	}

	index_t mMesh;                      /* Which mesh? */
	freyja_transform_action_t mAction;  /* Type of transform */
	hel::Vec3 mXYZ;                      /* Storage for 3d transform event */
};


class ActionTexCoordTransform : public Action
{
 public:
	ActionTexCoordTransform(freyja::Mesh mesh, index_t texcoord, vec_t u, vec_t v) :
		Action(),
		mMesh(mesh),
		mTexCoordArrayIndex(texcoord),
		mU(u),
		mV(v)
	{}

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		freyja::Mesh *m = mMesh;

		if (m)
		{
			vec3_t uvw = { mU, mV, 0.0f };
			m->SetTexCoord(mTexCoordArrayIndex, uvw);
			return true;
		}

		return false;
	}

	freyja::Mesh* mMesh;
	index_t mTexCoordArrayIndex;
	vec_t mU, mV;
};


class ActionGenericTransform : public Action
{
 public:
	ActionGenericTransform(freyja_transform_t t, freyja_transform_action_t a, 
						   index_t owner, index_t id, hel::Vec3 &v) :
		Action(),
		mTransform(t),
		mAction(a),
		mOwner(owner),
		mId(id),
		mXYZ(v)
	{ }

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		hel::Vec3 xyz;

		freyjaGetGenericTransform3fv(mTransform, mAction, mId, xyz.mVec);
		
		switch (mAction)
		{
		case fTranslate:
			xyz = mXYZ - xyz; // translate from origin not current translation
			break;

		default:
			xyz = mXYZ;
		}

		freyjaPrintMessage("! FreyjaStateTransform::Undo() %i %i %i %f %f %f",
						   mTransform, mAction, mId,
						   xyz.mVec[0], xyz.mVec[1], xyz.mVec[2]);

		freyjaGenericTransform3fv(mTransform, mAction, mId, xyz.mVec);
		return true;
	}

	freyja_transform_t mTransform;      /* Object type */
	freyja_transform_action_t mAction;  /* Type of transform */
	index_t mOwner;
	index_t mId;
	hel::Vec3 mXYZ;                      /* Storage for 3d transform event */
};


class ActionVertexTransformExt : public Action
{
 public:
	ActionVertexTransformExt(index_t mesh, index_t vertex, freyja_transform_action_t a, hel::Vec3 xyz, hel::Vec3 &v) :
		Action(),
		mCursorXYZ(v),
		mMesh(mesh),
		mVertex(vertex),
		mAction(a),
		mXYZ(xyz)
	{ }

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		if (mAction == fTranslate)
			freyjaMeshVertexPos3fv(mMesh, mVertex, mXYZ.mVec);
		mCursorXYZ = mXYZ;
		return true;
	}

	hel::Vec3 &mCursorXYZ;
	index_t mMesh;
	index_t mVertex;
	freyja_transform_action_t mAction;  /* Type of transform */
	hel::Vec3 mXYZ;                     /* Storage for 3d transform event */
};


class ActionFacesTransform : public Action
{
 public:
	ActionFacesTransform(index_t mesh, Vector<index_t> &faces, hel::Mat44 &mat) :
		Action(),
		mMesh(mesh),
		mFaces(faces),
		mTransform(mat)
	{
		mat.Print();
	}

	~ActionFacesTransform() { }

	virtual bool Redo()
	{ 
		Mesh *m = Mesh::GetMesh(mMesh);
		if (m)
		{
			m->TransformFacesInList(mFaces, mTransform);
		}
		
		return true;
	}

	virtual bool Undo() 
	{
		Mesh *m = Mesh::GetMesh(mMesh);
		if (m)
		{
			hel::Mat44 inv;
			mTransform.GetInverse(inv);
			inv.Print();
			m->TransformFacesInList(mFaces, inv);
		}

		return true;
	}

	index_t mMesh;
	Vector<index_t> mFaces;
	hel::Mat44 mTransform;
};


class ActionBoneTransform : public Action
{
 public:
	ActionBoneTransform(index_t bone, 
						freyja_transform_action_t action, hel::Vec3 v) :
		Action(),
		mBone(bone),
		mAction(action),
		mTransform(v)
	{
	}

	~ActionBoneTransform() { }

	virtual bool Redo()
	{ 
		freyjaBoneTransform3fv(mBone, mAction, mTransform.mVec);
		return true;
	}

	virtual bool Undo() 
	{
		freyjaBoneInverseTransform3fv(mBone, mAction, mTransform.mVec);
		return true;
	}

	index_t mBone;
	freyja_transform_action_t mAction;
	hel::Vec3 mTransform;
};


class ActionVertexListTransformExt : public Action
{
 public:
	ActionVertexListTransformExt(index_t mesh, const Vector<index_t> &list, 
								 freyja_transform_action_t a, Vector<hel::Vec3> &list2, 
								 hel::Vec3 &v) :
		Action(),
		mCursorXYZ(v),
		mMesh(mesh),
		mVertexList(list),
		mAction(a),
		mXYZList(list2)
	{ }

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		switch (mAction)
		{
		case fTranslate:
			{
				uint32 i;
				foreach(mVertexList, i)
				{
					freyjaMeshVertexPos3fv(mMesh, mVertexList[i], mXYZList[i].mVec);
				}
			}
			break;

		default:
			freyja_print("(%s:%i) %s %i Not implemented", mAction, 
						 __FILE__, __LINE__, __func__);
		}

		mCursorXYZ = mXYZList[0];
		return true;
	}

	hel::Vec3 &mCursorXYZ;
	index_t mMesh;
	Vector<index_t> mVertexList;
	freyja_transform_action_t mAction;  /* Type of transform */
	Vector<hel::Vec3> mXYZList;         /* Storage for 3d transform event */
};


class ActionMeshDelete : public Action
{
 public:
	ActionMeshDelete(freyja::Mesh* mesh) :
		Action(),
		mMesh(mesh)
	{
		freyja::Mesh *m = mMesh;

		if (m)
		{
			// Copy of mesh outside of scene
			mMesh = new Mesh(*m);
			gScene->Remove( mMesh );
		}
	}

	virtual bool Redo() { return false; }

	virtual bool Undo() 
	{
		if (mMesh)
		{
			gScene->Add( mMesh );
		}

		return true;
	}

	freyja::Mesh *mMesh;
};

#endif // FIXME

#endif // GUARD__FREYJA3D_FREYJASTATE_H_
