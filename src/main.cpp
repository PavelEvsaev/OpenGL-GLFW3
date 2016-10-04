#include "Application.h"

int main()
{
  	Application *application = new Application();
  	application->run(application);
  	delete application;
  
  	return 1;
}