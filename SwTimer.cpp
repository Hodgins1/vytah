#include "SwTimer.h"

//Metóda vracia hodnotu, či už časovať nadobudol požadovanú hodnotu
bool SwTimer::isExpired(){   
  return  GlTimeFn() >= (endTime);
}

//Inicializácia časovača
void SwTimer::startTimer(unsigned int delay){
  _delay=delay;
  endTime =  GlTimeFn() + delay;
}

//Reštartovanie časovača
void SwTimer::restartTimer(){
  SwTimer::startTimer(_delay);
}
