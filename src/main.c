#include "milis.h"
#include "stm8s.h"
#include "max.h"

#define DIN_PORT GPIOD          // data in
#define DIN_PIN GPIO_PIN_4
#define CS_PORT GPIOD           // chip select
#define CS_PIN GPIO_PIN_5
#define CLK_PORT GPIOD          // clock
#define CLK_PIN GPIO_PIN_6

#define LOW(BAGR) GPIO_WriteLow(BAGR##_PORT, BAGR##_PIN)
#define HIGH(BAGR) GPIO_WriteHigh(BAGR##_PORT, BAGR##_PIN)
#define REVERSE(BAGR) GPIO_WriteReverse(BAGR##_PORT, BAGR##_PIN)

int send_max(uint8_t address, uint8_t data)
{
    uint8_t mask;
    LOW(CS);
    
    mask = 1 << 7; // 0b10000000
    while(mask) {
        if(address & mask) {
            HIGH(DIN);
        } else {
            LOW(DIN);
        }
        HIGH(CLK);
        mask = mask >> 1;
        LOW(CLK);
    }

    mask = 1 << 7;
    while(mask) {
        if(data & mask) {
            HIGH(DIN);
        } else {
            LOW(DIN);
        }
        HIGH(CLK);
        mask = mask >> 1;
        LOW(CLK);
    }

    HIGH(CS);
}

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();
}


void main(void)
{
    int32_t time = 0;
    uint8_t bagr = 1;

    setup();
    LOW(CS);
    
    send_max(DECODE_MODE, DECODE_ALL); // zapnutí znakové sady na všech cifrách
    send_max(INTENSITY, 3); //nastavení jasu
    send_max(SCAN_LIMIT, 7); //velikost displeje je 8 cifer (počítáno od nuly)


    send_max(DISPLAY_TEST, 0); //
    send_max(SHUTDOWN, 1); // zapne display

    send_max(1, 1);
    send_max(2, 2);
    send_max(3, 3);
    send_max(4, 4);
    send_max(5, 5);
    send_max(6, 6);
    send_max(7, 7);

    while (1) {
        if (milis() - time > 333) {
            time = milis();
            send_max(8, bagr << 1);
            if(bagr) {
                bagr = 1;
            }

        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"