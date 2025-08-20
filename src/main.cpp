#include "Logger.hpp"

int main(void)
{
  DEBUGMODE(true);
  DBG(METHOD_NAME + " called.");
  LOG("Starting application.");
  LOG("Application finished successfully.");
  return (0);
}
