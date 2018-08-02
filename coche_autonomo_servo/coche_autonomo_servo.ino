#include <NewPing.h>
#include <Servo.h>
Servo servo1;
//// esto para los mensajes de consola/////
///String direccion[] = {"delante izquierda", "delente derecha", "atras izquierda", "atras derecha"};
String lado[] = {" derecha", "delante ", "izquierda", "atras "};

////////////////variables////////////
boolean adelante = false; //adelante o no
boolean moverse = false;/// si se mueve
int laststate = 0; ///leyenda= 0 parado 1 aelante 2 atras
/////las distancias en cm///
int izquierda = 0;
int derecha = 0;
int detras = 0;
int delante = 0;
byte margen = 15;
byte margengiro = 50;
byte choque = 35;


///byte ena = 12;
byte pingdet = 13;
byte echodet = 12;
/////modulo motores
//byte in1 = 11;
//byte in2 = 10;

////servo aqui
int pinservo = 10;
int pulsomin = 600;
int pulsomax = 2400;
byte in3 = 9;
byte in4 = 8;
//byte enb = 7;
byte echoizq = 7;
byte pingizq = 6;
byte echoder = 5;
byte pingder = 4;
byte pingcentro = 3; // Arduino pin tied to trigger pin on the ultrasonic sensor.
byte echocentro = 2; // Arduino pin tied to echo pin on the ultrasonic sensor.

#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonarcentro(pingcentro, echocentro, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarder(pingder, echoder, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarizq(pingizq, echoizq, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonardetras(pingdet, echodet, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  /////modulo motores
  //pinMode(in1, OUTPUT);
  //pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  servo1.attach(pinservo, pulsomin, pulsomax);
}

void loop() {
  medir();
  colision();
  mover();

}

void medir() {
  ////String lado[] = {"derecha ","delante ", "izquierda ", "detras "};
  derecha = radar(sonarder, 0);
  delante = radar(sonarcentro, 1);
  izquierda = radar(sonarizq, 2);
  detras = radar(sonardetras, 3);
  Serial.println();
}

int radar(NewPing sonar, int indice) {
  int distancia = 0;
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print(lado[indice]);
  distancia = uS / US_ROUNDTRIP_CM;
  if (distancia > 240 || distancia < 0) {
    distancia = 0;
  }
  Serial.print(distancia);
  Serial.print(" cm ");
  return distancia;
}

////
/*byte margen = 20;
  byte margengiro = 35;
  byte choque = 20;*/
void mover() {
  if (moverse) {
    ///// se me acerca un obstaculo////
    if (delante != 0 && delante < margengiro) {
      antichoque();
    } else {
      alante();
      dondegiro();
    }
  }
}

void antichoque() {
  if (derecha > izquierda) {
    girarderecha();
  } else if (izquierda > derecha) {
    girarizquierda();
  }
}
void dondegiro() {
  if (derecha > izquierda && (derecha - izquierda) > margen) {
    girarderecha();
  } else if (izquierda > derecha && (izquierda - derecha) > margen) {
    girarizquierda();
  } else {
    centro();
  }
}

void colision() {
  boolean choquedel = false;
  boolean choquetras = false;
  moverse = true;
  if (delante < choque && delante > 0) {
    choquedel = true;
    moverse = false;
  }
  if (detras < choque && detras > 0) {
    choquetras = true;
    moverse = false;
  }
  if (choquedel == true && choquetras == true) {
    ///aqui no me puedo mover
    moverse = false;
    paro();
    centro();
    Serial.println("Quieto parao");
  } else if (choquedel == true) {
    atras();
    centro();
    //antichoque();
    Serial.println("He chocado delante----atras voy");
  } else if (choquetras == true) {
    alante();
    centro();
    //antichoque();
    Serial.println("He chocado atras----delante voy");
  } else {
    moverse = true;
  }
}

/////actuo sobre los pines aqui
void alante() {
  //digitalWrite(ena, HIGH);///ena es motor principal de movimiento
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("adelante");
}
void atras() {
  //digitalWrite(ena, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("atras");
}
void paro() {
  //digitalWrite(ena, LOW);///ena es motor principal de movimiento
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.println("paro");
}

/////no tengo servo
/*void girarderecha() {
  //analogWrite(enb, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  Serial.println("derecha");
}
void girarizquierda() {
  Serial.println("izquierda");
  ///analogWrite(enb, HIGH);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}
void centro() {
  Serial.println("centro");
  ///analogWrite(enb, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}
*/
//////////con servo
void girarderecha() {
  angulo = derecha - izquierda;
  if (angulo>45){
    angulo = 45;}
  servo1.write(90+angulo);///90 es centro, 
  Serial.println("derecha");
}
void girarizquierda() {
    angulo = izquierda - derecha;
  if (angulo>45){
    angulo = 45;}
  servo1.write(90-angulo);///90 es centro, 
  Serial.println("izquierda");
}
void centro() {
  Serial.println("centro");
servo1.write(90);
}
