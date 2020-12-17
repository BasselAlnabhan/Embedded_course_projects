#include <switches.h>

typedef enum
{
    CHOICE,
    VALUE
} input_types_t;

static char choice;
static displays_t display_type = MAIN_MENU;
static components_t chosen_sen;
static components_t chosen_act;
static uint8_t act_name_index = 0;

static displays_t set_cal_menu_switch(void)
{
    static bool sensors_menu_flag = true;
    if (sensors_menu_flag)
    {
        set_cal_menu_text();
    }
    sensors_menu_flag = false;
    switch (choice)
    {
    case '1' ... '2':
        sensors_menu_flag = true;
        return CAL_SET_WINDOW;
        break;
    case '0':
        sensors_menu_flag = true;
        return MAIN_MENU;
        break;
    default:
        return SET_CAL_MENU;
        break;
    }
}

static displays_t set_tar_menu_switch(void)
{
    static bool set_tar_menu_flag = true;
    if (set_tar_menu_flag)
    {
        set_tar_menu_text();
    }
    set_tar_menu_flag = false;
    switch (choice)
    {
    case '1' ... '6':
        set_tar_menu_flag = true;
        return TAR_SET_WINDOW;
        break;
    case '0':
        set_tar_menu_flag = true;
        return MAIN_MENU;
        break;
    default:
        return SET_TAR_MENU;
        break;
    }
}

static displays_t act_overwrite_menu_switch(void)
{
    static bool act_overwrite_menu_flag = true;
    if (act_overwrite_menu_flag)
    {
        act_overwrite_menu_text();
    }
    act_overwrite_menu_flag = false;
    switch (choice)
    {
    case '1' ... '6':
        act_overwrite_menu_flag = true;
        return OVERWRITE_ACT_WINDOW_STATE;
        break;
    case '0':
        act_overwrite_menu_flag = true;
        return MAIN_MENU;
        break;
    default:
        return OVERWRITE_ACT_MENU;
        break;
    }
}

static displays_t main_menu_switch()
{
    static bool main_menu_flag = true;
    if (main_menu_flag)
    {
        main_menu_text();
    }
    main_menu_flag = false;
    switch (choice)
    {
    case '1':
        main_menu_flag = true;
        return SYS_OVERVIEW;
        break;
    case '2':
        main_menu_flag = true;
        return COMPONENTS_STATUS;
        break;
    case '3':
        main_menu_flag = true;
        return SET_CAL_MENU;
        break;
    case '4':
        main_menu_flag = true;
        return SET_TAR_MENU;
        break;
    case '5':
        main_menu_flag = true;
        return OVERWRITE_ACT_MENU;
        break;
    default:
        return MAIN_MENU;
        break;
    }
}

static void reset_flags(bool *first, bool *second, bool *third, bool *forth, bool *fifth, bool *sixth, bool *seventh)
{
    *first = true;
    *second = true;
    *third = true;
    *forth = true;
    *fifth = true;
    *sixth = true;
    *seventh = true;
}

