//vamos a hacer un seguidor solar// este va a ser el codigo
#include <Servo.h>
Servo servo_X;
Servo servo_Y;

//////////////pines/////////////////
byte pinservo_X = 6; // byte: 0 - 255
byte pinservo_Y = 7;
int pulsomin = 800;
int pulsomax = 2400;
///byte gradosmax = 120;

//////////////variables///////////////////////
int angulo_servo_X = 90;
int angulo_servo_Y = 90;

///pines ldr//
//no hace falta arrancarlos
///variables
int arriba_derecha = 0;
int arriba_izquierda = 0;
int abajo_derecha = 0;
int abajo_izquierda = 0;
///orden previsto: arriba derecha, arriba izquierda, abajo derecha, abajo izquierda
///verificaremos despues en el codigo

int arriba = 0;
int abajo = 0;
int derecha = 0;
int izquierda = 0;
int margen = 100;
String mensajes[4] = {"arr_der ", " arr_izqu ", " aba_der ", " aba_izqu "};

void setup() {
  Serial.begin(9600);
  servo_X.attach(pinservo_X, pulsomin, pulsomax);
  servo_Y.attach(pinservo_Y, pulsomin, pulsomax);
  servo_X.write(90);///90 abajo
  servo_Y.write(90);///90 abajo
  delay(1000);
}

void loop() {
  leerldrs();
  mostrarpantalla();
  decirdirmover();
  //delay(1000);
  /*test_eje_X();
  test_eje_Y();*/
}

void leerldrs() {
  arriba_derecha = analogRead(2);
  arriba_izquierda = analogRead(3);
  abajo_derecha = analogRead(0);
  abajo_izquierda = analogRead(1);
  ///este es mi orden testeado con el monitor serie
  sumar();
}
void sumar() {
  margen = 200;///valor umbral para mvernos o no, variaciones minimas nos dan igual
  ////empezamos con las sumas
  arriba = arriba_derecha + arriba_izquierda;
  abajo = abajo_derecha + abajo_izquierda;
  derecha = arriba_derecha + abajo_derecha;
  izquierda = arriba_izquierda + abajo_izquierda;
}

void decirdirmover() {
  ////empezamos con las comparaciones
  if (arriba > abajo) {
    if ((arriba - abajo) > margen) {
      subir();
    } else {
      quieto();
    }///depende del codigo lo usaremos o no
  } else if (abajo > arriba) { ///abajo mayor que arriba
    if ((abajo - arriba) > margen) {
      bajar();
    } else {
      quieto();
    }///depende del codigo lo usaremos o no
  } else {};

  if (derecha > izquierda) {
    if ((derecha - izquierda) > margen) {
      girarderecha();
    } else {
      quietogiro();
    }
  } else if (izquierda > derecha) { ///izquierda > derecha
    if ((izquierda - derecha) > margen) {
      girarizquierda();
    } else {
      quietogiro();
    }
  } else {}
  ///delay(20);
}//fin del decirdirmover

void mostrarpantalla() {
  ///String mensajes[4] = {"arriba_derecha ", " arriba_izquierda ", " abajo_derecha ", " abajo_izquierda "};

  Serial.print(mensajes[0]);//en programacion el cero es el uno nuestro
  Serial.print(arriba_derecha);///variable
  Serial.print(mensajes[1]);
  Serial.print(arriba_izquierda);
  Serial.print(mensajes[2]);
  Serial.print(abajo_derecha);
  Serial.print(mensajes[3]);
  Serial.print(abajo_izquierda);
}

////necesario para saber que valores son cada lado
void test_eje_X() {
  servo_X.write(90);
  delay(1000);
  servo_X.write(45);///45 izquierda
  delay(1000);
  servo_X.write(0);///45 izquierda
  delay(1000);
}
void test_eje_Y() {
  servo_Y.write(90);///90 abajo
  delay(1000);
  servo_Y.write(45);////45 arriba
  delay(1000);
  servo_Y.write(0);////45 arriba
  delay(1000);
}

///funciones que definen movimientos
///nos da igual ahora que sea un servo, un actuador lineal, vete a saber
///hecho aqui para servo
void subir() {
  angulo_servo_Y = angulo_servo_Y-5;
  if (angulo_servo_Y < 60) {///limites de recorrido
    angulo_servo_Y = 60;
  }
  servo_Y.write(angulo_servo_Y);
  Serial.print( "subiendo ");
  Serial.print(angulo_servo_Y);
}
void bajar() {
  angulo_servo_Y = angulo_servo_Y+5;
  if (angulo_servo_Y > 180) {
    angulo_servo_Y = 180;
  }
  servo_Y.write(angulo_servo_Y);
  Serial.print(" bajando ");
  Serial.print(angulo_servo_Y);
}
void quieto() {
  Serial.print(" quieto ");
}

void girarizquierda() {
  angulo_servo_X = angulo_servo_X+5;
  if (angulo_servo_X > 180) {
    angulo_servo_X = 180;
  }
  servo_X.write(angulo_servo_X);
  Serial.print(" girando izquierda");
  Serial.println(angulo_servo_X);///con salto de linea
}
void girarderecha() {
  angulo_servo_X = angulo_servo_X-5;
  if (angulo_servo_X < 0) {
    angulo_servo_X = 0;
  }
  servo_X.write(angulo_servo_X);
  Serial.print(" girando derecha ");///con salto de linea
  Serial.println(angulo_servo_X);///con salto de linea
}
void quietogiro() { //pueden haber dos saltos de linea, no me gusta
  Serial.println(" quieto_giro ");///con salto de linea
}
///la salida prevista formateada sera
//pos valor pos valor pos valor pos valor decision  decision
