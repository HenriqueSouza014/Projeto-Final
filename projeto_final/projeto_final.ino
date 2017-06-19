#include <stdio.h>
#include "TimerOne.h"
#include <SoftwareSerial.h>


SoftwareSerial mySerial(13, 12); // RX, TX


int LedVerde = 9;
int LedVermelho = 10;
int LedAmarelo = 11;
int sensordir = 0, sensoresq = 2, sensorcen = 3;
int valorsensor1 = 0, valorsensor2 = 0, valorsensor3 = 0;
int flag_check_command = 0;
int motorPin1 = 4; //amarela
int motorPin2 = 5; //laranja
int motorPin3 = 6; //preto
int motorPin4 = 7; //marrom
int delayTime = 10;
int buzzer = 3;
int buzzer_flag = 0;
int ref = 2;
int cont = 0;


/* Rotina auxiliar para comparacao de strings */
int str_cmp(char *s1, char *s2, int len) {
  /* Compare two strings up to length len. Return 1 if they are
      equal, and 0 otherwise.
  */
  int i;
  for (i = 0; i < len; i++) {
    if (s1[i] != s2[i]) return 0;
    if (s1[i] == '\0') return 1;
  }
  return 1;
}

/* Processo de bufferizacao. Caracteres recebidos sao armazenados em um buffer. Quando um caractere
    de fim de linha ('\n') e recebido, todos os caracteres do buffer sao processados simultaneamente.
*/

/* Buffer de dados recebidos */
#define MAX_BUFFER_SIZE 50
typedef struct {
  char data[MAX_BUFFER_SIZE];
  unsigned int tam_buffer;
} serial_buffer;

volatile serial_buffer Buffer;

/* Limpa buffer */
void buffer_clean() {
  Buffer.tam_buffer = 0;
}

/* Adiciona caractere ao buffer */
int buffer_add(char c_in) {
  if (Buffer.tam_buffer < MAX_BUFFER_SIZE) {
    Buffer.data[Buffer.tam_buffer++] = c_in;
    return 1;
  }
  return 0;
}

/* Ao receber evento da UART */
void serialEvent() {
  char c;
  while (Serial.available()) {
    c = Serial.read();
    if (c == '\n') {
      buffer_add('\0'); /* Se recebeu um fim de linha, coloca um terminador de string no buffer */
      flag_check_command = 1;
    } else {
      buffer_add(c);
    }
  }

}

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  buffer_clean();
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Timer1.initialize(25000);
  Timer1.attachInterrupt(temporizador);
}

void temporizador() {

  if (ref == 0) {


    if ((valorsensor1 < 800) || (valorsensor2 < 800) || (valorsensor3 < 800)) {
      buzzer_flag = 0;
      if ((valorsensor1 > valorsensor2) && (valorsensor1 > valorsensor3) && ((valorsensor1 - valorsensor3 >= 10) || (valorsensor2 - valorsensor3 >= 10))) {

        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, HIGH);
        delay(delayTime);
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, HIGH);
        delay(delayTime);
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motorPin3, HIGH);
        digitalWrite(motorPin4, LOW);
        delay(delayTime);
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);
        digitalWrite(motorPin3, HIGH);
        digitalWrite(motorPin4, LOW);
        delay(delayTime);

      }
      if ((valorsensor2 > valorsensor1) && (valorsensor2 > valorsensor3) && ((valorsensor1 - valorsensor3 >= 10) || (valorsensor2 - valorsensor3 >= 10))) {

        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, HIGH);
        delay(delayTime);
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);
        digitalWrite(motorPin3, HIGH);
        digitalWrite(motorPin4, LOW);
        delay(delayTime);
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motorPin3, HIGH);
        digitalWrite(motorPin4, LOW);
        delay(delayTime);
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, HIGH);
        delay(delayTime);

      }

      if ((valorsensor3 < valorsensor2) && (valorsensor3 < valorsensor1) && ((valorsensor1 - valorsensor3 <= 20) || (valorsensor2 - valorsensor3 <= 20))) {
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, LOW);
        delay(delayTime);
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, LOW);
        delay(delayTime);
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, LOW);
        delay(delayTime);
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, LOW);
        delay(delayTime);

      }
    }
  }
}





void loop() {
  // char out_buffer[127];
  int flag_write = 0, i = 0;
  int flag_bluetooth_command = 0;
  char bluetooth_read[3];

  valorsensor1 = analogRead(sensordir);
  valorsensor2 = analogRead(sensoresq);
  valorsensor3 = analogRead(sensorcen);

  if (mySerial.available()) {
    for (i = 0; i < 2; i++) {
      bluetooth_read[i] = bluetooth_read[i + 1];
    }
    bluetooth_read[2] = mySerial.read();
    flag_bluetooth_command = 1;
  }

  if (ref == 0) {

    if ((valorsensor1 < 800) || (valorsensor2 < 800) || (valorsensor3 < 800)) {
      if ((valorsensor1 < valorsensor2) && (valorsensor1 < valorsensor3)) {
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        digitalWrite(11, LOW);
      }
      if ((valorsensor2 < valorsensor1) && (valorsensor2 < valorsensor3)) {
        digitalWrite(10, LOW);
        digitalWrite(9, HIGH);
        digitalWrite(11, LOW);
      }
      if ((valorsensor3 < valorsensor1) && (valorsensor3 < valorsensor2)) {
        digitalWrite(10, LOW);
        digitalWrite(9, LOW);
        digitalWrite(11, HIGH);

      }
    }
  }

  if (ref == 0) {
    if ((valorsensor1 > 800) && (valorsensor2 > 800) && (valorsensor3 > 800)) {
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      mySerial.write("NOITE\n\r");
      Serial.write("NOITE\n\r");
      if (buzzer_flag == 0) {
        tone(3, 400);
        delay(1000);
        noTone(3);
        delay(1000);
        tone(3, 400);
        delay(1000);
        noTone(3);
        buzzer_flag = 1;
      }
    }
  }


  if (ref == 1) {
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);

  }
  if (flag_bluetooth_command == 1) {
    if (str_cmp(bluetooth_read, "INI", 3)) {
      Serial.write("DIA\n");
      mySerial.write("DIA\n\r");
      ref = 0;
      flag_write = 1;
    }
    if (str_cmp(bluetooth_read, "STP", 3)) {
      ref = 1;
      flag_write = 1;
      Serial.write("PLACA PARADA\n");
      mySerial.write("PLACA PARADA\n\r");
    }
    //flag_bluetooth_command = 0;
  }
  if (flag_check_command == 1) {

    if (str_cmp(Buffer.data, "INI", 3) ) {
      ref = 0;
      flag_write = 1;
      Serial.write("DIA\n");
      mySerial.write("DIA\n\r");
    }
    if (str_cmp(Buffer.data, "STP", 3) ) {
      ref = 1;
      flag_write = 1;
      Serial.write("PLACA PARADA\n");
      mySerial.write("PLACA PARADA\n\r");
    }
    else {
      buffer_clean();
    }

    if (flag_write == 1) {
      buffer_clean();
      flag_write = 0;
    }
    flag_check_command = 0;
  }
  //sprintf(out_buffer, "ValorSensorDir: %-8d ValorSensorEsq: %-8d ValorSensorCen: %-8d\n", ref, valorsensor2, valorsensor3);

  delay(200);

}
