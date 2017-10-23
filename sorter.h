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
void process_csv(char **(argv));
char * strtok_blanks (char * str, char const * delims);
void print_to_csv(data_row**,int);
void merge(data_row **db, int column, int data_type, int left, int middle,
           int right);
#endif
