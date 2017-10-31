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

int main(int argc, char **(argv)) {
  // Quit if # of arguments are incorrect
  if (argc < 4 || argc > 7) {
    printf("invalid arguments\n");
    return 0;
  }
  // Master count of processes
  int num_processes = 1;
  // Array to store 
  int *pid_list = (int *)(malloc(sizeof(int)));
  int pid_len = 1; //counter for num of ints in 'pid_list'
  int init_pid = getpid(); //get init PID
  pid_list[0] = init_pid; //add to PID_list
  char buf[_POSIX_PATH_MAX] = {0};                                                                                                                                                                                                                                    
  char curr_dir[_POSIX_PATH_MAX] = {0};
  char *path = NULL;
  strcat(curr_dir, argv[4]);                                                                                                                                                                                                          
  
  // Open the given directory
  struct dirent *entry;
  DIR *directory;
  directory = opendir(argv[4]);
  // Check for NULL
  if(directory == NULL){
     printf("Invalid path\n");
     return 0;
  }
  
  
  // Begin traversing directory
  while((entry = readdir(directory)) != NULL){
    // If the current file is a csv, fork and sort
    if(strstr(entry->d_name,".csv") != NULL){
      //make sure we don't sort for already sorted CSV files that may exist in the same dir(see assignment 1 page methodology section for more info)
      if(strstr(entry->d_name,"-sorted-") != NULL){
	continue;
      }
        int pid = fork();        
        switch(pid){
            case 0: // This is the child
	          printf("Preparing to process CSV %s\n",entry->d_name);
		  FILE *csv_file;
		  strcat (curr_dir, "/");
		  strcat (curr_dir, entry->d_name);
	          path = realpath(curr_dir, buf);
		  printf("CSV detected with path: %s\n", buf);
                  csv_file = fopen(buf, "r");
                  process_csv(argv, csv_file, entry->d_name);
	          printf("Processed CSV...exiting child process %d\n", pid);
  	          exit(0); // End process
                
            case -1: //Fork unsuccessfull
	          printf("Fork Unsuccessfull\n");
                  return 0; //Exit the program

            default: 
	          // This is the parent
                  // Put child process in list
	          printf("Adding Child PID %d to PID_list\n", pid);
	          pid_len++;
	          pid_list = (int *)realloc(pid_list, pid_len * sizeof(int)); //realloc int array for pointer increment
                  pid_list[pid_len-1] = pid;
	          printf("Added child PID successfully!\n");
	          printf("Waiting on child process %d\n", pid);
	          wait(NULL); //To create more parallelism move wait till after all processes have been forked
	          printf("Done waiting for CSV file processing...Moving on.\n");
              break;
        }
	continue; //Continue processing
    } // End of .csv processing
    
    
    // QA: Implement directory check
    // DONE: Check that the stat checking is actually checking if it's a directory
    struct stat s;
    if(fstatat(dirfd(directory),entry->d_name,&s,0) == 0){ //fstatat supports relative pathing. Se we're in the mix
      //Check to see if the entry is either '.' or '..'
      if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0){
	continue;
      } 
      if(S_ISDIR(s.st_mode)) //it's a directory
        {
           int pid = fork();
	   switch(pid){
	       case 0:
		     printf("Changing directory to forked dir: %s\n", entry->d_name);
		     strcat (curr_dir, "/");
		     strcat (curr_dir, entry->d_name);
		     path = realpath(curr_dir, buf);
		     printf("Subdirectory detected with path: %s\n", buf);
		     directory = opendir(buf);
		     printf("Done changing directory to dir: %s\n", entry->d_name);
		     break;

	       default:
		     printf("Adding Child PID %d to PID_list\n", pid);
		     pid_len++;
		     pid_list = (int *)realloc(pid_list, pid_len * sizeof(int)); //realloc int array for pointer increment
		     pid_list[pid_len-1] = pid;                                                                                                                            
		     printf("Added child PID successfully!\n");  
		     printf("Waiting on child process %d\n", pid);
		     wait(NULL); //To create more parallelism move wait till after all processes have been forked
		     printf("Done waiting for directory parsing...moving on.\n");
		     break;			
 	   }
	   continue;
        }
    } // End of directory checking
  } // End of Directory traversal

  //IF current process is child
  if(getpid() != init_pid){
    printf("Current child process %d is not parent...exiting.\n", getpid());
    exit(0);
  }

  //Close the directory and output metadata
  closedir(directory); 

  printf("----------------------------------------------------\n");
  printf("METADATA SUMMARY                                    \n");
  printf("Initial PID: %d\n", init_pid);
  printf("PIDs of all the child processes: ");
  int i ;
  for(i=0; i < pid_len; i++){
    if(i == (pid_len-1))
      printf("%d\n", pid_list[i]);
    else{
      printf("%d,", pid_list[i]);
    }
  }
  printf("Total number of processes: %d\n", pid_len-1);
  printf("----------------------------------------------------\n");

  //Exit program
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

