#include <bsp.h>
#include <bsp_time.h>
#include <pitches.h>
#include <buzzer_driver.h>
#include <display.h>
#include <eeprom_driver.h>
#include <keypad_driver.h>

//how fast we want the song to be played
#define TEMPO 85

static int divider = 0;
static int note_duration = 0;
static int wholenote = (60000 * 4) / TEMPO;

static int melody[] = {

    //Based on the arrangement at https://www.flutetunes.com/tunes.php?id=169

    NOTE_AS4, -2, NOTE_F4, 8, NOTE_F4, 8, NOTE_AS4, 8, //1
    NOTE_GS4, 16, NOTE_FS4, 16, NOTE_GS4, -2,
    NOTE_AS4, -2, NOTE_FS4, 8, NOTE_FS4, 8, NOTE_AS4, 8,
    NOTE_A4, 16, NOTE_G4, 16, NOTE_A4, -2,
    0, 1,

    NOTE_AS4, 4, NOTE_F4, -4, NOTE_AS4, 8, NOTE_AS4, 16, NOTE_C5, 16, NOTE_D5, 16, NOTE_DS5, 16, //7
    NOTE_F5, 2, NOTE_F5, 8, NOTE_F5, 8, NOTE_F5, 8, NOTE_FS5, 16, NOTE_GS5, 16,
    NOTE_AS5, -2, NOTE_AS5, 8, NOTE_AS5, 8, NOTE_GS5, 8, NOTE_FS5, 16,
    NOTE_GS5, -8, NOTE_FS5, 16, NOTE_F5, 2, NOTE_F5, 4,

    NOTE_DS5, -8, NOTE_F5, 16, NOTE_FS5, 2, NOTE_F5, 8, NOTE_DS5, 8, //11
    NOTE_CS5, -8, NOTE_DS5, 16, NOTE_F5, 2, NOTE_DS5, 8, NOTE_CS5, 8,
    NOTE_C5, -8, NOTE_D5, 16, NOTE_E5, 2, NOTE_G5, 8,
    NOTE_F5, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 8, NOTE_F4, 16, NOTE_F4, 8,

    NOTE_AS4, 4, NOTE_F4, -4, NOTE_AS4, 8, NOTE_AS4, 16, NOTE_C5, 16, NOTE_D5, 16, NOTE_DS5, 16, //15
    NOTE_F5, 2, NOTE_F5, 8, NOTE_F5, 8, NOTE_F5, 8, NOTE_FS5, 16, NOTE_GS5, 16,
    NOTE_AS5, -2, NOTE_CS6, 4,
    NOTE_C6, 4, NOTE_A5, 2, NOTE_F5, 4,
    NOTE_FS5, -2, NOTE_AS5, 4,
    NOTE_A5, 4, NOTE_F5, 2, NOTE_F5, 4,

    NOTE_FS5, -2, NOTE_AS5, 4,
    NOTE_A5, 4, NOTE_F5, 2, NOTE_D5, 4,
    NOTE_DS5, -2, NOTE_FS5, 4,
    NOTE_F5, 4, NOTE_CS5, 2, NOTE_AS4, 4,
    NOTE_C5, -8, NOTE_D5, 16, NOTE_E5, 2, NOTE_G5, 8,
    NOTE_F5, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 16, NOTE_F4, 8, NOTE_F4, 16, NOTE_F4, 8

};
void setup()
{
    buzzer_driver_init();
}

void loop()
{
    int notes = sizeof(melody) / sizeof(*melody) / 2;
    // iterate over the notes of the melody.
    // Remember, the array is twice the number of notes (notes + durations)
    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2)
    {
        // calculates the duration of each note
        divider = melody[thisNote + 1];
        if (divider > 0)
        {
            // regular note, just proceed
            note_duration = (wholenote) / divider;
        }
        else if (divider < 0)
        {
            // dotted notes are represented with negative durations!!
            note_duration = (wholenote) / abs(divider);
            note_duration *= 1.5; // increases the duration in half for dotted notes
        }

        // we only play the note for 90% of the duration, leaving 10% as a pause
        buzzer_driver_tone(melody[thisNote], note_duration);

        // Wait for the speciefed duration before playing the next note.
        bsp_delay(note_duration);

        // stop the waveform generation before the next note.
        buzzer_driver_no_tone();
    }
}