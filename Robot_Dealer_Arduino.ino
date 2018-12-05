#include "SoftwareSerial.h"
#include <Stepper.h>

int jogo = 0, estado = 0;
int mte1 = 10, mte2 = 6, mtd1 = 9, mtd2 = 5, LDR1 = A0, LDR2 = A1, LDR3 = A2;
const int stepsPerRevolution = 500;
const int ve1 = 140; //Velocidade dos motores que embaralham
const int ve2 = 130; //Velocidade dos motores que embaralham
const int ve = 255; //Velocidade dos motores que embaralham
const int vd1 = 255;
String s = "";

Stepper myStepper(stepsPerRevolution, 7, 8, 12, 13);
SoftwareSerial bluetooth(3, 2); //TX, RX (Bluetooth)

void setup() {

  myStepper.setSpeed(10);
  Serial.begin(9600);
  bluetooth.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(mte1, OUTPUT);
  pinMode(mte2, OUTPUT);
  pinMode(mtd1, OUTPUT);

}

void acelerar(int passo) {
  for (int k = 10; k <= 20; k = k + 10) {

    myStepper.setSpeed(k);
    myStepper.step(passo / 2);

  }
  delay(2000);
}

void truco(int nj) {
  Serial.println("Jogo = Truco");
  embaralhar();
  Serial.println(nj);
  int p = 2048 / nj;
  int p2 = 2048 / (nj + 1);

  for (int i = 0; i < nj; i++) {
    Serial.print("Jogador = ");
    Serial.println(i + 1);

    for (int j = 0; j < 3; j++) {
      Serial.print("Carta = ");
      Serial.println(j + 1);
      distribuir(1);
      delay(1000);
    }

    acelerar(p); //Movimenta o robô

  }

  acelerar(p2); //Movimenta o robô
  distribuir(1);
  delay(1000);
  zerarCartas(p2);
}

void fodinha(int nj, int nc) {
  Serial.println("Jogo = Fodinha");
  embaralhar();
  int p = 2048 / nj;
  int p2 = 2048 / (nj + 1);
  for (int i = 0; i < nj; i++) {
    Serial.print("Jogador = ");
    Serial.println(i);
    for (int j = 0; j < nc; j++) {
      Serial.print("Carta = ");
      Serial.println(j);
      distribuir(1);
      delay(800);
    }
    acelerar(p); //Movimenta o robô
  }
  acelerar(p2); //Movimenta o robô
  distribuir(1);
  delay(2000);
  zerarCartas(p2);
}

void poker(int nj) {
  Serial.println("Jogo = Poker");
  embaralhar();
  int p = 2048 / nj;
  int p2 = 2048 / (nj + 1);
  for (int j = 1; j <= nj; j++) {
    Serial.print("Jogador = ");
    Serial.println(j);
    for (int i = 1; i <= 2; i++) {
      Serial.print("Carta = ");
      Serial.println(i);
      distribuir(1);
      delay(1000);
    }
    //acelerar(p); //Movimenta o robô
  }

  //acelerar(p2); //Movimenta o robô
  for (int i = 1; i <= 5; i++) {
    Serial.print("Mesa = ");
    Serial.println(i);
    distribuir(1);
    delay(600);
  }
  zerarCartas(p2);
}

void buraco (int nj) {
  Serial.println("Jogo = Buraco");
  embaralhar();
  int p = 2048 / nj;
  int p2 = 2048 / (nj + 1);
  for (int i = 1; i <= nj; i++) {
    for (int j = 1; j <= 11; j++) {
      distribuir(1);
      delay(1000);
    }
    acelerar(p);
  }
  for (int j = 1; j <= 2; j++) {
    for (int j = 1; j <= 11; j++) {
      distribuir(1);
      delay(1000);
    }
    acelerar(p2);
  }
  zerarCartas(p2);
}

void copas() {
  Serial.println("Jogo = Copas");
  embaralhar();
  int p = 2048 / 4;
  for (int i = 1; i <= 13; i++) {
    for (int j = 1; j <= 4; j++) {
      distribuir(1);
      delay(1000);
      //myStepper.step(p);
    }
  }
}

void napas() {
  Serial.println("Jogo = Napas");
  embaralhar();
  int p = 2048 / 5;
  int p2 = 2048 / 6;
  for (int i = 1; i <= 5; i++) {
    Serial.print("Jogador = ");
    Serial.println(i);
    delay(1000);
    for (int j = 1; j <= 10; j++) {
      Serial.print("Carta = ");
      Serial.println(j);
      distribuir(1);
      delay(1000);
    }
    acelerar(p);
  }
  for (int j = 1; j <= 3; j++) {
    Serial.print("Jogador = ");
    Serial.println(j);
    distribuir(1);
    delay(1000);
  }
}

