#include "Communication.h"
#include "ElevatorThreads.h"
#include "ElevatorMessenge.h"
#include "FreeFall.h"
#include "WatchDog.h"

Serial serial(USBTX, USBRX, BAUDRATE);
Ticker elevatorOpen; //Časovač, ktorý sa spustí pri orvorení dverý, aby sa hneď nezavreli

uint32_t tick = 0; // Globalna premenna - pocet tikov Tikera od jeho spustenia
struct paket buffer[MESS_BUFFER]; //Zásobník priajtých správ
int indexBuffer = 0;              //Index, ktorý ukazuje ktorá správa bola pridaná naposledy

int elevatorButton[] = {0, 0, 0, 0, 0}; //Pole pre tlačidlá výťahu, index reprezentuje dané poschodie
int elevatorSensor[] = {0, 0, 0, 0, 0}; //Pole pre senzory výťahu, index reprezentuje dané poschodie

int actualMess = 0;     //Index správy ktorá bola spracovaná ako posledná
int endFloor;           //Poschodie kam sa výťah musí po stlačení tlačidla dosať
int actualFloor = 5;    //Poschodie, kde sa výŤah aktuálne nachádza
int moveDirection = 0;  //Smer pohybu výťahu

//Funkcia, ktorá počíta tiky - vyvolávaná prerušením
void timeTick(){ tick++; }

//Funkcia, ktora vracia cas v ms od zaciatku behu programu
int getTimer(){ return (tick * msPerTick); }

//Funkcia zisťuje, či sa má výťah kam pohnúť alebo má ostať stáť
bool moveOn(int actPos)
{
    for (int i = 0; i < 5; i++) //Cyklus prechádza všetky poschodia
    {
        if (elevatorButton[i] > 0) // Ak bolo stlačené tlačidlo
        {
            if (i == actPos) //AK bolo stlačené tlačidlo na poschodí, kde stojí výťah
            {
                elevatorButton[i] = 0;      //Nulovanie tohot tlačidla
                elevatorLedOFF(i);          //Vypnutie LED na tomto poschodí
                elevatorCabineLock(false);   // Otvorenie dveri
            }
            else
            {
                elevatorCabineLock(true);   // Zatvorenie dveri
                return true;  //Výťah sa má kam pohnúť
            }
        }
    }
    return false; //Výťah má stáť na danom poschodí
}

//Funkcia vracia číslo poschodia, kam sa má výťah pohnúť
int nextFloor()
{
    for(int i=actualFloor; i<5; i++)
    {
        if( elevatorButton[i] > 0)
        {
            return i;
        }
    }
    
    for(int i=actualFloor - 1; i>=0; i--)
    {
        if( elevatorButton[i] > 0)
        {
            return i;
        }
    }
    return -1; 
}

