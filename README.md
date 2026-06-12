# Arduino Digital Agenda

Agenda digitale basata su Arduino, progettata per gestire segnalazioni temporizzate (tipo campanella scolastica) o promemoria giornalieri.

## Breve Descrizione
Orologio con 24 eventi programmabili, gestione allarmi e funzione emergenza.

## Funzionalità
- **Gestione Eventi:** 24 slot (E-00 a E-23) per orari programmabili.
- **Interfaccia:** 5 pulsanti per navigazione e impostazioni.
- **Feedback:** Supporto per LED o Buzzer.
- **Funzione Emergenza:** Tasto dedicato per attivazione rapida.
- **Gestione non bloccante:** Utilizzo di `millis()` per non interferire con il conteggio del tempo.

## Hardware Richiesto
- Arduino UNO
- Display 0.56" 7-Segment (HT16K33)
- 5x Pulsanti
- LED o Buzzer
- Breadboard e cavi jumper

## Utilizzo
1. **Modalità Normale:** Visualizzazione ora corrente.
2. **Set Eventi:** Premere [SET], navigare con [+] / [-], confermare con [OK].
3. **Modifica Ora:** Regolare ore/minuti nello slot scelto e salvare con [OK].
4. **Emergenza:** Premere [EMER] per attivare l'allarme immediato.

## Requisiti Software
È necessario installare le seguenti librerie nell'IDE Arduino:
- `Adafruit_GFX`
- `Adafruit_LEDBackpack`

## Licenza
Progetto open-source. Sentiti libero di modificare e migliorare il codice!