void embaralhar() {
  Serial.println("embaralhando");
  estado = 1;
  while (estado == 1)
  {
    int v1 = analogRead(LDR1);
    int v2 = analogRead(LDR2);
    Serial.println(v1);
    Serial.println(v2);
    if (v1 < 950 || v2 < 950) {
      analogWrite(mte1, ve);
      analogWrite(mte2, ve2);
      delay(130);
      analogWrite(mte1, ve1);
      analogWrite(mte2, ve);
      delay(130);
    } else {
      delay(1000);
      analogWrite(mte1, 0);
      analogWrite(mte2, 0);
      estado = 0;
    }
    Serial.println("Done!");
  }
  delay(1000);
}

void zerarCartas(int passo) {

  acelerar(passo); //Movimenta o robô

  analogWrite(mtd1, 150);//Acertar a velocidade
  analogWrite(mtd2, 255);
  delay(10000);
  analogWrite(mtd1, 0);
  analogWrite(mtd2, 0);

}

void distribuir(int nc) {
  Serial.println("Distribuir");
  analogWrite(mtd1, 130); //Liga motor de lançamento
  int i = 0; //Contador das cartas

  while (i < nc) {
    analogWrite(mtd2, 130); //Liga motor de coleta da carta
    int estado = 1; //Controle do Loop
    int c = 0; //Flag presença da carta
    int t = 0; //Tentativa
    while (estado == 1) {
      int v3 = analogRead(LDR3);
      Serial.print("Sensor = ");
      Serial.println(v3);
      if ( v3 < 950) {
        if (c == 0) {
          Serial.println("Entrei no contador!");
          i += 1;
          analogWrite(mtd2, 0);
          c = 1;
          delay(500);
        } else {
          if (t >= 3) {
            analogWrite(mtd2, 90);
            delay(100);
            analogWrite(mtd2, 0);
            t = 0;
            estado = 0;
          } else {
            delay(500);
            t++;
          }
        }
      } else {
        estado = 0;
        c = 0;
      }
    }
  }
  analogWrite(mtd1, 0);
  analogWrite(mtd2, 0);
  delay(1000);
}

String lerBT() {
  s = "";//Inicializa a string vazia
  if (bluetooth.available()) {
    Serial.println("Recebi o BT");
    s = "";//Inicializa a string vazia
    char r = bluetooth.read(); //Lê o char enviado
    while (r != 'X') { //Enquanto o char lido não for o char de término
      if (r != "⸮") {
        s += r;
      }
      bluetooth.available();
      r = bluetooth.read();
    }
    Serial.println(s);
  }
  return s;

}

void rotacionar() {
  acelerar(2048);
  estado = 1;
  //Loop
  s = "";
  while (estado == 1) {
    myStepper.step(2048);
    delay(6000);
    s = lerBT();
    if (s != "") {
      Serial.println("Entrei no Decode");
      char J = s[0]; //
      if (J == 'P') {
        estado = 0;
      } else {
        s = "";
      }
    } //Fim if S
  } //Fim While
} //Fim Rotacionar

void blackJack(int nj) {
  int estado = 1;
  while (estado == 1) {
    Serial.println("Jogo = Black Jack");
    embaralhar();
    int estado = 0;
    nj = nj+1;
    for (int i = 1; i <= nj; i++) {
      Serial.print("Jogador = ");
      Serial.println(i + 1);
      for (int j = 1; j <= 2; j++) {
        Serial.print("Carta = ");
        Serial.println(j);
        distribuir(1);
        delay(800);
      }
    }
    int controle = 1;
    while (controle == 1) {
      Serial.println("Loop controle");
      s = lerBT();
      Serial.println(s);
      char J = s[0];
      if (J == 'N') {
        distribuir(1);
        delay(800);
      } else if (J == 'R') {
        controle = 0;
        estado = 0;
      }
    }
  }
}

void loop() {

  //Recebe comando BT
  s = "";//Inicializa a string vazia
  s = lerBT();

  //Decode
  if (s != "") {
    Serial.println("Entrei no Decode");
    char J = s[0]; //Jogo
    Serial.println(J);
    int nj = (int)s[1] - 48; //Número de Jogadores
    Serial.println(nj);
    int nc = (int)s[2] - 48; //Número de Cartas
    Serial.println(nc);

    //Seleção do Jogo
    if (J == 'T') {
      truco(nj);
    } else if (J == 'F') {
      fodinha(nj, nc);
    } else if (J == 'P') {
      poker(nj);
    } else if (J == 'B') {
      buraco(nj);
    } else if (J == 'C') {
      fodinha(nj, nc);
    } else if (J == 'N') {
      napas();
    } else if (J == 'R') {
      rotacionar();
    } else if (J == 'V') {
      blackJack(nj);
      s="";
    }
  }
}
