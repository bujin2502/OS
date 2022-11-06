#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>

using namespace std;

int *najveci, *trazim, *broj;

int procesi, id_dijeljena_memorija;

void prekid(int sig)
{
    for (int i = 0; i < procesi; i++)
        wait(NULL);
        shmdt(najveci);
    shmdt(trazim);
    shmdt(broj);
    shmctl(id_dijeljena_memorija, IPC_RMID, NULL);
    exit(0);
}

void k_o(int i)
{
    trazim[i] = 1;
    broj[i] = 0;
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

    id_dijeljena_memorija = shmget(IPC_PRIVATE, sizeof(int) + sizeof(int) * procesi * 2, 0600);

    trazim = new int[procesi];
    broj = new int[procesi];

    najveci = new int;
    najveci = 0;

    trazim = (int *)shmat(id_dijeljena_memorija, NULL, 0);
    broj = (int *)shmat(id_dijeljena_memorija, NULL, 0);
    najveci = (int *)shmat(id_dijeljena_memorija, NULL, 0);

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
