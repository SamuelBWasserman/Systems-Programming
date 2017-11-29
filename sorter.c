#include "mergesort.c"
#include "sorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/syscall.h>


// GLOBALS
// Will lock total_num_threads to increment
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;
int TOTAL_THREADS = 0;
pthread_t INIT_TID;
pthread_mutex_t DB_LOCK = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t BIG_LINE_COUNTER_LOCK;
data_row **big_db;
int BIG_LINE_COUNTER = 0;
char *first_line;
int tids[999999];

int main(int argc, char **(argv)) {

  //Intialize Variables
  big_db = (data_row**)malloc(sizeof(data_row)); // 1 data row
  argc = argc; // WHAT?
  INIT_TID = pthread_self();
  DIR *directory;
  int is_directory_specified = 0;
  int directory_index = 0;
  char curr_dir[_POSIX_PATH_MAX] = {0};
  // Print Metadata Summary
  printf("Metadata Summary\n");
  printf("----------------------------------------------------\n");
  printf("Initial TID: %d\n", INIT_TID);
  printf("tid's of all child threads: ");
  // Check command line args
  if (argc < 2 || argc > 7) {
    printf("invalid arguments\n");
    return 0;
  }
  int p;
  for(p = 0; p < argc; p++){
  	if(strcmp(argv[p],"-d") == 0){
   		is_directory_specified = 1;
   		directory_index = p + 1;
   		}
  }
  // If direct not specified, use current directory
  if(!is_directory_specified){
    getcwd(curr_dir, 255);
    directory = opendir(curr_dir);
  }
  else{
   if(argc >= 3){
     strcat(curr_dir, argv[directory_index]);
     directory = opendir(argv[directory_index]);
    }
  }
  // Check for NULL
  if(directory == NULL){
     printf("Invalid path\n");
     return 0;
  }
  
  
  // Creates a structure to hold the arguments needed for process_CSV to pass to the thread
  directory_args *d_args = (directory_args *)malloc(sizeof(directory_args));
  d_args->argv = (char **)malloc(sizeof(char **)*argc);
  d_args->argc = argc;
	// Malloc enough size for argv array and copy values
  int arg_count;
	for(arg_count = 0 ;arg_count < argc;arg_count ++){
	  d_args->argv[arg_count] = (char *)malloc(sizeof(char)*100);
	  strcpy(d_args->argv[arg_count], argv[arg_count]);
	}
  
  strcpy(d_args->curr_dir, curr_dir);
  big_db = (data_row**)malloc(sizeof(data_row)); // 1 data row
  // Spawn new threads to process the top lvl dir and wait for completion
  pthread_t tid_dir;
  pthread_create(&tid_dir, NULL, process_dir, (void*)d_args);
  pthread_join(tid_dir, NULL);
  // Sort the aggregated data structure
  sort(big_db,column_to_sort(argv),determine_data_type(column_to_sort(argv)),0,BIG_LINE_COUNTER-1);
  
  // Check for specified output dir
  int is_output_specified = 0;
  int j;
  int output_index = 0;
  for(j = 0;j < argc; j++){
   if(strcmp(argv[j], "-o") == 0){
	  is_output_specified = 1;
	  output_index = j + 1;
   }	
  }
  char file_path[50];
  if(is_output_specified){
 	  strcpy(file_path, argv[output_index]);
	  mkdir(argv[output_index], 0700);
  } else { // No output dir given; use working dir
          char curr_dir[_POSIX_PATH_MAX] = {0}; 
	        getcwd(curr_dir,255);
	        strcpy(file_path,curr_dir);
	    
  }

  // Construct the output file path
  strcat(file_path, "/AllFiles-sorted-");
  strcat(file_path,argv[2]);
  strcat(file_path,".csv");
  printf("\nTotal number of threads: %d\n", TOTAL_THREADS);
  fflush (stdout);
  // Construct output CSV file
  print_to_csv(argv,big_db,BIG_LINE_COUNTER,file_path,first_line);
  

  //Exit program
  return 0;
}


int is_csv_correct(char *first_line){
  /* Determines if CSV is in correct format
     Returns: 1 if correct format, 0 otherwise
  */
	int column_count = 0;
	char *token;
	token = strtok(first_line, ",");
	while(token != NULL){
		column_count++;
		token = strtok(NULL, ",");
	}
	if(column_count == 28)
		return 1;
	return 0;
}


