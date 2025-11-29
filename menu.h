#include <iostream>
#include <string>
#include <initializer_list>
#include <windows.h>
using namespace std;

int menu(string titulo, initializer_list<string> lista) {
    string linea = "";
    int op;
    int i=0;
    int tam = static_cast<int>(titulo.size());

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 1);
    cout << "\n________________ " << titulo << " _______________";
    for (const auto& s : lista){
        ++i; 
        cout << "\n (" << i << ") " << s;
    }
    cout << "\n (0) Salir";
    for (int i=0; i<tam; i++) { 
        linea += "_";
    }
    cout << "\n_________________" << linea << "________________";

    cout << "\n Que opcion deseas realizar : ";
    cin >> op;
    SetConsoleTextAttribute(h, 7);
    return op;
}