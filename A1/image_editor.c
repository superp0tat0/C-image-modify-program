#include <stdio.h>
#include <stdlib.h>

/*
 * Include the function prototypes of all those functions implemented in image_processing.c.
 * Do not remove or modify them in any way.
 */

void remove_red();
void convert_to_black_and_white();
void instagram_square();

int main(int argc, char *argv[]) {
        /* Task 1 needs to be completed here: Make sure that the command line arguments are correct. In case an error exists,
         * print the error message and terminate the program. Otherwise, invoke the corresponding
         * image processing function. */
	    if (argc == 1){
	    	printf("Error: Expecting one command-line argument,which needs to be either 1,2 or 3.");
	    	return 0;
	    }
	    int counter;//turn the first argument from str to int so that we can check it.
	    counter = atoi(argv[1]);
         if((argc == 2) &&(counter == 1)){
        	 /*this condition will execute the remove_red function, 
        	  * which will remove all the red in poxel.
        	  */
        	 remove_red(); //this function will print the result to the screen.
         }else if((argc == 2) && (counter == 2)){
        	 /*this condition will execute the black_and_white function,
        	  * which will get the average of RGB of every pixels then 
        	  * replace with the original value.
        	  */
        	 convert_to_black_and_white();
         }else if((argc == 2) && (counter == 3)){
        	 /*this condition will execute the square function, which
        	  * will set the format to a square,
        	  * Eg:(cut the left columns for 6 x 4)
        	  */
        	 instagram_square();
         }else{
        	 /*this contion are for invalid format of commands,if it is not one of
        	  * 1 2 3 then stop accepting commands and print the error message.
        	  */
        	 printf("Error: Expecting one command-line argument,which needs to be either 1,2 or 3.");
        	 return 0;//terminate the functio(failure)
         }
        return 0;
}