#include <iostream>
#include "ADDR.h"

//using namespace std;

int main(int argc, char* argv[]) {
	ADDR client;
	while(client.connection()){}
	return 0;
}
