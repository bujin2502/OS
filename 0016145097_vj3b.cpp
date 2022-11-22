#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

using namespace std;

struct podaci
{
    int ULAZ;
    int IZLAZ;
    int UKUPNO;
    int M[5];
};
podaci *pokazivac;

int broj_proizvodjaca, broj_brojeva, memorijaID, semaforID;

sembuf Semafori;

void postavi_sem(int id_semafor)
{
    Semafori.sem_num = id_semafor;
    Semafori.sem_op = 1;
    semop(semaforID, &Semafori, 1);
}

void ispitaj_sem(int id_semafor)
{
    Semafori.sem_num = id_semafor;
    Semafori.sem_op = -1;
    semop(semaforID, &Semafori, 1);
}

void prekid(int sig)
{
    for (int i = 0; i < broj_proizvodjaca; i++)
        wait(NULL);
    shmdt(pokazivac);
    shmctl(memorijaID, IPC_RMID, NULL);
    semctl(semaforID, IPC_RMID, 0);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        system("clear");
        printf("Ovaj program mora primiti dva argumenta - broj proizvodjaca i slucajnih brojeva!\n");
        return 0;
    }

    broj_proizvodjaca = atoi(argv[1]);
    broj_brojeva = atoi(argv[2]);

    memorijaID = shmget(IPC_PRIVATE, sizeof(podaci), 0600);
    pokazivac = (podaci *)shmat(memorijaID, NULL, 0);

    semaforID = semget(IPC_PRIVATE, 3, 0600);
    Semafori.sem_flg = 0;
    semctl(semaforID, 0, SETVAL, 5); // PUN
    semctl(semaforID, 1, SETVAL, 1); // PISI
    semctl(semaforID, 2, SETVAL, 0); // PRAZAN

    sigset(SIGINT, prekid);

    pokazivac->UKUPNO = broj_brojeva;
    pokazivac->ULAZ = 0;
    pokazivac->IZLAZ = 0;

    srand(time(0));

    for (int i = 0; i < broj_proizvodjaca; i++)
    {
        if (fork() == 0)
        {
            for (int z = 0; z < pokazivac->UKUPNO; z++)
            {
                ispitaj_sem(0);
                ispitaj_sem(1);
                pokazivac->M[pokazivac->ULAZ] = (long double)rand() / (RAND_MAX - 1) * 1000;
                printf("Proizvodjac %d salje %d\n", i + 1, pokazivac->M[pokazivac->ULAZ]);
                sleep(1);
                pokazivac->ULAZ = (pokazivac->ULAZ + 1) % 5;
                postavi_sem(1);
                postavi_sem(2);
            }
            printf("Proizvodjac %d je zavrsio sa slanjem\n", i + 1);
            exit(0);
        }
    }

    if (fork() == 0)
    {
        int zbroj = 0;
        for (int i = 0; i < pokazivac->UKUPNO * broj_proizvodjaca; i++)
        {
            ispitaj_sem(2);
            printf("Potrosac prima %d\n", pokazivac->M[pokazivac->IZLAZ]);
            sleep(1);
            zbroj += pokazivac->M[pokazivac->IZLAZ];
            pokazivac->IZLAZ = (pokazivac->IZLAZ + 1) % 5;
            postavi_sem(0);
        }
        printf("Potrosac - zbroj primljenih brojeva = %d\n", zbroj);
        exit(0);
    }
    for (int i = 0; i < broj_proizvodjaca; i++)
        wait(NULL);
    prekid(0);
}
