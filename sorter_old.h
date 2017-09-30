// For the movie data set I counted 28 columns, but for a general CSV,
// I would need the # of columns as meta data.
// Suggestion: define a struct that mirrors a record (row) of the data set
typedef struct data_rows {
  // I figure we can track all of the values as integers for sorting.
  char *color; //0
  char *director_name; //1
  int num_critic_for_reviews; //2
  float duration; //3
  int director_facebook_likes; //4
  int actor_3_facebook_likes; //5
  char *actor_2_name; //6
  int actor_1_facebook_likes; //7
  float gross; //8
  char *genres; //9
  char *actor_1_name; //10
  char *movie_title; //11
  int num_voted_users; //12
  int cast_total_facebook_likes; //13
  char *actor_3_name; //14
  int facenumber_in_poster; //15
  char *plot_keywords; //16
  char *movie_imdb_link; //17
  int num_user_for_reviews; //18
  char *language; //19
  char *country; //20
  char *content_rating; //21
  float budget; //22
  int title_year; //23
  int actor_2_facebook_likes; //24
  float imdb_score; //25
  float aspect_ratio; //26
  int movie_facebook_likes; //27

} data_row;

enum column {
  color,
  director_name,
  num_critic_for_reviews,
  duration,
  director_facebook_likes,
  actor_3_facebook_likes,
  actor_2_name,
  actor_1_facebook_likes,
  gross,
  genres,
  actor_1_name,
  movie_title,
  num_voted_users,
  cast_total_facebook_likes,
  actor_3_name,
  facenumber_in_poster,
  plot_keywords,
  movie_imdb_link,
  num_user_for_reviews,
  language,
  country,
  content_rating,
  budget,
  title_year,
  actor_2_facebook_likes,
  imdb_score,
  aspect_ratio,
  movie_facebook_likes
};

// Suggestion: prototype a mergesort function
// Merge sort can take in the column number that needs to be sorted and return a
// new sorted array of rows.
void mergesort(data_row **, char *, enum column);
