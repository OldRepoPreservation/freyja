/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Vector
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: mtk template vector
 *
 *           UINT_MAX is an error condition, used in place of -1
 * 
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.07.01:
 * Mongoose - ISO C++98 update
 *
 * 2002.08.31:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_VECTOR_H_
#define GUARD__FREYJA_MONGOOSE_VECTOR_H_

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#define VECTOR_BASE_EXPAND 16

namespace mstl {

template <class Object> class Vector
{
public:

	Vector() :
		mData(0x0),
		mReserve(0),
		mStart(0),
		mEnd(0),
		mExpand(VECTOR_BASE_EXPAND)
	{
	}


	Vector(const Vector &v) :
		mData(0x0),
		mReserve(0),
		mStart(0),
		mEnd(0),
		mExpand(VECTOR_BASE_EXPAND)
	{
		copy(v);
	}


	Vector(unsigned int size) :
		mData(0x0),
		mReserve(0),
		mStart(0),
		mEnd(0),
		mExpand(VECTOR_BASE_EXPAND)
	{
		resize(size);
	}


	Vector &operator=(const Vector<Object> &v)
	{
		mData = NULL;
		mReserve = v.mReserve;
		mStart = v.mStart;
		mEnd = v.mEnd;

		if ( v.mData != NULL && mReserve )
		{
			mData = new Object[mReserve];
			memcpy(mData, v.mData, sizeof(Object) * mReserve);			
		}

		return *this;
	}


	~Vector()
	{
		clear();

		if (mData)
		{
			delete [] mData;
		}
		
		mReserve = 0;
	}

	////////////////////////////////////////////

	void Replace(Object oldObj, Object newObj)
	{
		unsigned int i;

		for (i = begin(); i< end(); ++i)
		{
			if (mData[i] == oldObj)
			{
				assign(i, newObj);
			}
		}
	}

	unsigned int SearchIndex(Object obj)
	{
		unsigned int i;

		for (i = begin(); i < end(); ++i)
		{
			if (mData[i] == obj)
			{
				return i;
			}
		}

		return UINT_MAX;
	}


	long findFirstMatch(Object obj)
	{
		unsigned int i;

		for (i = begin(); i < end(); ++i)
		{
			if (mData[i] == obj)
			{
				return i;
			}
		}

		return -1;
	}

	////////////////////////////////////////////


	void clear()
	{
		mStart = 0;
		mEnd = 0;
	}

	void erase()
	{
		unsigned int i;


		for (i = begin(); i < end(); ++i)
		{
			if (mData[i])
			{
				delete mData[i];
			}
		}
		
		clear();
	}

	/* Returns false if reserve changes */
	bool reserve(unsigned int count)
	{
		Object *swap = 0x0;
		unsigned int i;

		if (count > mReserve)
		{
			// Yeah it's stupid, but I don't trust pure scaling
			if (count + mReserve > 100)
				mExpand += 10;
			
			if (count + mReserve > 500)
				mExpand += 100;
			
			if (count + mReserve > 7000)
				mExpand += 1000;

			swap = mData;
			mReserve = count + mExpand;
			mData = new Object[count + mExpand];
		}

		if (swap)
		{
			for (i = begin(); i < end(); ++i)
			{
				mData[i] = swap[i];
			}
			
			delete [] swap;
		}

		return (swap == 0x0);
	}


	void resize(unsigned int count)
	{
#if OLD_VECTOR_PTR_HELPER
		resize(count, 0x0);
#else
		unsigned int i;

		if (!reserve(count))
		{
			for (i = 0; i < count; ++i)
			{
				if (i < begin() || i >= end())
				{
					mData[i] = Object();
				}
			}
		}

		mEnd = count;
#endif
	}


	void resize(unsigned int count, Object obj)//Object &obj = Object())
	{
		unsigned int i;


		if (!reserve(count))
		{
			for (i = 0; i < count; ++i)
			{
				if (i < begin() || i >= end())
				{
					mData[i] = obj;
				}
			}
		}

		mEnd = count;
	}


	void pushBack()
	{
		pushBack(0x0);
	}
	

	void pushBack(Object object)
	{
		resize(size() + 1);
		mData[size()-1] = object;
	}


	bool empty()
	{
		return (begin() == end());
	}


	unsigned int capacity()
	{
		return mReserve;
	}
	

	unsigned int begin()
	{
		return mStart;
	}
	

	unsigned int end()
	{
		return mEnd;
	}


	unsigned int size()
	{
		return mEnd;
	}
	

	void copy(const Vector<Object> &v)
	{
		unsigned int i, count;

		//if (&v == 0x0)
		//	return;

		if (v.mReserve/*v.capacity()*/ > capacity())
		{
			resize(v.mReserve/*v.capacity()*/);
		}

		mStart = v.mStart;//v.begin();
		mEnd = v.mEnd;//v.end();

		for (i = mStart, count = mEnd; i < count; ++i)
		{
			mData[i] = v.mData[i];//v[i];
		}
	}


	void qSort(int (*compare_func)(const void *, const void *))
	{
		qsort(mData, end(), sizeof(Object), compare_func);
	}
	

	void swap(unsigned int a, unsigned int b)
	{
		if (a < begin() || a > end())
			return;
		
		if (b < begin() || b > end())
			return;

		Object swap = mData[a];
		mData[a] = mData[b];
		mData[b] = swap;
	}

	
	void assign(unsigned int index, Object object)
	{
		mData[index] = object;
	}


	/* Mongoose 2002.08.31, The burden of bounds checking is on you
	 *   this way you can handle your own problems w/o exceptions */
	Object &operator [] (unsigned int index)
	{
		return mData[index];
	}


	void print(void (*print_func)(Object))
	{
		unsigned int i;


		for (i = begin(); i < end(); ++i)
		{
			if (print_func)
			{
				(*print_func)(mData[i]);
			}

			fflush(stdout);
		}
		
		printf("\n");    
	}


#ifdef VECTOR_H_OLD_INTERNAL_ITERATOR
	// Built-in iterator methods ////////////////////////////////

 	unsigned int mIndex;

 	void start()
 	{
 		mIndex = begin();
 	}


 	void finish()
 	{
 		mIndex = end() - 1;
 	}


 	bool forward()
 	{
 		return (mIndex < end());
 	}


 	bool backward()
 	{
 		return (mIndex + 1 > begin());
 	}


 	void next()
 	{
 		++mIndex;
 	}


 	void prev()
 	{
 		--mIndex;
 	}


 	unsigned int currentIndex()
 	{
 			return mIndex;
 	}


 	Object current()
 	{
 			return mData[mIndex];
 	}

#endif



	// Instead of appending objects this apptempts replacing 'removed' objects
	unsigned int add(Object object)
	{
		if (begin() > 0)
		{
			mData[begin() - 1] = object;
			--mStart;

			return begin();
		}

		pushBack(object);
		return size();
	}


	void remove(unsigned int index)
	{
		if (index < end())
			mData[index] = 0x0;  // No, just invalidate it

		// Hhhmm... dangerous and fun - this gets your data out of order	
		//mData[index] = mData[begin()];
		//++mStart;
	}


	Object *getVectorArray() /* Danger, Danger ;) */
	{
		return mData;
	}
	

private:

	Object *mData;

	unsigned int mReserve;
	unsigned int mStart;
	unsigned int mEnd;
	unsigned int mExpand;
};




#define VectorIterator_BEGIN(v) for (v.start(); v.forward(); v.end()) {
#define VectorIterator_END }

template <class Object> class VectorIterator
{
public:

	VectorIterator(Vector<Object> *vector) :
		mVector(vector),
		mIndex(0)
	{
	}

	void start()
	{
		mIndex = mVector->begin();
	}


	void finish()
	{
		mIndex = mVector->end() - 1;
	}


	bool forward()
	{
		return (mIndex < mVector->end());
	}


	bool backward()
	{
		return (mIndex + 1 > mVector->begin());
	}


	void next()
	{
		++mIndex;
	}

	void prev()
	{
		--mIndex;
	}

	unsigned int index()
	{
		return mIndex;
	}

	Object current()
	{
		return (*mVector)[mIndex];
	}


	/* Requires objects to support '=='  */
	bool find(Object object)
	{
		Object *array = mVector->getVectorArray();
		unsigned int i = mVector->begin(), n = mVector->end();

		for (; i < n; ++i)
		{
			if (object == array[i])
			{
				return true;
			}
		}

		return false;
	}

	Vector<Object> *mVector;
	unsigned int mIndex;
};



class VectorUnitTest
{
public:
	VectorUnitTest();
	~VectorUnitTest();
	
