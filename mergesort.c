#include "sorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void merge(data_row db[], int l, int m, int r, int column);
void mergesort(data_row db[], int col, int l, int r){
	if(l < r){
		// Calculate the middle index of the array for splitting
		int m = l + (r-1)/2;

		// Recrusively sort both halves
		mergesort(db, col, 1, m);
		mergesort(db, col,  m+1, r);

		// Merge halves together
		merge(db, l, m, r, col);
	}

};

void merge(data_row db[], int l, int m, int r, int column){
	int i, j, k;
	int size1 = m - l + 1;
	int size2 = r - m;
	
	// temp arrays
	data_row temp_left[size1];
	data_row temp_right[size2];

	// Copy data into the temp arrays
	for(i = 0; i < size1; i++){
		temp_left[i] = db[l + i];
	}
	for(j = 0;j < size2; j++){
		temp_right[j] = db[m+1+j];
	}

	// reset indices
	i = 0;
	j = 0;
	k = l;
	
	while(i < size1 && j < size2){
		if (temp_left[i].col[column] <= temp_right[j].col[column])
        	{
            		db[k] = temp_left[i];
            		i++;
        	}
        	else
        	{
            		db[k] = temp_right[j];
            		j++;
        	}
        	k++;
    	}
 
    	/* Copy the remaining elements of L[], if there
 	* are any */
    	while (i < size1)
    	{
        	db[k] = temp_left[i];
        	i++;
        	k++;
    	}
 
    	/* Copy the remaining elements of R[], if there
 	*  are any */
    	while (j < size2)
    	{
        	db[k] = temp_right[j];
        	j++;
        	k++;
    	}
};
