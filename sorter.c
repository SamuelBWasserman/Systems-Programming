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
	data_row *row = (data_row*)malloc(sizeof(data_row)); // Initialize empty data row
	fscanf(stdin, "%*[^\n]"); // Read and discard a line
    char line[400]; // one line from the file
    char first_line[400];
    fgets(first_line,400,stdin);// discard first line
    int line_counter = 0; // count what line we're on to keep track of the struct array
    int word_counter = 0; // keep track of what word were on for assignment in the struct
	while(fgets(line,400,stdin) != NULL){
            char *word;
            word = strtok(line,",");
            while(word != NULL){
		if(strpbrk(word,"\"") != NULL){
			char tmp1[100];
			strcpy(tmp1,word);
			char *tmp = tmp1 + 1;
			word = strtok(NULL,"\"");
			strcat(tmp,word);
			strcpy(word,tmp);
		}
	    }
                word = strtok(NULL,",");
                word_counter++;
            }
            word_counter = 0;
            line_counter++;
	}
}
