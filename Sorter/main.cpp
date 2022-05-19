#include <iostream>
#include "Application.h"

int main() {
	
	Sorter::Application app;
	app.Init();
	
	bool exit_status = app.Run();

	return exit_status;
}