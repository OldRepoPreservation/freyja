/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Action
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the 'event reel atom' for freyja undo/redo/macro usage
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_ACTION - Builds Action class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.05.06:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_ACTION_H_
#define GUARD__FREYJA_MONGOOSE_ACTION_H_

#include <mstl/SystemIO.h>
#include <mstl/Vector.h>


namespace mstl {

class Action
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Action() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Action
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~Action() {}
	/*------------------------------------------------------
	 * Pre  : Action object is allocated
	 * Post : Deconstructs an object of Action
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	virtual bool Serialize(SystemIO::FileWriter &w) { return false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	virtual bool Redo() = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileReader &r) { return false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

 	virtual bool Undo() = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/



 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};


class ActionList : public Action
{
public:
	ActionList(Vector<Action *> &actions) :
		Action(),
		mActions(actions)
	{}

	~ActionList()
	{
		unsigned int idx;
		foreach (mActions, idx)
		{
			if (mActions.current(idx))
				delete mActions.current(idx);
		}
	}

	virtual bool Redo()
	{
		unsigned int idx;
		foreach (mActions, idx)
		{
			if (mActions.current(idx))
				mActions.current(idx)->Redo();
		}

		return true;
	}

	virtual bool Serialize(SystemIO::FileReader &r)
	{
		unsigned int idx;
		foreach (mActions, idx)
		{
			if (mActions.current(idx))
				mActions.current(idx)->Serialize(r);
		}

		return true;

	}

	virtual bool Serialize(SystemIO::FileWriter &w)
	{
		unsigned int idx;
		foreach (mActions, idx)
		{
			if (mActions.current(idx))
				mActions.current(idx)->Serialize(w);
		}

		return true;

	}

	virtual bool Undo() 
	{
		unsigned int idx;
		foreach (mActions, idx)
		{
			if (mActions.current(idx))
				mActions.current(idx)->Undo();
		}

		return true;
	}


	Vector<Action *> mActions;
};

} // namespace mstl

#endif // GUARD__FREYJA_MONGOOSE_ACTION_H_
