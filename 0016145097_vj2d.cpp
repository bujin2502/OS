#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include <stdio.h>

using namespace std;

int *najveci;
int *trazim;
int *broj;

int procesi;

int id_dijeljena_memorija;

void prekid(int sig)
{
    for (int i = 0; i < procesi; i++)
        wait(NULL);
    shmdt(najveci);
    shmctl(id_dijeljena_memorija, IPC_RMID, NULL);
    exit(0);
}

void k_o(int i)
{
    trazim[i] = 1;
    for (int z = 0; z < procesi; z++)
    {
        if (broj[z] > *najveci)
        {
            *najveci = broj[z];
        }
    }

    broj[i] = *najveci + 1;
    trazim[i] = 0;

    for (int j = 0; j < procesi; j++)
    {
        while (trazim[j] != 0)
            ;
        while ((broj[j] != 0) && ((broj[j] < broj[i]) || ((broj[j] == broj[i]) && j < i)))
            ;
    }
}

void izl_k_o(int i)
{
    broj[i] = 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        system("clear");
        printf("Ovaj program mora primiti jedan argument - broj procesa!\n");
        return 0;
    }

    procesi = atoi(argv[1]);

    id_dijeljena_memorija = shmget(IPC_PRIVATE, (sizeof(int) + sizeof(int) * 2 * procesi), 0600);

    najveci = (int *)shmat(id_dijeljena_memorija, NULL, 0);
    trazim = najveci + 1;
    broj = trazim + procesi;

    *najveci = 0;

    for (int i = 0; i < procesi; i++)
    {
        broj[i] = 0;
        trazim[i] = 0;
    }

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
            cout << "Fatalna greska!\n";
            exit(1);
        }
        }
    }
    prekid(0);
}