displays_t act_state_switch(void)
{
    static bool act_overwrite_window_text_flag = true;
    static bool auto_flag = true;
    static bool manual_flag = true;
    static bool auto_manual_text_flag = true;
    static bool on_off_text_flag = true;
    static bool on_flag = true;
    static bool off_flag = true;

    static uint8_t click_conter = 0;

    components_t component = (components_t)(act_name_index + 7);
    if (act_overwrite_window_text_flag)
    {
        default_state_window_text(act_name_index);
    }
    if (auto_manual_text_flag && (component == HEATER || component == LAMP))
    {

        manual_auto_text(click_conter, auto_flag, manual_flag);
        auto_manual_text_flag = false;
        auto_flag = false;
        manual_flag = false;
    }

    if (on_off_text_flag && (component == PUMP || component == VALAE || component == WINDOW || component == FANS))
    {
        on_off_text(click_conter, auto_flag, on_flag, off_flag);
        auto_flag = false;
        on_flag = false;
        off_flag = false;
        on_off_text_flag = false;
    }
    act_overwrite_window_text_flag = false;

    switch (choice)
    {
    case '1':
        if (component == LAMP || component == HEATER)
        {
            auto_manual_text_flag = true;
            if (click_conter == 0)
            {
                click_conter++;
                manual_flag = true;
                break;
            }
            if (click_conter == 1)
            {
                click_conter = 0;
                manual_flag = false;
                auto_flag = true;
                break;
            }
            auto_manual_text_flag = false;
        }
        else
        {
            on_off_text_flag = true;
            if (click_conter == 0)
            {
                click_conter++;
                auto_flag = false;
                on_flag = true;
                off_flag = false;
                break;
            }
            if (click_conter == 1)
            {
                click_conter++;
                auto_flag = false;
                on_flag = false;
                off_flag = true;
                break;
            }
            if (click_conter == 2)
            {
                click_conter = 0;
                auto_flag = true;
                on_flag = false;
                off_flag = false;
                break;
            }
            on_off_text_flag = false;
        }
    case '#':
        if (click_conter == 0)
        {
            click_conter = 0;
            set_act_to_auto(component);
            reset_flags(&act_overwrite_window_text_flag,
                        &auto_flag,
                        &manual_flag,
                        &auto_manual_text_flag,
                        &on_off_text_flag,
                        &on_flag,
                        &off_flag);
            return OVERWRITE_ACT_MENU;
        }
        else if (click_conter == 1 && (component == LAMP || component == HEATER))
        {
            click_conter = 0;
            reset_flags(&act_overwrite_window_text_flag,
                        &auto_flag,
                        &manual_flag,
                        &auto_manual_text_flag,
                        &on_off_text_flag,
                        &on_flag,
                        &off_flag);
            return OVERWRITE_ACT_WINDOW_VALUE_PART;
        }
        else if (click_conter == 1 && component != LAMP && component != HEATER)
        {
            click_conter = 0;
            set_act_to_on_open(component);
            reset_flags(&act_overwrite_window_text_flag,
                        &auto_flag,
                        &manual_flag,
                        &auto_manual_text_flag,
                        &on_off_text_flag,
                        &on_flag,
                        &off_flag);
            return OVERWRITE_ACT_MENU;
        }
        else if (click_conter == 2)
        {
            click_conter = 0;
            reset_flags(&act_overwrite_window_text_flag,
                        &auto_flag,
                        &manual_flag,
                        &auto_manual_text_flag,
                        &on_off_text_flag,
                        &on_flag,
                        &off_flag);
            set_act_to_off_close(component);
            return OVERWRITE_ACT_MENU;
        }
        break;
    case '0':
        click_conter = 0;
        reset_flags(&act_overwrite_window_text_flag,
                    &auto_flag,
                    &manual_flag,
                    &auto_manual_text_flag,
                    &on_off_text_flag,
                    &on_flag,
                    &off_flag);
        return OVERWRITE_ACT_MENU;
        break;

    default:
        break;
    }
    return OVERWRITE_ACT_WINDOW_STATE;
}

displays_t system_overview_switch(char choice, cal_tar_data_t data)
{
    static bool page_one_flag = true;
    static bool page_two_flag = true;
    static bool page_three_flag = true;
    static bool page_four_flag = true;

    static displays_t return_display;
    static uint8_t page_number = 1;

    if (choice == '1' && page_number < 4)
    {
        page_number++;
    Serial.printf(" page no %d", page_number);
    Serial.printf(" page three flage %d", page_three_flag);
    }
    if (choice == '0')
    {
        page_number--;
        page_one_flag = true;
        page_two_flag = true;
        page_three_flag = true;
        page_four_flag = true;
    }
    if (choice == '#' && page_number !=1)
    {
        page_number = 0U;
    }

    switch (page_number)
    {
    case 1:
        if (page_one_flag)
        {
            sys_info_page_one(&page_one_flag);
        }
        break;
    case 2:
        if (page_two_flag)
        {
            sys_info_page_two(&page_two_flag);
        }
        break;
    case 3:
        if (page_three_flag)
        {
            sys_info_page_three(&page_three_flag);
        }
        break;
    case 4:
        if (page_four_flag)
        {
            sys_info_page_four(&page_four_flag, data);
        }
        break;
    default:
        break;
    }
    if (page_number == 0)
    {
        return_display = MAIN_MENU;
        page_one_flag = true;
        page_two_flag = true;
        page_three_flag = true;
        page_four_flag = true;
        page_number = 1;
    }
    else
    {
        return_display = SYS_OVERVIEW;
    }

    return return_display;
}

