#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <float.h>
#include "worker.h"


/*
 * Read an image from a file and create a corresponding 
 * image struct 
 */

Image* read_image(char *filename)
{
    // create the variables for storing image type, size, and maximum color 
    char type[9999];
    int size_x = 0;
    int size_y = 0;
    int maxcolor = 0;

    // read the given file
    FILE *file;
    file = fopen(filename, "r");

    // Check if the given image file was opened properly:
    if (file == NULL) {
        fprintf(stderr, "Error occurred opening the file.\n");
        return NULL;
    } else { // otherwise

        // get the first 4 values (type, sizes and max colour)
        fscanf(file, "%s %d %d %d", type, &size_x, &size_y, &maxcolor);

        // if the image type is not P3
        if(strcmp("P3", type) != 0){
            fprintf(stderr, "Image is an incompatible type.\n");
            return NULL;
        }

        // create an array of Pixels of width * height size
        Pixel *image = malloc((size_x * size_y) * sizeof(Pixel));

        // read each "word" (pixel) until the end of the file (limited to 3 chars per pixel)
        for (int i = 0; i < size_x * size_y; i++) {
            // create a place to store currently read RGB values 
            int pixel_r = 0;
            int pixel_g = 0;
            int pixel_b = 0;

            // store RGB values
            fscanf(file, " %3d", &pixel_r);
            fscanf(file, " %3d", &pixel_g);
            fscanf(file, " %3d", &pixel_b);

            // create a new Pixel struct for each pixel
            Pixel imagePixel;
            imagePixel.red = pixel_r;
            imagePixel.green = pixel_g;
            imagePixel.blue = pixel_b;

            // set the Pixel for the Pixel array to this this Pixel
            image[i] = imagePixel;
        }

        // create a new Image to store the image attributes and pixels 
        Image *newImage = malloc(sizeof(Image *));
        (*newImage).width = size_x;
        (*newImage).height = size_y;
        (*newImage).max_value = maxcolor;
        (*newImage).p = image;

        return newImage;
    }
}

/*
 * Print an image based on the provided Image struct 
 */

void print_image(Image *img){
        printf("P3\n");
        printf("%d %d\n", img->width, img->height);
        printf("%d\n", img->max_value);
       
        for(int i=0; i<img->width*img->height; i++)
           printf("%d %d %d  ", img->p[i].red, img->p[i].green, img->p[i].blue);
        printf("\n");
}

/*
 * Compute the Euclidian distance between two pixels 
 */
float eucl_distance (Pixel p1, Pixel p2) {
        return sqrt( pow(p1.red - p2.red,2 ) + pow( p1.blue - p2.blue, 2) + pow(p1.green - p2.green, 2));
}

/*
 * Compute the average Euclidian distance between the pixels 
 * in the image provided by img1 and the image contained in
 * the file filename
 */

float compare_images(Image *img1, char *filename) {
      //read the image from the file
      Image *img2;
      img2 = read_image(filename);
      if(img2 == NULL){
      return FLT_MAX;
      }else{
      //get their number of pixels
      int cpixels_img1;
      cpixels_img1 = img1->width*img1->height;
      //check are they the same and choose the condition
      if((img1->width == img2->width)&&(img1->height == img2->height)){
        //create the sub_result,and loop every pixel by eucl_distance func
        float sub_result = 0;
        int counter;
        for (counter = 0; counter < cpixels_img1; counter++) {
            sub_result += eucl_distance(img1->p[counter],img2->p[counter]);
        }
        //return the result
        free((*img2).p);
        free(img2);
        return sub_result/cpixels_img1;
      }else{
        float result = FLT_MAX;
       free((*img2).p);
       free(img2);
       return result;
      }}
}

/* process all files in one directory and find most similar image among them
* - open the directory and find all files in it 
* - for each file read the image in it 
* - compare the image read to the image passed as parameter 
* - keep track of the image that is most similar 
* - write a struct CompRecord with the info for the most similar image to out_fd
*/
CompRecord process_dir(char *dirname, Image *img, int out_fd){
        char path[PATHLENGTH];
        CompRecord CRec; 
        strcpy(CRec.filename,"");
        CRec.distance = FLT_MAX;
        float compare;
        //open the dir provided by the user
        DIR *dirp;
	if((dirp = opendir(dirname)) == NULL) {
		perror("opendir");
		exit(1);
	}
        struct dirent *dp;
        //read every file and dir inside the super_dir
        while((dp = readdir(dirp)) != NULL) {

               if(strcmp(dp->d_name, ".") == 0 || 
		   strcmp(dp->d_name, "..") == 0){
			continue;
		}

                //create the path of the file
                strncpy(path, dirname, PATHLENGTH);
		strncat(path, "/", PATHLENGTH - strlen(path) - 1);
		strncat(path, dp->d_name, PATHLENGTH - strlen(path) - 1);

                struct stat sbuf;
		if(stat(path, &sbuf) == -1) {
	   	//This should only fail if we got the path wrong
	   	// or we don't have permissions on this entry.
	   	perror("stat");
	   	exit(1);
	  	}
                //check if it is a regular file
        	if(S_ISREG(sbuf.st_mode)) {
                  compare = compare_images(img,path);
                  //printf("the image %s with distance %f\n",path,compare);
                  if(compare < CRec.distance){
                    CRec.distance = compare;
                    strncpy(CRec.filename,path,PATHLENGTH);
                  }
                }
        }
        //printf("in the dir %s the image %s with distance %f\n",dirname,CRec.filename,CRec.distance);
        if(out_fd != -1){
        int write_status = write(out_fd, &CRec, sizeof(CRec));
        if (write_status != sizeof(CRec)){
            perror("Error occurred writing to pipe from child process");
        }}
        return CRec;
}
