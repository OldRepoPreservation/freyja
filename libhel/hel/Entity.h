/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Entity
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: This is the base thing class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_ENTITY - Builds Entity class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2002.11.09:
 * Mongoose - Created
 ================================================================*/

#ifndef GUARD__HEL_MONGOOSE_ENTITY_H_
#define GUARD__HEL_MONGOOSE_ENTITY_H_

#include "hel/math.h"
#include "hel/BoundingVolume.h"
#include "hel/CollisionObject.h"
#include "hel/Quat.h"
#include "hel/Vec3.h"
#include "hel/Mass.h"

namespace hel {

typedef enum
{
	EntityFlag_Visible    = 1,
	EntityFlag_Collision  = 2,
	EntityFlag_Particle   = 4,
	EntityFlag_World      = 8

} EntityFlag;


class Entity
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Entity();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Entity
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.11.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Entity();
	/*------------------------------------------------------
	 * Pre  : Entity object is allocated
	 * Post : Deconstructs an object of Entity
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.11.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	vec_t time, eventTime, eventTimer;
	int32 state, nextState;
	vec_t health;

	uint32 mId;                /* Unique identifier */

	Vec3 mLastPos;             /* World position */

	Quat mOrientation;	      /* Orientation (Pitch, Yaw, Roll) */

	CollisionObject *obj;		/* Physical representation */

	Mass *mMass;					/* Mass of this entity */

	EntityFlag mFlag;	 			/* Flags of this entity */

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
};

} // namespace hel

#endif
