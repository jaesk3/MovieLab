/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Fall 2017                     */
/*                                                                   */
/* Author: Tim Schmidt                                               */
/* Date: 11/09/2017                                                  */
/*                                                                   */
/* ImageList.c: source file for image list manipulations             */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "ImageList.h"
#include "Image.h"

/* Create a new image list */
ILIST *CreateImageList(void)
{
    ILIST *iList;
    iList = malloc(sizeof(ILIST));
    iList->Length = 0;
    iList->First = NULL;
    iList->Last = NULL;

    return iList;
}

/* Delete an image list (and all entries) */
void DeleteImageList(ILIST *list)
{
    IENTRY *temp;
    assert(list);
    temp = list->First;
    assert(temp);

    while (temp != NULL) {
    	temp = DeleteImageFirstEntry(list,temp);
    }
    free(list);
}

/* Insert a RGB image to the image list at the end */
void AppendRGBImage(ILIST *list, IMAGE *RGBimage)
{
	IENTRY *new_entry;
	new_entry = malloc(sizeof(IENTRY));

	new_entry->YUVImage = NULL;
	assert(RGBimage);
	new_entry->RGBImage = RGBimage;
	new_entry->List = list;

	if (list->Last == NULL) {
		list->First = new_entry;
		list->Last = new_entry;
		new_entry->Prev = NULL;
	}
	else {
		list->Last->Next = new_entry;
		new_entry->Prev = list->Last;
		list->Last = new_entry;
	}
	new_entry->Next = NULL;
	list->Length++;
}

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVIMAGE *YUVimage)
{
    IENTRY *new_entry;
	new_entry = malloc(sizeof(IENTRY));

	new_entry->RGBImage = NULL;
	assert(YUVimage);
	new_entry->YUVImage = YUVimage;
	new_entry->List = list;

	if (list->First == NULL) {
		list->First = new_entry;
		list->Last = new_entry;
		new_entry->Prev = NULL;
	}
	else {
		list->Last->Next = new_entry;
		new_entry->Prev = list->Last;
		list->Last = new_entry;
	}
	new_entry->Next = NULL;
	list->Length++;
}


/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end)
{
    assert(start < list->Length);
    assert(end <= list->Length);

    unsigned int count = 1;
    IENTRY *temp;
    IENTRY *temp2;
    temp = list->First;
    temp2 = temp->Next;

    assert(temp);
    while (count < start) {
    	temp = DeleteImageFirstEntry(list, temp);
    	count++;
    }
    list->First = temp;
    while (count < end) {
    	temp = temp->Next;
    	count++;
    }
    list->Last = temp;
    temp = temp->Next;
    temp2 = temp->Next;
    temp->Prev = NULL;
    list->Last->Next = NULL;
    while (temp != NULL){
    	temp->List = NULL;
    	temp->Next = NULL;

    	if (temp->RGBImage != NULL) {
    		IMAGE *temp_image;
    		temp_image = temp->RGBImage;
    		DeleteImage(temp_image);
    		temp_image = NULL;
    	}
    	else if (temp->YUVImage != NULL) {
	    	YUVIMAGE *temp_YUVImage;
	    	temp_YUVImage = temp->YUVImage;
	    	DeleteYUVImage(temp_YUVImage);
	   		temp_YUVImage = NULL;
	   	}
    	
    	if (temp2 != NULL) {
    		temp2->Prev = NULL;
    		free(temp);
    		temp = temp2;
    		temp2 = temp2->Next;
    	}
    	else {
    		free(temp);
    		temp = NULL;
    	}
    	list->Length--;
    }
}

/* Fast forward an image list */
void FastImageList(ILIST *list, unsigned int factor)
{
    IENTRY *temp, *temp2;
    temp = list->First;
    temp2 = temp->Next;
    int count = 1;

    assert(temp);
    assert(temp2);
    while (temp != NULL) {
    	if (count % factor != 1) {
    		DeleteImageEntry(list, temp, temp2);
    	}
    	temp = temp2;
    	if (temp2 != NULL){
    		temp2 = temp2->Next;
    	}
    	count++;
    }
}

/* Reverse an image list */
void ReverseImageList(ILIST *list)
{
   	IENTRY *temp, *temp_next;
   	temp = list->First;
   	temp_next = temp->Next;
   	temp->Next = NULL;

   	assert(temp);
   	assert(temp_next);
   	while (temp != NULL && temp_next != NULL) {
   		temp->Prev = temp_next;
   		temp_next = temp_next->Next;
   		temp->Prev->Next = temp;
   		temp = temp->Prev;
   	}
   	temp->Prev = NULL;
   	temp = list->First;
   	list->First = list->Last;
   	list->Last = temp;
   	temp = NULL;
}

IENTRY *DeleteImageFirstEntry(ILIST *list, IENTRY *temp) 
{
		if (temp->RGBImage != NULL) {
    		IMAGE *temp_image;
    		temp_image = temp->RGBImage;
    		DeleteImage(temp_image);
    		temp_image = NULL;
    	}
    	else if (temp->YUVImage != NULL) {
    		YUVIMAGE *temp_YUVImage;
    		temp_YUVImage = temp->YUVImage;
    		DeleteYUVImage(temp_YUVImage);
    		temp_YUVImage = NULL;
    	}

    	if (temp == list->Last) {
    		temp->List = NULL;
    		list->First = NULL;
    		list->Last = NULL;
    		free(temp);
    		list->Length--;
    		temp = NULL;
    	}
    	else {
	    	assert(temp->Next->Prev);
	    	temp->Next->Prev = NULL;

	    	temp = temp->Next;
	    	list->First->Next = NULL;

	    	assert(list->First->List);
	    	list->First->List = NULL;

	    	free(list->First);
	    	list->First = temp;

	    	list->Length--;
	    }
	return temp;
}

void DeleteImageEntry(ILIST *list, IENTRY *temp, IENTRY *temp2)
{
	if (temp->RGBImage != NULL) {
    	IMAGE *temp_image;
    	temp_image = temp->RGBImage;
   		DeleteImage(temp_image);
    	temp_image = NULL;
    }
    else if (temp->YUVImage != NULL) {
    	YUVIMAGE *temp_YUVImage;
    	temp_YUVImage = temp->YUVImage;
    	DeleteYUVImage(temp_YUVImage);
    	temp_YUVImage = NULL;
    }

    if (temp2 != NULL) {
    	temp2->Prev = temp->Prev;
	    temp->Prev->Next = temp2;
	    temp->Prev = NULL;
	    temp->Next = NULL;
	    temp->List = NULL;
	    free(temp);
	}
	else {
		list->Last = temp->Prev;
		temp->Prev = NULL;
		temp->List = NULL;
		free(temp);
		list->Last->Next = NULL;
	}
	list->Length--;

}
