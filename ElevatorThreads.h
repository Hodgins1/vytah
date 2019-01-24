#include "Protothread.h"
#include "SwTimer.h"

/**
 *  Trieda na spracovanie prijatých správ z buffra.
 *  Potomok triedy ProtoThread, beží v samostatnom proto-vlákne
 *  Definíca a dokumentácia proto-vlákna sa nachádza v "Protothread.h" hlavičkovom súbore
 */
class MessExecute : public Protothread
{
public:

    /** @brief  Vytvorenie MessExecute inštancie
      * @param id - identifikátor vlákna
      */
    MessExecute(int id) {
        identificator = id;
    };

    virtual bool Run();

private:
    int identificator;
};

/** Trieda, ktorá sa stará o funkcionalitu samotného výťahu
 *  Potomok triedy ProtoThread, beží v samostatnom proto-vlákne
 *  Definíca a dokumentácia proto-vlákne sa nachádza v "Protothread.h" hlavičkovom súbore
 *  Definíca a dokumentácia časovača sa nachádza v "SwTimer.h" hlavičkovom súbore
 */
class Elevator : public Protothread
{
public:
    /** @brief Vytvorenie MessExecute inštancie, následná inicializícia časovca
      * @param id - identifikátor vlákna
      * @param GetGLTime - parameter s globálnym časom
      * @param delay - čas, ktorý bude timer odpočítavať
      */
    Elevator(int id, f_int_t GetGLTime,int delay):swTimer1(GetGLTime) {
        _timer = delay;
        identificator = id;
    };
    virtual bool Run();
    
private:
    int identificator ;
    int _timer;
    SwTimer swTimer1;
};