#include "sorter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void sort(data_row db[], int col, int data_type, int left, int right) {
  if (left < right) {
    // Calculate the middle index of the array for splitting
    int middle = left + (right - left) / 2;

    // Recrusively sort both halves
    sort(db, col, data_type, left, middle);
    sort(db, col, data_type, middle + 1, right);

    // Merge halves together
    merge(db, col, data_type, left, middle, right);
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
    temp_left[i] = (data_row) db[left + i];
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
      if(NullCheck(temp_left[i].col[column], temp_right[j].col[column]) == 0) {
      char val_left[10];
      char val_right[10];

      //Get the numeric value of NULL_VALUE
      strncpy(val_left, temp_left[i].col[column] + 10, strlen(temp_left[i].col[column]) - 10);
      strncpy(val_right, temp_right[j].col[column] + 10, strlen(temp_right[j].col[column]) - 10);
      val_left[strlen(temp_left[i].col[column]) - 10] = '\0';
      val_right[strlen(temp_right[j].col[column]) - 10] = '\0';


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
    else if (NullCheck(temp_left[i].col[column], temp_right[j].col[column]) > 0) {
      if(NullCheck(temp_left[i].col[column], temp_right[j].col[column]) == 1){
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
                      temp_right[j].col[column]);
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
      int right_int = atoi(temp_right[j].col[column]);
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
      // Cast and set values for comparison
      float left_float = atof(temp_left[i].col[column]);
      float right_float = atof(temp_right[j].col[column]);
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

   // Copy the remaining elements of left
  while (i < size1) {
    db[k] = temp_left[i];
    i++;
    k++;
  }

  // Copy the remaining elements of Right
  while (j < size2) {
    db[k] = temp_right[j];
    j++;
    k++;
  }
}

