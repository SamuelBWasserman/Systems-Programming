#include "sorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		word = strtok(line,",");
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
			    // Allocate enough space for the string to be placed in the array
			    db[line_counter].col[word_counter] = (char *) malloc(strlen(word) * sizeof(char));
			    // Copy the string into the array
			    strcpy(db[line_counter].col[word_counter],word);
			    //Move to the next token
                word = strtok(NULL,",");
                word_counter++;
           }
           word_counter = 0;
           line_counter++;
	    }
	    int column_to_sort; // will be passed to merge sort
	        if(strcmp(argv[2],"color") == 0)
	            column_to_sort = 1;
	        else if(strcmp(argv[2],"director_name") == 0)
	            column_to_sort = 2;
	        else if (strcmp(argv[2],"num_critic_for_reviews") == 0)
	            column_to_sort = 3;
	        else if (strcmp(argv[2],"duration") == 0)
	            column_to_sort = 4;
	        else if (strcmp(argv[2],"director_facebook_likes") == 0)
	            column_to_sort = 5;
	        else if (strcmp(argv[2],"actor_3_facebook_likes") == 0)
	            column_to_sort = 6;
	        else if (strcmp(argv[2],"actor_2_name") == 0)
	            column_to_sort = 7;
	        else if (strcmp(argv[2],"actor_1_facebook_likes") == 0)
	            column_to_sort = 8;
	        else if (strcmp(argv[2],"gross") == 0)
	            column_to_sort = 9;
	        else if (strcmp(argv[2],"genres") == 0)
	            column_to_sort = 10;
	        else if (strcmp(argv[2],"actor_1_name") == 0)
	            column_to_sort = 11;
	        else if (strcmp(argv[2],"movie_title") == 0)
	            column_to_sort = 12;
	        else if (strcmp(argv[2],"num_voted_users") == 0)
	            column_to_sort = 13;
	        else if (strcmp(argv[2],"cast_total_facebook_likes") == 0)
	            column_to_sort = 14;
	        else if (strcmp(argv[2],"actor_3_name") == 0)
	            column_to_sort = 15;
	        else if (strcmp(argv[2],"facenumber_in_poster") == 0)
	            column_to_sort = 16;
	        else if (strcmp(argv[2],"plot_keywords") == 0)
	            column_to_sort = 17;
	        else if (strcmp(argv[2],"movie_imdb_link") == 0)
	            column_to_sort = 18;
	        else if (strcmp(argv[2],"num_user_for_reviews") == 0)
	            column_to_sort = 19;
	        else if (strcmp(argv[2],"language") == 0)
	            column_to_sort = 20;
	        else if (strcmp(argv[2],"country") == 0)
	            column_to_sort = 21;
	        else if (strcmp(argv[2],"content_rating") == 0)
	            column_to_sort = 22;
	        else if (strcmp(argv[2],"budget") == 0)
	            column_to_sort = 23;
	        else if (strcmp(argv[2],"title_year") == 0)
	            column_to_sort = 24;
	        else if (strcmp(argv[2],"actor_2_facebook_likes") == 0)
	            column_to_sort = 25;
	        else if (strcmp(argv[2],"imdb_score") == 0)
	            column_to_sort = 26;
	        else if (strcmp(argv[2],"aspect_ration") == 0)
	            column_to_sort = 27;
	        else if (strcmp(argv[2],"movie_facebook_likes") == 0)
	            column_to_sort = 28;
	    	// Call merge sort with db and column_to_sort

		

};

void mergesort(data_row db[], int col, int left, int right){
	if(l < r){
		// Calculate the middle index of the array for splitting
		int m = left + (right-1)/2;

		// Recrusively sort both halves
		mergesort(db, col, 1, middle);
		mergesort(db, col,  middle+1, right);

		// Merge halves together
		merge(db, left, middle, right, col);
	}

}

void merge(data_row db[], int left, int middle, int right, int column){
	int i, j, k;
	int size1 = middle - left + 1;
	int size2 = right - middle;
	
	// temp arrays
	data_row temp_left[size1];
	data_row temp_right[size2];

	// Copy data into the temp arrays
	for(i = 0; i < size1; i++){
		temp_left[i] = db[l + i];
	}
	for(j = 0;j < size2; j++){
		temp_right[j] = db[m+1+j];
	}

	// reset indices
	i = 0;
	j = 0;
	k = l;
	
	while(i < size1 && j < size2){
		if (temp_left[i].col[column] <= temp_right[j].col[column])
        	{
            		db[k] = temp_left[i];
            		i++;
        	}
        	else
        	{
            		db[k] = temp_right[j];
            		j++;
        	}
        	k++;
    	}
 
    	/* Copy the remaining elements of L[], if there
 	* are any */
    	while (i < size1)
    	{
        	db[k] = temp_left[i];
        	i++;
        	k++;
    	}
 
    	/* Copy the remaining elements of R[], if there
 	*  are any */
    	while (j < size2)
    	{
        	db[k] = temp_right[j];
        	j++;
        	k++;
    	}
}
