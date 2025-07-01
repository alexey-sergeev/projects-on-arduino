// v. 1.1


//
#include <GyverSegment.h>
// Disp595_4 disp(PIN_DIO, PIN_CLK, PIN_LAT);

// #define DIO_PIN 12
// #define CLK_PIN 13

// Disp1637_4 disp(DIO_PIN, CLK_PIN);
// Disp1637_4 disp(PIN_DIO, PIN_CLK);
// Disp595_4 disp(DIO_PIN, CLK_PIN, LAT_PIN);


// Disp595_4 disp(DIO_PIN, CLK_PIN, LAT_PIN);
// Disp595_8 disp(DIO_PIN, CLK_PIN, LAT_PIN);
// Disp595_8v2 disp(DIO_PIN, CLK_PIN, LAT_PIN);
// Disp595Static<4> disp(DIO_PIN, CLK_PIN, LAT_PIN);
// Disp1637_4 disp(DIO_PIN, CLK_PIN);
// Disp1637_6 disp(DIO_PIN, CLK_PIN);
Disp1637Colon disp(DIO_PIN, CLK_PIN);
// Disp7219<1> disp(DIO_PIN, CLK_PIN, LAT_PIN);  // 1 чип - 8 цифр


class Display {
  
  public:

    void init() {

      // disp.brightness(4);

      disp.setCursor(0);
      disp.print("hello");
      disp.update();

    }  



    void run() {

      disp.tick();
    
    }  


    // v. 1.1 ( bool f = false)

    // void print(String s) {
    void print(String s, bool f = false) {

      // static uint32_t timer;
      static String sOld;
      sOld.reserve(8);
      
      if ( sOld != s ) {

        disp.setCursor(0);
        
        // if ( f ) {

        //   disp.printRight(true);
        //   disp.setCursorEnd();
        //   // disp.fillChar('0');

        // }  

        disp.print(s);
        disp.print("   ");
        disp.update();

        if ( f ) disp.colon(true); else disp.colon(false);

        sOld = s;

      }

      // if ( millis() - timer > 3000 ) {
    
      //   timer = millis();
      //   pln(s);

      // }



    }  




    void power(bool f) {

      disp.power(f);

    }



  private:



};

Display display;
