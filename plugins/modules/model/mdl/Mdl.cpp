/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : MDDC 
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : Mdl
 * Comments: This is the MDL model class.
 *
 *           See file COPYING for license details.
 *
 *           Quake (c) id Software 1995 
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2004.12.23:
 * MEndian replaced with FreyjaFileWriter/Reader use
 *
 * 2004.04.22:
 * Mongoose - RGB Texture generation and internal palettes
 *
 * 2000-03-16
 * Mongoose: Endian support
 *           Created from old MDDC source
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <mstl/SystemIO.h>

#include "Mdl.h"

using namespace mstl;


unsigned char Mdl::mPaletteHexen2[] = {
	0, 0, 0,
	0, 0, 0,
	8, 8, 8,
	16, 16, 16,
	24, 24, 24,
	32, 32, 32,
	40, 40, 40,
	48, 48, 48,
	56, 56, 56,
	64, 64, 64,
	72, 72, 72,
	80, 80, 80,
	84, 84, 84,
	88, 88, 88,
	96, 96, 96,
	104, 104, 104,
	112, 112, 112,
	120, 120, 120,
	128, 128, 128,
	136, 136, 136,
	148, 148, 148,
	156, 156, 156,
	168, 168, 168,
	180, 180, 180,
	184, 184, 184,
	196, 196, 196,
	204, 204, 204,
	212, 212, 212,
	224, 224, 224,
	232, 232, 232,
	240, 240, 240,
	252, 252, 252,
	8, 8, 12,
	16, 16, 20,
	24, 24, 28,
	28, 32, 36,
	36, 36, 44,
	44, 44, 52,
	48, 52, 60,
	56, 56, 68,
	64, 64, 72,
	76, 76, 88,
	92, 92, 104,
	108, 112, 128,
	128, 132, 152,
	152, 156, 176,
	168, 172, 196,
	188, 196, 220,
	32, 24, 20,
	40, 32, 28,
	48, 36, 32,
	52, 44, 40,
	60, 52, 44,
	68, 56, 52,
	76, 64, 56,
	84, 72, 64,
	92, 76, 72,
	100, 84, 76,
	108, 92, 84,
	112, 96, 88,
	120, 104, 96,
	128, 112, 100,
	136, 116, 108,
	144, 124, 112,
	20, 24, 20,
	28, 32, 28,
	32, 36, 32,
	40, 44, 40,
	44, 48, 44,
	48, 56, 48,
	56, 64, 56,
	64, 68, 64,
	68, 76, 68,
	84, 92, 84,
	104, 112, 104,
	120, 128, 120,
	140, 148, 136,
	156, 164, 152,
	172, 180, 168,
	188, 196, 184,
	48, 32, 8,
	60, 40, 8,
	72, 48, 16,
	84, 56, 20,
	92, 64, 28,
	100, 72, 36,
	108, 80, 44,
	120, 92, 52,
	136, 104, 60,
	148, 116, 72,
	160, 128, 84,
	168, 136, 92,
	180, 144, 100,
	188, 152, 108,
	196, 160, 116,
	204, 168, 124,
	16, 20, 16,
	20, 28, 20,
	24, 32, 24,
	28, 36, 28,
	32, 44, 32,
	36, 48, 36,
	40, 56, 40,
	44, 60, 44,
	48, 68, 48,
	52, 76, 52,
	60, 84, 60,
	68, 92, 64,
	76, 100, 72,
	84, 108, 76,
	92, 116, 84,
	100, 128, 92,
	24, 12, 8,
	32, 16, 8,
	40, 20, 8,
	52, 24, 12,
	60, 28, 12,
	68, 32, 12,
	76, 36, 16,
	84, 44, 20,
	92, 48, 24,
	100, 56, 28,
	112, 64, 32,
	120, 72, 36,
	128, 80, 44,
	144, 92, 56,
	168, 112, 72,
	192, 132, 88,
	24, 4, 4,
	36, 4, 4,
	48, 0, 0,
	60, 0, 0,
	68, 0, 0,
	80, 0, 0,
	88, 0, 0,
	100, 0, 0,
	112, 0, 0,
	132, 0, 0,
	152, 0, 0,
	172, 0, 0,
	192, 0, 0,
	212, 0, 0,
	232, 0, 0,
	252, 0, 0,
	16, 12, 32,
	28, 20, 48,
	32, 28, 56,
	40, 36, 68,
	52, 44, 80,
	60, 56, 92,
	68, 64, 104,
	80, 72, 116,
	88, 84, 128,
	100, 96, 140,
	108, 108, 152,
	120, 116, 164,
	132, 132, 176,
	144, 144, 188,
	156, 156, 200,
	172, 172, 212,
	36, 20, 4,
	52, 24, 4,
	68, 32, 4,
	80, 40, 0,
	100, 48, 4,
	124, 60, 4,
	140, 72, 4,
	156, 88, 8,
	172, 100, 8,
	188, 116, 12,
	204, 128, 12,
	220, 144, 16,
	236, 160, 20,
	252, 184, 56,
	248, 200, 80,
	248, 220, 120,
	20, 16, 4,
	28, 24, 8,
	36, 32, 8,
	44, 40, 12,
	52, 48, 16,
	56, 56, 16,
	64, 64, 20,
	68, 72, 24,
	72, 80, 28,
	80, 92, 32,
	84, 104, 40,
	88, 116, 44,
	92, 128, 52,
	92, 140, 52,
	92, 148, 56,
	96, 160, 64,
	60, 16, 16,
	72, 24, 24,
	84, 28, 28,
	100, 36, 36,
	112, 44, 44,
	124, 52, 48,
	140, 64, 56,
	152, 76, 64,
	44, 20, 8,
	56, 28, 12,
	72, 32, 16,
	84, 40, 20,
	96, 44, 28,
	112, 52, 32,
	124, 56, 40,
	140, 64, 48,
	24, 20, 16,
	36, 28, 20,
	44, 36, 28,
	56, 44, 32,
	64, 52, 36,
	72, 60, 44,
	80, 68, 48,
	92, 76, 52,
	100, 84, 60,
	112, 92, 68,
	120, 100, 72,
	132, 112, 80,
	144, 120, 88,
	152, 128, 96,
	160, 136, 104,
	168, 148, 112,
	36, 24, 12,
	44, 32, 16,
	52, 40, 20,
	60, 44, 20,
	72, 52, 24,
	80, 60, 28,
	88, 68, 28,
	104, 76, 32,
	148, 96, 56,
	160, 108, 64,
	172, 116, 72,
	180, 124, 80,
	192, 132, 88,
	204, 140, 92,
	216, 156, 108,
	60, 20, 92,
	100, 36, 116,
	168, 72, 164,
	204, 108, 192,
	4, 84, 4,
	4, 132, 4,
	0, 180, 0,
	0, 216, 0,
	4, 4, 144,
	16, 68, 204,
	36, 132, 224,
	88, 168, 232,
	216, 4, 4,
	244, 72, 0,
	252, 128, 0,
	252, 172, 24,
	252, 252, 252
};

