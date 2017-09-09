#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <float.h>
#include "worker.h"

int main(int argc, char **argv) {
	
	char ch;
	char path[PATHLENGTH];
	char *startdir = ".";
        char *image_file = NULL;

        //this part will deal with the errors. 
        //in default it will check the input command is valid, and in if statement it will check the
        //input format
        //else means there is no input command, which means do the operation in the current dir
	while((ch = getopt(argc, argv, "d:")) != -1) {
		switch (ch) {
			case 'd':
			startdir = optarg;
			break;
			default:
			fprintf(stderr, "Usage: queryone [-d DIRECTORY_NAME] FILE_NAME\n");
			exit(1);
		}
	}

        if (optind != argc-1) {
	     fprintf(stderr, "Usage: queryone [-d DIRECTORY_NAME] FILE_NAME\n");
        } else
             image_file = argv[optind];

	// Open the directory provided by the user (or current working directory)
	
	DIR *dirp;
	if((dirp = opendir(startdir)) == NULL) {
		perror("opendir");
		exit(1);
	} 
	
	/* For each entry in the directory, eliminate . and .., and check
	* to make sure that the entry is a directory, then call run_worker
	* to process the image files contained in the directory.
	*/
		
	struct dirent *dp;
        CompRecord CRec;
        strcpy(CRec.filename,"");
        CRec.distance = FLT_MAX;
        CompRecord result;
	while((dp = readdir(dirp)) != NULL) {

		if(strcmp(dp->d_name, ".") == 0 || 
		   strcmp(dp->d_name, "..") == 0 ||
		   strcmp(dp->d_name, ".svn") == 0){
			continue;
		}
		strncpy(path, startdir, PATHLENGTH);
		strncat(path, "/", PATHLENGTH - strlen(path) - 1);
		strncat(path, dp->d_name, PATHLENGTH - strlen(path) - 1);

		struct stat sbuf;
		if(stat(path, &sbuf) == -1) {
			//This should only fail if we got the path wrong
			// or we don't have permissions on this entry.
			perror("stat");
			exit(1);
		} 

		// Only call process_dir if it is a directory
		// Otherwise ignore it.

        //added:open the image file
        Image *image_in = read_image(image_file);
                if(image_in == NULL){
                  exit(1);
                  }
		if(S_ISDIR(sbuf.st_mode)) {
            printf("Processing all images in directory: %s \n", path);
            //added: use process_dir to get the closest image of the dir
            result = process_dir(path,image_in,-1);
            //added:use if to compare and replace
            if(result.distance < CRec.distance){
               CRec.distance = result.distance;
               strncpy(CRec.filename,result.filename,PATHLENGTH);
            }
		}

		free((*image_in).p);
		free(image_in);
		
	}

        printf("The most similar image is %s with a distance of %f\n", CRec.filename, CRec.distance);
	return 0;
}
