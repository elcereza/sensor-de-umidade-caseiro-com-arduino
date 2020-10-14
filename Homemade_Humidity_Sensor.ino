/*-------------------------------------------------------------------------------
* Projeto: Sensor de Umidade Caseiro com Arduino
* Saiba mais: https://elcereza.com/portfolio/
* Disponibilizado por: https://elcereza.com/
* Autor: Gustavo Cereza
---------------------------------------------------------------------------------*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#include <Thermistor.h>

Thermistor temps(1);                                                               // Entrada A1 do termistor seco
Thermistor tempm(2);                                                               // Entrada A2 do termistor umedecido

void setup() {

  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {                                 // Caso o display não funcione, teste com outro endereço i2c
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  display.display();
  delay(2000); 
  display.clearDisplay();
}

void loop() {
  double ts = temps.getTemp(),                                                     // Obtem o valor da temperatura do termistor seco
         tm = tempm.getTemp(),                                                     // Obtem o valor da temperatura do termistor umedecido
         bar = 759.75,                                                             // Pressão barométrica em mmHg
         pvsm,                                                                     // Pressão de vapor saturado à temperatura do bulbo umedecido
         pvs,                                                                      // Pressão de vapor saturado à temperatura do bulbo seco
         pv,                                                                       // Pressão de vapor d'água no ar
         ur;                                                                       // Umidade relativa do ar


  pvsm = pow(6.1078*10,(7.5*tm/(237.3+tm)));
  pvs = pow(6.1078*10,(7.5*ts/(237.3+ts)));
  pv = pvsm-(0.000667*bar*(ts - tm));
  ur = (pv/pvs)*100;

  /*---------------------  Display  ------------------------*/
  display.clearDisplay();
  display.setTextSize(2); 
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.print(F("T: "));
  display.println(ts);
  display.setCursor(10, 35);
  display.print(F("H: "));
  display.println(ur);
  display.display();      

  /*---------------------  Serial  ------------------------*/
  Serial.print("Ts : ");
  Serial.print(ts);
  Serial.print("*C | ");
  Serial.print("Tm : ");
  Serial.print(tm);
  Serial.print("*C | ");
  Serial.print("Ur : ");
  Serial.println(ur);
  Serial.println("-----------------------------------------");
  delay(100);
}
