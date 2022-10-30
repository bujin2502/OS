#include <iostream>
#include <signal.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

struct zajednicki
{
    int *ulaz;
    int *broj;
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

void k_o(int proces)
{
    podaci->ulaz = new int[procesi];
    podaci->broj = new int[procesi];
    podaci->najveci = 0;
    podaci->ulaz[proces] = 1;
    for (int i = 1; i <= procesi; i++)
    {
        if (podaci->broj[i] > podaci->najveci)
            podaci->najveci = podaci->broj[i];
    }
    podaci->broj[proces] = podaci->najveci + 1;
    podaci->ulaz[proces] = 0;
    printf("Ulaz: %d\n", podaci->ulaz[proces]);
    printf("Broj: %d\n", podaci->broj[proces]);
    printf("Najveci: %d\n", podaci->najveci);
    for (int j = 1; j <= procesi; j++)
    {
        while (podaci->ulaz[j] == 1)
            ;
        while ((podaci->broj[j] != 0) && ((podaci->broj[j] < podaci->broj[proces]) || ((podaci->broj[j] == podaci->broj[proces]) && j < proces)))
            ;
    }
}

void izl_k_o(int proces)
{
    podaci->broj[proces] = 0;
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

    for (int i = 1; i <= procesi; i++)
    {
        switch (fork())
        {
        case 0:
        {
            for (int k = 1; k <= 3; k++)
            {
                k_o(i);
                for (int m = 1; m <= 3; m++)
                {
                    printf("Proces: %d, K.O. br: %d (%d/3)\n", i, k, m);
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
