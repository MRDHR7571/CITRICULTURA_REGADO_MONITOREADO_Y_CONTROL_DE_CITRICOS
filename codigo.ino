/*  
 *   Proyecto Modular, Citricultura Sistema De Regado Automatizado
 *   Centro Universitario De Ciencias Exactas e Ingenierias
 *   @De Haro Rodriguez Mariano Rafael
 *   @Martinez Jimenez Leonardo Gabriel
 *   @Padilla Cordero Adan
     @Fecha de entrega: 10 de Noviembre del 2021     
*/

/* Bibliotecas --------------------------------------------------------------------------------------- */
#include<Wire.h>/*Comunicacion I2C */
#include<LCD.h>
#include<LiquidCrystal_I2C.h> /* Envio de datos I2C */
#include<RTClib.h>
#include<Keypad.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* Variables ----------------------------------------------------------------------------------------- */
const int higrometroAnalogico = A0;
const byte pinDatosDQ = 11;
int ELECTROVALVULA = 13;
int buzz = 12;
int contador = 1; /*Conteo del nivel de menu*/
/* Teclado 3x4 */
char TECLA; /*Almacenamiento de teclas*/
const byte FILAS = 4;
const byte COLUMNAS = 3;
char keys[FILAS][COLUMNAS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}, 
};
/*Pines reservados para las filas*/
byte pinesFilas[FILAS] = {9,8,7,6}; 
/*Pines reservados para las columnas*/
byte pinesColumnas[COLUMNAS] = {5,4,3};

/* Objetos para cada modulo ----------------------------------------------------------------------------------------------- */
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);/*Conexiones a los pines digitales DIR, E,RW,RS,D4,D5,D6,D7*/
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);
/*Inicializacion de clases OneWire y DallasTemperature para el sensor DS18B20*/
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

/* Inicia Setup() -------------------------------------------------------------------------------------------------------- */
void setup(){
  rtc.begin(); // Inicia la comunicacion con el RTC
  /* Establece la fecha y hora (Comentar una vez establecida la hora) */
  //rtc.adjust(DateTime(__DATE__, __TIME__));
  sensorDS18B20.begin(); // Inicia la comunicacion con el sensor DS18B20
  TECLA = teclado.getKey();
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  /* Tamaño LCD 20 Columnas X 4 Filas */
  lcd.begin(20, 4);
  pinMode(ELECTROVALVULA,OUTPUT);
  /* Pantalla de bienvenida */
  pantalla_bienvenida(); 
  
}
/* Inicia loop() -------------------------------------------------------------------------------------------------------- */
void loop(){
  /* Adquisicion de datos del sensor DS18B20 */
  sensorDS18B20.requestTemperatures();
  /* Obtencion de caracteres del teclado matricial */
  TECLA = teclado.getKey();
  if (TECLA != 0) {      
      lcd.clear();
      sonido();
      //delay(100);
  } 

 /* Pantallas */ 
 if(contador == 2){ pantalla_dos();accion_dos();}
 if(contador == 1){ pantalla_uno();accion_uno();}

}// Fin loop


/* Prototipado de Funciones ------------------------------------------------------------------------------------------------- */
/* Funcion Sonido*/
void sonido(){
  static long u = 0;
  long h = millis();
  if(h-u > 500){
    u = h;
    tone(buzz, 3700, 50);
    //delay(500);
  }
}

/* Funcion Fecha y hora */
void fecha_hora(){
  /* Obtiene la fecha y hora del RTC */
  DateTime now = rtc.now(); 
  /* Calendario */
  lcd.clear();
  lcd.setCursor(10,0);
  lcd.print(now.year(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.day(), DEC);
  lcd.print(" ");
  /* Reloj */
  lcd.setCursor(0,0);
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);
  //delay(1000);  //Información se actualiza cada 1s.
}

/* Funcion Borrado de pantalla */
void borra_pantalla(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("                     ");
  lcd.setCursor(0,1);
  lcd.print("                     ");
  lcd.setCursor(0,2);
  lcd.print("                     ");
  lcd.setCursor(0,3);
  lcd.print("                     ");  
  lcd.clear();
}

/* Funcion error*/
void error(){
  lcd.clear();
  lcd.setCursor(7,1);
  lcd.print("ERROR! ");
  tone(buzz, 700, 50);
  delay(500); 
}

/* Funcion Pantalla de Bienvenida*/
void pantalla_bienvenida(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("BIENVENIDO");
  lcd.setCursor(2,1);
  lcd.print("PROYECTO MODULAR");
  lcd.setCursor(2,2);
  lcd.print("SISTEMA DE RIEGO");
  lcd.setCursor(4,3);
  lcd.print("AUTOMATIZADO");
  delay(5000);
  lcd.clear(); 
}

/* Funcion Pantalla  de Inicio*/
void pantalla_uno (){
  lcd.setCursor(0,1);
  lcd.print("1.APAGAR SISTEMA");
  lcd.setCursor(0,2);
  lcd.print("2.ACTIVAR SISTEMA");
  lcd.setCursor(0,3);
  lcd.print("3.SISTEMA AUTOMATICO");
}

void accion_uno(){
  if(TECLA == '1') digitalWrite(ELECTROVALVULA,HIGH);
  if(TECLA == '2') digitalWrite(ELECTROVALVULA,LOW);
  if(TECLA == '3') contador = 2;
  
}

/* Funcion Pantalla Automatica*/
void pantalla_dos(){
  static long u = 0;
  long h = millis();
  if(h-u > 1000){
    u = h;
    borra_pantalla();
    fecha_hora();
    /*Sub menu pantalla 2, accion 1*/
    int valorHumedadTierra = analogRead(higrometroAnalogico);
    valorHumedadTierra = constrain(valorHumedadTierra, 380, 1023);
    valorHumedadTierra = map(valorHumedadTierra, 380, 1023, 100, 0);//380 es el 100% y 1023 es el 0% de humedad
    lcd.setCursor(0,1);
    lcd.print("H.TIERRA: % ");
    lcd.print(valorHumedadTierra);
    /*Sub menu pantalla 2, accion 2*/
   //  sensorDS18B20.requestTemperatures();
    lcd.setCursor(0,2);
    lcd.print("T.AMBIENTE: C ");
    lcd.print(sensorDS18B20.getTempCByIndex(0));
    /*Sub menu pantalla 2, accion regreso menu*/
    lcd.setCursor(0,3);
    lcd.print("#.BACK TO MENU");

/****************** Algoritmo Sistema Automatico  ********************************************************/
  /* Variables locales temperatura y humedad*/
  int humedad = valorHumedadTierra;
  int temperatura = sensorDS18B20.getTempCByIndex(0);
  
  if((humedad >= 30 & humedad <= 95) & (temperatura >= 26 & temperatura <= 35)){
    digitalWrite(ELECTROVALVULA,HIGH);
  }else{
      digitalWrite(ELECTROVALVULA,LOW);
   }

  /*Error de limite*/
    if((humedad >=95 & humedad <= 100) & (temperatura >= 36 & temperatura <= 100)){
      error();
      digitalWrite(ELECTROVALVULA,LOW);
    } /* Fin if error limite*/
  } /* Fin del if millis */
} /* Fin de la funcion*/

void accion_dos(){
  if(TECLA == '#') contador = 1;  
}