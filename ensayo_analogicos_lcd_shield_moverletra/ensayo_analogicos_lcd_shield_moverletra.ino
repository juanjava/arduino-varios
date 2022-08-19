/////lcd shield vamos a ver
#include <LiquidCrystal.h>
LiquidCrystal  lcd ( 8 ,  9 ,  4 ,  5 ,  6 ,  7 );

//////////////////variables///////////////////////
int posletra_X  = 0;
int posletra_Y  = 0;

int lectura_boton = 0;
boolean botonpulsado = false;//anti aguantar//
String botones[] = {"select", "izquierda", "abajo", "arriba", "derecha", "nada"};
int boton = 0;
int estado_boton_antes = 0;
int estado_boton_ahora = 0;
long millisantes = 0;

void setup() {
  lcd.begin(16, 2);              // inicia la biblioteca
  lcd.setCursor ( 0 , 0 );
  Serial.begin(9600);
}

void loop() {
  revisar_analogicos();
  ///nombreboton(); ya lo llamo desde el de arriba
  moverletra2();
  imprimirletra();
  imprimirtodo();////debug serial
}

//////////////////FUNCIONES///////////////////////
void revisar_analogicos() {
  lectura_boton = analogRead(0);
  if (lectura_boton > 1000) {////sin pulsar nada
    botonpulsado = false;
    boton = 5;
  } else {
    nombreboton();
    estado_boton_ahora = boton;
    if (estado_boton_ahora != estado_boton_antes) {
      estado_boton_antes = estado_boton_ahora;
      millisantes = millis();
      botonpulsado = false;
    } else if (estado_boton_ahora == estado_boton_antes && millis() > (millisantes + 400)) {
      ///400 milisegundos para que te haga caso///antirebote
      if (!botonpulsado) { ////////anti pulsar y aguantar
        ///nombreboton();
        botonpulsado = true;
      } else {////////anti pulsar y aguantar
        boton = 5;
      }
    } else {////no ha hecho los 400 milisegundos///no hago nada, reseteo boton si no me mueve letra
      botonpulsado = false;
      boton = 5;
    }
  }
}
///String botones[] = {"select", "izquierda", "abajo", "arriba", "derecha", "nada"};
void nombreboton() {
  if (lectura_boton > 600 && lectura_boton < 700) {
    boton = 0;
  } else  if (lectura_boton > 350 && lectura_boton < 450) {
    boton = 1;
  } else  if (lectura_boton > 200 && lectura_boton < 300) {
    boton = 2;
  } else  if (lectura_boton > 50 && lectura_boton < 150) {
    boton = 3;
  } else if (lectura_boton >= 0 && lectura_boton < 50) {
    boton = 4;
  } else {
    boton = 5;
  }
}
void nombreboton2(){
   ///String botones[] = {"select", "izquierda", "abajo", "arriba", "derecha", "nada"}; 
  if (lectura_boton > 1000){boton = 5;}/// return 5;
  else if (lectura_boton < 50){   boton = 4;}//return btnRIGHT;
  else if (lectura_boton < 195){  boton = 3;}//return btnUP;
  else if (lectura_boton < 380){  boton = 2;}//return btnDOWN;
  else if (lectura_boton < 555){  boton = 1;}//return btnLEFT;
  else if (lectura_boton < 790){  boton = 0;}//return btnSELECT;
  }
int read_LCD_buttons() {              // read the buttons
  ///adc_key_in = analogRead(0);       // read the value from the sensor

  // Nota de Paco: Select 639, izquierda 409-408, abajo 255, arriba 99, derecha 0, sin pulsar 1023.
  // Probado por Paco, funciona bien.
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  // We make this the 1st option for speed reasons since it will be the most likely result

  ///if (adc_key_in > 1000) return btnNONE;
  // For V1.1 us this threshold  comment the other threshold and use the one below:
    /*if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if (adc_key_in < 850)  return btnSELECT;
  */
  /*
  // For V1.0
  if (adc_key_in < 50)   return btnRIGHT;/////////////////este va
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  return btnNONE;                // when all others fail, return this.*/
}
void moverletra1() {////switch case
  ///byte posletra[2] = {0, 0};   /////select 638;left 405;down 253;up 500;right 0;
  ///Serial.println("moverletra");
  switch (boton) {
      Serial.println("moverletra");
    case 0:
      byte aux = posletra_X;
      posletra_X = 16 - aux;
      break;
    case 1:
      if (posletra_X > 0) {
        posletra_X--;
        Serial.println("moverletra izquierda");
      }
      break;
    case 2:
      if (posletra_Y == 0) {
        posletra_Y = 1;
      }
      break;
    case 3:
      if (posletra_Y == 1) {
        posletra_Y = 0;
      }
      break;
    case 4:
      if (posletra_X < 15) {
        posletra_X++;
        Serial.println("moverletra derecha");
      }
      break;
    case 5:
      break;
    default:
      Serial.println("No vale ninguna de los valores anteriores");
  }
}

void moverletra2() {///if else if ....
  if (boton == 0) {
    byte aux = posletra_X;
    posletra_X = 15 - aux;
  } else if (boton == 1) {
    if (posletra_X > 0) {
      posletra_X--;
      Serial.println("moverletra izquierda");
    }
  } else if (boton == 2) {
    if (posletra_Y == 0) {
      posletra_Y = 1;
    }
  } else if (boton == 3) {
    if (posletra_Y == 1) {
      posletra_Y = 0;
    }
  } else if (boton == 4) {
    if (posletra_X < 15) {
      posletra_X++;
      Serial.println("moverletra derecha");
    }
  } else if (boton == 5) {
  } else {
    Serial.println("No vale ninguna de los valores anteriores");
  }
}
void imprimirletra() {
  lcd.setCursor ( 0 , 0);
  if (posletra_Y == 1) {
    //lcd.print("123456789123456")}
    lcd.print("               ");////borro linea cero
    lcd.setCursor ( 0 , 1);///me pongo en la 1
    imprimir_hache();
  } else {///esta en la cero
    imprimir_hache();
    lcd.setCursor ( 0 , 1);///me pongo en la 1
    lcd.print("               ");////borro linea uno
  }////fin else
}
void imprimir_hache() {
  for (int i = 0; i < posletra_X; i++) {
    lcd.print(" ");////borrando
  }///fin for
  lcd.print("H");////escribiendo
  for (int i = posletra_X; i < 16; i++) {
    lcd.print(" ");////borrando
  }///fin for_despues hache
}
void imprimirtodo() {////"DEBUG"////
  //Serial.print(" lectura_pin ");
  //Serial.print(lectura_pin );
  Serial.print(" lectura_boton ");
  Serial.print(lectura_boton);
  Serial.print(" botonpulsado ");
  if (botonpulsado) {
    Serial.print(" si ");
  } else {
    Serial.print(" no ");
  }
  Serial.print(botonpulsado);
  Serial.print(" boton ");
  Serial.print(boton);
  Serial.print("  ");
  Serial.print(botones[boton]);
  Serial.print("  posletra[2] (");
  Serial.print(posletra_X);
  Serial.print(",");
  Serial.print(posletra_Y);
  Serial.println(")");
}
/////select 638;left 405;down 253;up 500;right 0;
/////select 644;left 425;down 256;up 97;right 9;
////640 406  up 97 260 r 0
///rangos:
/* right 0-50
  80-150 up
  200-300 down
  350-450
  select 600-700*/
