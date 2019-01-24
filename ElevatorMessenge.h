/** @file ElevatorMessenge.h 
 *  @author Robert Sochuliak
 *  @date 11 dec 2017
 */

/**
 * @brief Funkcia zasiela príkaz výťahu, kam sa má pohnúť alebo zastať.
 * V prípad zle zadaného parametra, príkaz sa ignoruje.
 * 
 * @param i - Paremeter je smer, ktorým sa má výťah pohnúť:
 *          -1 - dolu,
 *           0 - stop,
 *           1 - hore
 */
void moveElevator(int i);

/**
 * @brief Funkcia zasiela príkaz výťahu na rozsvietenie LED indikátorov
 *  V prípade zle zadaného parametru sa správa odošle s neplatnými dátami.
 * 
 * @param id - parameter je dvojciferné číslo.
 *  Prvá číslica rozpoznáva, či sa má LED rozsvietiť vo výťahu alebo na poschodí:
 *           1 - LED na poschodí,
 *           2 - LED vo výťahu
 *  Druhá číslica vyjadruje, na ktorom poschodí sa má LED rozsvietiť.
 *  Príklad: 0x10 - vonkajšia LED, prízemie
 */
void elevatorLedON(int id);

/**
 * @brief Funkcia zhasne oba LED inikátory na poschodí.
 * 
 * @param id - Číslo poschodia, na ktorom sa majú shasnúť LED indikátory.
 */
void elevatorLedOFF(int id);

/**
 * @brief Funkcia odošle predpirpavenú správu, na konzolu výťahu. 
 *  Využitie tejto medódy bolo najmä pri ladení programu.
 */
void sendError();

/**
 * @brief Funkcia odosiela výťahu príkaz na ovládanie zámku kabíny.
 * 
 * @param lock - Paremeter je boolovska premenná:
 *          true - kabína sa uzamkne,
 *          false - kabína sa odomkne
 */
void elevatorCabineLock(bool lock);

/**
 * @brief Funkcia odosiela výťahu príkazy na ovládanie dispeja.
 *  Displej zobrazuje, či je výťah v pohybe a na ktorom poschodí sa nachádza.
 * 
 * @param direction - Paremeter je smer pohybu výťahu: 
 *          -1 - ide smerom dolu,
 *           0 - stojí,
 *           1 - ide smerom hore
 * @param floor - Parameter je číslo aktuálneho poschodi, kde sa výtáh nachádza.
 *          V prípade neplatného údaju sa vykreslí na displeji "-".
 */
void elevatorDisplay(int direction, int floor);
