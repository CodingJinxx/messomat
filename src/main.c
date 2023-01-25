#include <avr/io.h>
#include "deps/main.h"
#include "interrupt_worker.h"


int main(int argc, char **argv) {
  atmega_setup();
  while(1)
  {
    processButtons();
  }
}
