#include <Engine.h>
#include "App.h"

int main(int argc, char** argv) {
	
	App* app = new App();
	app->Run();
	delete app;
	return 0;
}