// Proto-vlakno na spracovanie prijatých správ zo zásobníka
bool MessExecute::Run()
{
  PT_BEGIN();  // inicializácia vlákna
  while(1) {   // nekonečná slučka
    PT_WAIT_UNTIL(actualMess != indexBuffer);  //Čakanie kým nepribudne správa, ktorá nebola spracovaná    
    switch (buffer[actualMess].adresa)         //Vykonanie správy, podľa toho komu bola správa odoslaná
    {
        //Správa indikovala, že bolo stlačené tlačidlo na privolanie výťahu
        case 0xc0: elevatorLedON(01);elevatorButton[0] = 1; break;  //Tlačidlo na poschodí, prízemie
        case 0xb0: elevatorLedON(02);elevatorButton[0] = 2; break;  //Tlačidlo vo výťahu, prízemie
        case 0xc1: elevatorLedON(11);elevatorButton[1] = 1; break;
        case 0xb1: elevatorLedON(12);elevatorButton[1] = 2; break;
        case 0xc2: elevatorLedON(21);elevatorButton[2] = 1; break;
        case 0xb2: elevatorLedON(22);elevatorButton[2] = 2; break;
        case 0xc3: elevatorLedON(31);elevatorButton[3] = 1; break;
        case 0xb3: elevatorLedON(32);elevatorButton[3] = 2; break;
        case 0xc4: elevatorLedON(41);elevatorButton[4] = 1; break;
        case 0xb4: elevatorLedON(42);elevatorButton[4] = 2; break;  //Tlačidlo vo výťahu, 4.poschodie

        //Správa indikovala, že bol zopnutý/uvolnený niektorý zo snímačov
        case 0xe0: if(buffer[actualMess].data[0] == 0x00){ elevatorSensor[0] = 0;break;}  //Uvolenie spínačov, prízemie
                   if(buffer[actualMess].data[0] == 0x01){ elevatorSensor[0] = 1;break;}  //Žltý spínač, prízemie
                   if(buffer[actualMess].data[0] == 0x02){ elevatorSensor[0] = 2;}break;  //Červený snímač, prízemie
        case 0xe1: if(buffer[actualMess].data[0] == 0x00){ elevatorSensor[1] = 0;break;} 
                   if(buffer[actualMess].data[0] == 0x01){ elevatorSensor[1] = 1;break;} 
                   if(buffer[actualMess].data[0] == 0x02){ elevatorSensor[1] = 2;}break; 
        case 0xe2: if(buffer[actualMess].data[0] == 0x00){ elevatorSensor[2] = 0;break;} 
                   if(buffer[actualMess].data[0] == 0x01){ elevatorSensor[2] = 1;break;} 
                   if(buffer[actualMess].data[0] == 0x02){ elevatorSensor[2] = 2;}break;
        case 0xe3: if(buffer[actualMess].data[0] == 0x00){ elevatorSensor[3] = 0;break;} 
                   if(buffer[actualMess].data[0] == 0x01){ elevatorSensor[3] = 1;break;} 
                   if(buffer[actualMess].data[0] == 0x02){ elevatorSensor[3] = 2;}break;
        case 0xe4: if(buffer[actualMess].data[0] == 0x00){ elevatorSensor[4] = 0;break;} 
                   if(buffer[actualMess].data[0] == 0x01){ elevatorSensor[4] = 1;break;}
                   if(buffer[actualMess].data[0] == 0x02){ elevatorSensor[4] = 2;}break;  //Červený snímač, 4.poschodie
        
        //Správa pre konzolu alebo pre iné periférie, odosielanie na konzolu výťahu           
        default:
                char sendBackMess[buffer[actualMess].dlzkaDat + 1]; //Vysklanie paketu pre odoslanie správy
                sendBackMess[0] = buffer[actualMess].adresa;
                for (int i=0; i < buffer[actualMess].dlzkaDat; i++)
                {
                    sendBackMess[i+1] = buffer[actualMess].data[i];    
                }
                paket messege = {.adresa = 0xd0, .dlzkaDat = buffer[actualMess].dlzkaDat + 1, .data = sendBackMess};
                sendData(messege); //Odoslanie správy
    }
    
    free(buffer[actualMess].data);   // Uvolenie pamäte, kde boli uložené dáta
    if((actualMess++) > MESS_BUFFER) // Cyklický Buffer
    {
       actualMess = 0; 
    }
  }
  PT_END();
}

//Vlakon na pohyb vytahu
bool Elevator::Run()
{
  PT_BEGIN();  // inicializácia vlákna
  while(1) {   // nekonečná slučka

      PT_WAIT_UNTIL(moveOn(actualFloor)); //Vlákno čaká, kým sa výťah nemá kam pohnúť
      endFloor = nextFloor();             //kam idem - poschodie

      if ((endFloor - actualFloor) > 0)   //Zisťovanie smeru pohybu
      {
          moveDirection = 1;
      }
      else
      {
          moveDirection = -1;
      }
      moveElevator(moveDirection);                 //pohnem sa smerom, ktorý som si vyššie zistil
      elevatorDisplay(moveDirection, actualFloor); //Zobrazenie pohybu na dispeji
      elevatorCabineLock(true);                    //Zamknutie kabíny
      
      while (((endFloor - actualFloor) * moveDirection) > 1)   // Ošetrenie medzizatávok
      {
          elevatorLedOFF(actualFloor);            

          PT_WAIT_UNTIL(elevatorSensor[actualFloor + moveDirection] == 1);   //Čakám na žltý senzor
          if (elevatorButton[actualFloor + moveDirection] > 0)               //Zisťujem, či na danom poschodí mám zastaviť
          {
              moveElevator(0);
          }
          PT_WAIT_UNTIL(elevatorSensor[actualFloor + moveDirection] == 2);   //čakám na červený senzor
          if (elevatorButton[actualFloor + moveDirection] > 0)               //Zisťujem či mám zastaviť
          {
              elevatorDisplay(0, actualFloor + moveDirection);               //Rutina pri zastavení výťahu
              elevatorButton[actualFloor + moveDirection] = 0;
              elevatorLedOFF(actualFloor + moveDirection);
              elevatorCabineLock(false);

              swTimer1.startTimer(_timer);              //Doba na nástup pasažierov
              while (!swTimer1.isExpired())
              {
                  PT_YIELD();                           //Uspanie vlákna
              }
              
              elevatorDisplay(moveDirection, actualFloor);
              elevatorCabineLock(true);
              moveElevator(moveDirection);              //Pohnutie sa výŤahu na ďalšie poschodie
          }
          
          actualFloor += moveDirection;                //Zápis, o aktuálnom poschodí
          elevatorDisplay(moveDirection, actualFloor); 
    }
    
    PT_WAIT_UNTIL(elevatorSensor[endFloor] == 1);       // Čakanie na žltý snezor na koncovom poschodí
    moveElevator(0);                                    // Zastavenie
    
    PT_WAIT_UNTIL(elevatorSensor[endFloor] == 2);       // Čakanie na Červený snezor na koncovom poschodí
    elevatorButton[endFloor] = 0;                       // Runita  pri zastavení
    actualFloor = endFloor;
    
    elevatorDisplay(0,actualFloor);
    elevatorLedOFF(actualFloor);
    elevatorCabineLock(false);
    
    swTimer1.startTimer(_timer);                        //Doba na nástup pasažierov
    while(!swTimer1.isExpired()) {
          PT_YIELD();
    }
  }
  PT_END();
}

