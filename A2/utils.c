/*
 * Created by Sotirios-Efstathios Maneas on 2017-01-02.
 */

#include <stdio.h>
#include <string.h>

#include "utils.h"


/*
 * Tokenize the string stored in cmd based on DELIMITERS as separators.
 * Return the number of tokens, and store pointers to them in cmd_argv.
 */
int tokenize(char *cmd, char **cmd_argv) {
        // returning 0 for now, so the compiler does not complain
	//counter the number of the tokens
        //clean the whitespace after the string
        if (strlen(cmd) != 1){
          int countera = strlen(cmd)-2;
          while(cmd[countera] == ' '){
             countera -= 1;
          }
          cmd[countera+1] = '\0';
	}
	//put the tokens in the cmd_argv
	int counter3 = 0;
        int counter;
	const char ch[2] = " ";
	char *token;
	token = strtok(cmd,ch);
	while( token != NULL ){
	      cmd_argv[counter3] = token;//get the token to the cmd_argv[index]
	      counter3 ++;
	      token = strtok(NULL, ch);
	   }
	for(counter = 0;counter < strlen(cmd_argv[counter3-1]);counter++){
		if(cmd_argv[counter3-1][counter] == '\n'){
			cmd_argv[counter3-1][counter] = '\0';
		}
	}
	return counter3;
}
