#include "interrupt_worker.h"
#include "button.h"
#include "config.h"
#include <util/atomic.h>
#include <util/delay.h>

static uint8_t BUFFER_A = 0x00;
static uint8_t BUFFER_B = 0x00;

static uint8_t *BUTTON_STATE = &BUFFER_A;

void processButtons() {
    uint8_t tmpState;
    uint8_t *idleBuffer;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmpState = *BUTTON_STATE;
        idleBuffer = BUTTON_STATE;
        if(BUTTON_STATE == &BUFFER_A) {
            BUTTON_STATE = &BUFFER_B;
        }
        else {
            BUTTON_STATE = &BUFFER_A;
        }
    }

    if(tmpState & (1 << BUTTON_0)) {
        tmpState &= BUTTON_0;
        ButtonOne();
    }
    if(tmpState & (1 << BUTTON_1)) {
        tmpState &= BUTTON_1;
        ButtonTwo();
    }
    if(tmpState & (1 << BUTTON_2)) {
        tmpState &= BUTTON_2;
        ButtonThree();
    }

    // Resetting underlying buffer
    *idleBuffer = 0x00;
    // Now tmpState should be 0x00 - *idleBuffer == tmpState
}

ISR(PCINT0_vect) {
    if(!(PINB & (1<<BUTTON_0))) {
        *BUTTON_STATE |= (1 << BUTTON_0);
    }
    if(!(PINB & (1<<BUTTON_1))) {
        *BUTTON_STATE |= (1 << BUTTON_1);
    }
    if(!(PINB & (1<<BUTTON_2))) {
        *BUTTON_STATE |= (1 << BUTTON_2);
    }
}