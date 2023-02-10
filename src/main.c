#include "milis.h"
#include "stm8s.h"
#include "max7219.h"

#include "uart1.h"
// toto jenom kvuli uart1
#include "stdio.h"

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    init_max();

    init_milis();

    init_uart1();
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

    char chart = 'D';


    while (1) {
        if (milis() - time > 333) {
            time = milis();
            send_max(8, bagr << 1);
            if(bagr) {
                bagr = 1;
            }

            putchar(chart);
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"