int determine_data_type(int column_to_sort){
  int type_flag = 0; // 0:STRING, 1:INT, 2:FLOAT
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
  return type_flag;
}

void *process_dir(void *args){
 // Increment thread count
  pthread_mutex_lock(&MUTEX);
  tids[TOTAL_THREADS] = pthread_self();
  TOTAL_THREADS++;
  pthread_mutex_unlock(&MUTEX);
  /* Handles the processing for the directory */
  printf("%d, ", pthread_self());
  fflush(stdout);
  // cast the arguments passed from pthread_create
  directory_args *d_args = args;
  DIR *directory = opendir(d_args->curr_dir);
  char *path = NULL;
  struct dirent *entry;
  char buf[PATH_MAX];
  path = realpath(d_args->curr_dir, buf);
  //printf("%s\n", d_args->curr_dir);
  //fflush(stdout);
  // Begin traversing directory
  while((entry = readdir(directory)) != NULL){
   
    // If the current file is a csv, create a new thread to sort
    if(strstr(entry->d_name,".csv") != NULL){
      // Make sure we don't sort for already sorted CSV files that exist in the same dir
      if(strstr(entry->d_name,"-sorted-") != NULL){
	      continue;
      }
      
      // Creates a structure to hold the arguments needed for process_CSV to pass to the thread
      thread_args *t_args = (thread_args *)malloc(sizeof(thread_args));
	    path = realpath(d_args->curr_dir, buf);
	    strcat(buf, "/");
	    strcat(buf, entry->d_name);
	    FILE *csv_file;
	    csv_file = fopen(buf, "r");

      // Create and initialize thread args from dir args
	    t_args->argc = d_args->argc;
	    t_args->argv = (char **)malloc(sizeof(char **)*d_args->argc);
	    // Copy values from dir args to thread args
	    int arg_count;
	    for(arg_count = 0 ;arg_count < d_args->argc;arg_count ++){
	      t_args->argv[arg_count] = (char *)malloc(sizeof(char)*100);
	      strcpy(t_args->argv[arg_count], d_args->argv[arg_count]);
	    }
	    t_args->csv_file = csv_file;
	  
	    // Create a thread that will sort the csv
	    pthread_t tid_csv;
	    pthread_create(&tid_csv, NULL, process_csv, (void*)t_args);
        pthread_join(tid_csv, NULL);
        
	    // tid_list = (pthread_t*)realloc(tid_list, sizeof(tid_list) + sizeof(pthread_t));
	    continue; //Continue processing
    } // End of .csv processing

    
    // Directory handling
    struct stat s;
    if(fstatat(dirfd(directory),entry->d_name,&s,0) == 0){
      // Check to see if the entry is either '.' or '..'
      if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0){
	      continue;
      }

      // IF it's a directory
      if(S_ISDIR(s.st_mode)){
        // Create a thread and process
        pthread_t tid_dir;
        directory_args *new_d_args = (directory_args *)malloc(sizeof(directory_args));
        strcpy(new_d_args->curr_dir, d_args->curr_dir);
        strcat (new_d_args->curr_dir, "/");
		    strcat (new_d_args->curr_dir, entry->d_name);
		    path = realpath(new_d_args->curr_dir, buf);
		    
		    // Creates a structure to hold the arguments needed for process_CSV to pass to the thread
          new_d_args->argv = (char **)malloc(sizeof(char **)*d_args->argc);
	      new_d_args->argc = d_args->argc;
	      int arg_count;
	      for(arg_count = 0 ;arg_count < d_args->argc;arg_count++){
	        new_d_args->argv[arg_count] = (char *)malloc(sizeof(char)*100);
	        strcpy(new_d_args->argv[arg_count], d_args->argv[arg_count]);
	      }
        
        // Create the thread for a new subdir
        pthread_create(&tid_dir, NULL, process_dir, (void*)new_d_args);
        
        pthread_join(tid_dir, NULL);
	      continue;
      } else {
		      continue;
 	    }
    } // End of directory checking
  } // End of Directory traversal

    //IF current process is child...exit
    return;
} // End of Process CSV

