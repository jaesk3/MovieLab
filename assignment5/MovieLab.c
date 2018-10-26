/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Fall 2017                     */
/*                                                                   */
/* Author: Tim Schmidt                                               */
/* Date: 11/09/2017                                                  */
/*                                                                   */
/* MovieLab.c: source file for the main function                     */
/*                                                                   */
/* Please use this template for your HW5.                            */
/*********************************************************************/

// Jae Kim
// 12/2/17
// Assignment5
// MovieLab.c 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Image.h"

/* Load one movie frame from the input file */
YUVIMAGE* LoadOneFrame(const char* fname, int n,
                       unsigned int width, unsigned height);

/* Load the movie frames from the input file */
MOVIE *LoadMovie(const char *fname, int frameNum,
                 unsigned int width, unsigned height);

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVIMAGE *image, const char *fname, FILE *file);

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie);

/* Print the command-line arguments usage of the program */
void PrintUsage();

int main(int argc, char *argv[])
{
        int x = 0;
        char *fin = NULL;
        char *fout = NULL;

        unsigned int width = 480;
        unsigned int height = 270;
        int frameNum = 0;

        MOVIE *Movie;
        Movie = NULL;

        /* the while loop to check options in the command line */
        while (x < argc) {
                /* the input file name */
                if (strcmp(argv[x], "-i") == 0) {
                        if (x < argc - 1) {
                            fin = argv[x + 1];
                        } 
                        else {
                                printf("Missing argument for the input file name!\n");
                                PrintUsage();
                                return 5;
                        }

                        x += 2;
                        continue;
                } 

                /* the output file name */
                if (strcmp(argv[x], "-o") == 0) {
                        if (x < argc - 1) {
                                fout = argv[x + 1];
                        } /*fi*/
                        else {
                                printf("Missing argument for the output file name!\n");
                                PrintUsage();
                                return 5;
                        } /*esle*/
                        x += 2;
                        continue;
                } /*fi*/

                if (strcmp(argv[x], "-f") == 0) {
                    if (x < argc - 1) {
                        frameNum = atoi(argv[x + 1]);
                    }
                    else {
                        printf("Missing argument for the frame number!\n");
                        PrintUsage();
                        return 5;
                    }
                    x += 2;
                    continue;
                }

                if (strcmp(argv[x], "-s") == 0) {
                    if (sscanf(argv[x + 1], "%ux%u", &width, &height) == 2) {
                        /* input is correct */
                        /* the image width is stored in width */
                        /* the image height is stored in height */
                        } 
                    else {
                        printf("Invalid WIDTHxHEIGHT input!\n");
                        PrintUsage();
                        return 5;
                    }
                    x += 2;
                    continue;
                }

                /* the help option */
                if (strcmp(argv[x], "-h") == 0) {
                    PrintUsage();
                    return 0;
                } 

            x++;
        }

        Movie = LoadMovie(fin, frameNum, width, height);
        YUV2RGBMovie(Movie);

        x = 0;
        while (x < argc) {
                if (strcmp(argv[x], "-aging") == 0) {
                    IENTRY *temp;
                    assert(Movie);
                    temp = Movie->Frames->First;

                    for (int i = 0; i < frameNum; i++) {
                        assert(temp);
                        temp->RGBImage = Aging(temp->RGBImage);
                        temp = temp->Next;
                    }
                    printf("Operation Aging is done!\n");
                    x++;
                    break;
                }
            x++;
        }
        x = 0;
        while (x < argc) {
                if (strcmp(argv[x], "-hflip") == 0) {
                    IENTRY *temp;
                    assert(Movie);
                    temp = Movie->Frames->First;

                    for (int i = 0; i < frameNum; i++) {
                        assert(temp);
                        temp->RGBImage = HFlip(temp->RGBImage);
                        temp = temp->Next;
                    }
                    printf("Operation HFlip is done!\n");
                    x++;
                    break;
                }
            x++;
        }
        x = 0;
        while (x < argc) {
                if (strcmp(argv[x], "-edge") == 0) {
                    IENTRY *temp;
                    assert(Movie);
                    temp = Movie->Frames->First;

                    for (int i = 0; i < frameNum; i++) {
                        assert(temp);
                        temp->RGBImage = Edge(temp->RGBImage);
                        temp = temp->Next;
                    }
                    printf("Operation Edge is done!\n");
                    x++;
                    break;
                }
            x++;
        }
        x = 0;
        while (x < argc) {
                if (strcmp(argv[x], "-crop") == 0) {
                    if (x < argc - 1) {
                        unsigned int start, end;
                        sscanf(argv[x + 1], "%u-%u", &start, &end);
                        CropImageList(Movie->Frames, start, end);
                        printf("Operation Crop is done! New number of frames is %d\n", Movie->Frames->Length);
                    }
                    else {
                        printf("Missing argument for the crop start-end!\n");
                        PrintUsage();
                        return 5;
                    }
                    x += 2;
                    break;
                }
            x++;
        }
        x = 0;
        while (x < argc) {
                if (strcmp(argv[x], "-fast") == 0) {
                    if (x < argc - 1) {
                        unsigned int factor;
                        factor = atoi(argv[x + 1]);
                        FastImageList(Movie->Frames, factor);
                        printf("Operation Fast Forward is done! New number of frames is %d\n", Movie->Frames->Length);
                    }
                    else {
                        printf("Missing argument for the fast factor!\n");
                        PrintUsage();
                        return 5;
                    }
                    x += 2;
                    break;
                }
            x++;
        }
        x = 0;
        while (x < argc) {
                if (strcmp(argv[x], "-reverse") == 0) {
                    ReverseImageList(Movie->Frames);
                    printf("Operation Reverse is done!\n");
                    x++;
                    break;
                }
            x++;
        }
        x = 0;
        while (x < argc) {
                if (strcmp(argv[x], "-watermark") == 0) {
                    if (x < argc - 1) {
                        IENTRY *temp;
                        IMAGE *watermark_image;
                        char *Watermark_fname;
                        unsigned int topLeftX, topLeftY;
                        srand(time(NULL));

                        Watermark_fname = argv[x + 1];
                        watermark_image = LoadImage(Watermark_fname);

                        assert(watermark_image);
                        assert(Movie);
                        temp = Movie->Frames->First;

                        for (int i = 0; i < frameNum; i++) {
                            assert(temp);
                            if (i % WATERMARK_PER_FRAMES == 0) {
                                topLeftX = rand() % width;
                                topLeftY = rand() % height;
                            }
                            temp->RGBImage = Watermark(temp->RGBImage, watermark_image, topLeftX, topLeftY);
                            temp = temp->Next;
                        }
                        DeleteImage(watermark_image);
                        watermark_image = NULL;
                        printf("Operation Watermark is done!\n");
                    }
                    else {
                        printf("Missing argument for the watermark file name!\n");
                        PrintUsage();
                        return 5;
                    }
                    x += 2;
                    break;
                }
            x++;
        }
        x = 0;
        while (x < argc) {
                if (strcmp(argv[x], "-spotlight") == 0) {

                    IENTRY *temp;
                    temp = Movie->Frames->First;

                    assert(temp);
                    unsigned int radius, length;
                    int centerX, centerY, dec_factor_multiplier;
                    double inc_factor, dec_factor, max_radius;

                    length = Movie->Frames->Length;
                    if (length < 60) {
                        printf("Frame length must have at least 60 to run spotlight function!\n");
                        break;
                    }
                    else {
                        centerX = (Movie->Frames->First->RGBImage->W - 1) / 2;
                        centerY = (Movie->Frames->First->RGBImage->H - 1) / 2;
                        max_radius = sqrt(pow(centerX,2) + pow(centerY,2));
                        inc_factor = max_radius / 39;
                        dec_factor = max_radius / 19;

                        for (int i = 0; i < 40; i++) {
                            radius = inc_factor * i;
                            temp->RGBImage = Spotlight(temp->RGBImage, centerX, centerY, radius);
                            temp = temp->Next;
                        }
                        for (int i = 40; i < length - 20; i++) {
                            temp = temp->Next;
                        }
                        for (int i = length - 20; i < length; i++) {
                            dec_factor_multiplier = length - i - 1;
                            radius = dec_factor * dec_factor_multiplier;
                            temp->RGBImage = Spotlight(temp->RGBImage, centerX, centerY, radius);
                            temp = temp->Next;
                        }
                        printf("Operation Spotlight is done!\n");
                        x++;
                        break;
                    }
                }
            x++;
        }
        x = 0;
        while (x < argc) {
                if (strcmp(argv[x], "-zoom") == 0) {

                    IENTRY *temp;
                    int numLoops, numInnerLoop, length;
                    temp = Movie->Frames->First;

                    assert(temp);
                    length = Movie->Frames->Length;
                    numLoops = length / 50;
                    if (length % 50 > 0) {
                        numLoops++;
                    }

                    for (int j = 0; j < numLoops; j++) {
                        if (length > 50) {
                            numInnerLoop = 50;
                        }
                        else if (length % 50 == 0) {
                            numInnerLoop = 49;
                        }
                        else {
                            numInnerLoop = (length % 50) - 1;
                        }
                        if (j % 2 == 0) {
                            for (int i = 0; i <= numInnerLoop; i++) {
                                temp->RGBImage = Zoom(temp->RGBImage, i * 2);
                                temp = temp->Next;
                                length--;
                            }
                        }
                        else {
                            for (int i = 0; i <= numInnerLoop; i++) {
                                temp->RGBImage = Zoom(temp->RGBImage, (100 - (i *2)));
                                temp = temp->Next;
                                length--;
                            }
                        }
                    }
                    printf("Operation Zoom is done!\n");
                    x++;
                    break;
                }
            x++;
        } 

        if (fin == NULL) {
                printf("Missing argument for the input file name!\n");
                PrintUsage();
                return 5;
        }

        if (fout == NULL) {
                printf("Missing argument for the output file name!\n");
                PrintUsage();
                return 5;
        }

        if (frameNum == 0) {
                printf("Missing argument for the frame number!\n");
                PrintUsage();
                return 5;
        }


        RGB2YUVMovie(Movie);
        SaveMovie(fout, Movie);
        DeleteImageList(Movie->Frames);
        DeleteMovie(Movie);
        Movie = NULL;
        fin = NULL;
        fout = NULL;

        return 0;
}

