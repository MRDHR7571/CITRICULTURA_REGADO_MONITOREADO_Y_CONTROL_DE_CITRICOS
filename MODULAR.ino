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
int contador = 1; /*Conteo del nivel de menu*/

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
  Serial.begin(9600);
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
  pantalla_bienvenida(); /* Pantalla de bienvenida */
  
}
/* Inicia loop() -------------------------------------------------------------------------------------------------------- */
void loop(){
  /* Adquisicion de datos del sensor DS18B20 */
  sensorDS18B20.requestTemperatures();
  /* Obtencion de caracteres del teclado matricial */
  TECLA = teclado.getKey();
  if (TECLA != 0) {      
      lcd.clear();
      Serial.print(TECLA);
      delay(100);
  } 

 /* Pantallas */ 
 if(contador == 3){ pantalla_tres();accion_tres();}
 if(contador == 2){ pantalla_dos();accion_dos();}
 if(contador == 1){ pantalla_uno();accion_uno();}

}// Fin loop




/* Funciones pantalla ------------------------------------------------------*/
void pantalla_uno (){
  lcd.setCursor(0,0);
  lcd.print("1.ACTIVAR SISTEMA");
  lcd.setCursor(0,1);
  lcd.print("2.APAGAR SISTEMA");
  lcd.setCursor(0,2);
  lcd.print("3.VISUALZACION DATOS");
  lcd.setCursor(0,3);
  lcd.print("4.SISTEMA AUTOMATICO");
}

void accion_uno(){
  if(TECLA == '1') digitalWrite(ELECTROVALVULA,HIGH);
  if(TECLA == '2') digitalWrite(ELECTROVALVULA,LOW);
  if(TECLA == '3') contador = 2;
  if(TECLA == '4') contador = 3;
  
}

//*************************************
void pantalla_dos(){
  
  static long u = 0;
  long h = millis();
  if(h-u > 1000){
    u = h;
    borra_pantalla();
    fecha_hora();
    /*Sub menu pantalla 3, accion 1*/
    //int valorHumedadTierra = analogRead(higrometroAnalogico);
    //valorHumedadTierra = constrain(valorHumedadTierra, 380, 1023);
    //valorHumedadTierra = map(valorHumedadTierra, 380, 1023, 100, 0);//380 es el 100% y 1023 es el 0% de humedad
    fc_28();
    //lcd.setCursor(0,1);
    //lcd.print("H.TIERRA: % ");
    //lcd.print(valorHumedadTierra);
    /*Sub menu pantalla 3, accion 2*/
   //  sensorDS18B20.requestTemperatures();
    lcd.setCursor(0,2);
    lcd.print("T.AMBIENTE: C ");
    lcd.print(sensorDS18B20.getTempCByIndex(0));
    /*Sub menu pantalla 3, accion regreso menu*/
    lcd.setCursor(0,3);
    lcd.print("#.BACK TO MENU");
  }
}
void accion_dos(){
  if(TECLA == '#') contador = 1;  
} 

//**************************************************************


void pantalla_tres (){
  static long u = 0;
  long h = millis();
  if(h-u > 5000){
    u = h;
    borra_pantalla();
    lcd.setCursor(0,0);
    lcd.print(" ESTABLECER VALORES ");
    lcd.setCursor(0,1);
    lcd.print("1.H.TIERRA:    % ");
    lcd.setCursor(0,2);
    lcd.print("2.T.AMBIENTE:  C ");
    lcd.setCursor(0,3);
    lcd.print("#.BACK TO MENU");
  }
  //delay(500); 
}
void accion_tres(){
  if(TECLA == '1')h_tierra();
  if(TECLA == '2')t_ambiente();
  if(TECLA == '#') contador = 1;
}
/*Sub-menus para la pantalla 2*/
void h_tierra(){
borra_pantalla();
  lcd.setCursor(0,0);
  lcd.print("Humedad Max ");
  lcd.setCursor(0,1);
  lcd.print("Humedad Min ");
  lcd.setCursor(0,3);
  lcd.print("*.BACK TO SUBMENU");
  
}

void t_ambiente(){
  borra_pantalla();
  lcd.setCursor(0,0);
  lcd.print("Temperatura Max ");
  lcd.setCursor(0,1);
  lcd.print("Temperatura Min ");
  lcd.setCursor(0,3);
  lcd.print("*.BACK TO SUBMENU");
  }

//POSIBLE ALGORITMO
/*
int tmin, tmax [2];
int hmin, hmax [2];
int humedad =0;// valorHumedadTierra;
int temeratura = sensorDS18B20.getTempCByIndex(0);

//printf("ESTABLECE EL VALOR DE HUMEDAD\n");
//scanf("%d",&humedad);
//printf("ESTABLECE EL VALOR DE TEMPERATURA\n");
//scanf("%d",&temperatura);

if(humedad >= hmin && humedad < = hmax || temperatura >= tmin && temperatura < = tmax){
    digitalWrite(ELECTROVALVULA,HIGH);
}else{
    digitalWrite(ELECTROVALVULA,LOW);
}
*/


//**************************************************************************************************
void fc_28(){
  const int higrometroAnalogico = A0;
  int valorHumedadTierra = analogRead(higrometroAnalogico);
  valorHumedadTierra = constrain(valorHumedadTierra, 380, 1023);
  valorHumedadTierra = map(valorHumedadTierra, 380, 1023, 100, 0);//380 es el 100% y 1023 es el 0% de humedad
  //Serial.print(valorHumedadTierra);  
    lcd.setCursor(0,1);
    lcd.print("H.TIERRA: % ");
    lcd.print(valorHumedadTierra);
}
