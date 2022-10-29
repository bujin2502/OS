#include <iostream>
#include <signal.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

struct zajednicki
{
    int *trazim;
    int *broj;
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

void k_o(int x)
{
}

void izl_k_o(int x)
{
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

    sigset(SIGINT, prekid);

    for (int i = 0; i < procesi; i++)
    {
        switch (fork())
        {
        case 0:
        {
            for (int k = 1; k <= 5; k++)
            {
                k_o(i);
                for (int m = 1; m <= 5; m++)
                {
                    printf("Proces: %d, K.O. br: %d (%d/5)\n", i, k, m);
                    sleep(1);
                }
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
