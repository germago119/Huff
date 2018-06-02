/*
 * main.cpp
 *
 *  Created on: 2016��8��18��
 *      Author: zhenlin
 */

#include"HuffmanCode.h"
#include<iostream>
#include<fstream>
#include<string>
#include<cstring> // for strcmp
#include<ctime>
#include<iomanip> // for setprecision

using namespace std;

bool DoesFileExist(char *path) {
    ifstream is(path);
    return !!is;
}

int main() {

	int opcion;
    char *path_in = nullptr;
    char *path_out = nullptr;

	system("cls");
	cout << "-------Menu Principal------" << endl << endl << endl;
	cout << "1. Compresor " << endl;
	cout << "2. Decompresor " << endl;
	cout << "Digite una opcion: ";
	cin >> opcion;

	cout << "Digite el path de entrada: ";
	cin >> path_in;

	cout << "Digite el path de salida: ";
	cin >> path_out;

    string a = to_string(opcion);

    string b{"1"};
    string c{"2"};


    if (a == b) {
        if (DoesFileExist(path_in)) {
            time_t start, end;
            start = clock();
            HuffmanCode hc;
            hc.Compress(path_in, path_out);
            end = clock();
            cout << "Done! time: " << setprecision(2) << (end - start) / CLOCKS_PER_SEC << " seconds\n";
        } else {
            cout << "Error: " << path_in << " doesn't exist!\n";
        }
    } else if (a == c) {
        if (DoesFileExist(path_in)) {
            time_t start, end;
            start = clock();
            HuffmanCode hc;
            hc.Decompress(path_in, path_out);
            end = clock();
            cout << "Done! time: " << setprecision(2) << (end - start) / CLOCKS_PER_SEC << " seconds\n";
        } else {
            cout << "Error: " << path_in << " doesn't exist!\n";
        }
    }

    return 0;
}
