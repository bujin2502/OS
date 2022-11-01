#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>

using namespace std;

struct zajednicki
{
    int trazim[10];
    int broj[10];
    int najveci;
};

zajednicki *podaci;

int procesi;

key_t id_dijeljena_memorija;

void prekid(int sig)
{
    for (int i = 0; i < procesi; i++)
        wait(NULL);
    shmdt(podaci);
    shmctl(id_dijeljena_memorija, IPC_RMID, NULL);
    exit(0);
}

void k_o(int i)
{
    podaci->trazim[i] = 1;
    for (int z = 0; z < procesi; z++)
    {
        if (podaci->broj[z] > podaci->najveci)
        {
            podaci->najveci = podaci->broj[z];
        }
    }
    podaci->broj[i] = podaci->najveci + 1;
    podaci->trazim[i] = 0;
    for (int j = 0; j < procesi; j++)
    {
        while (podaci->trazim[j] != 0)
            ;
        while ((podaci->broj[j] != 0) && ((podaci->broj[j] < podaci->broj[i]) || ((podaci->broj[j] == podaci->broj[i]) && j < i)))
            ;
    }
}

void izl_k_o(int i)
{
    podaci->broj[i] = 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        system("clear");
        printf("Ovaj program mora primiti jedan argument - broj procesa!\n");
        return 0;
    }

    id_dijeljena_memorija = shmget(IPC_PRIVATE, sizeof(zajednicki), 0600);

    podaci = (zajednicki *)shmat(id_dijeljena_memorija, NULL, 0);

    procesi = atoi(argv[1]);

    /*     podaci->trazim = new int[10];
        podaci->broj = new int[10]; */

    podaci->najveci = 0;

    sigset(SIGINT, prekid);

    for (int i = 0; i < procesi; i++)
    {
        switch (fork())
        {
        case 0:
        {

            for (int k = 0; k < 5; k++)
            {
                k_o(i);
                for (int m = 0; m < 5; m++)
                {
                    printf("Proces: %d, K.O. br: %d (%d/5)\n", i + 1, k + 1, m + 1);
                    sleep(1);
                }
                izl_k_o(i);
            }
            exit(0);
        }
        case -1:
        {
            cout << "Fatalna pogreška!\n";
            exit(1);
        }
        }
    }

    prekid(0);
}
