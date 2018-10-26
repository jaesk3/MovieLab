/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Fall 2016                     */
/*                                                                   */
/* Author: Tim Schmidt                                               */
/* Date: 11/13/2016                                                  */
/*                                                                   */
/* ImageList.h: header file for image list manipulations             */
/*                                                                   */
/*********************************************************************/

#ifndef IMAGELIST_H
#define IMAGELIST_H

#include "Image.h"

typedef struct ImageEntry IENTRY;
typedef struct ImageList ILIST;

struct ImageEntry {
	ILIST *List; /* Pointer to the list which this entry belongs to */
	IENTRY *Next; /* Pointer to the next entry, or NULL */
	IENTRY *Prev; /* Pointer to the previous entry, or NULL */
	IMAGE *RGBImage; /* Pointer to the RGB image, or NULL */
	YUVIMAGE *YUVImage; /* Pointer to the YUV image, or NULL */
};

struct ImageList {
    unsigned int Length;
    IENTRY *First;
    IENTRY *Last;
};

/* Create a new image list */
ILIST *CreateImageList(void);

/* Delete an image list (and all entries) */
void DeleteImageList(ILIST *list);

/* Insert a RGB image to the image list at the end */
void AppendRGBImage(ILIST *list, IMAGE *RGBimage);

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVIMAGE *YUVimage);

/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end);

/* Fast forward an image list */
void FastImageList(ILIST *list, unsigned int factor);

/* Reverse an image list */
void ReverseImageList(ILIST *list);

// Delete the first entry of a list and return a pointer to the next entry
IENTRY *DeleteImageFirstEntry(ILIST *list, IENTRY *temp);

// Delete an entry of a list
void DeleteImageEntry(ILIST *list, IENTRY *temp, IENTRY *temp2);

#endif