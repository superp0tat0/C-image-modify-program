/*
 * Created by Sotirios-Efstathios Maneas on 2017-01-02.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "utils.h"

int main(void) {

        // char array to hold a line of input
	char buf[BUFFER_SIZE] = {'\0'};

        // char* array to hold the pointers to tokens
	char *args[INPUT_ARG_MAX_NUM];

        // the root of the tree
        struct TreeNode root;
        root.value = "";
        root.child = NULL;
        root.sibling = NULL;
	
	struct TreeNode *root_ptr = &root;

        // Add your code below:
	//declare the size of arguments;
	int size;
	//use while loop
	while(fgets(buf,BUFFER_SIZE,stdin)){
		if((strlen(buf) == 1)&&(strcmp(buf,"p") != 0)){
			fprintf(stderr,"Invalid command\n");
		}else{
		        size = tokenize(buf,args);

		//check the command input by user
		if(strcmp(args[0],"i") == 0){
			//check the size of tokens
			if(size == 5){
				tree_insert(root_ptr,args);
			}else{
				fprintf(stderr,"Invalid command\n");
			}
		}else if(strcmp(args[0],"q") == 0){
			if(size == 4){
				tree_search(root_ptr,args);
			}else{
				fprintf(stderr,"Invalid command\n");
			}
		}else if(strcmp(args[0],"p") == 0){
			if(size == 1){
				tree_print(root_ptr);
			}else{
				fprintf(stderr,"Invalid command\n");
			}
		}else{
			fprintf(stderr,"Invalid command\n");
		}}
	}

        return 0;
}
