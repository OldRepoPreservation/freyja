/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Simulation
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: This class simulates mass physics
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_SIMULATION - Builds Simulation class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2003.06.08:
 * Mongoose - Generic collision and basic resolution w/o generic
 *            Hel Resolvers
 *
 * 2003.06.02:
 * Mongoose - Created, based on algorithms from Erkin Tunca
 ================================================================*/


#ifndef GUARD__HEL_MONGOOSE_SIMULATION_H_
#define GUARD__HEL_MONGOOSE_SIMULATION_H_

#include <mstl/Vector.h>
#include "hel/CollisionObject.h"
#include "hel/Mass.h"
#include "hel/math.h"

using namespace mstl;

namespace hel {

class Simulation
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Simulation();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Simulation
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~Simulation();
	/*------------------------------------------------------
	 * Pre  : Simulation object is allocated
	 * Post : Deconstructs an object of Simulation
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	Mass *getMass(unsigned int i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get <i>th Mass element of this simulation
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	virtual void attachCollisionObject(CollisionObject *obj);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Adds <Obj> to the collision list for this simulation
	 *
	 * TODO : Allow multiple types of collision to be excluded?
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void clear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Removes all masses from this simulation
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual bool collisionDetected(Mass *mass);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if <Mass> collided with object
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void generateMasses(unsigned int count, vec_t mass);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : <Count> number of masses with <mass> value are added
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void setMass(unsigned int idx, vec_t mass,
								const Vec3 &pos, const Vec3 &velocity);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : <idx>th mass is set with 
	 *        <mass>, <position>, and <velocity>
	 ------------------------------------------------------*/

	virtual void generateMasses(unsigned int count, vec_t mass,
										 const Vec3 &position, const Vec3 &velocity);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : <Count> number of masses with 
	 *        <mass>, <position>, and <velocity> are added
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void init();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Reset forces acting on masses
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void resolveCollision(Mass *mass, CollisionObject *obj);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resolves a collision of <Obj> and <Mass>
	 *
	 *        The default resolution here is to set mass pos
	 *        to intersection point with the object, there
	 *        is no affect on velocity
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void run(vec_t timeDelta);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Does full init, solve, simulate cycle
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void simulate(vec_t timeDelta);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Update change in time for each mass
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void solve ();
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Have forces act on masses  
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual unsigned int getMassCount() { return mMassCount; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Have forces act on masses  
	 ------------------------------------------------------*/


	Vector<Mass *> mMasses;			/* It's a collection of masses, silly */

	Vector<CollisionObject *> mObjects;	/* Object(s) to test collision with */

	unsigned int mMassCount;		/* Cache of number of masses in collection  */


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
};


class GravitySimulation : public Simulation
{
public:

	GravitySimulation() : 
		Simulation(),
		gravitation(0.0f, -9.81f, 0.0f)		 
	{
		//gravitation = Vector3d(0.0f, -9.81f, 0.0f);
	}


	virtual void solve()                                                    
	{
		Mass *m;
		unsigned int i;


		// Apply gravitational force to each mass
		for (i = mMasses.begin(); i < mMasses.end(); ++i)
		{
			m = mMasses[i];

			// Gravitational force, Force = (Mass)(Gravity)
			m->applyForce(gravitation * m->mMass);
			
			if (collisionDetected(m))
			{
				m->mVelocity = Vec3(0, 0, 0);
			}
		}
	}


	Vec3 gravitation;     /* Gravitational acceleration */
};


class SpringSimulation : public Simulation
{
public:

	SpringSimulation() : Simulation()
	{
	}

	
	virtual void generateMasses(unsigned int count, vec_t mass,
										 const Vec3 &position, 
										 const Vec3 &velocity)
	{
		Simulation::generateMasses(count, mass, 
											connectionPos + position, velocity);
	}


	virtual void solve()
	{
		Mass *m;
		unsigned int i;


		// Apply spring force to each mass
		for (i = mMasses.begin(); i < mMasses.end(); ++i)
		{
			m = mMasses[i];

			// Vector from mass to spring connection
			Vec3 springVector = m->mPos - connectionPos;
			
			// Apply spring force, spring force formulation
			m->applyForce(-springVector * springConstant);

			if (collisionDetected(m))
			{
				//m->mVelocity = Vector3d(0, 0, 0);
			}
		}
	}

	vec_t springConstant;     /* Stiffness, higher values more spring force */

	Vec3 connectionPos;       /* Location where mass is connected to spring */
};

} // namespace hel

#endif
