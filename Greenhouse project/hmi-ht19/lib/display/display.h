/**
 * @file display.h
 * @author Wael Al Henawe (wael.henawe@gmail.com)
 * @brief This modular is for initialize an instance of a display (240*320 2.0 TFT ST7789) and give the ability 
 *        to draw different types of shaps and also write a text or one char.
 * @version 0.1
 * @date 2020-10-27
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

typedef struct
{
    int16_t x;
    int16_t y;
} position_t;

typedef struct
{
    position_t center;
    int16_t radius;
    uint16_t color;
} circle_t;

typedef struct
{
    position_t pos;
    const char *text;
    uint16_t color;
    uint16_t bg_color;
    uint8_t size;
} text_t;

typedef struct
{
    position_t pos;
    uint16_t width;
    uint16_t height;
    uint16_t color;
} rectangle_t;

typedef struct
{
    position_t pos;
    const char chr;
    uint16_t color;
    uint16_t bg_color;
    uint16_t size;
} char_t;

typedef struct
{
    position_t pos;
    int16_t length;
    uint16_t color;
} line_t;

typedef struct
{
    position_t pos1;
    position_t pos2;
    position_t pos3;
    uint16_t color;
} triangle_t;

/**
 * @brief This function will inislize the display and   
 *        set the width, height, background color and the rotation.
 */
void display_init(void);

/**
 * @brief This function will update the display content.
 * 
 */
void display_update(void);

/**
 * @brief This function will draw a text on the display.
 * 
 * @param text is a struct of type text_t which have a struct of type postion_t for postion (x,y), 
 *        (char *) a pointer to char array which hold the text to display,(uint16_t) the color of the text,
 *        (uint16_t)the text background color, (uint8_t)the size of the text(Available size of text for this func is 8 to 14).
 */
void display_draw_text(text_t text);

/**
 * @brief This function will draw a circle on the display.
 * 
 * @param circle is a struct of circle_t type which have a struct of type postion_t for center (x,y), 
 *        (uint16_t)the radius of the circle, (uint16_t)the color of the circle.
 */
void display_draw_circle(circle_t circle);

/**
 * @brief This function will draw a single char on the display.
 * 
 * @param chr is a struct of type char_t which have a struct of type postion_t for postion (x,y), 
 *        (char) a char that will be displayed on display,(uint16_t) the color of the char,
 *        (uint16_t)the char background color, (uint8_t)the size of the char.
 */
void display_draw_char(char_t chr);

/**
 * @brief This function will draw a line on the display.
 * 
 * @param line is a struct of line_t type which have a struct of type postion_t for postion (x,y), 
 *        (uint16_t)the length of the line, (uint16_t)the color of the line.
 */
void display_draw_line(line_t line);

/**
 * @brief This function will draw a rectangle on the display.
 * 
 * @param rectangle is a struct of rectangle_t type which have a struct of type postion_t for postion (x,y), 
 *        (uint16_t)the width of the rectangle, (uint16_t)the height of the rectangle, (uint16_t)the color of the rectangle.
 */
void display_draw_rect(rectangle_t rectangle);

/**
 * @brief This function will draw a triangle on the display.
 * 
 * @param triangle is a struct of triangle_t type which has three structs of type postion_t for postion (x,y) 
 *        for each angle of the triangle, (uint16_t)the color of the triangle.
 */
void display_draw_triangle(triangle_t triangle);

#endif
