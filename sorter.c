#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorter.h"

int main(int argc, char **(argv)){
	// Quit if # of arguments are incorrect
	if (argc < 3 || argc > 3) {
		printf("invalid arguments\n");
    		return 0;
  	}
	data_row db[5100]; // enough space to store every row
	fscanf(stdin, "%*[^\n]"); // Read and discard a line
	char line[400]; // one line from the file
    	char first_line[400];
    	fgets(first_line,400,stdin);// discard first line
    	int line_counter = 0; // count what line we're on to keep track of the struct array
    	int word_counter = 0; // keep track of what word were on for assignment in the struct
	while(fgets(line,400,stdin) != NULL){
		char *word;
            	data_row *row = (data_row*)malloc(sizeof(data_row)); // Initialize empty data row
		word = strtok(line,",");
	    	char *col[28]
		// Tokenize until end of line
	    	while(word != NULL){
			// The movie column has commas in between quotes but needs to be stores as one token
			if(strpbrk(word,"\"") != NULL){
				char tmp1[100];
				strcpy(tmp1,word);
				char *tmp = tmp1 + 1;
				word = strtok(NULL,"\"");
				strcat(tmp,word);
				strcpy(word,tmp);
			}
			// Assign value to the row
	    		col[word_counter] = word;	
                	word = strtok(NULL,",");
                	word_counter++;
            	}
		data_row -> col = col;
		db[line_counter] = data_row;
            	word_counter = 0;
            	line_counter++;
	}
}