void PrintUsage()
{
        printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> -s <WIDTHxHEIGHT> [options]\n"
               "Options:\n"
               "-aging                Activate the aging filter on every movie frame\n"
               "-hflip                Activate horizontal flip on every movie frame\n"
               "-edge                 Activate the edge filter on every movie frame\n"
               "-crop <start-end>     Crop the movie frames from <start> to <end>\n"
               "-fast <factor>        Fast forward the movie by <factor>\n"
               "-reverse              Reverse the frame order of the input movie\n"
               "-watermark <file>     Add a watermark from <file> to every movie frame\n"
               "-spotlight            Spotlight the fade in and fade out\n"
               "-zoom                 Zoom in and out the input movie\n"
               "-h                    Display this usage information\n"
              );
}

/* Load one movie frame from the input file */
YUVIMAGE* LoadOneFrame(const char* fname, int n,
                       unsigned int width, unsigned height)
{
        FILE *file;
        unsigned int x, y;
        unsigned char c;
        YUVIMAGE* YUVimage;

        /* Check errors */
        assert(fname);
        assert(n >= 0);

        YUVimage = CreateYUVImage(width, height);
        if (YUVimage == NULL) {
                return NULL;
        }

        /* Open the input file */
        file = fopen(fname, "r");
        if (file == NULL) {
                DeleteYUVImage(YUVimage);
                return NULL;
        }

        /* Find the desired frame */
        fseek(file, 1.5 * n * width * height, SEEK_SET);

        for (y = 0; y < height; y++) {
                for (x = 0; x < width; x++) {
                        c = fgetc(file);
                        SetPixelY(YUVimage, x, y, c);
                } /*rof*/
        }

        for (y = 0; y < height; y += 2) {
                for (x = 0; x < width; x += 2) {
                        c = fgetc(file);
                        SetPixelU(YUVimage, x, y, c);
                        SetPixelU(YUVimage, x + 1, y, c);
                        SetPixelU(YUVimage, x, y + 1, c);
                        SetPixelU(YUVimage, x + 1, y + 1, c);
                }
        }

        for (y = 0; y < height; y += 2) {
                for (x = 0; x < width; x += 2) {
                        c = fgetc(file);
                        SetPixelV(YUVimage, x, y, c);
                        SetPixelV(YUVimage, x + 1, y, c);
                        SetPixelV(YUVimage, x, y + 1, c);
                        SetPixelV(YUVimage, x + 1, y + 1, c);
                }
        }

        /* Check errors */
        assert(ferror(file) == 0);

        /* Close the input file and return */
        fclose(file);
        file = NULL;
        return YUVimage;
}

