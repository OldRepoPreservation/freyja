/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000 Mongoose
 * Comments: Png plug-in
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2000-10-15:
 * Mongoose - Created
 ================================================================*/

#ifndef __MONGOOSE_PPM_H
#define __MONGOOSE_PPM_H

#include <stdio.h>

int mtk_image__ppm_check(FILE *f);

int mtk_image__ppm_load(FILE *f, unsigned char **image, 
			 unsigned int *width, unsigned int *height, char *type);

#endif