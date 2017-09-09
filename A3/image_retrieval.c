#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <float.h>
#include <wait.h>
#include "worker.h"

int main(int argc, char **argv) {
	int fork_status;
	int status = 0;
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

    // new place to store pipe
    // create a pipe to store the returned CompRecords
	int pipe_fd[2];

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

		// create a new pipe for this loop
    	int pipe_status = pipe(pipe_fd);
    	if (pipe_status == -1){ // if an error ocurred making the pipe
    		perror("pipe");
    		exit(1);
    	} 

		// fork a process for the current directory
    	fork_status = fork();

		// if an error occurs during forking
		if (fork_status < 0) {
			perror("Error occurred during forking");
			exit(1);

		} else if (fork_status == 0) { // if this process is a child
			// close the read of the pipe
			if (close(pipe_fd[0]) == -1){
				perror("Closing read pipe");
			}

			// Only call process_dir if it is a directory
			// Otherwise ignore it.

	        //added:open the image file
	        Image *image_in = read_image(image_file);
			if(S_ISDIR(sbuf.st_mode)) {
                printf("Processing all images in directory: %s \n", path);
                //added: use process_dir to get the closest image of the dir
                process_dir(path, image_in, pipe_fd[1]); // pass the writing end of the pipe to the process_dir
            }

            // once done, close the write end of the pipe
            // close the write of the pipe once done
			if (close(pipe_fd[1]) == -1){
				perror("Closing write pipe");
			}
			// do not fork on next loop
			free((*image_in).p);
			free(image_in);
			exit(0);
		} else { // if this process is a parent

            // close the write of the pipe once done
			if (close(pipe_fd[1]) == -1){
				perror("Closing write pipe");
			}

		}

		if(S_ISDIR(sbuf.st_mode)) {
			// read from the pipe and store to result
			if (read(pipe_fd[0], &result, sizeof(CRec)) == -1){
				perror("Error when reading from pipe in parent process");
				exit(1);
			}

	        // added:use if to compare and replace
	        // printf("%f < %f\n", result.distance, CRec.distance);
	        // printf("%s < %s\n", result.filename, CRec.filename);
	        if(result.distance < CRec.distance){
	        	CRec.distance = result.distance;
	        	strncpy(CRec.filename,result.filename,PATHLENGTH);
	        }

	        // printf("current: %f %s\n", CRec.distance, CRec.filename);
	    }
		
	}
	while ((fork_status = wait(&status)) > 0);	// wait for all child processes to finish	// wait for all child processes to finish
    printf("The most similar image is %s with a distance of %f\n", CRec.filename, CRec.distance);
	return 0;
}
