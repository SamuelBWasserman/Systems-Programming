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


//TODO: MERGESORT HUGE ARRAY AND IMPLEMENT JOINING

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


int main(int argc, char **(argv)) {
  big_db = (data_row**)malloc(sizeof(data_row)); // 1 data row
  argc = argc;
  INIT_TID = syscall(SYS_gettid);
  DIR *directory;
  int is_directory_specified = 0;
  char curr_dir[_POSIX_PATH_MAX] = {0};
  // Quit if # of arguments are incorrect
  if (argc < 2 || argc > 7) {
    printf("invalid arguments\n");
    return 0;
  }
  int p;
   for(p = 0; p < argc; p++){
  	if(strcmp(argv[p],"-d") == 0)
   		is_directory_specified = 1;
  }
  // If it is not use the current directory
  if(!is_directory_specified){
    getcwd(curr_dir, 255);
    directory = opendir(curr_dir);
  }
 else{
   if(argc >= 3){
     strcat(curr_dir, argv[4]);
     directory = opendir(argv[4]);
    }
 }
  // Check for NULL
  if(directory == NULL){
     printf("Invalid path\n");
     return 0;
  }
  
  
  // Creates a structure to hold the arguments needed for process_CSV to pass to the thread
      directory_args *d_args = (directory_args *)malloc(sizeof(directory_args));
	  d_args -> argc = argc;
	  // Malloc enough size for argv array and copy values
	  int arg_count;
	  for(arg_count = 0 ;arg_count < argc;arg_count ++){
	      d_args->argv[arg_count] = (char *)malloc(sizeof(char)* strlen(argv[arg_count]));
	      strcpy(d_args->argv[arg_count], argv[arg_count]);
	  }
	  d_args -> argv = argv;
	  // Malloc enough size for filename and copy value
	  strcpy(d_args->curr_dir, curr_dir);
  // End of args thread argument struct creation
  pthread_t tid_dir;
  // Call a thread to process the top level directory and wait for it to complete
  pthread_create(&tid_dir, NULL, process_dir, (void*)d_args);
  pthread_join(tid_dir, NULL);
  // Sort the aggregated data structure
  sort(big_db, column_to_sort(argv), determine_data_type(column_to_sort(argv)),0, BIG_LINE_COUNTER-1);
  
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
  }
  else{
	// If no output directory is given, process in same directory as files
      int is_directory_specified = 0;
      int p;
      for(p = 0; p < argc; p++){
        if(strcmp(argv[p],"-d") == 0)
          is_directory_specified = 1;
       }
       if(is_directory_specified)	
         strcpy(file_path,argv[4]);
       else{
           char curr_dir[_POSIX_PATH_MAX] = {0}; 
	   getcwd(curr_dir,255);
	   strcpy(file_path,curr_dir);
	}
  }
  char buffer[100];
  strcat(file_path, "/");
  strcat(file_path, buffer);
  strcat(file_path,"-sorted-");
  strcat(file_path,argv[2]);
  strcat(file_path,".csv");
  print_to_csv(argv, big_db, BIG_LINE_COUNTER, file_path, first_line);
  

  printf("Metadata Summary\n");
  printf("----------------------------------------------------\n");
  printf("Initial TID: %d\n", INIT_TID);
  printf("tid's of all child threads: ");
  fflush (stdout);           
  
  printf("\nTotal number of threads: %d\n", TOTAL_THREADS);
  printf("----------------------------------------------------\n");
  
  //Exit program
  return 0;
}


