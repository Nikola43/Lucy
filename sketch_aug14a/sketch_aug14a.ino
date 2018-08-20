#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define LED 13

void setup()
{
 pinMode(LED, OUTPUT);
 MIDI.begin(); // Configura MIDI y escucha todos los canales
 Serial.begin(115200);
}
void loop()
{
 if (MIDI.read()) // Comprueba si hay mensajes
 {
 int tipo=MIDI.getType();
 int nota=MIDI.getData1();
 if (tipo == 0x90 && nota == 60)
 {
 digitalWrite(LED,HIGH);
 }
 else
 digitalWrite(LED,LOW);
 }
}
