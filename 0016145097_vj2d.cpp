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

int max () {
    int zadnji = 0;
	for(int i = 0; i < procesi; i++){
		if(podaci->broj[i] > zadnji) zadnji = podaci->broj[i];
	}
	return zadnji + 1;
}

void k_o(int i)
{
    podaci->trazim[i] = 1;

    podaci->broj[i] = max();
    
    podaci->trazim[i] = 0;
    
    printf("%d, %d, %d, %d\n", i, podaci->trazim[i], podaci->broj[i], podaci->najveci);
sleep(1);
    for (int j = 0; j < procesi; j++)
    {
        while (podaci->trazim[j] != 0);
        while ((podaci->broj[j] != 0) && ((podaci->broj[j] < podaci->broj[i]) || ((podaci->broj[j] == podaci->broj[i]) && j < i)));
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

    podaci->trazim = new int[procesi];
    podaci->broj = new int[procesi];

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
            }            
            izl_k_o(i);
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
