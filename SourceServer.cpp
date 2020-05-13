#include <iostream>
#include "ADDR.h"

int main(int argc, char* argv[]) {
	ADDR server;
	while (server.connection()) {}
	return 0;
}