unsigned char Mdl::mPaletteQuake[] = {
	0, 0, 0,
	15, 15, 15,
	31, 31, 31,
	47, 47, 47,
	63, 63, 63,
	75, 75, 75,
	91, 91, 91,
	107, 107, 107,
	123, 123, 123,
	139, 139, 139,
	155, 155, 155,
	171, 171, 171,
	187, 187, 187,
	203, 203, 203,
	219, 219, 219,
	235, 235, 235,
	15, 11, 7,
	23, 15, 11,
	31, 23, 11,
	39, 27, 15,
	47, 35, 19,
	55, 43, 23,
	63, 47, 23,
	75, 55, 27,
	83, 59, 27,
	91, 67, 31,
	99, 75, 31,
	107, 83, 31,
	115, 87, 31,
	123, 95, 35,
	131, 103, 35,
	143, 111, 35,
	11, 11, 15,
	19, 19, 27,
	27, 27, 39,
	39, 39, 51,
	47, 47, 63,
	55, 55, 75,
	63, 63, 87,
	71, 71, 103,
	79, 79, 115,
	91, 91, 127,
	99, 99, 139,
	107, 107, 151,
	115, 115, 163,
	123, 123, 175,
	131, 131, 187,
	139, 139, 203,
	0, 0, 0,
	7, 7, 0,
	11, 11, 0,
	19, 19, 0,
	27, 27, 0,
	35, 35, 0,
	43, 43, 7,
	47, 47, 7,
	55, 55, 7,
	63, 63, 7,
	71, 71, 7,
	75, 75, 11,
	83, 83, 11,
	91, 91, 11,
	99, 99, 11,
	107, 107, 15,
	7, 0, 0,
	15, 0, 0,
	23, 0, 0,
	31, 0, 0,
	39, 0, 0,
	47, 0, 0,
	55, 0, 0,
	63, 0, 0,
	71, 0, 0,
	79, 0, 0,
	87, 0, 0,
	95, 0, 0,
	103, 0, 0,
	111, 0, 0,
	119, 0, 0,
	127, 0, 0,
	19, 19, 0,
	27, 27, 0,
	35, 35, 0,
	47, 43, 0,
	55, 47, 0,
	67, 55, 0,
	75, 59, 7,
	87, 67, 7,
	95, 71, 7,
	107, 75, 11,
	119, 83, 15,
	131, 87, 19,
	139, 91, 19,
	151, 95, 27,
	163, 99, 31,
	175, 103, 35,
	35, 19, 7,
	47, 23, 11,
	59, 31, 15,
	75, 35, 19,
	87, 43, 23,
	99, 47, 31,
	115, 55, 35,
	127, 59, 43,
	143, 67, 51,
	159, 79, 51,
	175, 99, 47,
	191, 119, 47,
	207, 143, 43,
	223, 171, 39,
	239, 203, 31,
	255, 243, 27,
	11, 7, 0,
	27, 19, 0,
	43, 35, 15,
	55, 43, 19,
	71, 51, 27,
	83, 55, 35,
	99, 63, 43,
	111, 71, 51,
	127, 83, 63,
	139, 95, 71,
	155, 107, 83,
	167, 123, 95,
	183, 135, 107,
	195, 147, 123,
	211, 163, 139,
	227, 179, 151,
	171, 139, 163,
	159, 127, 151,
	147, 115, 135,
	139, 103, 123,
	127, 91, 111,
	119, 83, 99,
	107, 75, 87,
	95, 63, 75,
	87, 55, 67,
	75, 47, 55,
	67, 39, 47,
	55, 31, 35,
	43, 23, 27,
	35, 19, 19,
	23, 11, 11,
	15, 7, 7,
	187, 115, 159,
	175, 107, 143,
	163, 95, 131,
	151, 87, 119,
	139, 79, 107,
	127, 75, 95,
	115, 67, 83,
	107, 59, 75,
	95, 51, 63,
	83, 43, 55,
	71, 35, 43,
	59, 31, 35,
	47, 23, 27,
	35, 19, 19,
	23, 11, 11,
	15, 7, 7,
	219, 195, 187,
	203, 179, 167,
	191, 163, 155,
	175, 151, 139,
	163, 135, 123,
	151, 123, 111,
	135, 111, 95,
	123, 99, 83,
	107, 87, 71,
	95, 75, 59,
	83, 63, 51,
	67, 51, 39,
	55, 43, 31,
	39, 31, 23,
	27, 19, 15,
	15, 11, 7,
	111, 131, 123,
	103, 123, 111,
	95, 115, 103,
	87, 107, 95,
	79, 99, 87,
	71, 91, 79,
	63, 83, 71,
	55, 75, 63,
	47, 67, 55,
	43, 59, 47,
	35, 51, 39,
	31, 43, 31,
	23, 35, 23,
	15, 27, 19,
	11, 19, 11,
	7, 11, 7,
	255, 243, 27,
	239, 223, 23,
	219, 203, 19,
	203, 183, 15,
	187, 167, 15,
	171, 151, 11,
	155, 131, 7,
	139, 115, 7,
	123, 99, 7,
	107, 83, 0,
	91, 71, 0,
	75, 55, 0,
	59, 43, 0,
	43, 31, 0,
	27, 15, 0,
	11, 7, 0,
	0, 0, 255,
	11, 11, 239,
	19, 19, 223,
	27, 27, 207,
	35, 35, 191,
	43, 43, 175,
	47, 47, 159,
	47, 47, 143,
	47, 47, 127,
	47, 47, 111,
	47, 47, 95,
	43, 43, 79,
	35, 35, 63,
	27, 27, 47,
	19, 19, 31,
	11, 11, 15,
	43, 0, 0,
	59, 0, 0,
	75, 7, 0,
	95, 7, 0,
	111, 15, 0,
	127, 23, 7,
	147, 31, 7,
	163, 39, 11,
	183, 51, 15,
	195, 75, 27,
	207, 99, 43,
	219, 127, 59,
	227, 151, 79,
	231, 171, 95,
	239, 191, 119,
	247, 211, 139,
	167, 123, 59,
	183, 155, 55,
	199, 195, 55,
	231, 227, 87,
	127, 191, 255,
	171, 231, 255,
	215, 255, 255,
	103, 0, 0,
	139, 0, 0,
	179, 0, 0,
	215, 0, 0,
	255, 0, 0,
	255, 243, 147,
	255, 247, 199,
	255, 255, 255,
	159, 91, 83
};


