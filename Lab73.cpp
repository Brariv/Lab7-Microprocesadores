#include <pthread.h>
#include <stdio.h>
#include <unistd.h> 


typedef struct {
    int impresoras;
    int escaneres;
    int plotter;
} Recursos;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

Recursos recursos = {2, 1, 1};  


void adquirir_recursos(int impresoras_necesitadas, int escaneres_necesitados, int plotter_necesitado, int dept) {
    pthread_mutex_lock(&lock);


    while (recursos.impresoras < impresoras_necesitadas || recursos.escaneres < escaneres_necesitados || recursos.plotter < plotter_necesitado) {
        printf("Departamento %d esperando recursos...\n", dept);
        pthread_cond_wait(&cond, &lock);
    }

    recursos.impresoras -= impresoras_necesitadas;
    recursos.escaneres -= escaneres_necesitados;
    recursos.plotter -= plotter_necesitado;

    printf("Departamento %d adquirió recursos: impresoras: %d, escáneres: %d, plotter: %d\n", dept, impresoras_necesitadas, escaneres_necesitados, plotter_necesitado);
    pthread_mutex_unlock(&lock);
}

void liberar_recursos(int impresoras_liberadas, int escaneres_liberados, int plotter_liberado, int dept) {
    pthread_mutex_lock(&lock);


    recursos.impresoras += impresoras_liberadas;
    recursos.escaneres += escaneres_liberados;
    recursos.plotter += plotter_liberado;

    printf("Departamento %d liberó recursos\n", dept);


    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
}


void* departamento(void* arg) {
    int dept = *(int*)arg;

    if (dept == 1) {

        adquirir_recursos(1, 1, 0, dept);
        sleep(2); 
        liberar_recursos(1, 1, 0, dept);
    } else if (dept == 2) {
     
        adquirir_recursos(1, 0, 1, dept);
        sleep(2); 
        liberar_recursos(1, 0, 1, dept);
    } else if (dept == 3) {
     
        adquirir_recursos(0, 1, 1, dept);
        sleep(2); 
        liberar_recursos(0, 1, 1, dept);
    }

    return NULL;
}

int main() {
    pthread_t d1, d2, d3;
    int dept1 = 1, dept2 = 2, dept3 = 3;


    pthread_create(&d1, NULL, departamento, &dept1);
    pthread_create(&d2, NULL, departamento, &dept2);
    pthread_create(&d3, NULL, departamento, &dept3);

    pthread_join(d1, NULL);
    pthread_join(d2, NULL);
    pthread_join(d3, NULL);


    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
