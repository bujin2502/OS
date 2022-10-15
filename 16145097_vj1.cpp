#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>

using namespace std;

int N;
int n;

void prekid(int sig)
{
    cout << "Izlazak iz igre!" << endl;
    exit(0);
}

void odabir_A(int sig)
{
    sighold(SIGINT);
    sighold(SIGQUIT);
    cout << "Zigica igrac A uzeo sa stola:" << endl;
    do
    {
        cin >> n;
        if (n < 1 || n > 3 || n > N)
        {
            cout << "Broj mora biti izmedu 1 i 3 i ne moze biti veci od broja zigica na stolu!" << endl;
        }
    } while (n < 1 || n > 3 || n > N);
    N = N - n;
}

void odabir_B(int sig)
{
    sighold(SIGINT);
    sighold(SIGQUIT);
    cout << "Zigica igrac B uzeo sa stola:" << endl;
    do
    {
        cin >> n;
        if (n < 1 || n > 3 || n > N)
        {
            cout << "Broj mora biti izmedu 1 i 3 i ne moze biti veci od broja zigica na stolu!" << endl;
        }
    } while (n < 1 || n > 3 || n > N);
    N = N - n;
}

int main(int argc, char *argv[])
{

    pause();
    if (argc != 2)
    {
        cout << "Ovaj program mora primiti jedan argument!" << endl;
        cout << "Pri unosu programa u komandnoj liniji na kraju dodajte jedan broj veci od broj 3." << endl;
        return 0;
    }
    N = atoi(argv[1]);

    if (N < 4)
    {
        cout << "Unesen je broj manji od broja 4! Morate unjeti broj veci od broja 3." << endl;
        return 0;
    }

    cout << "Parametri su ispravno uneseni." << endl;
    cout << "Unesen je broj zigica i isti iznosi: " << N << "." << endl;
    cout << "Tijekom igre mozete povlaciti jednu, dvije ili tri zigice." << endl;

    sigset(SIGTSTP, prekid);

    bool igrac_a = true;

    do
    {
        if (igrac_a)
        {
            sighold(SIGQUIT);
            cout << "Zigica na stolu: " << N << ". Na redu igrac A. Unesi Ctrl-C." << endl;
            sigset(SIGINT, odabir_A);
            pause();
        }
        else
        {
            sighold(SIGINT);
            cout << "Zigica na stolu: " << N << ". Na redu igrac B. Unesi Ctrl-\\" << endl;
            sigset(SIGQUIT, odabir_B);
            pause();
        }
        igrac_a = !igrac_a;
    } while (N > 0);

    if (N < 1)
    {
        cout << "Ostalo je " << N << " zigica na stolu." << endl;
        if (igrac_a)
        {
            cout << "Pobjednik je igrac A! Cestitam!" << endl;
        }
        else
        {
            cout << "Pobjednik je igrac B! Cestitam!" << endl;
        }
    }
    return 0;
}
