// For the movie data set I counted 28 columns, but for a general CSV,
// I would need the # of columns as meta data.
const int NUMBER_OF_COLUMNS = 28;
//Suggestion: define a struct that mirrors a record (row) of the data set
typedef struct data_row{
	// I figure we can track all of the values as integers for sorting.
	int column_values[NUMBER_OF_COLUMNS];
}data_row;

// Suggestion: prototype a mergesort function
// Merge sort can take in the column number that needs to be sorted and return a new sorted array of rows. 
*data_row mergesort(int);