Mdl::Mdl()
{
	_sign = true;
	_triangle = NULL;
	_vertex = NULL;
	_skin = NULL;
	_frame = NULL;
}

Mdl::Mdl(char *filename)
{
	Load(filename);
}

//FIXME: Not finished
Mdl::~Mdl()
{
	int i, j;


	if (_triangle)
		delete [] _triangle;

	if (_vertex)
		delete [] _vertex;

	if (_skin)
	{ 
		for (i = 0; i < _numskins; i++)
		{     
			if (_skin[i].pixmap)
				delete [] _skin[i].pixmap;

			if (_skin[i].gskin_interval)
				delete [] _skin[i].gskin_interval;
		}

		delete [] _skin;
	}

	if (_frame)
	{
		for (i = 0; i < _numframes; i++)
		{
			if (_frame[i].gframe.gsframe)
			{
				for(j = 0; j < (int)_frame[i].gframe.num_gsframes; j++)
				{
					if (_frame[i].gframe.gsframe[j].tv)
						delete [] _frame[i].gframe.gsframe[j].tv;
				}
	    
				delete _frame[i].gframe.gsframe;
			}

			if (_frame[i].sframe.tv)
				delete [] _frame[i].sframe.tv;

			if (_frame[i].gframe.gsframe_interval)
				delete [] _frame[i].gframe.gsframe_interval;
		}

		delete _frame;
	}
}

void Mdl::Header(MDL_t *header)
{
	if (!header)
		return;

	_ident = header->ident;
	_version = header->version;
	_scale[0] = header->scale[0];
	_scale[1] = header->scale[1];
    _scale[2] = header->scale[2];
	_scale_origin[0] = header->scale_origin[0];
	_scale_origin[1] = header->scale_origin[1];
	_scale_origin[2] = header->scale_origin[2];
	_boundingradius = header->boundingradius;
	_eyeposition[0] = header->eyeposition[0];
	_eyeposition[1] = header->eyeposition[1];
	_eyeposition[2] = header->eyeposition[2];
	_numskins = header->numskins;   
	_skinwidth = header->skinwidth;                            
	_skinheight = header->skinheight;    
	_numverts = header->numverts;     
	_numtris = header->numtris;      
	_numframes = header->numframes;    
	_synctype = header->synctype;    
	_flags = header->flags;       
	_size = header->size;     
}

MDL_t *Mdl::Header()
{
	MDL_t *header = new MDL_t;

	header->ident = _ident;
	header->version = _version;
	header->scale[0] = _scale[0];
	header->scale[1] = _scale[1];
	header->scale[2] = _scale[2];
	header->scale_origin[0] = _scale_origin[0];
	header->scale_origin[1] = _scale_origin[1];
	header->scale_origin[2] = _scale_origin[2];
	header->boundingradius = _boundingradius;
	header->eyeposition[0] = _eyeposition[0];
	header->eyeposition[1] = _eyeposition[1];
	header->eyeposition[2] = _eyeposition[2];
	header->numskins = _numskins;   
	header->skinwidth = _skinwidth;                            
	header->skinheight = _skinheight;    
	header->numverts = _numverts;     
	header->numtris = _numtris;      
	header->numframes = _numframes;    
	header->synctype = _synctype;    
	header->flags = _flags;       
	header->size = _size;  

	return header;
}

stvert_t *Mdl::Vertex()
{
	return _vertex;
}

void Mdl::Vertex(stvert_t *vertex)
{
	_vertex = vertex;
}

dtriangle_t *Mdl::Triangle()
{
	return _triangle;
}

void Mdl::Triangle(dtriangle_t *triangle)
{
	_triangle = triangle;
}

unsigned char *Mdl::getTextureRGB24(palette_type type, int index)
{
	unsigned char *palette = mPaletteQuake;
	unsigned char *image;
	unsigned int size, i;


	if (index < 0 || index > _numskins)
		return 0x0;

	size = getTextureWidth() * getTextureHeight();
	image = new unsigned char[size*3];

	switch (type)
	{
	case Quake:
		palette = mPaletteQuake;
		break;
	case Hexen2:
		palette = mPaletteHexen2;
		break;
	}

	for (i = 0; i < size; ++i)
	{
		image[i*3+0] = palette[_skin[index].pixmap[i]*3];
		image[i*3+1] = palette[_skin[index].pixmap[i]*3+1];
		image[i*3+2] = palette[_skin[index].pixmap[i]*3+2];
	}

	return image;
}

int Mdl::getTextureWidth()
{
	return _skinwidth;
}

int Mdl::getTextureHeight()
{
	return _skinheight;
}

void Mdl::Skin(MDL_Skin_t *skin)
{
	_skin = skin;
}

MDL_Frame_t *Mdl::Frame()
{
	return _frame;
}

void Mdl::Frame(MDL_Frame_t *frame)
{
	_frame = frame;
}

void Mdl::Sign(bool set)
{
	_sign = set;
}

