#pragma once

#include <stdint.h>

/**
 @return 0: success
 @param[in] width
 @param[in] height
 @param[in] depth
 @param[in] clock_MHz
 */
int InitSdk(int width, int height, int depth, int clock_MHz);

/**
 Specify the color of each LED.
 Just by calling this function, drawing data is not transferred to 3D LED CUBE.
 @param[in] x 0 <= x < CUBE_WIDTH
 @param[in] y 0 <= y < CUBE_HEIGHT
 @param[in] z 0 <= z < CUBE_DEPTH
 @param[in] rgb 0x000000 <= rgb <= 0xFFFFFF
*/
void SetLed(int x, int y, int z, int rgb);

/**
 Clear all LED.
 Just by calling this function, drawing data is not transferred to 3D LED CUBE.
*/
void Clear(void);

/**
 Show 3D LED simulator window and transfer color of each LED to 3D LED CUBE.
*/
void Show(void);

/**
 Wait for the specified time.
 This function stores the time waiting for the previous time,
 taking into consideration the processing delay.
*/
void Wait(int ms);

void DrawAll(uint8_t red, uint8_t green, uint8_t blue);