/* Load the movie frames from the input file */
MOVIE *LoadMovie(const char *fname, int frameNum,
                 unsigned int width, unsigned height)
{
        MOVIE *Movie;
        Movie = CreateMovie();
        ILIST *iList;
        iList = CreateImageList();
        YUVIMAGE *temp_YUVImage;

        assert(fname);
        assert(Movie);
        assert(iList);
        Movie->Frames = iList;
        iList->First = NULL;
        iList->Last = NULL;
        iList->Length = 0;

        for (int i = 0; i < frameNum; i++) {
            temp_YUVImage = LoadOneFrame(fname, i, width, height);
            AppendYUVImage(iList, temp_YUVImage);
        }

        assert(Movie->Frames->First);
        assert(Movie->Frames->Last);

        printf("The movie file %s has been read successfully!\n", fname);

        return Movie;

}
    /* Save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie)
{
        int count;
        FILE *file;
        IENTRY *curr;

        /* Open the output file */
        file = fopen(fname, "w");
        if (file == NULL) {
                return 1;
        }

        count = 0;
        curr = movie->Frames->First;
        while (curr != NULL) {

                SaveOneFrame(curr->YUVImage, fname, file);
                curr = curr->Next;
                count++;
        }

        fclose(file);
        file = NULL;

        printf("The movie file %s has been written successfully!\n", fname);
        printf("%d frames are written to the file %s in total.\n", count, fname);
        return 0;
}

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVIMAGE *image, const char *fname, FILE *file)
{
        int x, y;
        for (y = 0; y < image->H; y++) {
                for (x = 0; x < image->W; x++) {
                        fputc(GetPixelY(image, x, y), file);
                }
        }

        for (y = 0; y < image->H; y += 2) {
                for (x = 0; x < image->W; x += 2) {
                        fputc(GetPixelU(image, x, y), file);
                }
        }

        for (y = 0; y < image->H; y += 2) {
                for (x = 0; x < image->W; x += 2) {
                        fputc(GetPixelV(image, x, y), file);
                }
        }
}

/* EOF */
