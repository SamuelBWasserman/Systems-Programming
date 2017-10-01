// For the movie data set I counted 28 columns, but for a general CSV,
// I would need the # of columns as meta data.
// Suggestion: define a struct that mirrors a record (row) of the data set
typedef struct data_rows {
  // I figure we can track all of the values as integers for sorting.
  char *col[28];

} data_row;

// Suggestion: prototype a mergesort function
// Merge sort can take in the column number that needs to be sorted and return a
// new sorted array of rows.
int strallcmp(char const *a, char const *b);
void sort(data_row db[], int col, int data_type, int left, int right);
void merge(data_row db[], int column, int data_type, int left, int middle,
           int right);
char * strtok_blanks (char * str, char const * delims);
void print_to_csv(data_row[]);
