/** @file FreeFall.h 
 *  @author Robert Sochuliak
 *  @date 8 Jan 2018
 */

#include <I2C.h>
#include "MMA8451Q.h"

#define MMA8451_I2C_ADDRESS (0x1d<<1)  // Adresa pre nastavenie akcelerometra

PinName const SDA = PTE25;             // Konektor pre komunikáciu pomocou I2C
PinName const SCL = PTE24;             // Konektor pre komunikáciu pomocou I2C

/**
 * @brief Inicializácia akcelerometra na doske, nastanievie na voľný pád.
 * Frekvencia akcelerometra je nastavená na 400Hz.
 * Na konci inicializácie sa nastaví preruženie, ktoré bude vyvolaná v prípade voľného pádu.
 * Nastavenie je vykonané presne podľa inštrukcií v priloženom odkaze.
 * @see: https://www.nxp.com/docs/en/application-note/AN4070.pdf?&fasp=1&WT_TYPE=Applicatio
 */
void init_freeFall();

/**
 * @brief Funkcia, ktorú vyvolá prerušenie indikujúce voľný pád
 * Funkcia pošle výťahu príkaz na okamžité zastavenie, otvorí kabínu a vzpíše na konzolu varovný výpis.
 * Indikačná dióda na doske začne svietiť na červeno.
 */
void freeFall();

/**
 * @brief Funkcia ziťuje, či nastal voľný pád
 * 
 * @return true - Nastal voľný pád.
 * @return false - Všetko funguje ako má.
 */
bool freeFall_status();

void ledBlick();