#include "Application.h"

int main()
{
  Application *application = new Application();
  application->run();
  delete application;
  
  return 1;
}