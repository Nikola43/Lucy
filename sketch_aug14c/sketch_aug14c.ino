// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  MIDIEvent noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MIDIUSB.write(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  MIDIEvent noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MIDIUSB.write(noteOff);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  MIDIEvent event = {0x0B, 0xB0 | channel, control, value};
  MIDIUSB.write(event);
}

//MIDI channel to use
#define MIDI_CHANNEL 0

//CC slots 14-31 are recognized as assignable controls by MIDI standard
#define MIDI_CC_START 14

#define MUX_NUM_CHANNELS 1

void setup() {

}

void loop() {
  /*
  noteOn(0, 48, 64);   // Channel 0, middle C, normal velocity
  MIDIUSB.flush();
  delay(500);
  
  noteOff(0, 48, 64);  // Channel 0, middle C, normal velocity
  MIDIUSB.flush();
  delay(1500);
  */
  
  delay(15000);
  
  while(true){
    int muxChannel = 0;
    
    
    for (byte muxChannel = 0; muxChannel < MUX_NUM_CHANNELS; ++muxChannel)
    {
        for(int i = 0; i < 255; i++){
        controlChange(MIDI_CHANNEL, MIDI_CC_START + muxChannel, i);
        delay(50);
        }
      
        for(int i = 255; i > 1; i--){
        controlChange(MIDI_CHANNEL, MIDI_CC_START + muxChannel, i);
        delay(50);
        }
    }
  }
}
