#include "WatchDog.h"
#include "Communication.h"


Ticker watchDog;

//Odosielanie správy s dátami pre reštart WatchDog
void resetWatchDog()
{
    char command [] = {0xff};
    paket reset = {.adresa = 0xfe, .dlzkaDat = 0x01, .data = command};
    sendData(reset);
}

//Inicializácia časovača, ktorý vyvolá po uplynutí času prerušenie a
//zavolá obsluhu prerušenia, parameter je časový interval medzi prerušeniami
void init_watchDog(float time)
{
    watchDog.attach(&resetWatchDog, time);
}