int Mdl::Save(char *filename)
{
	int i, k, ii, jj, skin_size;
	unsigned int j;
	FILE *f;
	char tag[64];

   
	if (!(_vertex && _triangle && _skin && _frame))
	{
		printf("Mdl::Save> Invalid MDL was loaded in memory.\n");
		return -1;
	}

	if (_sign)
		sprintf(tag, "\n[MDDC by Mongoose<stu7440@westga.edu>]");

	f = fopen(filename, "wb");

	if (!f)
	{  
		printf("Mdl::Save> Couldn't write %s.\n", filename);
		return -2;
	}
	else
		printf("Mdl::Save> Exporting '%s' as MDL.\n", filename);

	/* Mongoose: Start header */

	fwrite(&_ident,4,1,f);
	fwrite(&_version,4,1,f);
	fwrite(&_scale[0],4,1,f);
	fwrite(&_scale[1],4,1,f); 
	fwrite(&_scale[2],4,1,f);
	fwrite(&_scale_origin[0],4,1,f);
	fwrite(&_scale_origin[1],4,1,f);
	fwrite(&_scale_origin[2],4,1,f);
	fwrite(&_boundingradius,4,1,f);
	fwrite(&_eyeposition[0],4,1,f);           
	fwrite(&_eyeposition[1],4,1,f);
	fwrite(&_eyeposition[2],4,1,f);
	fwrite(&_numskins,4,1,f);
	fwrite(&_skinwidth,4,1,f);
	fwrite(&_skinheight,4,1,f);
	fwrite(&_numverts,4,1,f);
	fwrite(&_numtris,4,1,f);
	fwrite(&_numframes,1,4,f);
	fwrite(&_synctype,1,4,f); 
	fwrite(&_flags,4,1,f);     
	fwrite(&_size,4,1,f);  
   
	/* Mongoose: End header */

	/* Mongoose: Skin info */
	for(k = 0; k < _numskins; k++)
	{
		fwrite(&_skin[k].type, 4, 1, f);

		if(_skin[k].type == 0)
		{
			skin_size = _skinwidth * _skinheight;

			for(i = 0; i < skin_size; i++)
				fputc(_skin[k].pixmap[i], f);
		}
		else
		{
			fwrite(&_skin[k].num_gskins, 4, 1, f);

			for(j = 0; j < _skin[k].num_gskins; j++)
			{
				fwrite(&_skin[k].gskin_interval[j], 4, 1, f);
			}

			for(j = 0; j < _skin[k].num_gskins; j++)
			{
				for(jj = 0; jj < _skinheight; jj++)
				{
					for(ii = 0; ii < _skinwidth; ii++)
					{
						fputc(_skin[k].gskin[j].pixmap[ii+_skinwidth*jj],
							  f);
					}
				}
			}
		}
	}
	printf("Mdl::Save> Wrote %i skins.\n", k);

	/* Mongoose: Skin 2d Vertices */
	for(i = 0; i < _numverts; i++)
	{
		fwrite(&_vertex[i].onseam, 4, 1, f);
		fwrite(&_vertex[i].s, 4, 1,f);
		fwrite(&_vertex[i].t, 4, 1, f);
	}
	printf("Mdl::Save> Wrote %i vertices.\n", i);

	/* Mongoose: Triangle definitions */
	for(i = 0; i < _numtris; i++)
	{
		fwrite(&_triangle[i].facesfront, 4, 1, f);
		fwrite(&_triangle[i].vertindex[0], 4, 1, f);
		fwrite(&_triangle[i].vertindex[1], 4, 1, f);
		fwrite(&_triangle[i].vertindex[2], 4, 1, f);
	}
	printf("Mdl::Save> Wrote %i triangles.\n", i);

	/* Mongoose: Frames */
	for(k = 0; k < _numframes;  k++)
	{
		//_frame[k].type = 1;  // FIXME: hack to force multi frames

		fwrite(&_frame[k].type,4,1,f); 

		if (_frame[k].type == 0)  
		{
			fwrite(&_frame[k].sframe.bboxmin,1,4,f);
			fwrite(&_frame[k].sframe.bboxmax,1,4,f);
			fwrite(_frame[k].sframe.name,16,1,f);

			for(i=0;i<_numverts;i++)
			{
				fwrite(_frame[k].sframe.tv+i,1,4,f);
			}
		}
		else
		{
			fwrite(&_frame[k].gframe.num_gsframes,1,4,f);
			fwrite(&_frame[k].gframe.bboxmin,1,4,f);
			fwrite(&_frame[k].gframe.bboxmax,1,4,f);
			for(j=0; j<_frame[k].gframe.num_gsframes; j++)
			{
				fwrite(&_frame[k].gframe.gsframe_interval[j],4,1,f);
			}

			for(j=0; j<_frame[k].gframe.num_gsframes; j++)
			{
				fwrite(&_frame[k].gframe.gsframe[j].bboxmin,1,4,f);
				fwrite(&_frame[k].gframe.gsframe[j].bboxmax,1,4,f);
				fwrite(_frame[k].gframe.gsframe[j].name,16,1,f);

				for(i=0;i< _numverts;i++)
				{
					fwrite(_frame[k].gframe.gsframe[j].tv+i,1,4,f);
				}
			}
		}
	}
	printf("Mdl::Save> Wrote %i frames.\n", k);

	if (_sign)
		fwrite(tag, 1, strlen(tag), f);

	fclose(f);

	return 0;
}

bool fread_int_small(int *ptr, SystemIO::FileReader &r)
{
	*ptr = r.ReadInt32();
	return !r.IsEndOfFile();
}


bool fread_u_int_small(unsigned int *ptr, SystemIO::FileReader &r)
{
	*ptr = r.ReadInt32U();
	return !r.IsEndOfFile();
}


bool fread_float_small(float *ptr, SystemIO::FileReader &r)
{
	*ptr = r.ReadFloat32();
	return !r.IsEndOfFile();
}

bool fread_buffer(unsigned char *buffer, unsigned long sz, SystemIO::FileReader &r)
{
	return !r.ReadBuffer(sz, buffer);
}


