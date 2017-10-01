#include "sorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **(argv)) {
  // Quit if # of arguments are incorrect
  if (argc < 3 || argc > 3) {
    printf("invalid arguments\n");
    return 0;
  }
  char delims[] = ",";
  // Define variables
  data_row db[5100];        // enough space to store every row
  char line[600];           // one line from the file
  int line_counter =
      -2; // count what line we're on to keep track of the struct array
  int word_counter =
      0; // keep track of what word were on for assignment in the struct
  int type_flag = 0; // 0:STRING, 1:INT, 2:FLOAT
  int null_count = 0; 

  while (fgets(line, 600, stdin) != NULL) {
    int i; //for loops and shiz    
    //IF first char is ',' in the line
    if(line_counter < 0){
      line_counter++;
      continue;
    }
    if(line[0] == ','){
             char null_val[15];
	     char templine[600];
             sprintf(null_val, "NULL_VALUE%d", null_count);
	     null_count++;
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
	  strncpy(templine,line,i); //TODO: Change this to i+1 if it copies incorrent # of chars
	  templine[i+1] = '\0';
	  sprintf(null_val, "NULL_VALUE%d", null_count);
	  null_count++;
	  strcat(templine,null_val);
	  strcat(templine,line+i+1);
	  strcpy(line, templine);
	}
    }
    //IF the line ends with a ','
    if(line[strlen(line) -1] == ','){
             char null_val[15];
             sprintf(null_val, "NULL_VALUE%d", null_count);
	     null_count++;   
             strcat(line,null_val);
    }
    char *word;
    word = strtok(line, delims);
    //word = strtok_blanks(line,delims);
    // Tokenize until end of line
    while (word) {
      // The movie column has commas in between quotes but needs to be stored as
      // one token
      if (strpbrk(word, "\"") != NULL) {
        char tmp1[100];
        strcpy(tmp1, word);
        char *tmp = tmp1 + 1;
        word = strtok(NULL, "\"");
        strcat(tmp, word);
        strcpy(word, tmp);
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
  printf("Done with creating db! Line Counter at %d\n", line_counter);
  int sam;
  for(sam=0;sam < 5044; sam++){
    printf("%d %s\n", sam, db[sam].col[23]);
  }
  sort(db, column_to_sort, type_flag, 0, line_counter);
  printf("Building CSV\n");
  //print_to_csv(db);
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
  } else if((strstr(str1, "NULL_VALUE") != NULL) || (strstr(str2, "NULL_VALUE")) != NULL){
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

void sort(data_row db[], int col, int data_type, int left, int right) {
  if (left < right) {
    // Calculate the middle index of the array for splitting
    int middle = left + (right - left) / 2;

    // Recrusively sort both halves
    sort(db, col, data_type, left, middle);
    sort(db, col, data_type, middle + 1, right);

    printf("LMR: %d, %d, %d\n", left, middle, right);
    // Merge halves together
    merge(db, col, data_type, left, middle, right);
    printf("Done!\n");
  }
}

void merge(data_row db[], int column, int data_type, int left, int middle,
           int right) {
  int i, j, k;
  int size1 = middle - left + 1;
  int size2 = right - middle;

  // temp arrays
  data_row temp_left[size1];
  data_row temp_right[size2];

  // Copy data into the temp arrays
  for (i = 0; i < size1; i++) {
    temp_left[i] = db[left + i];
  }
  for (j = 0; j < size2; j++) {
    temp_right[j] = db[middle + 1 + j];
  }

  // reset indices
  i = 0;
  j = 0;
  k = left;

  // Do comparisons of db values to sort
  while (i < size1 && j < size2) {
    // Check for double NULL_VALUE
    if(NullCheck(temp_left[i].col[column], temp_right[i].col[column]) == 0) {
      char val_left[10];
      char val_right[10];

      //Get the numeric value of NULL_VALUE
      strncpy(val_left, temp_left[i].col[column] + 10, strlen(temp_left[i].col[column]) - 10);
      strncpy(val_right, temp_right[i].col[column] + 10, strlen(temp_right[i].col[column]) - 10);
      val_left[strlen(temp_left[i].col[column]) - 10] = '\0';
      val_right[strlen(temp_right[i].col[column]) - 10] = '\0';

      //compare the two values
      if(atoi(val_left) <= atoi(val_right)){
	db[k] = temp_left[i];                                                                                          
        i++;                                                                                                           
      } else {                                                                                                         
        db[k] = temp_right[j];                                                                                         
        j++;                                                                                                           
      }
    } 
    //Check for single NULL_VALUE
    else if (NullCheck(temp_left[i].col[column], temp_right[i].col[column]) > 0) {
      if(NullCheck(temp_left[i].col[column], temp_right[i].col[column]) == 1){
	db[k] = temp_left[i];
	i++;
      } else {
	db[k] = temp_right[j];
	j++;	
      }
    }
    // String Comp.
    else if (data_type == 0) {
      int result = strallcmp(temp_left[i].col[column],
                      temp_right[i].col[column]);
      if (result < 0 || result == 0) {
        db[k] = temp_left[i];
        i++;
      } else {
        db[k] = temp_right[j];
        j++;
      }
    }
    // Int Comp.
    else if (data_type == 1) {
      int left_int = atoi(temp_left[i].col[column]);
      int right_int = atoi(temp_right[i].col[column]);
      if (left_int <= right_int) {
        db[k] = temp_left[i];
        i++;
      } else {
        db[k] = temp_right[j];
        j++;
      }
    }
    // Float Comp.
    else if (data_type == 2) {
      // Cast and set values for comp.
      float left_float = atof(temp_left[i].col[column]);
      float right_float = atof(temp_right[i].col[column]);
      if (left_float <= right_float) {
        db[k] = temp_left[i];
        i++;
      } else {
        db[k] = temp_right[j];
        j++;
      }
    } else{
      printf("Error: data_type must be 0,1 or 2\n");
      return;
    }
    k++;
  }

  /* Copy the remaining elements of L[], if there
   * are any */
  while (i < size1) {
    db[k] = temp_left[i];
    i++;
    k++;
  }

  /* Copy the remaining elements of R[], if there
   *are any */
  while (j < size2) {
    db[k] = temp_right[j];
    j++;
    k++;
  }
}

void print_to_csv(data_row db[]) {
  int i, j;
  for (i = 0; i < 5050; i++) {
    for (j = 0; j < 28; j++) {
      fprintf(stdout, db[i].col[j]);
    }
    fprintf(stdout, "\n");
  }
}
char * strtok_blanks (char * str, char const * delims)
{
static char  *src = NULL;
    char  *p,  *ret = 0;

    if (str != NULL)
        src = str;

    if (src == NULL || *src == '\0')    // Fix 1
        return NULL;

    ret = src;                          // Fix 2
    if ((p = strpbrk(src, delims)) != NULL)
    {
        *p  = 0;
        ret = src;                    // Unnecessary
                src = ++p;
                    }
                        else
                               src += strlen(src);
        
                                    return ret;
}
