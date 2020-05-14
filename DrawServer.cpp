#include "DrawServer.h"

int* DrawServer::getN() {
	return N;
}

void DrawServer::setN(int a) {
	*N = a;
}

char* DrawServer::getMass(int i) {
	return Mass[i];
}

void DrawServer::changeMass(int* x, int* y, char* a) {
	Mass[*x][*y] = *a;
}

void DrawServer::draw() {
	system("cls");

	for (int i = 0; i < *N + 2; i++) {
		if (i == 0 || i == *N + 1)
			cout << " ";
		else
			cout << i;
	}
	cout << endl;
	for (int i = 0; i < *N + 2; i++) {
		for (int j = 0; j < *N + 2; j++) {
			cout << Mass[i][j];
			if (j == *N + 1) {
				if (i == 0 || i == *N + 1)
					cout << " ";
				else
					cout << i;
			}
		}
		cout << endl;
	}
	cout << endl;
}

void DrawServer::setup() {
	for (int i = 0; i < *N + 2; i++) {
		for (int j = 0; j < *N + 2; j++) {
			if (i == 0 || j == 0 || j == *N + 1 || i == *N + 1)
				Mass[i][j] = '#';
			else if (i == 1 && j == 1)
				Mass[i][j] = 'x';
			else if (i == *N && j == *N)
				Mass[i][j] = 'o';
			else
				Mass[i][j] = ' ';
		}
	}
}
