/**
 * @file Communication.h
 * @author Robert Sochuliak
 * @date 8 Jan 2018
 */

#include "mbed.h"

#define BAUDRATE 57600      // Rýchlosť komunikácie 
#define MY_ADDR 0x01        // Adresa zariadenia, našej dosky
#define START_BYTE 0xA0     // Definovanie štartovacieho znaku

#define MESS_BUFFER 100     // Veľkosť buffra pre prijaté správy

#define msPerTick 10        // Pocet mS na jeden takt(tick) Tikera

/**  Štrukúra, ktorá definuje ako vyzerá paket na odoslenie
  *  @note Atribút - packed = deklarovanie kompilátoru, aby nezarovnával dáta v pamäťových bunkách
  *  @param adresa - adresa, na ktorú sa ma daná správa poslať
  *  @param dlzkaDat - počet znakov v správe
  *  @param *data - smerník na pole dát, kde je správa umiestnená
  */  
typedef struct  paket {
    char adresa;
    int dlzkaDat;
    char *data;
}__attribute__((__packed__))  paket ;

/**
 * @brief Funkcia odosiela správy simulátor.
 *  K dátam dopĺňa štart-byte, svoju adresu a dopočítava CRC kód
 *
 * @param sprava Parameter je štruktúra, ktorá obsahuje adresu kam sa má správa odoslať,
 *  aká dlhá je správa a samotné dáta.
 */
void sendData(paket sprava);

/**
 * @brief Funkcia načítava prichádzajúce správy a vyhodnocuje ich CRC kód. Ak správa ktorá prišla
 * je správna, správu uloží do buffera správ. Funkcia je volaná na prerušenie cez sériový port.
 */
void recieveData();

/**
 * @brief  Funkcia zisťuje, na ktorom poschodí bolo stlačené tlačidlo
 * 
 * @return int - číslo poschodia, kde bolo stlačené tlačidlo
 */
int nextFloor();

/**
 * @brief Funkcia zisťuje, či bolo niektoré z tlačidiel na privolanie výťahu stlačené. 
 * Funkcia je využívaná v čakacích slučkách.
 * 
 * @param actPos Aktuálna poloha výťahu pre overovanie, či už výťah sa nenachádza na danom poschodí
 * @return true - Tlačidlo bolo stlačené, výťah sa má pohnúť
 * @return false - Žiadne tlačidlo nebolo stlačené, výťah stojí
 */
bool moveOn(int actPos);

//! Pomocná tabulka pri výpočte CRC8 - Cyclic redundancy check / kontolu cyklickým kódom
const unsigned char CRC8_TAB[] = {
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
    190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
    219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
    87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53    
};
