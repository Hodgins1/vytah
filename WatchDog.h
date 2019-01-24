/** @file WatchDog.h
 *  @author Robert Sochuliak
 *  @date 12 dec 2017
 */


/**
 * @brief Funkcia odosiela výťahu príkaz na reštartovanie Watchdog
 */
void resetWatchDog();

/**
 * @brief Funkcia inicializuje časovať, ktorý v pravidelných intervaloch bude reštartovať Watchdog
 * 
 * @param time - Parameter času v sekundách, v po ako intervale času sa má vygenerovať prerušenie a zavolať obslužná Funkcia
 */
void init_watchDog(float time);