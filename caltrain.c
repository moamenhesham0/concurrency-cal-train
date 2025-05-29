#include <pthread.h>
#include "caltrain.h"


void
station_init(struct station *station)
{
	station->available_seats     = 0;
	station->waiting_passengers  = 0;
	station->boarding_passengers = 0;

	pthread_mutex_init( &(station->train_mutex)  , NULL);
	pthread_cond_init( &(station->train_arrived) , NULL);
	pthread_cond_init( &(station->train_filled)  , NULL);
}

void
station_load_train(struct station *station, int count)
{
	/* Sets the availabe train seats*/
    pthread_mutex_lock(&(station->train_mutex));
    station->available_seats = count;
    pthread_mutex_unlock(&(station->train_mutex));

	// Singals train arrival
    pthread_cond_broadcast(&(station->train_arrived));

    pthread_mutex_lock(&(station->train_mutex));

	/* Waits for boarding passengers to take a seat and fill the train*/
	if( (station->available_seats && station->waiting_passengers)|| station->boarding_passengers)
	{
		pthread_cond_wait(&(station->train_filled), &(station->train_mutex));
	}

	station->available_seats =0;
    pthread_mutex_unlock(&(station->train_mutex));
}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&(station->train_mutex));
    ++station->waiting_passengers;

	/*Passengers waiting for a train in the station*/
    while (!station->available_seats)
    {
        pthread_cond_wait(&(station->train_arrived), &(station->train_mutex));
    }

	++station->boarding_passengers;
	--station->available_seats;
    --station->waiting_passengers;

    pthread_mutex_unlock(&(station->train_mutex));
}

void
station_on_board(struct station *station)
{
    pthread_mutex_lock(&(station->train_mutex));

	--station->boarding_passengers;

	const char train_filled = (!station->available_seats || !station->waiting_passengers) && !station->boarding_passengers;

    pthread_mutex_unlock(&(station->train_mutex));

	/* Checks to signal the train to leave*/
    if (train_filled)
	{
		pthread_cond_signal(&(station->train_filled));
	}

}


