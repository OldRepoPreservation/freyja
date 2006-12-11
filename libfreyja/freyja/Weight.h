/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Weight
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the freyja::Weight class.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.12.09:
 * Mongoose - Created, split from Mesh.h
 ==========================================================================*/

#ifndef GUARD__FREYJA_WEIGHT_H_
#define GUARD__FREYJA_WEIGHT_H_

#include <hel/math.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include "freyja.h"

using namespace mstl;


namespace freyja {

class Weight
{
public:
	
	Weight(index_t vertex, index_t bone, vec_t weight) : 
		mVertexIndex(vertex),
		mBoneIndex(bone),
		mWeight(weight)
	{
	}

	Weight(const Weight &weight) :
		mVertexIndex(weight.mVertexIndex),
		mBoneIndex(weight.mBoneIndex),
		mWeight(weight.mWeight)
	{
	}


	Weight() : mVertexIndex(INDEX_INVALID),
			   mBoneIndex(INDEX_INVALID),
			   mWeight(0.0f)
	{
	}

	~Weight()
	{
	}


	static size_t SerializedSize() { return 4*2 + 4; }

	bool Serialize(SystemIO::FileReader &r) 
	{ 
		mVertexIndex = r.ReadLong();
		mBoneIndex = r.ReadLong();
		mWeight = r.ReadFloat32();
		return true; 
	}

	bool Serialize(SystemIO::FileWriter &w) 
	{ 
		w.WriteLong(mVertexIndex);
		w.WriteLong(mBoneIndex);
		w.WriteFloat32(mWeight);
		return true;
	}

	bool Serialize(SystemIO::TextFileWriter &w) 
	{ 
		w.Print("\t\tw %u %u %f\n", mVertexIndex, mBoneIndex, mWeight);
		return true;
	}

	bool Serialize(SystemIO::TextFileReader &r) 
	{ 
		const char *symbol = r.ParseSymbol();
		if (symbol[0] == 'w')
			return false;

		mVertexIndex = r.ParseInteger();
		mBoneIndex = r.ParseInteger();
		mWeight = r.ParseFloat();
		
		return true;
	}

	index_t mVertexIndex;
	index_t mBoneIndex;
	vec_t mWeight;
};

} // End namespace freyja


#endif // GUARD__FREYJA_WEIGHT_H_