	static int Test()
	{
		Vector<int> numbers;
		VectorIterator<int> iter = VectorIterator<int>(&numbers);
		int num[8];
		int fill = 32;
		unsigned int i;


		num[0] = 14;
		num[1] = 311;   

		for (i = 2; i < 8; ++i)
		{
			num[i] = fill;
		}

		///////////////////////////////////////////////

		for (i = 0; i < 8; ++i)
		{
			numbers.pushBack(num[i]);
		}
	
		///////////////////////////////////////////////


		printf("Testing first use\n");
		for (iter.start(); iter.forward(); iter.next())
		{
			printf("numbers[%i] = %3i   |   iter[%i] = %3i\n",  
					 iter.index(), num[iter.index()],
					 iter.index(), iter.current());

			if (num[iter.index()] != iter.current())
				return -10;
		}

		printf("\n");

		printf("Testing reuse\n");
		for (iter.start(); iter.forward(); iter.next())
		{
			printf("numbers[%i] = %3i   |   iter[%i] = %3i\n",  
					 iter.index(), num[iter.index()],
					 iter.index(), iter.current());
		
			if (num[iter.index()] != iter.current())
				return -11;
		}

		printf("\n");

		printf("Testing reuse in reverse\n");
		for (iter.finish(); iter.backward(); iter.prev())
		{
			printf("numbers[%i] = %3i   |   iter[%i] = %3i\n",  
					 iter.index(), num[iter.index()],
					 iter.index(), iter.current());
		
			if (num[iter.index()] != iter.current())
				return -12;
		}
   

		return 0;
	}
};

} // namespace mstl

#endif