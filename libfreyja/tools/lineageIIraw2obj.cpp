#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *in;
	unsigned int i, u;
	unsigned int wedgeCount, wedgeOffset;
	unsigned int vertCount, vertOffset;
	unsigned int faceCount, faceOffset;
	unsigned short s;
	float f;
	char c;
	float uv[2];

	in = fopen(argv[1], "rb");

	if (!in)
	{
		printf("lineageIIraw2obj filename vertOffset vertCount wedgeOffset wedgeCount faceOffset faceCount\n");
		return -1;
	}

	vertOffset = atoi(argv[2]);
	vertCount = atoi(argv[3]);
	wedgeOffset = atoi(argv[4]);
	wedgeCount = atoi(argv[5]);
	faceOffset = atoi(argv[6]);
	faceCount = atoi(argv[7]);

	printf("# Generated by lineageIIraw2obj\n");
	printf("# %i %i %i %i %i %i\n", 
           vertOffset, vertCount, wedgeOffset, wedgeCount, faceOffset, faceCount);


	float vertices[vertCount][3];

	fseek(in, vertOffset, SEEK_SET);

	for (i = 0; i < vertCount; ++i)
	{
		fread(vertices[i]+0, 4, 1, in);
		fread(vertices[i]+1, 4, 1, in);
		fread(vertices[i]+2, 4, 1, in);
	}


	fseek(in, wedgeOffset, SEEK_SET);

	for (i = 0; i < wedgeCount; ++i)
	{
		fread(&s, 2, 1, in);
		fread(uv+0, 4, 1, in);
		fread(uv+1, 4, 1, in);

		printf("v %f %f %f\n", vertices[s][0],vertices[s][2],vertices[s][1]);
		printf("vt %f %f\n", uv[0], uv[1]);
	}


	fseek(in, faceOffset, SEEK_SET);

	for (i = 0; i < faceCount; ++i)
	{
		printf("f ");
		fread(&s, 2, 1, in);
		printf("%i/%i ", s+1, s+1);
		fread(&s, 2, 1, in);
		printf("%i/%i ", s+1, s+1);
		fread(&s, 2, 1, in);
		printf("%i/%i ", s+1, s+1);
		fread(&c, 1, 1, in); // mat
		fread(&c, 1, 1, in); // mat2
		fread(&u, 4, 1, in); // smoothing group
		printf("\n");
	}

	fclose(in);

	return 0;
}