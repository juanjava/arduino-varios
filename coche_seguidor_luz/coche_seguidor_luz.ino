                                                                                                                                                                                                                                                                                                                 #include <NewPing.h>
int ldr[] = {0, 1, 2, 3};//////creamos array
int delante = 0;
int detras = 0;
String direccion[] = {"delante izquierda", "delente derecha", "atras izquierda", "atras derecha"};

/////modulo motores
byte ena = 12;
byte in1 = 11;
byte in2 = 10;
byte in3 = 9;
byte in4 = 8;
byte enb = 7;
int valormin = 90  ;
////pro mini///
/*
  byte ena = 9;
  byte in1 = 8;
  byte in2 = 7;
  byte in3 = 6;
  byte in4 = 5;
  byte enb = 4;*/

boolean adelante = false; //adelante o no
boolean moverse = false;/// si se mueve
int distancia = 0;

#define TRIGGER_PIN  3  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  /////modulo motores
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
}
void loop() {
  medir();
  mover();
  colision();
}

void medir() {
  moverse = false;
  Serial.println("Valores ldr: ");
  for (int i = 0; i < 4; i++) { //////mido resistencias ldr
    int valor = analogRead(i);
    ldr[i] = valor;
    Serial.print(direccion[i]);
    Serial.print(" - ");
    Serial.print(valor);
    Serial.print(" - ");
    if (ldr[i] > valormin) {
      moverse = true;///me muevo
    }
  }
  Serial.println("medido. ");
}
void mover() {///funcion de movimiento
  delante = ldr[0] + ldr[1];//guardo valores
  detras = ldr[2] + ldr[3];
  ////comparamos///
  if (moverse) {
    if (delante > detras && (delante - detras) > 40) {
      if (ldr[0] > ldr[1] && (ldr[0] - ldr[1]) > 30) { ///comparo cada ldr de lado segun que caso
        girarizquierda();
      } else if ((ldr[1] - ldr[0]) > 30) {
        girarderecha();
      } else {
        centro();
      }
      alante();
    } else if(detras > delante&& (detras-delante) > 40){
      if (ldr[2] > ldr[3] && (ldr[2] - ldr[3]) > 30) { ///comparo cada ldr de lado segun que caso
        girarizquierda();
      } else if ((ldr[3] - ldr[2]) > 30) {
        girarderecha();
      }
      else {
        centro();
      }
      atras();
    }else{
    paro();
    centro();
    moverse = false;
      }
  } else {/////moverse es false
    paro();
    centro();
  }
}

/////actuo sobre los pines aqui
void alante() {
  digitalWrite(ena, HIGH);///ena es motor principal de movimiento
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  Serial.println("adelante");
}
void atras() {
  digitalWrite(ena, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  Serial.println("atras");
}
void paro() {
  digitalWrite(ena, LOW);///ena es motor principal de movimiento
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  Serial.println("paro");
}

/////no tengo servo
void girarderecha() {
  analogWrite(enb, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("derecha");
}
void girarizquierda() {
  Serial.println("izquierda");
  analogWrite(enb, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void centro() {
  Serial.println("centro");
  analogWrite(enb, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

  ////solo detecto hacia delante, hacia atras no
  void colision() {
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  distancia = uS / US_ROUNDTRIP_CM;
  Serial.print(distancia);
  Serial.println("cm ");/*
  if(distancia>200||distancia<0){
    distancia = 0;}*/
  if (distancia < 8 && distancia > 0) {
    atras();
    centro();
    Serial.println("He chocado");    
    delay(500);
    paro();
    moverse = false;
  }else{
    moverse=true;
    }
  }




/*
  void loop22() {////////editar aqui
  delay(500);  // Wait 500ms between pings (about 2 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  distancia = uS / US_ROUNDTRIP_CM;
  Serial.print(diatancia); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.println("cm");
  }*/
