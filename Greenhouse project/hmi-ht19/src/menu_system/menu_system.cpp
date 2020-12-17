#include <menu_system.h>
#include <eeprom_driver.h>
#include <display.h>
#include <switches.h>
#include <pitches.h>

//#define FIRST_START
#define DATE_AND_TIME_POS {7, 294}

static void update_and_draw_sys_status_circle(uint8_t *sys_status);
static void init_hmi_comps_and_set_okay(void);
static void welcome_screen(int16_t *melody_array, uint8_t size_of_melody);

static int16_t welcome_melody[] = {
 NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8,     
 REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,    
 NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8,     
 REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,    
 NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8};

void menu_system_init(void)
{
    init_hmi_comps_and_set_okay();
    welcome_screen(welcome_melody, sizeof(welcome_melody));

#ifdef FIRST_START
    eeprom_driver_init(true);
    config_eeprom(true);
#else
    config_cal_tar_val();
#endif
}

void menu_system_run(void)
{
    home_menu_switch();
    data_t data = get_candata();
    update_and_draw_sys_status_circle(&data.hmi.system.status);
    display_draw_text({{7, 280}, "Date and Time:", WHITE, BLACK, 8});
    display_draw_text({DATE_AND_TIME_POS, data.communication.rtc.datetime, WHITE, BLACK, 13});
}

static void update_and_draw_sys_status_circle(uint8_t *sys_status)
{
    static bool current_sys_statue_flag = true;
    static uint8_t current_sys_statue;
    if (current_sys_statue_flag)
    {
        current_sys_statue = *sys_status;
        draw_status_circle(current_sys_statue, {210, 287}, 11, true, false);
        current_sys_statue_flag = false;
    }
    if (current_sys_statue != *sys_status)
    {
        current_sys_statue_flag = true;
    }
}

static void init_hmi_comps_and_set_okay(void)
{
    display_init();
    buzzer_driver_init();
    keypad_driver_init();
    eeprom_driver_init(false);

    set_display_status(OKAY);
    set_eeprom_status(OKAY);
    set_keypad_status(OKAY);
    set_buzzer_status(OKAY);
    set_rgb_status(OKAY);
}


static void welcome_screen(int16_t *melody_array, uint8_t size_of_melody)
{
    static uint8_t x = 0;
    uint8_t tempo = 170U;
    int16_t divider = 0;
    int16_t note_duration = 0;
    int16_t wholenote = (60000 * 4) / tempo;
    int16_t notes = size_of_melody / sizeof(*melody_array) / 2;
    // iterate over the notes of the melody.
    // Remember, the array is twice the number of notes (notes + durations)
    display_draw_rect({{0, 0}, 240, 320, WHITE});
    for (uint16_t thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2)
    {
        // calculates the duration of each note
        divider = melody_array[thisNote + 1];
        if (divider > 0)
        {
            // regular note, just proceed
            note_duration = (wholenote) / divider;
        }
        else if (divider < 0)
        {
            // dotted notes are represented with negative durations!!
            note_duration = (wholenote) / divider;
            note_duration *= -1.5; // increases the duration in half for dotted notes
        }

        // we only play the note for 90% of the duration, leaving 10% as a pause
        for (int32_t k = 0; k < (note_duration * 1000); k++)
        {
            buzzer_driver_tone(melody_array[thisNote], note_duration);
        }
        if (x < 70)
        {
            int16_t x_new = 120 - x;
            int16_t h = (int16_t)(160 - x);
            int16_t w = (120 + x);
            uint16_t w_rect = x + x;
            display_draw_triangle({{x_new, 160}, 120, h, w, 160, 0x07E0});
            display_draw_rect({{51, 162}, w_rect, x, 0x07E0});
            x += 4;
        }
            if (x > 70)
            {
            x += 2;
            if (x == 80)
            {
                display_draw_text({{70, 70}, "WELCOME", 0x6400, WHITE, 14});
            }

            if (x == 90)
            {
                display_draw_text({{105, 180}, "TO", 0x6400, GREEN, 14});
            }

            if (x == 100)
            {
                display_draw_text({{50, 250}, "GREEN HOUSE", 0x6400, WHITE, 14});
            }
        }
        buzzer_driver_no_tone();
    }
    x = 0;
    display_draw_rect({{0, 0}, 240, 320, BLACK});
}
