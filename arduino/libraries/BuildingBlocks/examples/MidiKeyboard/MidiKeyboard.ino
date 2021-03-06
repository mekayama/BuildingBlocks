// MidiKeyboard example. Assumes Arduino connected to the Mainboard, and the Keyboard module connected to the first port (SV1).

#include <SPI.h>
#include <BuildingBlocks.h>

// Define an instance of Keyboard module.
OctaveKeyboard keyboard;

// Define an instance of the Mainboard module.
Mainboard mainboard(keyboard);

void setup() {
	// Setup serial communication for the MIDI port.
	Serial1.begin(31250);

	// Initialize the Mainboard module
	mainboard.init();

	// Set the function which will get called when keyboard events are detected.
	keyboard.setCallback(&onNoteEvent);
}

// Just update the Mainboard in order to receive the events that are happening in the attached modules.
void loop() {
	mainboard.update();
}

// This will get called while executing mainboard.update(), if events were detected.
void onNoteEvent(void *userdata, uint8_t note, bool isDown) {
	sendMidiNote(0, note, isDown ? 100 : 0);
}

// Send the 3 byte MIDI Note On / Off message through the Serial port.
void sendMidiNote(uint8_t channel, uint8_t pitch, uint8_t velocity) {
	Serial1.write(0x90 | channel);
	Serial1.write(pitch);
	Serial1.write(velocity);
}