int Mdl::Load(char *filename)
{
	SystemIO::FileReader f;
	int i, skin_sz;
	unsigned j;


	if (!f.Open(filename))
	{
		printf("Mdl::Load> File '%s' not found.\n", filename);
		return -1;
	}

	if (!fread_int_small(&_ident, f))
		return -2;

	if (!fread_int_small(&_version, f))
		return -3;

	if (_ident != (('O'<<24)+('P'<<16)+('D'<<8)+'I'))
	{
		printf("Mdl::Load> Expected MDL IDPO header.\n");
		return -4;
	}

	if (_version != 6)
	{
		printf("Mdl::Load> Expected MDL version 6.\n");
		return -5;
	}

	if (!fread_float_small(&_scale[0], f))
		return -6;

	if (!fread_float_small(&_scale[1], f))
		return -7;

	if (!fread_float_small(&_scale[2], f))
		return -8;

	if (!fread_float_small(&_scale_origin[0], f))
		return -9;

	if (!fread_float_small(&_scale_origin[1], f))
		return -10;

	if (!fread_float_small(&_scale_origin[2], f))
		return -11;

	if (!fread_float_small(&_boundingradius, f))
		return -12;

	if (!fread_float_small(&_eyeposition[0], f))
		return -13;

	if (!fread_float_small(&_eyeposition[1], f))
		return -14;

	if (!fread_float_small(&_eyeposition[2], f))
		return -15;

	if (!fread_int_small(&_numskins, f))
		return -16;

	if (!fread_int_small(&_skinwidth, f))
		return -17;

	if (!fread_int_small(&_skinheight, f))
		return -18;

	if (!fread_int_small(&_numverts, f))
		return -19;

	if (!fread_int_small(&_numtris, f))
		return -20;

	if (!fread_int_small(&_numframes, f))
		return -21;

	if (!fread_int_small(&_synctype, f))
		return -22;

	if (!fread_int_small(&_flags, f))
		return -23;

	if (!fread_float_small(&_size, f))
		return -24;


	/* Read in the skin(s) */

	_skin = new MDL_Skin_t[_numskins];

	skin_sz = _skinheight * _skinwidth;

	for(i = 0; i < _numskins; i++)
	{
		if (!fread_u_int_small(&_skin[i].type, f))
			return -25;

		if (_skin[i].type == 0)
		{
			_skin[i].pixmap = new unsigned char[skin_sz];

			if (fread_buffer(_skin[i].pixmap, skin_sz, f))
			{
				printf("Mdl::Load> Error in skin fread %ibytes.\n", skin_sz);
				return -26;
			}

			_skin[i].num_gskins = 0;
			_skin[i].gskin_interval = NULL;
			_skin[i].gskin = NULL;
		}
		else
		{      
			if (!fread_u_int_small(&_skin[i].num_gskins, f))
				return -27;

			if (_skin[i].num_gskins)
				_skin[i].gskin_interval = new float[_skin[i].num_gskins];
			else
				_skin[i].gskin_interval = NULL;

			for(j = 0; j < _skin[i].num_gskins; j++)
				if (!fread_float_small(&_skin[i].gskin_interval[j], f))
					return -28;

			if (_skin[i].num_gskins)
				_skin[i].gskin = new MDL_GSkin_t[_skin[i].num_gskins];
			else
				_skin[i].gskin = NULL;

			for(j = 0; j < _skin[i].num_gskins; j++)
			{
				_skin[i].gskin[j].pixmap = new unsigned char[skin_sz];

				if (fread_buffer(_skin[i].gskin[j].pixmap, skin_sz, f))
				{
					printf("Mdl::Load> Error in group skin fread.\n");
					return -29;
				}
			}
		}
	}
	printf("Mdl::Load> Read %i skin(s).\n", _numskins);

	/* Read in the texture's 2d vertex list mapping */


	_vertex = new stvert_t[_numverts];

	for(i = 0; i < _numverts; i++)
	{
		if (!fread_int_small(&_vertex[i].onseam, f))
			return -30;

		if (!fread_int_small(&_vertex[i].s, f))
			return -31;

		if (!fread_int_small(&_vertex[i].t, f))
			return -32;
	}

	printf("Mdl::Load> Read %i texels.\n", _numverts);

	/* Read in the index triangle list */

	_triangle = new dtriangle_t[_numtris];

	for(i = 0; i < _numtris; i++)
	{
		if (!fread_int_small(&_triangle[i].facesfront, f))
			return -33;

		if (!fread_int_small(&_triangle[i].vertindex[0], f))
			return -34;

		if (!fread_int_small(&_triangle[i].vertindex[1], f))
			return -35;

		if (!fread_int_small(&_triangle[i].vertindex[2], f))
			return -36;
	}
	printf("Mdl::Load> Read %i triangles.\n", _numtris);

	/* Read in frame animation information */

	_frame = new MDL_Frame_t[_numframes];

	printf("Mdl::Load> Reading %i frames\n", _numframes);

	for (i = 0; i < _numframes; i++)
	{
		if (!fread_u_int_small(&_frame[i].type, f))
			return -37;

		_frame[i].gframe.num_gsframes = 0;
		_frame[i].gframe.gsframe_interval = NULL;       	
		_frame[i].gframe.gsframe = NULL;

		if (_frame[i].type == 0)
		{
			if (fread_buffer((unsigned char*)&_frame[i].sframe.bboxmin, 4, f))
			{
				printf("Mdl::Load> frame bbox min fread failed.\n");
				return -38;
			}

			if (fread_buffer((unsigned char*)&_frame[i].sframe.bboxmax, 4, f))
			{
				printf("Mdl::Load> frame bbox max fread failed.\n");
				return -39;
			}

			if (fread_buffer((unsigned char*)_frame[i].sframe.name, 16, f))
			{
				printf("Mdl::Load> frame name fread failed.\n");
				return -40;
			}
			else
				printf("Mdl::Load> Frame[%i] %s\n", i, _frame[i].sframe.name);

			_frame[i].sframe.tv = new trivertex_t[_numverts];

			if (fread_buffer((unsigned char*)_frame[i].sframe.tv, 4 * _numverts, f))
			{
				printf("Mdl::Load> Frame vertex[%i] fread failed.\n", i);
				return -41;
			}
		}
		else
		{
			if (!fread_u_int_small(&_frame[i].gframe.num_gsframes, f))
				return -42;

			if (fread_buffer((unsigned char*)&_frame[i].gframe.bboxmin, 4, f))
				return -43;

			if (fread_buffer((unsigned char*)&_frame[i].gframe.bboxmax, 4, f))
				return -44;

			_frame[i].gframe.gsframe_interval = 
			new float[_frame[i].gframe.num_gsframes];

			_frame[i].gframe.gsframe =
			new MDL_MFrame_t[_frame[i].gframe.num_gsframes];

			for (j = 0; j < _frame[i].gframe.num_gsframes; j++)
			{
				if (!fread_float_small(&_frame[i].gframe.gsframe_interval[j], f))
					return -45;
			}

			for (j = 0; j < _frame[i].gframe.num_gsframes; j++)
			{
				if (fread_buffer((unsigned char*)&_frame[i].gframe.gsframe[j].bboxmin, 4, f))
					return -46;

				if (fread_buffer((unsigned char*)&_frame[i].gframe.gsframe[j].bboxmax, 4, f))
					return -47;

				if (fread_buffer((unsigned char*)&_frame[i].gframe.gsframe[j].name, 16, f))
					return -48;
				else
					printf("Mdl::Load> GFrame[%i][%i] %s\n", 
						   i, j, _frame[i].gframe.gsframe[j].name);

				_frame[i].gframe.gsframe[j].tv = new trivertex_t[_numverts];

				if (fread_buffer((unsigned char*)&_frame[i].gframe.gsframe[j].tv,4*_numverts,f))
					return -49;
			}
		}
	}

	f.Close();

	return 0;
}


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS

#include <string.h>
#include <stdio.h>
#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/Vector.h>

using namespace mstl;

extern "C" {

  int freyja_model__mdl_check(char *filename);
  int freyja_model__mdl_import(char *filename);
  int freyja_model__mdl_export(char *filename);
  int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginName1s("mdl");
	freyjaPluginDescription1s("Quake Model (*.mdl)");
	freyjaPluginAddExtention1s("*.mdl");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}


int import_model(char *filename)
{
	if (!freyja_model__mdl_check(filename))
		return freyja_model__mdl_import(filename);

	return -1;
}

int freyja_model__mdl_check(char *filename)
{
   FILE *f;
   char header[32];


   f = fopen(filename, "rb");

   if (!f)
   {
     perror("mdl_check> fopen failed");
     return -1;
   }

   fread(&header, 32, 1, f);      
   fclose(f);

   header[4] = 0;

   // Quake, Hexen2
   if (strcmp(header, "IDPO") == 0)
     return 0;

   return -2;
}
 

