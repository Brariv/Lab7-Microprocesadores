#include <iostream>
#include <pthread.h>
#include <unistd.h> 

#define Num_Planes 4
pthread_mutex_t Gate1;
pthread_mutex_t Gate2;
pthread_mutex_t Landing_Strip;

void* Landing(void* planeID){
    int PID = *(int*)planeID;

    std::cout << "Plane: " << PID << " Waiting to land" << std:: endl;

    pthread_mutex_lock(&Landing_Strip);
    // Sleep for some time to simulate landing
    std::cout << "Plane: " << PID << " Landing..." << std:: endl;
    sleep(3);
    

    std::cout << "Plane: " << PID << " Waiting for a gate" << std:: endl;

    while (true)
    {
        
        if (pthread_mutex_trylock(&Gate1) == 0){
            
            

            std::cout << "Plane: " << PID << " Arriving at gate 1" << std:: endl;
            pthread_mutex_unlock(&Landing_Strip);
            sleep(6);

            pthread_mutex_unlock(&Gate1);

            break;

        } else if (pthread_mutex_trylock(&Gate2) == 0){

            
            std::cout << "Plane: " << PID << " Arriving at gate 2" << std:: endl;
            pthread_mutex_unlock(&Landing_Strip);
            sleep(6);

            pthread_mutex_unlock(&Gate2);

            break;
        }
    }
    sleep(1);
    std::cout << "Plane: " << PID << " Leaving the airport" << std:: endl;
    
    return nullptr;
}

int main(){
    pthread_t Planes[Num_Planes];
    pthread_mutex_init(&Gate1, nullptr);
    pthread_mutex_init(&Gate2, nullptr);
    pthread_mutex_init(&Landing_Strip, nullptr);

    for (int i = 0;  i < Num_Planes; ++i){
        int* planeID = new int(i);
        pthread_create(&Planes[i], nullptr, Landing, (void*)planeID);
    }
        
    for (int i = 0;  i < Num_Planes; ++i){
        pthread_join(Planes[i], nullptr);
    }

    std::cout << "Airport Closes" << std::endl;

    return 0;
}