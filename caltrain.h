#include <pthread.h>

struct station {
	int available_seats , waiting_passengers , boarding_passengers;
	pthread_mutex_t train_mutex;
	pthread_cond_t train_arrived , train_filled;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);