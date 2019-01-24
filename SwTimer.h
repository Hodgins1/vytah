#ifndef SW_TIMER_H
#define SW_TIMER_H

typedef int (*f_int_t) (void); // Definovanie typu funkcie int XXX(void);
class SwTimer
{
 public:
   /*! Constructor */
    SwTimer(f_int_t func) :GlTimeFn(func) {}; //Konstruktor
     virtual bool isExpired();
     virtual void startTimer(unsigned int delay);
     virtual void restartTimer();

 private:
     f_int_t GlTimeFn;  //Odkaz na funciu ktora vracia globalny cas
     unsigned int endTime; //Pomocna premnena
     unsigned int _delay; //Perioda casovaca
 };

#endif
