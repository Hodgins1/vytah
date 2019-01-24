#include "FreeFall.h"
#include "Communication.h"

DigitalOut freeFall_LED(LED_RED); 
InterruptIn acc(PTA15);             //Nastavenie prerušenia od akcelerometra
I2C i2c(SDA, SCL);

bool freeFall_check; //Premenná s informáciou,ći nastal voľný pád

void ledBlick()
{
    freeFall_LED = 0;
    wait(100);
    freeFall_LED = 1;
}

//Funkcia vyvolaná pri vyvolaní prerušenia
void freeFall()
{
    uint8_t IntSourceSystem = 0;  
    char t[1] = {0x0C};
    i2c.write(MMA8451_I2C_ADDRESS , t, 1, true);
    i2c.read(MMA8451_I2C_ADDRESS , (char *)&IntSourceSystem, 1); //vyčítanie hodnoty z registra akcelerometra

    if ((IntSourceSystem&0x04)==0x04) {  //zisťovanie či nastal voľný pád
         char command [] = {0x01};
         paket emergencyBreak = {.adresa = 0x0f,.dlzkaDat = 0x01, .data = command};
         paket freeFall_console= {.adresa = 0xD0,.dlzkaDat = 0x1e,.data = "\n\r FreeFALL ! volajte udrzbu !"};
         sendData(emergencyBreak);      // Aktivácia núdzovej brzdy vo výťahu
         sendData(freeFall_console);    // Otovrenie kabíny výťahu  
         freeFall_LED = 0;              // Rozsvietenie LED na doske
         freeFall_check = true;            
    }
}

//Inicializácia akcelerometra na detekciu voľného pádu
void init_freeFall()
{
    //https://www.nxp.com/docs/en/application-note/AN4070.pdf?&fasp=1&WT_TYPE=Applicatio
    uint8_t data1[2] = {0x2A, 0x20}; // Deaktivacia senzora
    i2c.write(MMA8451_I2C_ADDRESS , (char *)data1, 2);
    uint8_t data2[2] = {0x15, 0xB8};
    i2c.write(MMA8451_I2C_ADDRESS , (char *)data2, 2);
    uint8_t data3[2] = {0x17, 0x03};
    i2c.write(MMA8451_I2C_ADDRESS , (char *)data3, 2);
    uint8_t data4[2] = {0x18, 0x06};
    i2c.write(MMA8451_I2C_ADDRESS , (char *)data4, 2);
    uint8_t data5[2] = {0x2D, 0x04};
    i2c.write(MMA8451_I2C_ADDRESS , (char *)data5, 2);
    uint8_t data6[2] = {0x2E, 0x00};
    i2c.write(MMA8451_I2C_ADDRESS , (char *)data6, 2);
    uint8_t data7[2]= {0x2A, 0x09 }; //Frekvencia je nastavena na 400 Hz, aktivacia senzora
    i2c.write(MMA8451_I2C_ADDRESS , (char *)data7, 2);
    
    char command [] = {0x00};
    paket emergencyBreak = {.adresa = 0x0f,.dlzkaDat = 0x01, .data = command};
    sendData(emergencyBreak);      // Deaktivácia núdzovej brzdy vo výťahu
    
    freeFall_check = false;
    freeFall_LED = 1;    // Inikačná dióda na doske je vypnutá
    
    acc.fall(&freeFall); // nastavenie prerušenia z akcelerometra
}


// Funkcia vracia informáciu či nastal voľný pád
bool freeFall_status()
{
    return freeFall_check;
}