/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/

//Suggestion: define a struct that mirrors a record (row) of the data set
typedef struct data_row{
	// I counted 28 columns in the data set.
	// I figure we can track all of the values as integers for sorting.
	int column_value[28];
}data_row;

// Suggestion: prototype a mergesort function
// Merge sort can take in the column number that needs to be sorted and return a new sorted array of rows. 
*data_row mergesort(int);
