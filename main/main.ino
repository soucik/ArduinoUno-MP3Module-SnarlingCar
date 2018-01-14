
#include "U8glib.h"

volatile int pwm_value = 0;
volatile int prev_time = 0;
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);

void draw(int value) {
  
  u8g.setFont(u8g_font_unifont);
  String  val = String(value);
  u8g.setPrintPos(0, 22);
  u8g.print("                      ");
  u8g.setPrintPos(0, 22);
  u8g.print(val);
}

void rising() {
  
  attachInterrupt(0, falling, FALLING);
  prev_time = micros();
}
 
void falling() {
  
  pwm_value = micros() - prev_time;
  Serial.println(pwm_value);
  u8g.firstPage();  
  do {
    draw(pwm_value);
  } while( u8g.nextPage() );
  delay(1000);
  draw(pwm_value);
  attachInterrupt(0, rising, RISING);
}

void setup(void) {
  
  //  setup display
  u8g.setRot180();
  if ( u8g.getMode() == U8G_MODE_R3G3B2 )
    u8g.setColorIndex(255);     // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);         // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);         // pixel on

    //  setup monitor serial port for pulses length in serial monitor
    Serial.begin(115200);

    //  set external interrupt on Digital PIN 2
    attachInterrupt(0, rising, RISING);
}

void loop(void) {
}




