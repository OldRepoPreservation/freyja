/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaFileReader
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: Reads file data from bin/text files for model parsers
 *
 *
 *           The SWAP_ and FIX_ macros provided by Paul Bourke in 1991
 *           (Public Domain)
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.31:
 * Mongoose - Fixed flaw in string creation for dir listing system
 *
 * 2004.08.16:
 * Mongoose - Created, based on my mendian reader
 ==========================================================================*/

#include <sys/stat.h>

#include "FreyjaFileReader.h"

#define SWAP_2(x) ( (((x) & 0xff) << 8) | ((unsigned short)(x) >> 8) )
#define SWAP_4(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX_SHORT(x) (*(unsigned short *)&(x) = SWAP_2(*(unsigned short *)&(x)))
#define FIX_INT(x)   (*(unsigned int *)&(x)   = SWAP_4(*(unsigned int *)&(x)))
#define FIX_FLOAT(x) FIX_INT(x)


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaFileReader::FreyjaFileReader()
{
	mFileHandle = 0x0;
	mTempBufferHackSz = 2047;
	mTempBufferHack = new char[mTempBufferHackSz+1];

	mDirectory = 0x0;
	mDirectoryListing = 0x0;
}


FreyjaFileReader::~FreyjaFileReader()
{
	closeDirectory();
	closeFile();

	if (mTempBufferHack)
		delete [] mTempBufferHack;

	if (mDirectoryListing)
		delete [] mDirectoryListing;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

int FreyjaFileReader::compareFilenameExtention(const char *filename, const char *ext)
{
	unsigned int len_f, len_e;
 
	
	/* Check for null or blank input strings */
	if (!filename || !filename[0] ||
		!ext || !ext[0])
	{
		return -1;
	}

	len_f = strlen(filename) - 1;
	len_e = strlen(ext) - 1;
	
	while (len_f+1 && len_e+1)
	{
		//printf("extcheck: %c %c\n", filename[len_f], ext[len_e]);
		
		if (filename[len_f] == ext[len_e])
		{
			if (ext[len_e] == '.')
			{
				//printf("extcheck: match found\n");
				return 0;
			}
		}
		else if (filename[len_f] < ext[len_e])
		{
			return -1;
		}
		else if (filename[len_f] > ext[len_e])
		{
			return 1;
		}
		
		--len_f;
		--len_e;
	}
	
	return 1;
}


bool FreyjaFileReader::endOfFile()
{
	return (feof(mFileHandle) != 0);
}


bool FreyjaFileReader::doesFileExist(const char *filename)
{
	if (!filename || !filename[0])
		return false;

	FILE *f = fopen(filename, "rb");

	if (f)
	{
		fclose(f);
		return true;
	}

	return false;
}


unsigned int FreyjaFileReader::getFileOffset()
{
	return ftell(mFileHandle);
}


bool FreyjaFileReader::isDirectory(const char *filename)
{
  struct stat status;


  stat(filename, &status);

  return ((S_ISDIR(status.st_mode)));
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaFileReader::closeDirectory()
{
	if (mDirectoryListing)
	{
		delete [] mDirectoryListing;
		mDirectoryListing = 0x0;
	}

	if (mDirectory)
	{
		closedir(mDirectory);
		mDirectory = 0x0;
	}
}


void FreyjaFileReader::closeFile()
{
	if (mFileHandle)
	{
		fclose(mFileHandle);
		mFileHandle = 0x0;
	}
}

/* TODO - check for buffer size errors */
char *FreyjaFileReader::getNextDirectoryListing()
{
	struct dirent *d_ptr;


	if (mDirectoryListing)
	{
		delete [] mDirectoryListing;
		mDirectoryListing = 0x0;
	}

	if (mDirectory && (d_ptr = readdir(mDirectory)))
	{
		unsigned int length;

		length = strlen(mDirectoryName) + strlen(d_ptr->d_name) + 2;
		mDirectoryListing = new char[length+1];
		snprintf(mDirectoryListing, length, "%s/%s",
				 mDirectoryName, d_ptr->d_name);
		mDirectoryListing[length] = 0;
	}

	return mDirectoryListing;
}


bool FreyjaFileReader::openDirectory(const char *dirname)
{
	unsigned int length;


	if (!dirname || !dirname[0])
		return false;

	mDirectory = opendir(dirname);
	
	if (!mDirectory)
	{
		perror(dirname);
		return false;
	}

	length = strlen(dirname);
	mDirectoryName = new char[length+1];
	strncpy(mDirectoryName, dirname, length);
	mDirectoryName[length] = 0;

	return true;
}


bool FreyjaFileReader::openFile(const char *filename)
{
	if (!filename || !filename[0])
		return false;

	mFileHandle = fopen(filename, "rb");
	
	if (!mFileHandle)
	{
		perror(filename);
		return false;
	}

	return true;
}


double FreyjaFileReader::parseFloat()
{
	double r;


	// ugly fscanf use for temp hack
	fscanf(mFileHandle, "%lf", &r);
	//r = atof(getSymbol());

	//printf("** %f\n", r);
	return r;
}


int FreyjaFileReader::parseInteger()
{
	int i;


	// ugly fscanf use for temp hack
	fscanf(mFileHandle, "%i", &i);
	//i = atoi(getSymbol());

	//printf("** %i\n", i);
	return i;
}


bool FreyjaFileReader::parseMatchingSymbol(const char *symbol)
{
	unsigned int l = strlen(symbol); /* Assumes !(!symbol) && !(!symbol[0]) */
	char *sym = parseSymbol();
	bool test = (!strncmp(symbol, sym, l));

	
	if (!test)
	{
		printf("Not matched: '%s' != '%s'\n", symbol, sym);
	}

	return test;
}


char *FreyjaFileReader::parseString()
{
	unsigned int l, i = 0, state = 0;
	char *s;
	char c, lc = 0;
	
	// ugly fscanf use for temp hack
	while (i < mTempBufferHackSz && fscanf(mFileHandle, "%c", &c) != EOF)
	{
		switch (state)
		{
		case 0:
			if (c == '"')
				state = 1;
			break;
		case 1:
			if (c == '"' && lc != '\\')  // Allow quote escapes?
			{
				i = mTempBufferHackSz;
			}
			else
			{
				mTempBufferHack[i++] = c;
				mTempBufferHack[i] = 0;
			}
			break;
		}

		lc = c;
	}
	
	l = strlen(mTempBufferHack);
	s = new char[l+1];
	strncpy(s, mTempBufferHack, l);
	s[l] = 0;
	//printf("** \"%s\"\n", mTempBufferHack);

	return s;
}


char *FreyjaFileReader::parseSymbol()
{
	unsigned int i = 0, state = 0;
	char c;


	mTempBufferHack[0] = 'F';
	mTempBufferHack[1] = 'U';
	mTempBufferHack[2] = 0;

	// ugly fscanf use for temp hack
	while (i < mTempBufferHackSz && fscanf(mFileHandle, "%c", &c) != EOF)
	{
		switch (state)
		{
		case 0:
			if (c == '/')
			{
				state = 1;
				mTempBufferHack[i++] = c;
				mTempBufferHack[i] = 0;	
			}
			else if (c == ' ' || c == '\r' || c == '\n' || c == '\t')
			{
				if (i > 0)
					i = mTempBufferHackSz;
			}
			else
			{
				mTempBufferHack[i++] = c;
				mTempBufferHack[i] = 0;
			}
			break;
		case 1:
			if (c == '/')
			{
				state = 2;
				--i;
				mTempBufferHack[i] = 0;
			}
			else
			{
				state = 0;
				mTempBufferHack[i++] = c;
				mTempBufferHack[i] = 0;
			}
			break;
		case 2:
			if (c == '\n')
			{
				/* Only wrap lines when given a only comment line(s) */
				if (i > 0)
					i = mTempBufferHackSz;
				else
					state = 0;
			}
			break;
		}
	}

	// ugly fscanf use for temp hack
	//	fscanf(mFileHandle, "%s", mTempBufferHack);
   	//printf("\n** <%s>\n", mTempBufferHack);
	
	return mTempBufferHack;
}


void FreyjaFileReader::readBufferUnsignedChar(unsigned int length, 
										   unsigned char *buffer)
{
	fread(buffer, length, 1, mFileHandle);  // heh, yeah
}


void FreyjaFileReader::readCharString(unsigned int length, char *buffer)
{
	fread(buffer, length, 1, mFileHandle);  // heh, yeah
}


float FreyjaFileReader::readFloat32()
{	
	float r;
	void *ptr = &r;
	size_t sz = fread(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit float\n");

#if HAVE_BIG_ENDIAN
	FIX_FLOAT(*ptr)
#endif

	return r;
}


char FreyjaFileReader::readInt8()
{
	char c;

	fread(&c, 1, 1, mFileHandle);  // heh, yeah

	return c;
}


unsigned char FreyjaFileReader::readInt8U()
{
	unsigned char u;

	fread(&u, 1, 1, mFileHandle);  // heh, yeah

	return u;
}


short FreyjaFileReader::readInt16()
{
	short int i;
	void *ptr = &i;
	size_t sz = fread(ptr, 2, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 16bit int\n");

#ifdef HAVE_BIG_ENDIAN
	FIX_SHORT(*ptr)
#endif
	return i;
}

	
unsigned short FreyjaFileReader::readInt16U()
{
	unsigned short int u;
	void *ptr = &u;
	size_t sz = fread(ptr, 2, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 16bit uint\n");

#ifdef HAVE_BIG_ENDIAN
	FIX_SHORT(*ptr)
#endif
	return u;
}


int FreyjaFileReader::readInt32()
{
	int i;
	void *ptr = &i;
	size_t sz = fread(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit int\n");

#ifdef HAVE_BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	return i;
}


unsigned int FreyjaFileReader::readInt32U()
{
	unsigned int u;
	void *ptr = &u;
	size_t sz = fread(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit uint\n");

#ifdef HAVE_BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	return u;
}


long FreyjaFileReader::readLong()
{
	long i;
	void *ptr = &i;
	size_t sz = fread(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit int\n");

#ifdef HAVE_BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	return i;
}


unsigned long FreyjaFileReader::readLongU()
{
	long u;
	void *ptr = &u;
	size_t sz = fread(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit uint\n");

#ifdef HAVE_BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	return u;
}


bool FreyjaFileReader::setFileOffset(unsigned int offset)
{
	return (fseek(mFileHandle, offset, SEEK_SET) == 0);
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

#ifdef UNIT_TEST_FREYJAFILEREADER
int runFreyjaFileReaderUnitTest(int argc, char *argv[])
{
	FreyjaFileReader test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaFileReader class test]\n");

	return runFreyjaFileReaderUnitTest(argc, argv);
}
#endif