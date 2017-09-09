#include <stdio.h>

/*
 * Read an image from the standard input and set the red value of each pixel to
 * zero.
 */
void remove_red(){
	char type_value[2];
	int column,raw,max_value;
	scanf("%s %d %d %d",type_value,&column,&raw,&max_value);
	printf("%s\n%d %d\n%d\n",type_value,column,raw,max_value);//print the output
	int pixels,counter,counter_col;
	counter_col = 0;
	pixels = column*raw;// that is the pixels
	int red,green,blue;//get the colour value
	for(counter = 0;counter < pixels;counter++){
		scanf("%d %d %d",&red,&green,&blue);
		red = 0;//change red to 0
		counter_col += 1;
		if(counter_col == column){
			printf("%d %d %d\n",red,green,blue);
			counter_col = 0;
		}else{
			printf("%d %d %d ",red,green,blue);
		}
	}
   
}

/*
 * Read an image from the standard input and convert it from colour to black and white.
 */
void convert_to_black_and_white(){
	char type_value[2];
		int column,raw,max_value;
		scanf("%s %d %d %d",type_value,&column,&raw,&max_value);
		printf("%s\n%d %d\n%d\n",type_value,column,raw,max_value);//print the output
		int pixels,counter,counter_col;
		counter_col = 0;
		pixels = column*raw;// that is the pixels
		int red,green,blue,average;//get the colour value
		for(counter = 0;counter < pixels;counter++){
			scanf("%d %d %d",&red,&green,&blue);
			average = (red+green+blue)/3;//calculate the average
			red = average;
			green = average;
			blue = average;//change all 3 to average
			counter_col += 1;
			if(counter_col == column){
				printf("%d %d %d\n",red,green,blue);
				counter_col = 0;
			}else{
				printf("%d %d %d ",red,green,blue);
			}
		}
  	  }

/*
 * Read an image from the standard input and convert it to a square image.
 */
void instagram_square(){
	/*this function is to remove the extra columns or raws to make it a 
		 * square,we gonna print the pixels we need , dont print the extras.
		 */
		char type_value[2];
		int column,raw,max_value;
		scanf("%s %d %d %d",type_value,&column,&raw,&max_value);
		if(column >= raw){
		printf("%s\n%d %d\n%d\n",type_value,raw,raw,max_value);//output the header
		}else if(raw > column){
			printf("%s\n%d %d\n%d\n",type_value,column,column,max_value);
		}
		int counter,counter2;
		int red,green,blue;
	    if(column > raw){
	    	//counter represent the nth raw,and the counter represent the nth column.
	    	for(counter = 0;counter < raw;counter++){
	    		for(counter2 = 0;counter2 < column;counter2++){
	    			//check if column <= raw then print it
	    		    if(counter2 < raw){
						scanf("%d %d %d",&red,&green,&blue);
						if(counter2 == raw - 1){
							printf("%d %d %d\n",red,green,blue);
						}else{
							printf("%d %d %d ",red,green,blue);
						}	
	    		    }else{//else just scan
	    			    scanf("%d %d %d",&red,&green,&blue);
	    		}}}
	    }else if(raw > column){
	    		//counter represent the nth raw,and the counter2 represent the nth column.
	    		for(counter = 0;counter < raw;counter++){
	    			for(counter2 = 0;counter2 < column;counter2++){
	    				if(counter < column){
							scanf("%d %d %d",&red,&green,&blue);
							if(counter2 == column - 1){
								printf("%d %d %d\n",red,green,blue);
							}else{
								printf("%d %d %d ",red,green,blue);
							}	
	    				}else{//else just scan
	    					scanf("%d %d %d",&red,&green,&blue);
	    		}}}
	    }else{//it is already square,just scan and print
	    	for(counter = 0;counter < raw;counter++){
	    		for(counter2 = 0;counter2 < column;counter2++){
	    			scanf("%d %d %d",&red,&green,&blue);
	    			if(counter2 == column - 1){
	    				printf("%d %d %d\n",red,green,blue);
	    			}else{
	    				printf("%d %d %d ",red,green,blue);
	    				}}}
	    		}
	    	}
