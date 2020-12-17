#include <display.h>
#include <ST7789_t3.h>
#include <st7735_t3_font_Arial.h>

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320
#define MIN_TEXT_S 8
#define MAX_TEXT_S 14

#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_CS 10
#define TFT_RST 24
#define TFT_DC 25

static ILI9341_t3_font_t available_text_size[7] = {Arial_8, Arial_9, Arial_10, Arial_11, Arial_12, Arial_13, Arial_14};

static ST7789_t3 display = ST7789_t3(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

static ILI9341_t3_font_t get_arial_text_size(uint8_t size);

void display_init(void)
{
    display.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    //digitalWrite(TFT_RST, HIGH);
    display.fillScreen(ST7735_BLACK);
    display.setRotation(2);
}

void display_draw_text(text_t text)
{
    display.setTextColor(text.color, text.bg_color);
    display.setFont(get_arial_text_size(text.size));
    display.setCursor(text.pos.x, text.pos.y);
    display.print(text.text);
}

void display_draw_rect(rectangle_t rectangle)
{
    display.fillRect(rectangle.pos.x, rectangle.pos.y, rectangle.width, rectangle.height, rectangle.color);
}

void display_draw_circle(circle_t circle)
{
    display.fillCircle(circle.center.x, circle.center.y, circle.radius, circle.color);
}

void display_draw_char(char_t chr)
{
    display.drawChar(chr.pos.x, chr.pos.y, chr.chr, chr.color, chr.bg_color, chr.size);
}

void display_update(void)
{
    display.updateScreen();
}

void display_draw_line(line_t line)
{
    display.drawFastHLine(line.pos.x, line.pos.y, line.length, line.color);
}

void display_draw_triangle(triangle_t triangle)
{
    display.fillTriangle(triangle.pos1.x, triangle.pos1.y, triangle.pos2.x, triangle.pos2.y, triangle.pos3.x, triangle.pos3.y, triangle.color);
}

/**
 * @brief This function will maped the uint8_t size value to one of the predefined Arial font size. 
 * its support size (between 8 and 14)
 * @param size, (uint8_t) size of the text. 
 * @return ILI9341_t3_font_t its will return a struct of type (ILI9341_t3_font_t) which contain number of values 
 * to display the text in Arial font type and in the text size that has been chosen.
 */
static ILI9341_t3_font_t get_arial_text_size(uint8_t size)
{
    if (size < MIN_TEXT_S)
        return Arial_8;
    else if (size > MAX_TEXT_S)
        return Arial_14;
    else
    {
        return available_text_size[size % MIN_TEXT_S];
    }
}
