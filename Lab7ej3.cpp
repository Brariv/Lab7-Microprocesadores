#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;

void* wait_for_condition(void* arg) {
    pthread_mutex_lock(&lock);
    while (ready == 0) { // Esperar hasta que 'ready' sea 1
        pthread_cond_wait(&cond, &lock);
    }
    // Una vez que 'ready' es 1, el hilo continúa
    printf("La condición se cumplió, continuando...\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* signal_condition(void* arg) {
    pthread_mutex_lock(&lock);
    ready = 1; // Cambiar la condición
    pthread_cond_signal(&cond); // Señalar a los hilos que esperan
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, wait_for_condition, NULL);
    pthread_create(&thread2, NULL, signal_condition, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
