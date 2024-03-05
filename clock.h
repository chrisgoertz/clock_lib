/**
 * @file clock.h
 * @author Christian Görtz (chris.goertz@gmx.de)
 * @brief My implementation for drawing a clock on an ssd1306 oled display.
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __NCDF_CLOCK_H__
#define __NCDF_CLOCK_H__


#include "../ssd1306/ssd1306.h"
#include "time_utils.h"


#define MATH_PI (3.141592653589793)

#define GFX_CLOCK_MAX_DEGREES                       (360-1)
#define GFX_CLOCK_DEGREE_STEPS                      (GFX_CLOCK_MAX_DEGREES / 12)
#define GFX_CLOCK_HANDS_GAP                         (0.2f)
#define GFX_CLOCK_HOUR_MARK_GAP                     (3)
#define GFX_CLOCK_MINUTE_MARK_GAP                   (1)
#define GFX_CLOCK_HOUR_MARK_FACTOR                  (0.8f)
#define GFX_CLOCK_MINUTE_MARK_FACTOR                (0.95f)
#define GFX_CLOCK_HOUR_MARK_SPACING                 (30.0f)
#define GFX_CLOCK_MINUTE_MARK_SPACING               (6.0f)
#define GFX_CLOCK_SECOND_MARK_SPACING               (GFX_CLOCK_MINUTE_MARK_SPACING)
#define GFX_PI_IN_DEGREES                           (180.0f)
#define GFX_CLOCK_HOUR_OFFSET                       (9)
#define GFX_CLOCK_MINUTE_OFFSET                     (45)
#define GFX_CLOCK_SECOND_OFFSET                     (GFX_CLOCK_MINUTE_OFFSET)

class Clock{
    
    private:
    TIME_UTIL_t time;
    SSD1306_VERTEX center;
    uint32_t radius;
    SSD1306_COLOR color;
    bool need_redraw;

    void drawFace();
    void drawHoursHand();
    void drawMinutesHand();
    void drawSecondsHand();
    
    public:
    Clock();
    Clock(SSD1306_VERTEX center, uint8_t radius, SSD1306_COLOR color);
    void draw();
    void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);
    void addTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);
    void increment();
};



#endif