int column_to_sort(char **(argv)){
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

  return column_to_sort;
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



void process_csv(char **(argv),FILE *csv_file, char *file_name){
  if(csv_file == NULL){
     printf("NULL FILE\n");
  }
  printf("Processing CSV\n");
  //Define variables here
  char file_path[50];
  if(argv[6] != NULL){
 	strcpy(file_path, argv[6]);
  }
  else{
 	strcpy(file_path,"");
  }
  char *first_line;
  strcat(file_path, "/");
  printf("%s\n",file_path);
  char delims[] = ",";
  data_row **db = (data_row**)malloc(sizeof(data_row)); // 1 data row
  db[0] = (data_row*) malloc(sizeof(data_row));
  char line[600]; // one line from the file
  int line_counter =
      -1; // count what line we're on to keep track of the struct array
  int word_counter =
      0; // keep track of what word were on for assignment in the struct
  int type_flag = 0; // 0:STRING, 1:INT, 2:FLOAT

  while (fgets(line, 600, csv_file) != NULL) {
    int i;
    if(line_counter < 0){
      line_counter++;
      first_line = (char *) malloc(sizeof(char)*strlen(line)+1);
      strcpy(first_line,line);
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
        db[line_counter]->col[word_counter] =
            (char *)malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(db[line_counter]->col[word_counter], buffer);
	word_counter++;
	word = strtok(NULL,",");
	continue;
      }
      // Allocate enough space for the string to be placed in the array
      db[line_counter]->col[word_counter] =
	    (char *)malloc((strlen(word) + 1) * sizeof(char));
      // Copy the string into the array and add trailing string ender
      strcpy(db[line_counter]->col[word_counter], word);
      // Move to the next token
      word_counter++;
      word = strtok(NULL, delims);
    }
    word_counter = 0;
    line_counter++;
    db = (data_row**)realloc(db, (sizeof(data_row) * (line_counter + 1)));
    db[line_counter] = (data_row*)malloc(sizeof(data_row));
  }
  type_flag = determine_data_type(column_to_sort(argv));
  int column = column_to_sort(argv);
  sort(db, column, type_flag, 0, line_counter - 1);
  char *buffer = (char *)malloc(sizeof(char)*strlen(file_name) - 3);
  int i;
  for(i=0;i<strlen(file_name);i++){
  	if(file_name[i] == '.')
	   break;
        buffer[i] = file_name[i];
  }
  strcat(file_path, buffer);
  strcat(file_path,"-sorted-");
  strcat(file_path,argv[2]);
  strcat(file_path,".csv");
  print_to_csv(argv, db, line_counter, file_path, first_line);
}



void print_to_csv(char **(argv),data_row **db, int line_counter, char *file_path_name, char *first_line) {
  printf("Printing to CSV\n");
  printf("%s\n",file_path_name);
  struct stat st = {0};
  char buffer[200];
  if (stat(argv[6], &st) == -1) {
    mkdir(argv[6], 0700);
  }
  
  FILE *f;
  f = fopen(file_path_name, "w");
  printf("opened");
  int i, j;
  for (i = -1; i < line_counter; i++) {
    //TODO: Print first line to csv
    if(i == -1){
<<<<<<< HEAD
    	// fprintf(f, );
=======
    	fprintf(f, first_line);
>>>>>>> a9e1f3bd8253a0ec5721d65930f1afe5116b1544
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
printf("Printed csv");
}
