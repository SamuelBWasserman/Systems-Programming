#include "sorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mergesort.c"

int main(int argc, char **(argv)) {
  // Quit if # of arguments are incorrect
  if (argc < 3 || argc > 3) {
    printf("invalid arguments\n");
    return 0;
  }
  char delims[] = ",";
  // Define variables
  data_row db[5045];        // enough space to store every row
  char line[600];           // one line from the file
  int line_counter =
      -1; // count what line we're on to keep track of the struct array
  int word_counter =
      0; // keep track of what word were on for assignment in the struct
  int type_flag = 0; // 0:STRING, 1:INT, 2:FLOAT

  while (fgets(line, 600, stdin) != NULL) {
    // Print first line to csv
    if(line_counter == -1){
        fprintf(stdout, line);
    }
    int i;
    //IF first char is ',' in the line
    if(line_counter < 0){
      line_counter++;
      continue;
    }
    if(line[0] == ','){
             char null_val[15];
	     char templine[600];
             sprintf(null_val, "NULL_VALUE%d", line_counter);
	     strcpy(templine,null_val);
	     templine[strlen(null_val)] = '\0';
	     strcat(templine, line);
	     strcpy(line, templine);
    }
    //IF ",," exists in the line
    for(i = 0;i<strlen(line);i++){
	if(line[i] == ',' && line[i+1] == ','){
	  char null_val[15];
	  char templine[600]; // Buffer to put the modified line in
	  strncpy(templine,line,i+1); //TODO: Change this to i+1 if it copies incorrent # of chars
	  templine[i+1] = '\0';
	  sprintf(null_val, "NULL_VALUE%d", line_counter);
	  strcat(templine,null_val);
	  strcat(templine,line+i+1);
	  strcpy(line, templine);
	}
    }
    //IF the line ends with a ','
    if(line[strlen(line) -1] == ','){
             char null_val[15];
             sprintf(null_val, "NULL_VALUE%d", line_counter);
             strcat(line,null_val);
    }
    char *word;
    word = strtok(line, delims);
    //word = strtok_blanks(line,delims);
    // Tokenize until end of line
    while (word) {
      //IF string has commas in the middle somwhere

	if  (word[0] == '\"') {
        char buffer[100];
        strcpy(buffer, word);
	word = strtok(NULL, ",");

	while(strpbrk(word, "\"") == NULL){
	  strcat(buffer, ",\0");
	  strcat(buffer, word);
	  word = strtok(NULL, ",");
	}
	strcat(buffer, ",\0");
        strcat(buffer, word);
	strcat(buffer, "\0");
        db[line_counter].col[word_counter] =
            (char *)malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(db[line_counter].col[word_counter], buffer);
	word_counter++;
	word = strtok(NULL,",");
	continue;
      }
      // Allocate enough space for the string to be placed in the array
      db[line_counter].col[word_counter] =
	    (char *)malloc((strlen(word) + 1) * sizeof(char));
      // Copy the string into the array and add trailing string ender
      strcpy(db[line_counter].col[word_counter], word);
      // Move to the next token
      word_counter++;
      word = strtok(NULL, delims);
    }
    word_counter = 0;
    line_counter++;
  }
  int column_to_sort; // will be passed to merge sort
  if (strcmp(argv[2], "color") == 0)
    column_to_sort = 0;
  else if (strcmp(argv[2], "director_name") == 0)
    column_to_sort = 1;
  else if (strcmp(argv[2], "num_critic_for_reviews") == 0)
    column_to_sort = 2;
  else if (strcmp(argv[2], "duration") == 0)
    column_to_sort = 3;
  else if (strcmp(argv[2], "director_facebook_likes") == 0)
    column_to_sort = 4;
  else if (strcmp(argv[2], "actor_3_facebook_likes") == 0)
    column_to_sort = 5;
  else if (strcmp(argv[2], "actor_2_name") == 0)
    column_to_sort = 6;
  else if (strcmp(argv[2], "actor_1_facebook_likes") == 0)
    column_to_sort = 7;
  else if (strcmp(argv[2], "gross") == 0)
    column_to_sort = 8;
  else if (strcmp(argv[2], "genres") == 0)
    column_to_sort = 9;
  else if (strcmp(argv[2], "actor_1_name") == 0)
    column_to_sort = 10;
  else if (strcmp(argv[2], "movie_title") == 0)
    column_to_sort = 11;
  else if (strcmp(argv[2], "num_voted_users") == 0)
    column_to_sort = 12;
  else if (strcmp(argv[2], "cast_total_facebook_likes") == 0)
    column_to_sort = 13;
  else if (strcmp(argv[2], "actor_3_name") == 0)
    column_to_sort = 14;
  else if (strcmp(argv[2], "facenumber_in_poster") == 0)
    column_to_sort = 15;
  else if (strcmp(argv[2], "plot_keywords") == 0)
    column_to_sort = 16;
  else if (strcmp(argv[2], "movie_imdb_link") == 0)
    column_to_sort = 17;
  else if (strcmp(argv[2], "num_user_for_reviews") == 0)
    column_to_sort = 18;
  else if (strcmp(argv[2], "language") == 0)
    column_to_sort = 19;
  else if (strcmp(argv[2], "country") == 0)
    column_to_sort = 20;
  else if (strcmp(argv[2], "content_rating") == 0)
    column_to_sort = 21;
  else if (strcmp(argv[2], "budget") == 0)
    column_to_sort = 22;
  else if (strcmp(argv[2], "title_year") == 0)
    column_to_sort = 23;
  else if (strcmp(argv[2], "actor_2_facebook_likes") == 0)
    column_to_sort = 24;
  else if (strcmp(argv[2], "imdb_score") == 0)
    column_to_sort = 25;
  else if (strcmp(argv[2], "aspect_ration") == 0)
    column_to_sort = 26;
  else if (strcmp(argv[2], "movie_facebook_likes") == 0)
    column_to_sort = 27;
  else{
    printf("Error: Column name not found!\n");
    return 0;
  }
  // Call merge sort with db and column_to_sort
  if (column_to_sort == 2 || column_to_sort == 4 || column_to_sort == 5 ||
      column_to_sort == 7 || column_to_sort == 12 || column_to_sort == 13 ||
      column_to_sort == 15 || column_to_sort == 18 || column_to_sort == 23 ||
      column_to_sort == 24 || column_to_sort == 27) {
    type_flag = 1;
  } else if (column_to_sort == 3 || column_to_sort == 8 ||
             column_to_sort == 22 || column_to_sort == 25 ||
             column_to_sort == 26) {
    type_flag = 2;
  } else {
    type_flag = 0;
  }
  sort(db, column_to_sort, type_flag, 0, line_counter - 1);
  print_to_csv(db, line_counter);
  return 0;
}

