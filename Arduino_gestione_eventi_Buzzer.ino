#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment display = Adafruit_7segment();

// Sostituito PIN_LED con PIN_BUZZER sul pin 6
const int BTN_SET = 2, BTN_PIU = 3, BTN_MENO = 4, BTN_OK = 5, BTN_EMER = 7, PIN_BUZZER = 6;

struct Evento { int o; int m; };
Evento agenda[24];

int idx = 0, stato = 0, ore = 8, min = 0;
unsigned long ultimoMillis = 0;
bool allarmeAttivo = false;

// Configurazione iniziale di pin, display e agenda
void setup() {
  pinMode(BTN_SET, INPUT_PULLUP); pinMode(BTN_PIU, INPUT_PULLUP);
  pinMode(BTN_MENO, INPUT_PULLUP); pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(BTN_EMER, INPUT_PULLUP); pinMode(PIN_BUZZER, OUTPUT); // Configurato pin come OUTPUT
  display.begin(0x70); display.setBrightness(10);
  for(int i=0; i<24; i++) agenda[i] = {8, 0};
}

// Scrive l'orario nel formato HH:MM sul display a 7 segmenti
void mostraOra(int o, int m) {
  display.clear();
  display.writeDigitNum(0, (o / 10)); display.writeDigitNum(1, (o % 10));
  display.drawColon(true);
  display.writeDigitNum(3, (m / 10)); display.writeDigitNum(4, (m % 10));
  display.writeDisplay();
}

// Scrive l'identificativo dell'evento nel formato E-XX
void mostraEvento(int n) {
  display.clear();
  display.writeDigitAscii(0, 'E'); 
  display.writeDigitRaw(1, 0x40);
  display.writeDigitNum(3, (n / 10)); display.writeDigitNum(4, (n % 10));
  display.writeDisplay();
}

// Incrementa o decrementa l'orario gestendo il riporto tra minuti e ore
void modificaTempo(int &o, int &m, int dir) {
  m += dir;
  if (m >= 60) { m = 0; o++; } else if (m < 0) { m = 59; o--; }
  if (o >= 24) o = 0; else if (o < 0) o = 23;
}

// Gestisce il suono intermittente del buzzer per 5 secondi senza bloccare il sistema
void gestisciBuzzer() {
  static unsigned long inizioAllarme = 0;
  static unsigned long ultimoSquillo = 0;
  static bool buzzerStato = false;

  // Se l'allarme è attivo ma non abbiamo ancora segnato l'inizio, lo segniamo ora
  if (allarmeAttivo && inizioAllarme == 0) {
    inizioAllarme = millis();
  }

  // Se l'allarme è attivo, controlliamo se sono passati 5 secondi (5000ms)
  if (allarmeAttivo) {
    if (millis() - inizioAllarme >= 5000) {
      allarmeAttivo = false; // Spegni l'allarme
      inizioAllarme = 0;     // Resetta il timer
      digitalWrite(PIN_BUZZER, LOW);
    } else {
      // Logica del suono intermittente (500ms acceso, 500ms spento)
      if (millis() - ultimoSquillo > 200) {
        ultimoSquillo = millis();
        buzzerStato = !buzzerStato;
        digitalWrite(PIN_BUZZER, buzzerStato);
      }
    }
  } else {
    // Se non è attivo, spegni tutto e resetta
    digitalWrite(PIN_BUZZER, LOW);
    inizioAllarme = 0;
  }
}

// Esegue il ciclo di allarme di emergenza (circa 5 secondi di suono intermittente rapido)
void emergenza() {
  allarmeAttivo = false; 
  for(int i = 0; i < 13; i++) { 
    digitalWrite(PIN_BUZZER, HIGH); delay(200);
    digitalWrite(PIN_BUZZER, LOW); delay(200);
  }
}

// Gestione continua del tempo, degli eventi e della navigazione tra i menu
void loop() {
  if (millis() - ultimoMillis >= 60000) {
    ultimoMillis = millis(); min++;
    if (min >= 60) { min = 0; ore++; }
    if (ore >= 24) ore = 0;
    for(int i=0; i<24; i++) {
      if (ore == agenda[i].o && min == agenda[i].m) allarmeAttivo = true;
    }
  }

  if (digitalRead(BTN_EMER) == LOW) {
    emergenza();
  } else {
    gestisciBuzzer(); // Sostituito il vecchio gestisciLed()
  }

  if (digitalRead(BTN_SET) == LOW && stato == 0) { stato = 1; delay(300); }
  if (digitalRead(BTN_OK) == LOW) { allarmeAttivo = false; stato = 0; delay(300); }

  switch(stato) {
    case 0: mostraOra(ore, min); break;
    case 1: 
      mostraEvento(idx);
      if (digitalRead(BTN_PIU) == LOW) { idx = (idx + 1) % 24; delay(250); }
      if (digitalRead(BTN_MENO) == LOW) { idx = (idx - 1 + 24) % 24; delay(250); }
      if (digitalRead(BTN_OK) == LOW) { stato = 2; delay(300); }
      break;
    case 2:
      if (digitalRead(BTN_PIU) == LOW) { modificaTempo(agenda[idx].o, agenda[idx].m, 1); delay(250); }
      if (digitalRead(BTN_MENO) == LOW) { modificaTempo(agenda[idx].o, agenda[idx].m, -1); delay(250); }
      mostraOra(agenda[idx].o, agenda[idx].m);
      break;
  }
}