displays_t components_status_switch(char choice)
{
    static bool page_one_flag = true;
    static bool page_two_flag = true;
    static bool page_three_flag = true;
    static bool page_four_flag = true;
    static displays_t return_display;
    static uint8_t page_number = 1;

    if (choice == '1' && page_number < 4)
    {
        page_number++;
        Serial.printf(" page no %d", page_number);
        Serial.printf(" page three flage %d", page_three_flag);
    }
    if (choice == '0')
    {
        page_number--;
        page_one_flag = true;
        page_two_flag = true;
        page_three_flag = true;
        page_four_flag = true;
    }
    if (choice == '#' && page_number != 1)
    {
        page_number = 0U;
    }
    switch (page_number)
    {
    case 1:
        if (page_one_flag)
        {
            comp_status_page_one(&page_one_flag);
        }
        break;
    case 2:
        if (page_two_flag)
        {
            comp_status_page_two(&page_two_flag);
        }
        break;
    case 3:
        if (page_three_flag)
        {
            comp_status_page_three(&page_three_flag);
        }
        break;
    case 4:
        if (page_four_flag)
        {
            comp_status_page_four(&page_four_flag);
        }
        break;
    default:
        break;
    }
    if (page_number == 0)
    {
        return_display = MAIN_MENU;
        page_one_flag = true;
        page_two_flag = true;
        page_three_flag = true;
        page_four_flag = true;

        page_number = 1;
    }
    else
    {
        return_display = COMPONENTS_STATUS;
    }

    return return_display;
}

void home_menu_switch(void)
{
    static input_types_t input_type = CHOICE;
    if (input_type == CHOICE)
    {
        choice = keypad_driver_get_key();
        switch (display_type)
        {
        case MAIN_MENU:
            display_type = main_menu_switch();
            break;
        case SYS_OVERVIEW:
            display_type = system_overview_switch(choice, get_cal_tar_values());
            break;
        case COMPONENTS_STATUS:
            display_type = components_status_switch(choice);
            break;
        case SET_CAL_MENU:
            display_type = set_cal_menu_switch();
            if (display_type == CAL_SET_WINDOW)
            {
                chosen_sen = (components_t)((choice - '0') - 1);
                input_type = VALUE;
            }
            break;
        case SET_TAR_MENU:
            display_type = set_tar_menu_switch();
            if (display_type == TAR_SET_WINDOW)
            {
                chosen_sen = (components_t)((choice - '0') - 1);
                input_type = VALUE;
            }
            break;
        case OVERWRITE_ACT_MENU:

            display_type = act_overwrite_menu_switch();
            if (display_type == OVERWRITE_ACT_WINDOW_STATE)
            {
                act_name_index = (uint8_t)((choice - '0') - 1);
                chosen_act = (components_t)(act_name_index + 7);
            }

            break;
        case OVERWRITE_ACT_WINDOW_STATE:
            display_type = act_state_switch();
            if (display_type == OVERWRITE_ACT_WINDOW_VALUE_PART)
            {
                input_type = VALUE;
            }
            break;
        default:
            break;
        }
    }
    else if (input_type == VALUE)
    {
        switch (display_type)
        {
        case SET_TAR_MENU:
        case SET_CAL_MENU:
        case OVERWRITE_ACT_MENU:
            input_type = CHOICE;
            break;
            case CAL_SET_WINDOW:
            display_type = cal_tar_values_input_window(chosen_sen, CAL_SET_WINDOW, DEFAULT_MIN_FIELD_POS);
            break;
        case TAR_SET_WINDOW:
            display_type = cal_tar_values_input_window(chosen_sen, TAR_SET_WINDOW, DEFAULT_MIN_FIELD_POS);
            break;
        case OVERWRITE_ACT_WINDOW_VALUE_PART:
            display_type = cal_tar_values_input_window(chosen_act, OVERWRITE_ACT_WINDOW_VALUE_PART, DEFAULT_OVERWRIE_FIELD_POS);
            break;
        default:
            break;
        }
    }
}

