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
	// Initialize empty row that stdin will populate
	data_row *row = (data_row*)malloc(sizeof(data_row));
	fscanf(stdin, "%*[^\n]"); // Read and discard a line
    char line[350];
    char first_line[350];
    fgets(first_line,350,stdin);
    int line_counter = 0;
    int word_counter = 0;
	while(fgets(line,350,stdin) != NULL){
            char *word;
            word = strtok(line,",");
            while(word != NULL){
                printf("%s\n",word);
                word = strtok(NULL,",");
            }
    break;
	}
}
