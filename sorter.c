#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorter.h"

int main(int argc, char **(argv)){
	// Quit if # of arguments are incorrect
	if (argc < 3 || argc > 3) {
		printf("invalid arguments\n");
    		return 0;
  	}
	
	// Initialize empty row that stdin will populate
	data_row *row = (data_row*) malloc(sizeof(data_row));
	// extremely long while loop that assings everything to everything	
	while(fscanf(stdin,"%s %s %d %d %d %d %s %d %d %s %s %s %d %d %s %d %s %s %d %s %s %s %f %d %d %f %f %d", row->color, row.director_name, row.num_critic_for_reviews, row.duration, row.director_facebook_likes, row.actor_3_facebook_likes, row.actor_2_name, row.actor_1_facebook_likes, row.gross, row.genres, row.actor_1_name, row.movie_title, row.num_voted_users, row.total_facebook_likes, row.actor_3_name, row.facenumber_in_poster, row.plot_keywords, row.movie_imbd_link, row.num_user_for_reviews, row.language, row.country, row.content_rating, row.budget, row.title_year, row.actor_2_facebook_likes, row.imdb_score, row.aspect_ratio, row.movie_facebook_likes ) != -1){
		print(row->language);
	
		
	}	
}


