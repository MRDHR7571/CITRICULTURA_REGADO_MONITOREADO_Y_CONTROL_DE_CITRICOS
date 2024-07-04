/* Prototipado de Funciones ------------------------------------------------------------------------------------------------- */
/* Funciones --------------------------------------------------------------------------------------------------------------- */
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