// This function will check if the given csv has 28 columns and fail if it does not
int is_csv_correct(char *first_line){
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
    // cast the arguments passed from pthread_create
  directory_args *d_args = args;
  DIR *directory = opendir(d_args->curr_dir);
  char *path = NULL;
  struct dirent *entry;
  char buf[PATH_MAX];
  path = realpath(d_args->curr_dir, buf);
  
  // Begin traversing directory
  while((entry = readdir(directory)) != NULL){
  
    // If the current file is a csv, create a new thread to sort
    if(strstr(entry->d_name,".csv") != NULL){
      //make sure we don't sort for already sorted CSV files that may exist in the same directory
      if(strstr(entry->d_name,"-sorted-") != NULL){
	    continue;
      }
      
      // Creates a structure to hold the arguments needed for process_CSV to pass to the thread
      thread_args *t_args = (thread_args *)malloc(sizeof(thread_args));
      strcat (d_args->curr_dir, "/");
      strcat (d_args->curr_dir, entry->d_name);
	  path = realpath(d_args->curr_dir, buf);
	  FILE *csv_file;
	  csv_file = fopen(buf, "r");
	  t_args -> argc = d_args->argc;
	  // Malloc enough size for argv array and copy values
	  int arg_count;
	  for(arg_count = 0 ;arg_count < d_args->argc;arg_count ++){
	      t_args->argv[arg_count] = (char *)malloc(sizeof(char)* strlen(d_args->argv[arg_count]));
	      strcpy(t_args->argv[arg_count], d_args->argv[arg_count]);
	  }
	  t_args -> argv = d_args->argv;
	  t_args -> csv_file = csv_file;
	  // Malloc enough size for filename and copy value
	  t_args -> file_name = (char *) malloc(sizeof(char) * strlen(entry->d_name));
	  strcpy(t_args->file_name, entry->d_name);
	  // End of args thread argument struct creation
	  
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
      //Check to see if the entry is either '.' or '..'
      if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0){
	  continue;
    }
    if(S_ISDIR(s.st_mode)) //it's a directory
    {
        // Create a thread and process
        pthread_t tid_dir;
        strcat (d_args->curr_dir, "/");
		strcat (d_args->curr_dir, entry->d_name);
		path = realpath(d_args->curr_dir, buf);
		// Creates a structure to hold the arguments needed for process_CSV to pass to the thread
        directory_args *new_d_args = (directory_args *)malloc(sizeof(directory_args));
	    new_d_args -> argc = d_args->argc;
	    // Malloc enough size for argv array and copy values
	    int arg_count;
	    for(arg_count = 0 ;arg_count < d_args->argc;arg_count++){
	      new_d_args->argv[arg_count] = (char *)malloc(sizeof(char)* strlen(d_args->argv[arg_count]));
	      strcpy(new_d_args->argv[arg_count], d_args->argv[arg_count]);
	    }
	    new_d_args -> argv = d_args->argv;
	    // Malloc enough size for filename and copy value
	    strcpy(new_d_args->curr_dir, d_args->curr_dir);
        // End of args thread argument struct creation
        pthread_create(&tid_dir, NULL, process_dir, (void*)new_d_args);
        pthread_join(tid_dir, NULL);
	    continue;
     }
     else{
		continue;
 	 }
    } // End of directory checking
  } // End of Directory traversal
#ifdef SYS_gettid
  //IF current process is child...exit
  if(syscall(SYS_gettid) != INIT_TID){
   exit(0);
  }
#else
#error "SYS_gettid broken"
#endif
}

void *process_csv(void *args){
  // cast the arguments passed from pthread_create
  thread_args *t_args = args;
  
  // Increment thread count
  pthread_mutex_lock(&MUTEX);
  TOTAL_THREADS++;
  pthread_mutex_unlock(&MUTEX);
  
  char curr_dir[_POSIX_PATH_MAX] = {0};
  char *path = NULL;
  if(t_args -> csv_file == NULL){
     printf("NULL FILE exiting\n");
     exit(1);
  }
  // printf("Processing CSV\n");
  //Define variables here
  // Check if an output directory is given

  // printf("%s\n",file_path);
  char delims[] = ",";
  // data_row **db = (data_row**)malloc(sizeof(data_row)); // 1 data row
  big_db[BIG_LINE_COUNTER] = (data_row*) malloc(sizeof(data_row));
  char line[600]; // one line from the file
  int line_counter =
      -1; // count what line we're on to keep track of the struct array
  int word_counter =
      0; // keep track of what word were on for assignment in the struct
  int type_flag = 0; // 0:STRING, 1:INT, 2:FLOAT

  while (fgets(line, 600, t_args -> csv_file) != NULL) {
  	pthread_mutex_lock(&DB_LOCK);
    pthread_mutex_lock(&BIG_LINE_COUNTER_LOCK);
    int i;
    if(line_counter < 0){
      line_counter++;
      first_line = (char *) malloc(sizeof(char)*strlen(line)+1);
      strcpy(first_line,line);
      if(!(is_csv_correct(first_line))){
        printf("Incorrect CSV");
        return;
      }
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
        big_db[line_counter]->col[word_counter] =
            (char *)malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(big_db[line_counter]->col[word_counter], buffer);
	word_counter++;
	word = strtok(NULL,",");
	continue;
      }
      // Allocate enough space for the string to be placed in the array
      big_db[BIG_LINE_COUNTER]->col[word_counter] =
	    (char *)malloc((strlen(word) + 1) * sizeof(char));
      // Copy the string into the array and add trailing string ender
      strcpy(big_db[line_counter]->col[word_counter], word);
      // Move to the next token
      word_counter++;
      word = strtok(NULL, delims);
    }
    word_counter = 0;
    line_counter++;
    BIG_LINE_COUNTER++;
    big_db = (data_row**)realloc(big_db, (sizeof(data_row) * (BIG_LINE_COUNTER + 1)));
    big_db[BIG_LINE_COUNTER] = (data_row*)malloc(sizeof(data_row));
    pthread_mutex_unlock(&DB_LOCK);
    pthread_mutex_unlock(&BIG_LINE_COUNTER_LOCK);
  }
  type_flag = determine_data_type(column_to_sort(t_args->argv));
  int column = column_to_sort(t_args ->argv);
  sort(big_db, column, type_flag, 0, line_counter - 1);
  char *buffer = (char *)malloc(sizeof(char)*strlen(t_args ->file_name) - 3);
  int i;
  for(i=0;i<strlen(t_args ->file_name);i++){
  	if(t_args ->file_name[i] == '.')
	   break;
        buffer[i] = t_args ->file_name[i];
  }
  
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
