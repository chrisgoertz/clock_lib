/**
 * @file clock.c
 * @author Christian Goertz (chris.goertz@gmx.de)
 * @brief
 * @version 0.1
 * @date 2024-02-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "clock.h"
#include "../ssd1306/ssd1306.h"
#include "time_utils.h"
#include <math.h>

Clock::Clock(SSD1306_VERTEX center, uint8_t radius, SSD1306_COLOR color) {
  this->center = center;
  this->radius = radius;
  this->color = color;
  this->need_redraw = true;
}
Clock::Clock():Clock{
  SSD1306_VERTEX(64,64), 63, White
}{};

void Clock::draw() {
  if (this->need_redraw) {
    drawFace();
    drawHoursHand();
    drawMinutesHand();
    drawSecondsHand();
  }
}

void Clock::drawFace() {
  /* prequesites */
  uint8_t steps = GFX_CLOCK_DEGREE_STEPS;
  SSD1306_VERTEX inner_point = {0, 0};
  SSD1306_VERTEX outer_point = {0, 0};
  const uint32_t r = radius - GFX_CLOCK_HOUR_MARK_GAP;
  /* clear clock face area out */
  ssd1306_FillCircle(this->center.x, this->center.y, this->radius, Black);

  for (uint16_t pos = 0; pos <= 60; ++pos) {
    float angle = pos * GFX_CLOCK_MINUTE_MARK_SPACING; // 360 Grad / 12 Stunden
                                                       // = 30 Grad pro Stunde
    float angle_rad =
        angle * (MATH_PI / GFX_PI_IN_DEGREES); // Umrechnung in Bogenmaß
    float cos = cosf(angle_rad);
    float sin = sinf(angle_rad);
    float mark_factor = 0.0f;
    // calculate coords for hour marks
    if (pos % 5) {
      mark_factor = GFX_CLOCK_MINUTE_MARK_FACTOR;
    } else {
      mark_factor = GFX_CLOCK_HOUR_MARK_FACTOR;
    }

    inner_point.x = static_cast<uint8_t>(round(
      (cos * (static_cast<float>(r) * mark_factor) + this->center.x)));
    inner_point.y = static_cast<uint8_t>(round(
      (sin * (static_cast<float>(r) * mark_factor) + this->center.y)));
    outer_point.x = static_cast<uint8_t>(round((cos * static_cast<float>(r) + this->center.x)));
    outer_point.y = static_cast<uint8_t>(round((sin * static_cast<float>(r) + this->center.y)));
    // Draw the hour marks
    ssd1306_Line(inner_point.x, inner_point.y, outer_point.x, outer_point.y,
                 White);
  }

  // Draw outer circle
  ssd1306_DrawCircle(this->center.x, this->center.y, this->radius, this->color);
}

/*******************************************************************************/
void Clock::drawHoursHand() {
  SSD1306_VERTEX center_point = {0, 0};
  SSD1306_VERTEX end_point = {0, 0};

  float angle =
      ((this->time.hours + GFX_CLOCK_HOUR_OFFSET) % 12) * GFX_CLOCK_HOUR_MARK_SPACING;
  float angle_rad = angle * (MATH_PI / GFX_PI_IN_DEGREES); // calculate radiant
  float cos = cosf(angle_rad);
  float sin = sinf(angle_rad);

  end_point.x = static_cast<uint8_t>(round(
    (cos * (static_cast<float>(this->radius) * (.6f - GFX_CLOCK_HANDS_GAP)) + this->center.x)));
  end_point.y = static_cast<uint8_t>(round(
    (sin * (static_cast<float>(this->radius) * (.6f - GFX_CLOCK_HANDS_GAP)) + this->center.y)));
  // draw the clock hand
  ssd1306_Line(this->center.x, this->center.y, end_point.x, end_point.y, this->color);
}

/*******************************************************************************/
void Clock::drawMinutesHand() {
  SSD1306_VERTEX end_point = {0, 0};

  float angle = ((this->time.minutes + GFX_CLOCK_MINUTE_OFFSET) % 60) *
                GFX_CLOCK_MINUTE_MARK_SPACING;
  float angle_rad = angle * (MATH_PI / GFX_PI_IN_DEGREES); // calculate radiant
  float cos = cosf(angle_rad);
  float sin = sinf(angle_rad);

  end_point.x = static_cast<uint8_t>(round(
    (cos * (static_cast<float>(this->radius) * (.9f - GFX_CLOCK_HANDS_GAP)) + this->center.x)));
  end_point.y = static_cast<uint8_t>(round(
    (sin * (static_cast<float>(this->radius) * (.9f - GFX_CLOCK_HANDS_GAP)) + this->center.y)));
  // draw the clock hand
  ssd1306_Line(this->center.x, this->center.y, end_point.x, end_point.y, this->color);
}

void Clock::drawSecondsHand() {
  SSD1306_VERTEX end_point = {0, 0};

  float angle = ((this->time.seconds + GFX_CLOCK_SECOND_OFFSET) % 60) *
                GFX_CLOCK_SECOND_MARK_SPACING;
  float angle_rad = static_cast<float>(
      angle * static_cast<float>(MATH_PI / GFX_PI_IN_DEGREES)); // calculate radiant
  float cos = cosf(angle_rad);
  float sin = sinf(angle_rad);

  end_point.x = static_cast<uint8_t>(
      round((cos * static_cast<float>(this->radius) + this->center.x)));
  end_point.y = static_cast<uint8_t>(
      round((sin * static_cast<float>(this->radius) + this->center.y)));

  // draw the clock hand
  ssd1306_Line(this->center.x, this->center.y, end_point.x, end_point.y, this->color);
}

void Clock::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds) {
  
  this->time.hours = hours > TIME_UTILS_HOURS_MAX ? TIME_UTILS_MIN : hours;
  this->time.minutes = minutes > TIME_UTILS_MINUTES_MAX ? TIME_UTILS_MIN: minutes;
  this->time.seconds = seconds > TIME_UTILS_SECONDS_MAX ? TIME_UTILS_MIN: seconds;
  this->time.milliseconds = milliseconds > TIME_UTILS_MILLISECONDS_MAX ? TIME_UTILS_MIN : milliseconds;
}

void Clock::addTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds){

}

void Clock::increment() {
  time_util_increment_seconds(&(this->time));
  // this->time.seconds++;
}