int strallcmp(char const *a, char const *b) {
  for (;; a++, b++) {
    int d = tolower(*a) - tolower(*b);
    if (d != 0 || !*a)
      return d;
  }
}

int NullCheck(char *str1, char *str2){
  /*Returns: -1 for no null vals, 0 for both null, 1 for 1st null, 2 for 2nd null*/
  int ret = -1;
  // Both are NULL_VALUES
  if((strstr(str1, "NULL_VALUE")) != NULL && (strstr(str2, "NULL_VALUE")) != NULL){
    ret = 0;
    return ret;
  } else if((strstr(str1, "NULL_VALUE") != NULL) || (strstr(str2, "NULL_VALUE") != NULL)){
    if(strstr(str1, "NULL_VALUE") != NULL){
      ret = 1;
      return ret;
    } else{
      ret = 2;
      return ret;
    }
    return ret;
  } else {
    return ret;
  }
}

void print_to_csv(data_row db[], int line_counter) {
  int i, j;
  for (i = 0; i < line_counter; i++) {
    for (j = 0; j < 28; j++) {
    
      if(strstr(db[i].col[j],"NULL") != NULL){
	    fprintf(stdout,",");
	    if(j == 27){
	        fprintf(stdout,"\n");
	    }
	    continue;
      }
      
      if(j != 27){
      	char tmp[175];
        strcpy(tmp,db[i].col[j]);
        strcat(tmp,",\0");
     	fprintf(stdout,tmp);
     	continue;
	  }
      fprintf(stdout,db[i].col[j]);
    }
  }
}
