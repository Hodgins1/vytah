#include "Communication.h"
#include "ElevatorMessenge.h"


// Funkcia, ktorá posiela príkazy výťahu na pohyb
void moveElevator(int i)
{
    if (i == 0) // Výťah má zastať
    {
        char stopMotor[] = {0x01};
        paket stop = {.adresa = 0xf1, .dlzkaDat = 0x01, .data = stopMotor};
        sendData(stop);
    }
    if (i == 1) // Výťah sa má pohnúť smerom na hor - rýchlosť 100
    {
        char up[] = {0x02, 0x00, 0x00, 0x00, 0x64};
        paket moveUp = {.adresa = 0xf1, .dlzkaDat = 0x05, .data = up};
        sendData(moveUp);
    }
    if (i == -1) // V=yťah sa má pohnúť smerom na dol - rýchlosť -100
    {
        char down[] = {0x02, 0xff, 0xff, 0xff, 0x9c};
        paket moveDown = {.adresa = 0xf1, .dlzkaDat = 0x05, .data = down};
        sendData(moveDown);
    }
}

//Funkcia rieši zasviedenie LED vo výťahu, nutný 2 miestny parameter
void elevatorLedON(int id)
{
    char command[] = {0x01};
    char adresa = 0x00;

    switch(id) // prvá vonkajšia/vnútorná LED, druhá číslica je poschodie 
    {   
        case 01: adresa = 0x10 ; break;   // Prízemie, LED na poschodí-vonkajšia
        case 02: adresa = 0x20 ; break;   
        case 11: adresa = 0x11 ; break;
        case 12: adresa = 0x21 ; break;   // 1.poschodie, vnútorná LED
        case 21: adresa = 0x12 ; break;
        case 22: adresa = 0x22 ; break;
        case 31: adresa = 0x13 ; break;
        case 32: adresa = 0x23 ; break;
        case 41: adresa = 0x14 ; break;
        case 42: adresa = 0x24 ; break;
    }
    paket ledOn = {.adresa = adresa, .dlzkaDat = 0x01, .data = command};
    sendData(ledOn); // Odoslanie správy
}

//Funkcia, ktorá posiela príkazy výťahu, čo sa má zobraziť na displeji
void elevatorDisplay(int direction, int floor)
{
    char command [2];
    switch(direction) // Zobrazuje pohyb výťahu
    {
        case -1: command[0]=0x02; break; // Smer dolu
        case  0: command[0]=0xFF; break; // Stojí
        case  1: command[0]=0x01; break; // Smer hore
        default:
                command[0]=0x00;  // Neplatné dáta
    }
    switch(floor) // Aktuálne poschodie
    {
        case 0: command[1]=0x50; break; // Prízemie
        case 1: command[1]=0x31; break; // 1.poschodie
        case 2: command[1]=0x32; break;
        case 3: command[1]=0x33; break;
        case 4: command[1]=0x34; break; // 4.poschodie
        default:
                command[1]=0x2D; // Nedefinovaný stav "-"
    }

    paket display = {.adresa = 0x30, .dlzkaDat = 0x02, .data = command};
    sendData(display); // Odoslanie správy
}

//Funkcia na zatváranie a otváranie dverí
void elevatorCabineLock(bool lock)
{
    char command[1];
    if (lock) //ak bol parameter true - dvere sa zavrú
    {
        command[0] = 0x01;
    }
    else //inak sa dvere otvoria
    {
        command[0] = 0x00;
    }
    paket reset = {.adresa = 0xf0, .dlzkaDat = 0x01, .data = command};
    sendData(reset); // Odoslanie správy
}

//Funkcia zasielajúca na konzolu výťahu chybovú hlášku
void sendError()
{
    paket errorText = {.adresa = 0xD0, .dlzkaDat = 0x0C, .data = "Error data\n\r"};
    sendData(errorText);
}

//Funkcia na zhas=inanie LED indikátorov - zhasnú obe LED na poschodí
void elevatorLedOFF(int id)
{
    char command [] = {0x00};
    char adresa_1=0x00;
    char adresa_2=0x00;
    
    switch(id) // Na ktorom poschodí sa má zhasnúť
    {
        case 0: adresa_1 =0x10; adresa_2 =0x20  ;break;  //Prízemie
        case 1: adresa_1 =0x11; adresa_2 =0x21  ;break;  //1.poschodie
        case 2: adresa_1 =0x12; adresa_2 =0x22  ;break;
        case 3: adresa_1 =0x13; adresa_2 =0x23  ;break;
        case 4: adresa_1 =0x14; adresa_2 =0x24  ;break;
    }
    paket ledOff_1 = {.adresa = adresa_1, .dlzkaDat = 0x01, .data = command};
    paket ledOff_2 = {.adresa = adresa_2, .dlzkaDat = 0x01, .data = command};
    sendData(ledOff_1); // LED na poschodí
    sendData(ledOff_2); // LED vo výťahu
}