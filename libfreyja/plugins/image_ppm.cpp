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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "image_ppm.h"


int mtk_image__ppm_check(FILE *f)
{
  char buffer[10];


  if (!f)
  {
    perror("ppm_check> Passed invalid file.\n");
    return -1;
  }

  /* Read the header */
  fseek(f, 0, SEEK_SET);
  fread(buffer, 8, 1, f);

  if (buffer[0] == 'P' && (buffer[1] == '3' || buffer[1] == '6'))
    return 0;  

  fprintf(stderr, "ppm_load> Unknown image format.\n");
  return -2;
}


char *ppm_get_next_line(char* buffer, size_t size, FILE *f)
{
  char *rc;

  while(1) 
  {
    rc = fgets(buffer, size, f);
    
    if (rc && *rc != '#') 
      return rc;
  }
}


int mtk_image__ppm_load(FILE *f, unsigned char **image, 
			unsigned int *w, unsigned int *h, char *type)
{
  int i, j, n;
  char buffer[256];
  int line;
  char c;
  bool done = false;


  if (!f)
  {
    perror("ppm_load> Failed to open file.\n");
    return -1;
  }
  
  *type = 1;

  fseek(f, 0, SEEK_SET);
  ppm_get_next_line(buffer, sizeof(buffer), f);
  fseek(f, 0, SEEK_SET);
  
  if (strcmp(buffer, "P6\n") == 0) 
  {
    line = 0;
    i = 0;

    while(!done && fread(&c, 1, 1, f) && line < 4)
    { 
      if (i > 250)
	i--;

      switch(c)
      {
      case '\n':
	i = 0;
	line++;
	
	if (line == 3)
	{
	  sscanf(buffer, "%d %d", &(*w), &(*h));
	}
	    
	if (line == 4)
	  done = true;
	break;
      default:
	buffer[i++] = c;
	buffer[i] = 0;
      }
    }

    n = *w * *h;
    *image = new unsigned char[n * 3];

    for (i = 0; i < n; i++)
    {
      fread(&(*image)[i*3], 1, 1, f);
      fread(&(*image)[i*3+1], 1, 1, f);
      fread(&(*image)[i*3+2], 1, 1, f);
    }

    return 0;
  }
  else if (strcmp(buffer, "P3\n") == 0)
  {
    ppm_get_next_line(buffer, sizeof(buffer), f);
    ppm_get_next_line(buffer, sizeof(buffer), f);

    sscanf(buffer, "%d %d", &(*w), &(*h));

    ppm_get_next_line(buffer, sizeof(buffer), f);

    n = *h * *w * 3;

    *image = new unsigned char[n * 3];

    if (!image)
    { 
      printf("ERROR: Could not allocate memory!\n");
      return 2;
    }

    for(i = 0; i < n; i++)  
    {
      fscanf(f, "%u\n", &j);
      (*image)[i] = j;
    }

    return 0;
  }

  return -1;
}