//Funkcia na odosielanie správ
void sendData(paket sprava) {
    unsigned char crc = 0;

    serial.putc(START_BYTE);               //Štartovaí znak

    crc = CRC8_TAB[crc ^ sprava.adresa];   //Výpočet CRC
    serial.putc(sprava.adresa);            //Odoslanie adresi príjemcu

    crc = CRC8_TAB[crc ^ MY_ADDR];         //Výpočet CRC
    serial.putc(MY_ADDR);                  //Odoslanie adresi odosielateľa

    serial.putc(sprava.dlzkaDat);          //Odoslanie informácií o veľkosti dát

    for (int i = 0; i < sprava.dlzkaDat ; i++)
    {
        crc = CRC8_TAB[crc ^ sprava.data[i]];  //Výpočet CRC
        serial.putc(sprava.data[i]);           //Odosielanie dát
    }

    serial.putc(crc);                      //Odoslanie CRC kontolného súčtu
}

//Funkcia na príjem správ
void recieveData()
{
    unsigned char adresa_O;
    unsigned char adresa_P;
    unsigned char dlzkaDat;
    unsigned char crc = 0;
    unsigned char recievedCRC;

    if(serial.getc() == START_BYTE)     //Čakaniena start byte a jeho kontrola 
    {
        adresa_O = serial.getc();       //Adresa odosielateľa
        crc = CRC8_TAB[crc ^ adresa_O];

        adresa_P = serial.getc();       //Adresa príjmateľa
        crc = CRC8_TAB[crc ^ adresa_P];

        dlzkaDat = serial.getc();       //Dĺžka dátovej časti
        char *data=(char*)malloc(sizeof(char)*dlzkaDat);   //Alokácia miesta pre dáta

        for(int i=0; i<dlzkaDat; i++)   //Príjem dát
        {
            data[i] = serial.getc();
            crc = CRC8_TAB[crc ^ data[i]];
            
        }
        recievedCRC = serial.getc();   // Príjem CRC

        if(recievedCRC == crc && dlzkaDat != 0) // Kontola CRC a potvrdzovacích správ
        {
            paket sprava = { .adresa = adresa_P,  .dlzkaDat = dlzkaDat, .data = data};            
            buffer [indexBuffer] = sprava;      //pridanie správy do buffra
            if((indexBuffer++) > MESS_BUFFER){  //Kruhový buffer
                indexBuffer = 0;
            }
        }
    }
    //serial.attach(&recieveData);        //Povolenie prerušenia
}

void initData()
{    
    elevatorButton[0] = 0;
    elevatorButton[1] = 0;
    elevatorButton[2] = 0;
    elevatorButton[3] = 0;
    elevatorButton[4] = 0;
    
    elevatorLedOFF(0);
    elevatorLedOFF(1);
    elevatorLedOFF(2);
    elevatorLedOFF(3);
    elevatorLedOFF(4);
    
    elevatorButton[0] = 1;                      //Inicializácia simulátora - výťah sa pohne na prízemie
}

//Začiatok aplikácie
int main()
{
    init_freeFall();        //Inicializácie kontroly na voľný pád
    init_watchDog(0.25f);   //Inicializácie pre WatchDog
    
    serial.attach(&recieveData);                                //Nastavenie prerušenia na sériový port
    elevatorOpen.attach(&timeTick, (float)msPerTick / 1000 );   //Nastavnie prerušenia na počítadlo
    
    initData();
    
    MessExecute messExecute(1);                 //Inicializácia 1. vlákno na spracovanie prijatých správ
    Elevator elevator(2, &getTimer, 3000);      //Inicializácia vlákna pre ovládanie výťahu

    wait(1);

    // Pokial nedostanem spravu o tom ze vytah pada volnym padom spustma vlakna
    while (!freeFall_status()) {    // Round-robin planovac
        messExecute.Run();          // Spustenie vlakna 1
        elevator.Run();             // Spustenie vlakna 2
    }
    
    elevatorCabineLock(false);      // Po volnom pade je nutne zavolat mechnika, vytah sa neda viac pouzivat
}