void *process_csv(void *args){
  printf("%d, ", pthread_self());
  fflush(stdout);
  /* Processes the CSV file */
  // cast the arguments passed from pthread_create
  thread_args *t_args = args;
  
  // Increment thread count
  pthread_mutex_lock(&MUTEX);
  tids[TOTAL_THREADS] = pthread_self();
  TOTAL_THREADS++;
  pthread_mutex_unlock(&MUTEX);
  
  // Define path variables
  char curr_dir[_POSIX_PATH_MAX] = {0};
  char *path = NULL;
  if(t_args->csv_file==NULL){
     printf("NULL FILE exiting\n");
     exit(1);
  }
  // printf("Processing CSV\n");
  // printf("%s\n",file_path);
  char delims[] = ",";
  // data_row **db = (data_row**)malloc(sizeof(data_row)); // 1 data row
  big_db[BIG_LINE_COUNTER] = (data_row*)malloc(sizeof(data_row)); // 1 data row
  char line[600]; // one line from the file
  int line_counter = -1; // count what line we're on to keep track of the struct array
  int word_counter = 0; // keep track of what word were on for assignment in the struct
  int type_flag = 0; // 0:STRING, 1:INT, 2:FLOAT

  while(fgets(line, 600, t_args->csv_file) != NULL){
  	pthread_mutex_lock(&DB_LOCK);
    pthread_mutex_lock(&BIG_LINE_COUNTER_LOCK);
    int i;
    if(line_counter < 0){
      line_counter++;
      if(first_line == NULL){
        first_line = (char *) malloc(sizeof(char)*strlen(line)+1);
        strcpy(first_line,line);
      }
      if(!(is_csv_correct(line))){
        printf("Incorrect CSV");
        fflush(stdout);
        return;
      }
      pthread_mutex_unlock(&DB_LOCK);
      pthread_mutex_unlock(&BIG_LINE_COUNTER_LOCK);
      continue;
    }
    
    //IF first char is ',' in the line
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

    // Tokenize until end of line
    char *word;
    word = strtok(line, delims);
    
    while(word){
      //IF string has commas in the middle somwhere
	    if(word[0] == '\"'){
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
        big_db[BIG_LINE_COUNTER]->col[word_counter] = (char *)malloc((strlen(buffer)+1)*sizeof(char));
	      strcpy(big_db[BIG_LINE_COUNTER]->col[word_counter], buffer);
	      word_counter++;
	      word = strtok(NULL,",");
	      continue;
      }

      // Allocate enough space for the string to be placed in the array
      big_db[BIG_LINE_COUNTER]->col[word_counter] = (char *)malloc((strlen(word)+1)*sizeof(char));
      // Copy the string into the array and add trailing string ender
      strcpy(big_db[BIG_LINE_COUNTER]->col[word_counter], word);
      // Move to the next token
      word_counter++;
      word = strtok(NULL, delims);
    }
    word_counter = 0;
    line_counter++;
    BIG_LINE_COUNTER++;
    big_db = (data_row**)realloc(big_db, (sizeof(data_row)*(BIG_LINE_COUNTER+1)));
    big_db[BIG_LINE_COUNTER] = (data_row*)malloc(sizeof(data_row));
    pthread_mutex_unlock(&DB_LOCK);
    pthread_mutex_unlock(&BIG_LINE_COUNTER_LOCK);
  }
   return;
}



void print_to_csv(char **(argv),data_row **db, int line_counter, char *file_path_name, char *first_line) {
  // printf("Printing to CSV\n");
  // printf("%s\n",file_path_name);
  struct stat st = {0};
  char buffer[200];
  //TODO BAD BAD BAD
  //Check to see if arg 5 and 6 are provided 
  
  FILE *f;
  f = fopen(file_path_name, "w");
  // printf("opened");
  int i, j;
  for (i = -1; i < line_counter; i++) {
    //Print first line to csv
    if(i == -1){
    	// fprintf(f, );
    	fprintf(f, first_line);
    	continue;
    }
    for (j = 0; j < 28; j++) {
    
      if(strstr(db[i]->col[j],"NULL") != NULL){
	    fprintf(f,",");
	    if(j == 27){
	        fprintf(f,"\n");
	    }
	    continue;
      }
      
      if(j != 27){
      	char tmp[200];
        strcpy(tmp,db[i]->col[j]);
        strcat(tmp,",\0");
     	fprintf(f,tmp);
     	continue;
	  }
      fprintf(f,db[i]->col[j]);
    }
   }
// printf("Printed csv");
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
