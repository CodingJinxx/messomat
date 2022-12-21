#include <avr/io.h>
#include "setup.h"
#include "button.h"
#include "interrupt_worker.h"
#include <stdlib.h>
#include <lcd.h>
#include <avr/delay.h>


int main(int argc, char **argv) {
  setup();
  while(1)
  {
    processButtons();
  }
}
