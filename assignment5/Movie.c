/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Fall 2017                     */
/*                                                                   */
/* Author: Tim Schmidt                                               */
/* Date: 11/09/2017                                                  */
/*                                                                   */
/* Movie.c: source file for basic movie manipulations                */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "Movie.h"
#include "ImageList.h"
#include "Image.h"

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
MOVIE *CreateMovie(void)
{
    MOVIE *movie;

    movie = malloc(sizeof(MOVIE));

    return movie;
}

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(MOVIE *movie)
{
	assert(movie);
	free(movie);
}

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(MOVIE *movie)
{
	IENTRY *temp;
	temp = movie->Frames->First;
	int temp_R, temp_G, temp_B, C, D, E;

	while (temp != NULL) {

		assert(temp->YUVImage);
		assert(temp->YUVImage->Y);
		assert(temp->YUVImage->U);
		assert(temp->YUVImage->V);

		temp->RGBImage = CreateImage(temp->YUVImage->W, temp->YUVImage->H);

		for (int j = 0; j < temp->YUVImage->H; j++) {
			for (int i = 0; i < temp->YUVImage->W; i++) {
				C = GetPixelY(temp->YUVImage, i, j) - 16;
				D = GetPixelU(temp->YUVImage, i, j) - 128;
				E = GetPixelV(temp->YUVImage, i, j) - 128;
				temp_R = (( 298 * C + 409 * E + 128) >> 8);
				temp_G = (( 298 * C - 100 * D - 208 * E + 128) >> 8);
				temp_B = (( 298 * C + 516 * D + 128) >> 8);

				temp_R = (temp_R>255)? 255 : (temp_R<0)? 0 : temp_R;
				temp_G = (temp_G>255)? 255 : (temp_G<0)? 0 : temp_G;
				temp_B = (temp_B>255)? 255 : (temp_B<0)? 0 : temp_B;

				assert(temp->RGBImage);

				SetPixelR(temp->RGBImage, i, j, temp_R);
				SetPixelG(temp->RGBImage, i, j, temp_G);
				SetPixelB(temp->RGBImage, i, j, temp_B);
			}
		}	
		DeleteYUVImage(temp->YUVImage);
		temp->YUVImage = NULL;
		temp = temp->Next;
	}
}

/* Convert a RGB movie to a YUV movie */
void RGB2YUVMovie(MOVIE *movie)
{
    IENTRY *temp;
	temp = movie->Frames->First;
	int temp_Y, temp_U, temp_V;

	while (temp != NULL) {

		assert(temp->RGBImage);
		assert(temp->RGBImage->R);
		assert(temp->RGBImage->G);
		assert(temp->RGBImage->B);

		temp->YUVImage = CreateYUVImage(temp->RGBImage->W, temp->RGBImage->H);

		for (int j = 0; j < temp->RGBImage->H; j++) {
			for (int i = 0; i < temp->RGBImage->W; i++) {
				temp_Y = ( ( 66 * GetPixelR(temp->RGBImage, i, j) + 129 * GetPixelG(temp->RGBImage, i, j) + 25 * GetPixelB(temp->RGBImage, i, j) + 128) >> 8) + 16;
				temp_U = ( ( -38 * GetPixelR(temp->RGBImage, i, j) - 74 * GetPixelG(temp->RGBImage, i, j) + 112 * GetPixelB(temp->RGBImage, i, j) + 128) >> 8) + 128;
				temp_V = ( ( 112 * GetPixelR(temp->RGBImage, i, j) - 94 * GetPixelG(temp->RGBImage, i, j) - 18 * GetPixelB(temp->RGBImage, i, j) + 128) >> 8) + 128;

				temp_Y = (temp_Y>255)? 255 : (temp_Y<0)? 0 : temp_Y;
				temp_U = (temp_U>255)? 255 : (temp_U<0)? 0 : temp_U;
				temp_V = (temp_V>255)? 255 : (temp_V<0)? 0 : temp_V;

				assert(temp->YUVImage);
				assert(temp->YUVImage->Y);
				assert(temp->YUVImage->U);
				assert(temp->YUVImage->V);

				SetPixelY(temp->YUVImage, i, j, temp_Y);
				SetPixelU(temp->YUVImage, i, j, temp_U);
				SetPixelV(temp->YUVImage, i, j, temp_V);
			}
		}	
		DeleteImage(temp->RGBImage);
		temp->RGBImage = NULL;
		temp = temp->Next;
	}
}

/* EOF */