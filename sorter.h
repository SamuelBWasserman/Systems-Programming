#include <stdio.h>
// For the movie data set I counted 28 columns, but for a general CSV,
// I would need the # of columns as meta data.
#ifndef SORTER_H
#define SORTER_H
typedef struct data_rows {
  // Track all values as string then compare using strcmp, atoi, or atof based on the columns
  char *col[28];

} data_row;

// Merge sort can take in the column number that needs to be sorted and return a
// new sorted array of rows.
int strallcmp(char const *a, char const *b);
int column_to_sort(char **(argv));
void process_csv(int argc,char **(argv), FILE *csv_file, char *);
char * strtok_blanks (char * str, char const * delims);
int is_csv_correct(char *first_line);
void print_to_csv(char **(argv),data_row**,int, char *, char *);
void merge(data_row **db, int column, int data_type, int left, int middle,
           int right);
#endif
