//
// Created by emir on 5/21/23.
//
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <random>

sem_t disla,tutturuyor;

int tutunSayisi = 0;
int kagitSayisi = 0;
int kibritSayisi = 0;

void *tutunVEkagit(void *);
void *tutunVEkibrit(void *);
void *kagitVEkibrit(void *);
void *iciyor(void *);


std::random_device rd;     // Only used once to initialise (seed) engine
std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
std::uniform_int_distribution<int> uni(1,4); // Guaranteed unbiased



int main(){
    sem_init(&disla,0,1);
    sem_init(&tutturuyor,0,1);
    pthread_t tutunVkagit, tutunVkibrit, kagitVkibrit, icici;

    pthread_create(&tutunVkagit,nullptr, tutunVEkagit,nullptr);

    pthread_create(&tutunVkibrit,nullptr, tutunVEkibrit,nullptr);

    pthread_create(&kagitVkibrit,nullptr, kagitVEkibrit,nullptr);

    pthread_create(&icici,nullptr, iciyor,nullptr);
    pthread_join(icici,nullptr);

    pthread_join(tutunVkagit,nullptr);
    pthread_join(tutunVkibrit,nullptr);
    pthread_join(kagitVkibrit,nullptr);

    sem_destroy(&disla);
    sem_destroy(&tutturuyor);
};

void *tutunVEkagit(void *){
    while(true) {
        sem_wait(&disla);
        sem_wait(&tutturuyor);

        tutunSayisi += 1;
        kagitSayisi += 1;
        printf("Masaya 1 tütün 1 kagit koyuyorum!     (t=%d,k=%d,ki=%d)\n", tutunSayisi, kibritSayisi, kagitSayisi);
        sem_post(&disla);
        sem_post(&tutturuyor);
        sleep(uni(rng));
    }
};
void *tutunVEkibrit(void *){
    while (true) {
        sem_wait(&disla);
        sem_wait(&tutturuyor);
        tutunSayisi += 1;
        kibritSayisi += 1;
        printf("Masaya 1 tütün 1 kibrit koyuyorum!    (t=%d,k=%d,ki=%d)\n", tutunSayisi, kibritSayisi, kagitSayisi);
        sem_post(&tutturuyor);
        sem_post(&disla);
        sleep(uni(rng));
    }
};
void *kagitVEkibrit(void *){
    while (true) {
        sem_wait(&disla);
        sem_wait(&tutturuyor);
        kagitSayisi += 1;
        kibritSayisi += 1;
        printf("Masaya 1 kibrit 1 kagit koyuyorum!    (t=%d,k=%d,ki=%d)\n", tutunSayisi, kibritSayisi, kagitSayisi);
        sem_post(&tutturuyor);
        sem_post(&disla);
        sleep(uni(rng));
    }
};
void *iciyor(void *){
    int i = 1;
    while(true) {
        if (tutunSayisi >= 1 and kibritSayisi >= 1 and kagitSayisi >= 1) {
            sem_wait(&tutturuyor);
            sem_wait(&disla);
            tutunSayisi -= 1;
            kagitSayisi -= 1;
            kibritSayisi -= 1;
            printf("%d.  içici tüttürüyor!   Geri kalan =   t=%d,k=%d,ki=%d\n",i,tutunSayisi,kagitSayisi,kibritSayisi);
            i+=1;
            sem_post(&disla);
            sem_post(&tutturuyor);
        }
    }
};
