// ============================================
// The graphics header file
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#ifndef GRAPHICS_H
#define GRAPHICS_H

/**
 * Takes a string image and draws it to the screen. The string is 121 characters
 * long, and represents an 11x11 tile in row-major ordering (across, then down,
 * like a regular multi-dimensional array). The available colors are:
 *      R = Red
 *      Y = Yellow
 *      G = Green
 *      D = Brown ("dirt")
 *      P = Purple
 *      3 = Gray
 *      B = Black
 *      S = SquidLair
 *      O = Orange
 *      Any other character is underwater color
 * More colors can be easily added by following the pattern already given.
 */
void draw_img(int u, int v, const char* img);

/**
 * Draws the player. This depends on the player state, so it is not a DrawFunc.
 */
void draw_player(int u, int v, int key);

/**
 * Draw the upper status bar.
 */
void draw_upper_status();

/**
 * Draw the lower status bar.
 */ 
void draw_lower_status();

/**
 * Draw the border for the map.
 */
void draw_border();


/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */
void draw_nothing(int u, int v);
void draw_wall(int u, int v);
void draw_plant(int u, int v);
void draw_mud(int u, int v);
void draw_door(int u, int v);
void draw_npc(int u, int v);
void draw_npc2(int u, int v);
void draw_stairs(int u, int v);
void draw_bubbles(int u, int v);
void draw_cave1(int u, int v);
void draw_cave2(int u, int v);
void draw_cave3(int u, int v);
void draw_cave4(int u, int v);
void draw_puffer(int u, int v);
void draw_squid(int u, int v);
void draw_bolt(int u, int v);
void draw_wave(int u, int v);
void draw_bubbleBlast(int u, int v);
void draw_fish(int u, int v);
void draw_seaweed(int u, int v);
void draw_eel(int u, int v);
void draw_npc_change1(int u, int v);
void draw_npc_change2(int u, int v);
void draw_npc_change3(int u, int v);
void draw_start_fish(int u, int v);
void draw_npc2_change(int u, int v);
#endif // GRAPHICS_H
