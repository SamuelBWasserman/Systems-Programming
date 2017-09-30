// For the movie data set I counted 28 columns, but for a general CSV,
// I would need the # of columns as meta data.
// Suggestion: define a struct that mirrors a record (row) of the data set
typedef struct data_rows {
  // I figure we can track all of the values as integers for sorting.
  char *col[28];

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
void mergesort(data_row db[], int col, int l, int r);
