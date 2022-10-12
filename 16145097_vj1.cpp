#include<iostream>
#include<cstdlib>
#include<signal.h>

using namespace std;

int N;
int n;

int main (int argc, char *argv[]) {

    if (argc != 2) {
        cout << "Ovaj program mora primiti jedan argument!\n";
        cout << "Pri unosu programa u komandnoj liniji na kraju dodajte jedan broj veci od broj 3.\n";
        return 0;
    }

    N = atoi(argv[1]);

    if (N<4) {
        cout << "Unesen je broj manji od broja 4! Morate unjeti broj veci od broja 3.\n";
        return 0;
    }

    cout << "Parametri su ispravno uneseni.\n";
    cout << "Unesen je broj zigica i isti iznosi: " << N << ".\n";
    cout << "Tijekom igre mozete povlaciti jednu, dvije ili tri zigice.\n";
    
    bool igrac_a = true;

    do {
            if (igrac_a) {
        cout << "Zigica na stolu: " << N << ". Na redu igrac A.\n";
        cout << "Zigica igrac A uzeo sa stola:\n";
        do {
            cin >> n;
        } while (n<1 || n>3 || n>N);
        N = N - n;
        }  else {
        cout << "Zigica na stolu: " << N << ". Na redu igrac B.\n";
        cout << "Zigica igrac B uzeo sa stola:\n";
        do {
            cin >> n;
        } while (n<1 || n>3 || n>N);
        N = N - n;
        }
    igrac_a = !igrac_a;
    } while (N>0);

    if (N<1) {
        cout << "Ostalo je " << N << " zigica na stolu.\n";
    if (igrac_a) {
        cout << "Pobjednik je igrac A\n";
        }
    else {
        cout << "Pobjednik je igrac B\n";
        }
    }
    return 0;
}
