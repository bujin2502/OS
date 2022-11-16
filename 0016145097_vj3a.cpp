#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <csignal>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

sem_t sem_0;
sem_t sem_1;
sem_t sem_2;
sem_t sem_3;
sem_t sem_4;
sem_t sem_5;
sem_t sem_6;

pthread_t *polje_dretvi;
int *polje_i;

void prekid(int sig)
{
    sem_destroy(&sem_0);
    sem_destroy(&sem_1);
    sem_destroy(&sem_2);
    sem_destroy(&sem_3);
    sem_destroy(&sem_4);
    sem_destroy(&sem_5);
    sem_destroy(&sem_6);
    for (int i = 0; i < 7; i++)
        pthread_join(polje_dretvi[i], NULL);
    delete[] polje_dretvi;
    delete[] polje_i;
    exit(0);
}

void *dretva(void *arg)
{
    int i = *((int *)arg);

    switch (i)
    {
    case 0:
    {
        int random = 1 + (rand() % 9);
        for (int z = 0; z < random; z++)
        {
            printf("izvodim zadatak %d: %d/%d\n", i + 1, z + 1, random);
            sleep(1);
        }
        sem_post(&sem_0);
    }
    break;

    case 1:
    {
        int random = 1 + (rand() % 9);
        for (int z = 0; z < random; z++)
        {
            printf("izvodim zadatak %d: %d/%d\n", i + 1, z + 1, random);
            sleep(1);
        }
        sem_post(&sem_1);
        sem_post(&sem_2);
    }
    break;

    case 2:
    {
        int random = 1 + (rand() % 9);
        sem_wait(&sem_1);
        for (int z = 0; z < random; z++)
        {
            printf("izvodim zadatak %d: %d/%d\n", i + 1, z + 1, random);
            sleep(1);
        }
        sem_post(&sem_3);
    }
    break;

    case 3:
    {
        int random = 1 + (rand() % 9);
        sem_wait(&sem_2);
        for (int z = 0; z < random; z++)
        {
            printf("izvodim zadatak %d: %d/%d\n", i + 1, z + 1, random);
            sleep(1);
        }
        sem_post(&sem_4);
    }
    break;

    case 4:
    {
        int random = 1 + (rand() % 9);
        sem_wait(&sem_0);
        for (int z = 0; z < random; z++)
        {
            printf("izvodim zadatak %d: %d/%d\n", i + 1, z + 1, random);
            sleep(1);
        }
        sem_post(&sem_5);
    }
    break;

    case 5:
    {
        int random = 1 + (rand() % 9);
        sem_wait(&sem_3);
        for (int z = 0; z < random; z++)
        {
            printf("izvodim zadatak %d: %d/%d\n", i + 1, z + 1, random);
            sleep(1);
        }
        sem_post(&sem_6);
    }
    break;

    case 6:
    {
        int random = 1 + (rand() % 9);
        sem_wait(&sem_4);
        sem_wait(&sem_5);
        sem_wait(&sem_6);
        for (int z = 0; z < random; z++)
        {
            printf("izvodim zadatak %d: %d/%d\n", i + 1, z + 1, random);
            sleep(1);
        }
    }
    break;
    }
    pthread_exit(arg);
}

int main()
{
    system("clear");

    polje_dretvi = new pthread_t[7];
    polje_i = new int[7];

    srand(time(0));

    sem_init(&sem_0, 0, 0);
    sem_init(&sem_1, 0, 0);
    sem_init(&sem_2, 0, 0);
    sem_init(&sem_3, 0, 0);
    sem_init(&sem_4, 0, 0);
    sem_init(&sem_5, 0, 0);
    sem_init(&sem_6, 0, 0);

    for (int i = 0; i < 7; i++)
    {
        polje_i[i] = i;
        pthread_create(&polje_dretvi[i], NULL, dretva, &polje_i[i]);
    }

    for (int i = 0; i < 7; i++)
        pthread_join(polje_dretvi[i], NULL);

    sigset(SIGINT, prekid);

    sem_destroy(&sem_0);
    sem_destroy(&sem_1);
    sem_destroy(&sem_2);
    sem_destroy(&sem_3);
    sem_destroy(&sem_4);
    sem_destroy(&sem_5);
    sem_destroy(&sem_6);

    delete[] polje_dretvi;
    delete[] polje_i;
    return 0;
}
