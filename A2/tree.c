/*
 * Created by Sotirios-Efstathios Maneas on 2017-01-02.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"


/**
 *  A helper function that allocates a new tree node.
 *
 *  @param value The entry's value. It represents either an attribute or a filename.
 *  @return A pointer to the newly allocated node.
 *  (for now it returns NULL, so the compiler does not complain)
 */
struct TreeNode *allocate_node(const char *value) {
	struct TreeNode *result;
	result = malloc(sizeof(struct TreeNode));
        result->value = malloc(strlen(value)+1);
	result->child = NULL;
	result->sibling = NULL;
	strcpy(result->value,value);
	return result;
}

/*
 * helper function:sibling search
 */
void sib_insert(struct TreeNode *root,char **value,int counter){
	if(root->child != NULL){
		//create the node we need
		struct TreeNode *current = root->child;
		struct TreeNode *previous = NULL;
		//create counter
		int counter_a = 1 ;
		//loop begins
		while(counter_a == 1){
			//compare they are the same
			if(strcmp(current->value,value[counter]) == 0){
				if(counter != 4){
					sib_insert(current,value,counter+1);
				}else{
					fprintf(stderr,"file exist\n");
				}
				counter_a = 0;
			//the new string is smaller than the current one
			}else if(strcmp(current->value,value[counter]) < 0){
				if(current->sibling != NULL){
					previous = current;
					current = current->sibling;
				}else{
					struct TreeNode *new = allocate_node(value[counter]);
					current->sibling = new;
					if(counter != 4){
						sib_insert(new,value,counter+1);
					}
					counter_a = 0;
				}
			//the string is greater than the current one
			}else if(strcmp(current->value,value[counter]) > 0){
				//make node
				struct TreeNode *new = allocate_node(value[counter]);
				//check sorted order
				if(previous != NULL){
					previous->sibling = new;
				}else{
					root->child = new;
				}
				new->sibling = current;
				if(counter != 4){
					sib_insert(new,value,counter+1);
				}
				counter_a = 0;
			}
		}
	//create the node then loop
	}else{
		struct TreeNode *new = allocate_node(value[counter]);
		root->child = new;
		if(counter != 4){
			sib_insert(new,value,counter + 1);
		}
	}
}

struct TreeNode *sib_search(struct TreeNode *const*root, char *value){
	//if the root value if the same as the value,then ret           curn the treenode we are at
	if(*root == NULL){
		return 0;
	}else if(strcmp((*root)->value,value) == 0){
                return *root;
	//else use recursion to search the next sibling node.
	}else{
		return sib_search(&((*root)->sibling),value);
	}
	return 0;
}

/**
 *  Insert a new image to a tree
 *  @param tree A pointer to the root of the tree.
 *  @param values An array, whose first three members are the attribute values for 
 *                the image and the last one is the filename
 */
void tree_insert(struct TreeNode *root, char **values) {
        sib_insert(root,values,1);

}

/**
 *  Searches a tree to print all files with matching attribute values.
 *
 *  @param tree A pointer to the root of the tree.
 *  @param values An array of attribute values
 */

void tree_search(const struct TreeNode *root, char **values) {
	struct TreeNode *child1,*child2,*child3,*child4;
        //char result[256][256];
	//int counter,counter2;
	//counter = 1;
        
	child1 = sib_search(&(root->child),values[1]);
	//for the first
        if(child1 != NULL){
           child2 = sib_search(&(child1->child),values[2]);
           //for the second 
           if(child2 != NULL){
              child3 = sib_search(&(child2->child),values[3]);
              if(child3 != NULL){
              child4 = child3->child;
              printf("%s ",child4->value);
              while(child4->sibling != NULL){
                   printf("%s ",child4->sibling->value);
                   child4 = child4->sibling;
	      } 
              }else{printf("(NULL)");}
           }else{printf("(NULL)");}
        }else{printf("(NULL)");}
	printf("\n");
}

/**
 *  Prints a complete tree to the standard output.
 *
 *  @param tree A pointer to the root of the tree.
 */
void tree_print(const struct TreeNode *tree) {
	//create an array of string, store the value inside the string, and clean it replace it when we need. total 4 include the root
	char *result[5];
	struct TreeNode *child1,*child2,*child3,*child4;
	//first store the root
	result[0] = tree->value;
	//then we do the loop to check every value
	//check the first child exist(loop1)
	if(tree->child != NULL){
		child1 = tree->child;
		while(child1 != NULL){
			//get the first child value
			result[1] = child1->value;
			//loop2
			if(child1->child != NULL){
				child2 = child1->child;
				while(child2 != NULL){
					//get the second value
					result[2] = child2->value;
					//loop3
					if(child2->child != NULL){
						child3 = child2->child;
						while(child3 != NULL){
							//get the second value
							result[3] = child3->value;
                                                        if(child3->child != NULL){
                                                           child4 = child3->child;
                                                           while(child4 != NULL){
                                                                result[4] = child4->value;
                                                                printf("%s %s %s %s %s\n",result[0],result[1],result[2],result[3],result[4]);
                                                                child4 = child4->sibling;
                                                           }
                                                        }
						        child3 = child3->sibling;
						}
					}
					child2 = child2->sibling;
				}
			}
			child1 = child1->sibling;
		}
	}else{printf("(NULL)\n");}
}
