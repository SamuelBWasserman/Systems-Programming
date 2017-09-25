// For the movie data set I counted 28 columns, but for a general CSV,
// I would need the # of columns as meta data.
const int NUMBER_OF_COLUMNS = 28;
//Suggestion: define a struct that mirrors a record (row) of the data set
typedef struct data_row{
  //I figure we can track all of the values as integers for sorting.
  int column_values[NUMBER_OF_COLUMNS];
  char * color;
  char * director_name;
  int num_critic_for_reviews;
  float duration; //Because I don't know how to make dateTime obj
  int director_facebook_likes;
  int actor_3_facebook_likes;
  char * actor_2_name;
  int actor_1_facebook_likes;
  float  gross;
  char * genres;
  char * actor_1_name;
  char * movie_title;
  int num_voted_users;
  int cast_total_facebook_likes;
  char * actor_3_name;
  int facenumber_in_poster;
  char * plot_keywords;
  char * movie_imdb_link;
  int num_user_for_reviews;
  char * language;
  char * country;
  char * content_rating;
  float budget;
  int title_year;
  int actor_2_facebook_likes;
  float imdb_score;
  float aspect_ratio;
  int movie_facebook_likes;

}data_row;

// Suggestion: prototype a mergesort function
// Merge sort can take in the column number that needs to be sorted and return a new sorted array of rows. 
*data_row mergesort(int);
