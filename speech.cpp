//=================================================================
// The speech class file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#include "speech.h"

#include "globals.h"
#include "hardware.h"

///////////////////////////////
//Static function declarations
///////////////////////////////
/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);


///////////////////////////////
//Drawing function declarations
///////////////////////////////


//***
// HINT: for this function and below: Check out the ULCD demo in the docs to see what
// uLCD methods you can use to draw graphics 

//Link: https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/
//***


void draw_speech_bubble()
{
    //****************
    // TODO: Implement
    //****************
    
    //1. Draw a speech bubble at the bottom of the screen 
    // Hint: You can stack ULCD rectangles to make a border

    uLCD.filled_rectangle(0, 100, 200, 200, 0x000000);
    
}

void erase_speech_bubble()
{
    //****************
    // TODO: Implement
    //****************
    
    //1. Erase the speech bubble at the bottom of the screen
    // Hint: You can overwrite the bubble with a empty (black) one
    uLCD.filled_rectangle(0, 100, 200, 200, 0x000000);
}

void draw_speech_line(const char* line, int which)
{
    //****************
    // TODO: Implement
    //****************
    //1. Set the location which line of text will go the uLCD
    // Hint: Change the y coordinate based on which line (top or bottom)
    uLCD.color(0xBFFF00);
    uLCD.textbackground_color(0x000000);
    if (which == 1) {
        uLCD.locate(0, 13);
    } else if (which == 2) {
        uLCD.locate(0, 14);
    }
    //2. For each character in the text, write it to uLCD
    for (int i = 0; line[i] && i < 17; i++) //We can simplify by limiting each line to 17 char
    {
        //TODO: Write the character
        uLCD.printf("%c", line[i]);
        wait_ms(30); 
    }
}

void action_bubble_wait()
{
    //****************
    // TODO: Implement
    //****************
    
    // 1. Keep waiting until the action button is pressed 
    //   Hint: What type of loop can we use to stay until a condition is met?
    while (button1) {
        continue;
    }
    // go to next line or erase bubble
    erase_speech_bubble();

}

void attack_bubble_wait()
{
    while (!navs.left() && !navs.right() && !navs.up() && !navs.down() && !navs.center()) {
        continue;
    }
    erase_speech_bubble();
}

void teleport_speech(const char* line1, const char* line2) 
{
    //****************
    // TODO: Implement
    //****************
    // 1. Place the two lines into an array of lines
    const char* lines[2];
    lines[0] = line1;
    lines[1] = line2;

    // 2. Pass in to long_speech with the corresponding number of lines
    long_teleport_speech(lines, 2);
}

void action_speech(const char* line1, const char* line2)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Place the two lines into an array of lines
    const char* lines[2];
    lines[0] = line1;
    lines[1] = line2;

    // 2. Pass in to long_speech with the corresponding number of lines
    long_action_speech(lines, 2);
}

void attack_speech(const char* line1, const char* line2)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Place the two lines into an array of lines
    const char* lines[2];
    lines[0] = line1;
    lines[1] = line2;

    // 2. Pass in to long_speech with the corresponding number of lines
    long_attack_speech(lines, 2);
}


void long_action_speech(const char* lines[], int n)
{

    //****************
    // TODO: Implement
    //****************
    //1. Create a speech bubble
    draw_speech_bubble();
    
    //2. For each lines, display only two lines at a time
    //   If two lines are displayed, make sure to wait (call the wait function)

    draw_speech_line(lines[0], 1);
    draw_speech_line(lines[1], 2);
    action_bubble_wait();
    
    //3. Erase the speech bubble when you are done -> done at the top
    erase_speech_bubble();
}

void long_attack_speech(const char* lines[], int n)
{

    //****************
    // TODO: Implement
    //****************
    //1. Create a speech bubble
    draw_speech_bubble();
    
    //2. For each lines, display only two lines at a time
    //   If two lines are displayed, make sure to wait (call the wait function)

    draw_speech_line(lines[0], 1);
    draw_speech_line(lines[1], 2);
    attack_bubble_wait();
    
    //3. Erase the speech bubble when you are done -> done at the top
    erase_speech_bubble();
}

void long_teleport_speech(const char* lines[], int n)
{

    //****************
    // TODO: Implement
    //****************
    //1. Create a speech bubble
    draw_speech_bubble();
    
    //2. For each lines, display only two lines at a time
    //   If two lines are displayed, make sure to wait (call the wait function)

    draw_speech_line(lines[0], 1);
    draw_speech_line(lines[1], 2);
    attack_bubble_wait();
    
    //3. Erase the speech bubble when you are done -> done at the top
    erase_speech_bubble();
}