int freyja_model__mdl_import(char *filename)
{
	float x, y, z, s, t;
	int i, f, v, num_frames;
	MDL_t *header;
	dtriangle_t *tris;
	MDL_Frame_t *frame;
	stvert_t *st_vert;
	Mdl mdl;
	Vector<long> transV;
	unsigned int vertex;
	unsigned char *image;


	if (mdl.Load(filename))
		return -1;

	/* Mongoose 2004.04.22, 
	 * Store both a Quake and Hexen2 texture for now */
	image = mdl.getTextureRGB24(Mdl::Quake, 0);

	if (image)
	{
		freyjaTextureCreateBuffer(image, 3,
								 mdl.getTextureWidth(), mdl.getTextureHeight(),
								 RGB_24);
	}
  
	image = mdl.getTextureRGB24(Mdl::Hexen2, 0);
	
	if (image)
	{
		freyjaTextureCreateBuffer(image, 3,
								 mdl.getTextureWidth(), mdl.getTextureHeight(), 
								 RGB_24);
	}

	header = mdl.Header();
	frame = mdl.Frame();
	tris = mdl.Triangle();
	st_vert = mdl.Vertex();

	num_frames = header->numframes;

	// Start a new mesh
	freyjaBegin(FREYJA_MESH);

	for (f = 0; f < 1/*num_frames*/; ++f)
	{
		// Start a new vertex group
		if (!f)
		{
			freyjaBegin(FREYJA_VERTEX_GROUP);  
		}
		else
		{
			freyjaBegin(FREYJA_VERTEX_FRAME);
		}

		for (v = 0; v < header->numverts; v++)
		{
			x = frame[f].sframe.tv[v].v[0] * header->scale[0]+header->scale_origin[0];
			y = frame[f].sframe.tv[v].v[1] * header->scale[1]+header->scale_origin[1];
			z = frame[f].sframe.tv[v].v[2] * header->scale[2]+header->scale_origin[2];
			
			if (!f)
			{
				// Store vertices in group
				vertex = freyjaVertexCreate3f(x, z, y); // freyja coordinates

				// Generates id translator list
				transV.push_back(vertex);
			}
			//else
			//{
			//	freyjaVertexFrame3f(transV[v], x, z, y);
			//}
		}

		// End FREYJA_GROUP
		freyjaEnd(); 
	}

	/* 0.9.5 vertex morph keyframes */
	index_t mesh = freyjaGetFSMMeshIndex();
	index_t track = freyjaMeshVertexTrackNew(mesh, (num_frames+3)/15.0f, 15.0f);
	vec3_t pos;

	for (f = 1; f < num_frames; ++f)
	{  
		//freyjaPrintMessage("Importing mesh: %d, frame: %d of %d", 
		//				m, f, md3_mesh[m].num_frames);
		index_t key = freyjaMeshVertexKeyFrameNew(mesh, 
												  track, (vec_t)f/15.0f);
		
		for (v = 0; v < header->numverts; ++v)
		{
			pos[0] = frame[f].sframe.tv[v].v[0] * header->scale[0]+header->scale_origin[0];
			pos[1] = frame[f].sframe.tv[v].v[1] * header->scale[1]+header->scale_origin[1];
			pos[2] = frame[f].sframe.tv[v].v[2] * header->scale[2]+header->scale_origin[2];
			
			freyjaMeshVertexKeyFrame3f(mesh, track, key,
									   transV[v],
									   pos[1], pos[2], pos[0]);
		}
	}
	/* End 0.9.5 vertex morph keyframes */


    for (i = 0; i < header->numtris; ++i)
	{
		// Start a new polygon
		freyjaBegin(FREYJA_POLYGON);

		// Store vertices by true id, using translator list
		freyjaPolygonVertex1i(transV[tris[i].vertindex[0]]);
		freyjaPolygonVertex1i(transV[tris[i].vertindex[1]]);
		freyjaPolygonVertex1i(transV[tris[i].vertindex[2]]);

		// Store texels into model and polygon ( store both at once in MDL )
		s = (float)st_vert[tris[i].vertindex[0]].s/(float)mdl.getTextureWidth();
		t = (float)st_vert[tris[i].vertindex[0]].t/(float)mdl.getTextureHeight();
      
		if (st_vert[tris[i].vertindex[0]].onseam == MDL_ALIAS_ONSEAM &&
			tris[i].facesfront == 0)
			s = (float)(st_vert[tris[i].vertindex[0]].s + mdl.getTextureWidth()/2) / 
			(float)mdl.getTextureWidth();

		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(s, t));
      
		s = (float)st_vert[tris[i].vertindex[1]].s / (float)mdl.getTextureWidth();
		t = (float)st_vert[tris[i].vertindex[1]].t / (float)mdl.getTextureHeight();

		if (st_vert[tris[i].vertindex[1]].onseam == MDL_ALIAS_ONSEAM &&
			tris[i].facesfront == 0)
			s = (float)(st_vert[tris[i].vertindex[1]].s + mdl.getTextureWidth()/2) / 
			(float)mdl.getTextureWidth();
		
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(s, t));

		s = (float)st_vert[tris[i].vertindex[2]].s / (float)mdl.getTextureWidth();
		t = (float)st_vert[tris[i].vertindex[2]].t / (float)mdl.getTextureHeight();

		if (st_vert[tris[i].vertindex[2]].onseam == MDL_ALIAS_ONSEAM &&
			tris[i].facesfront == 0)
			s = (float)(st_vert[tris[i].vertindex[2]].s + mdl.getTextureWidth()/2) / 
			(float)mdl.getTextureWidth();
		
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(s, t));
		
		freyjaPolygonMaterial1i(0);
		
		// End FREYJA_POLYGON
		freyjaEnd();
	}
	
	// End FREYJA_MESH
	freyjaEnd();
	
	return 0;
}


int freyja_model__mdl_export(char *filename)
{
#define STILL_WORKING_ON_IT
#ifdef STILL_WORKING_ON_IT
  printf("freyja_model__mdl_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);

  return -1;
#else
  MtkImage img;
  Mdl *model;
  MDL_t *mdl;
  stvert_t *vertex; 
  dtriangle_t *triangle;
  MDL_Frame_t *frame;
  MDL_Skin_t *qskin;
  freyja_vertex_list_t *evertex;
  freyja_frame_list_t *eframe;
  freyja_polygon_list_t *polygon;
  char buffer[32];
  int i, j;
  float radius;
  int skin_w;
  int skin_h;
  unsigned char *skin;
  freyja_mesh_t *mesh;
  int err = 0;
  float bbox_min[3];
  float epos[3] = {0.0, 64.0, 0.0}; // FIXME: Hack
  bool a, b, c;


  mesh = freyja->FindMesh(mesh_id);

  // Load texture
  img.Load(freyja_rc_map("textures/mdltest.pcx"));

  // Convert into quake palette
  img.PaletteLoad(freyja_rc_map("palettes/quake.pal"));
  img.IndexedImage(&skin);
  skin_w = img.Width();
  skin_h = img.Height();

  if (!mesh || !mesh->frames || !skin)
  {
    mtkPrint("FreyjaModel::MdlExport> Assertion error missing skin or mesh\n");
    return -1;
  }

  printf("FreyjaModel::MdlExport> WARNING this is EXPERIMENTAL\n");

  if (mesh->frames->frame)
  {
    radius =  fabs(mesh->frames->frame->bbox_min[0]);
    
    if (fabs(mesh->frames->frame->bbox_min[1]) > radius) 
      radius = mesh->frames->frame->bbox_min[1];
    if (fabs(mesh->frames->frame->bbox_min[2]) > radius) 
      radius = mesh->frames->frame->bbox_min[2];
    
    if (fabs(mesh->frames->frame->bbox_max[0]) > radius) 
      radius = mesh->frames->frame->bbox_max[0];
    if (fabs(mesh->frames->frame->bbox_max[1]) > radius) 
      radius = mesh->frames->frame->bbox_max[1];
    if (fabs(mesh->frames->frame->bbox_max[2]) > radius) 
      radius = mesh->frames->frame->bbox_max[2];
  }
  else
  {
    radius = 24;
    printf("FreyjaModel::MdlExport> ERROR bad raduis\n");
  }

  // Force all polygons to triangles
  //freyja->tesselate(mesh); 

  // Initialize MDL header
  mdl = new MDL_t;

  mdl.ident = MDL_IDPOLYGON;
  mdl.version = MDL_ALIAS_VERSION;
  mdl.boundingradius = radius;
  mdl.skinwidth = skin_w;
  mdl.skinheight = skin_h;
  mdl.scale[0] = 1.0;
  mdl.scale[1] = 1.0;
  mdl.scale[2] = 1.0;
  mdl.scale_origin[0] = mesh->frames->frame->center[0];
  mdl.scale_origin[1] = mesh->frames->frame->center[1];
  mdl.scale_origin[2] = mesh->frames->frame->center[2]; 
  mdl.numskins = 1;
  mdl.numverts = mesh->frames->frame->vertex_count;
  mdl.numtris = mesh->polygon_count; 
  mdl.numframes = mesh->frame_count; 
  mdl.flags = flags;
  mdl.eyeposition[0] = epos[0];
  mdl.eyeposition[1] = epos[1];
  mdl.eyeposition[2] = epos[2];
  mdl.synctype = ST_SYNC;       
  mdl.size = 3.0;              

  vertex = new stvert_t[mdl.numverts];
  triangle = new dtriangle_t[mdl.numtris];
  frame = new MDL_Frame_t[mdl.numframes];
  qskin = new MDL_Skin_t;

  qskin->type = 0;
  qskin->pixmap = skin;
  qskin->gskin_interval = NULL;

  polygon = mesh->polys;

  for (i = 0; i < mdl.numtris && polygon; i++)
  {
    if (polygon->poly && polygon->poly->index_list)
    {
      triangle[i].vertindex[0] = polygon->poly->index_list[0];
      triangle[i].vertindex[1] = polygon->poly->index_list[1];
      triangle[i].vertindex[2] = polygon->poly->index_list[2];

      vertex[triangle[i].vertindex[0]].s = (int)(skin_w *polygon->poly->st[0]);
      vertex[triangle[i].vertindex[0]].t = (int)(skin_h *polygon->poly->st[1]);
      vertex[triangle[i].vertindex[0]].onseam = 0;

      vertex[triangle[i].vertindex[1]].s = (int)(skin_w *polygon->poly->st[2]);
      vertex[triangle[i].vertindex[1]].t = (int)(skin_h *polygon->poly->st[3]);
      vertex[triangle[i].vertindex[1]].onseam = 0;

      vertex[triangle[i].vertindex[2]].s = (int)(skin_w *polygon->poly->st[4]);
      vertex[triangle[i].vertindex[2]].t = (int)(skin_h *polygon->poly->st[5]);
      vertex[triangle[i].vertindex[2]].onseam = 0;

      /*****************
      // From loader code, this shows how onseam and facesfront change
      // texel info on load

	if (st_vert[tris[i].vertindex[1]].onseam == MDL_ALIAS_ONSEAM &&
	    tris[i].facesfront == 0)
	  st[2+0] = (float)(st_vert[tris[i].vertindex[1]].s + 
	             mdl.SkinWidth()/2) / (float)mdl.SkinWidth();
      *****************/

      ////////////////////////////////////////////////////
      // Figure out facesfront set for value triangle
      // it faces front if s < 0.5, t isn't considered
      ////////////////////////////////////////////////////
      a = b = c = false;

      if (vertex[triangle[i].vertindex[0]].s < skin_w/2)
	a = true;

      if (vertex[triangle[i].vertindex[1]].s < skin_w/2)
	b = true;

      if (vertex[triangle[i].vertindex[2]].s < skin_w/2)
	c = true;
    
      if (a && b && c)
	triangle[i].facesfront = MDL_DT_FACES_FRONT;
      else
	triangle[i].facesfront = 0;

      ////////////////////////////////////////////////////
      // Figure out onseam value
      ////////////////////////////////////////////////////
//#define TEST_MDL_ONSEAM
#ifdef TEST_MDL_ONSEAM
      if (!a && (b || c))
	vertex[triangle[i].vertindex[0]].onseam = MDL_ALIAS_ONSEAM;

      if (!b && ( a || c))
	vertex[triangle[i].vertindex[1]].onseam = MDL_ALIAS_ONSEAM;

      if (!c && ( a || b))
	vertex[triangle[i].vertindex[2]].onseam = MDL_ALIAS_ONSEAM;
#endif
    }
    else
    {
      // FIXME: Handle error
      printf("FreyjaModel::MdlExport> ERROR in polygon, tris[%i]\n", i);
      triangle[i].vertindex[0] = 0;
      triangle[i].vertindex[1] = 0;
      triangle[i].vertindex[2] = 0;
    }

    polygon = polygon->next;
  }

  ///////////////////////////////////////////////////////
  // NOTE:
  // The trick of the pack hack is to scale your model
  // down to fit within a 0-255 bbox so it can be
  // converted to 0-255, so we transform the model to
  // and all postive bbox here
  //
  // FIXME: Add scaling to fit into 0-255
  ///////////////////////////////////////////////////////

  eframe = mesh->frames;

  bbox_min[0] = bbox_min[1] = bbox_min[2] = 0.0;

  while (eframe)
  {
    if (eframe->frame)
    {
      if (eframe->frame->bbox_min[0] < bbox_min[0])
	bbox_min[0] = eframe->frame->bbox_min[0];

      if (eframe->frame->bbox_min[1] < bbox_min[1])
	bbox_min[1] = eframe->frame->bbox_min[1];

      if (eframe->frame->bbox_min[2] < bbox_min[2])
	bbox_min[2] = eframe->frame->bbox_min[2];
    }

    eframe = eframe->next;
  }

  freyja->Transform(mesh, TRANSLATE, 
		  fabs(bbox_min[0]), fabs(bbox_min[1]), fabs(bbox_min[2]));

  eframe = mesh->frames;

  for(i = 0; i < mdl.numframes && eframe; i++)
  {
    frame[i].type = 0;  // single frames

    // FIXME: Frame name hack
    sprintf(buffer, "frame%i", i); 

    strcpy(frame[i].sframe.name, buffer);

    frame[i].sframe.tv = new trivertex_t[mdl.numverts];

    if (eframe->frame)
    {
      // Bounding box lightnormals aren't used, so 0 is ok
      frame[i].sframe.bboxmin.lightnormalindex = 0;
      frame[i].sframe.bboxmax.lightnormalindex = 0;

      // Init bboxmin at greatest possible value
      frame[i].sframe.bboxmin.v[0] = 
	frame[i].sframe.bboxmin.v[1] = 
	  frame[i].sframe.bboxmin.v[2] = 255;
       
      // Init bboxmax at least possible value
      frame[i].sframe.bboxmax.v[0] = 
	frame[i].sframe.bboxmax.v[1] = 
	  frame[i].sframe.bboxmax.v[2] = 0;

      evertex = eframe->frame->verts;

      for (j = 0; j < mdl.numverts && evertex; j++)
      {
	if (evertex->vertex)
	{
	  // Convert vertices to packed postions
	  frame[i].sframe.tv[j].v[0] = (unsigned char)
	    ((evertex->vertex->pos[0] - mdl.scale_origin[0]) / mdl.scale[0]);

	  frame[i].sframe.tv[j].v[1] = (unsigned char)
	    ((evertex->vertex->pos[1] - mdl.scale_origin[1]) / mdl.scale[1]);

	  frame[i].sframe.tv[j].v[2] = (unsigned char) 
	    ((evertex->vertex->pos[2] - mdl.scale_origin[2]) / mdl.scale[2]);

	  if (frame[i].sframe.tv[j].v[0] < frame[i].sframe.bboxmin.v[0])
	    frame[i].sframe.bboxmin.v[0] = frame[i].sframe.tv[j].v[0];

	  if (frame[i].sframe.tv[j].v[1] < frame[i].sframe.bboxmin.v[1])
	    frame[i].sframe.bboxmin.v[1] = frame[i].sframe.tv[j].v[1];

	  if (frame[i].sframe.tv[j].v[2] < frame[i].sframe.bboxmin.v[2])
	    frame[i].sframe.bboxmin.v[2] = frame[i].sframe.tv[j].v[2];
	
	  if (frame[i].sframe.tv[j].v[0] > frame[i].sframe.bboxmax.v[0])
	    frame[i].sframe.bboxmax.v[0] = frame[i].sframe.tv[j].v[0];

	  if (frame[i].sframe.tv[j].v[1] > frame[i].sframe.bboxmax.v[1])
	    frame[i].sframe.bboxmax.v[1] = frame[i].sframe.tv[j].v[1];

	  if (frame[i].sframe.tv[j].v[2] > frame[i].sframe.bboxmax.v[2])
	    frame[i].sframe.bboxmax.v[2] = frame[i].sframe.tv[j].v[2];

	  //FIXME: Shouldn't just 'have one'
	  frame[i].sframe.tv[j].lightnormalindex = 1; 
	}

	evertex = evertex->next;
      }
    }

//#define DEBUG_MDL_BBOX
#ifdef DEBUG_MDL_BBOX
    // DEBUG: Dump bbox info to console
    printf("freyja bbox ( %.0f, %.0f, %.0f), ( %.0f, %.0f, %.0f)\n",
	   eframe->frame->bbox_min[0], 
	   eframe->frame->bbox_min[1],
	   eframe->frame->bbox_min[2],
	   eframe->frame->bbox_max[0],
	   eframe->frame->bbox_max[1],
	   eframe->frame->bbox_max[2]);

    printf("mdl bbox ( %u, %u, %u), ( %u, %u, %u)\n",
	   frame[i].sframe.bboxmin.v[0],
	   frame[i].sframe.bboxmin.v[1],
	   frame[i].sframe.bboxmin.v[2],
	   frame[i].sframe.bboxmax.v[0],
	   frame[i].sframe.bboxmax.v[1],
	   frame[i].sframe.bboxmax.v[2]);
#endif

    frame[i].gframe.num_gsframes = 0;
    frame[i].gframe.gsframe = NULL;
    frame[i].gframe.gsframe_interval = NULL;

    eframe = eframe->next;
  }

  model = new Mdl();
  model->Header(mdl);
  model->Vertex(vertex);
  model->Triangle(triangle);
  model->Frame(frame);
  model->Skin(qskin);

  printf("FreyjaModel::MdlExport> Exporting mesh to '%s'\n", filename);
  err = model->Save(filename);

  delete model;

  return err;
#endif
}

#endif


#ifdef TEST_MDL
int main(int argc, char *argv[])
{
	Mdl mdl;


	printf("[MDL class test]\n");

	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
			if (!mdl.Load(argv[2]))
				printf("main: Load reports success.\n");
		}
		else if (strcmp(argv[1], "save") == 0)
		{
			// FIXME: Add code here to generate a small mdl

			if (!mdl.Save(argv[2]))
				printf("main: Save reports success.\n");
		}
		else if (strcmp(argv[1], "test") == 0 && argc > 3)
		{
			if (!mdl.Load(argv[2]))
				printf("main: Load reports success.\n");
			if (!mdl.Save(argv[3]))
				printf("main: Save reports success.\n");
		}
		else
			printf("\n\n%s [save | load | test] filename.mdl [testout.mdl]\n", 
				   argv[0]);
	}
	else
	{
		printf("\n\n%s [save | load | test] filename.mdl [testout.mdl]\n", 
			   argv[0]);
	}
}
#endif

