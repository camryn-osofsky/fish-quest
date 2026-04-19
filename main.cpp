//=================================================================
// The main program file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


// Projet includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include <cstdio>
#include <math.h>

#define CITY_HIT_MARGIN 1
#define CITY_UPPER_BOUND (SIZE_Y-(LANDSCAPE_HEIGHT+MAX_BUILDING_HEIGHT))

#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2

// Helper function declarations
//void playSound(char* wav);
void gameOverScreen();
void update_status(int x, int y);
int lower_status_timer();
//void countdown();

Timer playTimer;
Timer bossTimer;

/////////////////////////
// Struct of Player 
/////////////////////////

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;            // Current locations
    int px, py;         // Previous locations
    bool has_key;       // flag when obtained the key
    bool game_solved;   // flag when game is complete
    bool talked_to_npc; // flag when you've talked to npc
    bool has_hook;      // flag when player has obtained puffer's hook
    bool learned_skill; // flag when player has learned Bubble Blast
    bool talked_to_npc2; // flag when player has spoken to second NPC
    bool passed_quiz;   // flag when player has succeeded in side quest
    bool learned_skill2; // flag when player has learned the second skill
    int points;         // keep track of # of player's points
    int bossesDefeated; // keep track of # bosses defeated
    int totalDefeated; // keep track of total # defeated
    bool usedKey;        // flag when player has "unlocked" door
    bool inChamber;     // flag when player is inside chamber

    //You will need to add more flags as needed

} Player;


/////////////////////////
// Get Action
/////////////////////////

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define ATTACK_BUTTON 3
#define GO_LEFT 4
#define GO_RIGHT 5
#define GO_UP 6
#define GO_DOWN 7
#define TELEPORT 8


int get_action(GameInputs inputs)
{
    //******************
    // TODO: Implement
    //******************
    
    // 1. Check your action and menu button inputs and return the corresponding action value
    if (inputs.b1) {
        return ACTION_BUTTON;
    } else if (inputs.b2) {
        return MENU_BUTTON;
    } else if (inputs.b3) {
        return ATTACK_BUTTON;
    }
    // 2. Check for your navigation switch inputs and return the corresponding action value
    if (inputs.ns_left) {
        return GO_LEFT;
    } else if (inputs.ns_right) {
        return GO_RIGHT;
    } else if (inputs.ns_up) {
        return GO_UP;
    } else if (inputs.ns_down) {
        return GO_DOWN;
    } else if (inputs.ns_center) {
        return TELEPORT;
    }
    // If no button is pressed, just return no action value
    return NO_ACTION;
}


/////////////////////////
// Helper Functions:
/////////////////////////
// Feel free to define any helper functions here for update game

void update_status(int x, int y) {
    uLCD.textbackground_color(BLACK);
    uLCD.color(0xBFFF00);
    uLCD.locate(0, 0);
    uLCD.printf("X: %2d, Y: %2d", Player.x, Player.y);
}

int lower_status_timer() {
    bossTimer.start();
    uLCD.textbackground_color(BLACK);
    uLCD.color(0xBFFF00);
    uLCD.locate(0, 15);
    uLCD.printf("%3.1f minutes remain", ((120 - bossTimer.read())/60));
    if (bossTimer.read() >= 120) {
        bossTimer.stop();
        gameOverScreen();
        return GAME_OVER;
    }

}

// void countdown() {
//     if(get_active_map_index() == 1) {
//             bossTimer.start();
//             while ((10 - bossTimer.read()) >= 0) {
//                 lower_status_timer();
//                 pc.printf("%f\n", bossTimer.read());
//             }
//             bossTimer.stop();
//             gameOverScreen();
//     }
// }

/////////////////////////
// Update Game
/////////////////////////

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */

int update_game(int action)
{
    
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    MapItem* item;
    

    //******************
    // TODO: Implement
    //******************

    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.

    Map* map = get_active_map();
    int key;

    switch(action)
    {
        case GO_UP:
            //TODO: Implement
            //1. Check the item north of the player
            //2. Make sure to not walk through walls
            //3. If it is not a wall, the walk up by updating player's coordinates
            if (get_north(Player.x, Player.y)->type == WALL || get_north(Player.x, Player.y)->type == NPC || get_north(Player.x, Player.y)->type == ENEMY || get_north(Player.x, Player.y)->type == BOSS || get_north(Player.x, Player.y)->type == NPC2 || get_north(Player.x, Player.y)->type == FISH) { 
                break;
            } else if (get_north(Player.x, Player.y)->type == DOOR){
                if (Player.has_key == false) {
                    action_speech("You must get the", "golden key to");
                    action_speech("enter the Chamber", "of Champions...");
                } else if (Player.usedKey == false) {
                    action_speech("Use the action", "button to unlock");
                    action_speech("the Chamber", "of Champions!");
                } else {
                    break;
                }
            } else if (Player.inChamber == true) {
                Player.y -= 1;
                action_speech("You win!", "");
                gameOverScreen();
            }else {
                Player.y -= 1;
                update_status(Player.x, Player.y);
            }
            break;
            
        case GO_LEFT:
            //TODO: Implement
            if (get_west(Player.x, Player.y)->type == WALL || get_west(Player.x, Player.y)->type == NPC || get_west(Player.x, Player.y)->type == ENEMY || get_west(Player.x, Player.y)->type == BOSS || get_west(Player.x, Player.y)->type == NPC2 || get_west(Player.x, Player.y)->type == FISH) {
                break;
            } else {
                Player.x -= 1;
                update_status(Player.x, Player.y);
            }
            break;
            
        case GO_DOWN:
            //TODO: Implement
            if (get_south(Player.x, Player.y)->type == WALL || get_south(Player.x, Player.y)->type == NPC || get_south(Player.x, Player.y)->type == ENEMY || get_south(Player.x, Player.y)->type == BOSS || get_south(Player.x, Player.y)->type == NPC2 || get_south(Player.x, Player.y)->type == FISH) {
                break;
            } else {
                Player.y += 1;
                update_status(Player.x, Player.y);
            }
            break;
            
        case GO_RIGHT:
            //TODO: Implement
            if (get_east(Player.x, Player.y)->type == WALL || get_east(Player.x, Player.y)->type == NPC || get_east(Player.x, Player.y)->type == ENEMY || get_east(Player.x, Player.y)->type == BOSS || get_east(Player.x, Player.y)->type == NPC2 || get_east(Player.x, Player.y)->type == FISH) {
                break;
            } else {
                Player.x += 1;
                update_status(Player.x, Player.y);
            }
            break;
        
        case TELEPORT:
            //implement teleportation (cannot teleport into winner's chamber, cannot teleport through walls, cannot land on an enemy or an NPC)
            if ((get_here(Player.x + 1, Player.y)->type == WALL) || (get_here(Player.x + 2, Player.y)->type == WALL) || (get_here(Player.x + 3, Player.y)->type == WALL) || (get_here(Player.x + 4, Player.y)->type == WALL) || (get_here(Player.x + 5, Player.y)->type == WALL)) {
                teleport_speech("Cannot teleport", "to this location.");
                return FULL_DRAW;
            } else if (get_here(Player.x + 5, Player.y)->type == ENEMY || get_here(Player.x + 5, Player.y)->type == FISH || get_here(Player.x + 5, Player.y)->type == NPC || get_here(Player.x + 5, Player.y)->type == BOSS) {
                teleport_speech("Cannot teleport", "to this location.");
                return FULL_DRAW;
            } else {
                Player.x = Player.x + 5;
                update_status(Player.x, Player.y);
                return FULL_DRAW;
            }
            break;

        case ACTION_BUTTON:

            //******************
            // TODO: Implement
            //******************

            // 1. Check if near NPC, 
            //     - if so, mark the player has talked and give instructions on what to do
            //     - if the game is solved (defeated Buzz), give the player the key
            //     - return FULL_DRAW to redraw the scene
            if ((get_east(Player.x, Player.y)->type == NPC) || (get_west(Player.x, Player.y)->type == NPC) || (get_north(Player.x, Player.y)->type == NPC) || (get_south(Player.x, Player.y)->type == NPC)) {
                if (Player.talked_to_npc == false) {
                    Player.talked_to_npc = true;
                    action_speech("If you wish to", "defeat Sinister");
                    action_speech("Squid, you must", "first learn the");
                    action_speech("Bubble Blast.", "I will teach you");
                    action_speech("but only if you", "bring me the");
                    action_speech("hook of my enemy,", "Pointy Pufferfish");
                    add_npc_change1(10, 5);
                } else if (Player.talked_to_npc && Player.has_hook) {
                    Player.learned_skill = true;    
                    action_speech("You have defeated", "Pointy Pufferfish");
                    action_speech("and brought me", "his hook. You now");
                    action_speech("have the ability", "of the Bubble");
                    action_speech("Blast! Venture", "into Sinister");
                    action_speech("Squid's lair to", "complete your");
                    action_speech("quest. Good luck!", "Remember, if you");
                    action_speech("wish to defeat", "Evil Eel, you");
                    action_speech("must complete the", "side quest...");
                    Player.has_hook = false;
                    add_npc_change2(10, 5);
                } else if (Player.has_key == true) {
                    Player.game_solved = true;
                    action_speech("Congratulations!", "You now have the");
                    action_speech("key to the", "Chamber of");
                    action_speech("Champions. Find", "the door to gain");
                    action_speech("entry to the", "Chamber.");
                    add_npc_change3(10, 5);
                }
                return FULL_DRAW;
            }

            if ((get_east(Player.x, Player.y)->type == NPC2) || (get_west(Player.x, Player.y)->type == NPC2) || (get_north(Player.x, Player.y)->type == NPC2) || (get_south(Player.x, Player.y)->type == NPC2)) {
                if (Player.talked_to_npc2 == false) {
                    Player.talked_to_npc2 = true;
                    action_speech("If you wish to", "defeat Evil Eel,");
                    action_speech("you must first", "learn the Seaweed");
                    action_speech("Strike. I will", "teach you but");
                    action_speech("only if you pass", "my test.");
                    uLCD.background_color(BLACK);
                    uLCD.textbackground_color(BLACK);
                    uLCD.color(0xBFFF00);
                    uLCD.cls();
                    uLCD.text_width(2); //2X size text
                    uLCD.text_height(2);
                    uLCD.locate(1, 0);
                    uLCD.printf("MY TEST:");

                    uLCD.text_width(1);
                    uLCD.text_height(1);
                    uLCD.locate(0, 2);
                    uLCD.printf("What kind of fish");
                    uLCD.locate(0, 3);
                    uLCD.printf("is the slowest?");


                    uLCD.locate(1, 5);
                    uLCD.printf("Action Button:");
                    uLCD.locate(1, 6);
                    uLCD.printf("Seahorse");

                    uLCD.locate(1, 8);
                    uLCD.printf("Attack Button:");
                    uLCD.locate(1, 9);
                    uLCD.printf("Blobfish");

                    uLCD.locate(1, 11);
                    uLCD.printf("Menu Button:");
                    uLCD.locate(1, 12);
                    uLCD.printf("Koi Fish");
                    
                    uLCD.locate(0, 14);
                    uLCD.printf("Press the correct");
                    uLCD.locate(0, 15);
                    uLCD.printf("button or YOU LOSE");

                    while (button1) {
                        if (!button2) {
                            gameOverScreen();
                            return GAME_OVER;
                        } else if (!button3) {
                            gameOverScreen();
                            return GAME_OVER;
                        }
                        continue;
                    }
                    uLCD.cls();
                    Player.passed_quiz = true;
                    add_npc2_change(20, 20);
                    return FULL_DRAW; 
                    break;

                } else if ((Player.talked_to_npc2 == true) && (Player.passed_quiz == true)){
                    Player.learned_skill2 = true;     
                    action_speech("You have passed", "my test. You now");
                    action_speech("have the ability", "of the Seaweed");
                    action_speech("Strike! Venture", "into Sinister");
                    action_speech("Squid's lair to", "complete your");
                    action_speech("quest.", "Good luck!");
                    return FULL_DRAW;
                }
                return FULL_DRAW;
            }

            // 2. Check if near a door
            //    - if the player has the key, you win the game
            //    - if not, show speech bubbles that the play needs to get the key 
            //     -return FULL_DRAW to redraw the scene
            if ((get_east(Player.x, Player.y)->type == DOOR) || (get_west(Player.x, Player.y)->type == DOOR) || (get_north(Player.x, Player.y)->type == DOOR) || (get_south(Player.x, Player.y)->type == DOOR)) {
                if (Player.game_solved == true && Player.points >= 60) {
                    action_speech("You have unlocked", "the Chamber");
                    action_speech("of Champions!", "");
                    Player.usedKey =  true;
                    Player.y -= 2;
                    Player.inChamber = true;
                } else {
                    action_speech("You must have", "Sinister Squid's");
                    action_speech("golden key to", "unlock the");
                    action_speech("Chamber of", "Champions...");
                }
                return FULL_DRAW;
            }

            // 3. Check if on Pete's cave
            //    - if the player has talked to the npc, then start the speech bubble to fight buzz
            //    - set the players coordinates for the small map
            //    - and set the map to the small map
            //     -return FULL_DRAW to redraw the scene
            if (get_here(Player.x, Player.y)->type == CAVE) {
                if (Player.learned_skill) {
                    action_speech("Entering Sinister", "Squid and Evil");
                    action_speech("Eel's secret", "lair...");
                    action_speech("BEWARE!", "");
                    Player.x = Player.y = 1;
                    set_active_map(1);
                    //countdown();
                } else {
                    action_speech("You must learn", "the Bubble Blast");
                    action_speech("and the Seaweed", "Strike to be");
                    action_speech("ready to face", "Sinister");
                    action_speech("Squid", "and Evil Eel...");
                }
                return FULL_DRAW;
            }

            // 4. Check if on a stairs
            //    - if so, move the player the correct coordinates
            //    - and set the map back to the main big map
            //     -return FULL_DRAW to redraw the scene
            if (get_here(Player.x, Player.y)->type == STAIRS) {
                action_speech("Leaving Sinister", "Squid's lair...");
                set_active_map(0);
                bossTimer.stop();
                bossTimer.reset();
                Player.x = 5;// go to correct coordinates
                Player.y = 20;
                return FULL_DRAW;
            }
            
            // Feel free to add more functions as you make the game!
    
            break;
        
        case ATTACK_BUTTON:

            //******************
            // TODO: Implement
            //******************

            // 1. Check if near enemy, 
            //     - if so, mark enemy as defeated
            //      use speech bubbled to show what item was dropped
            //      update the player struct as needed
            //      if enemy is Pete, make sure that the right attack is being used
            //      If pete is defeated, update game as nescessary
            if ((get_east(Player.x, Player.y)->type == ENEMY) || (get_west(Player.x, Player.y)->type == ENEMY) || (get_north(Player.x, Player.y)->type == ENEMY) || (get_south(Player.x, Player.y)->type == ENEMY) || (get_here(Player.x, Player.y)->type == ENEMY)) {
                Player.has_hook = true;
                attack_speech("HOOK", "DROPPED!");
                add_slain_enemy(15, 10);
                Player.points += 20;
                Player.totalDefeated += 1;
            }

            if ((get_east(Player.x, Player.y)->type == FISH) || (get_west(Player.x, Player.y)->type == FISH) || (get_north(Player.x, Player.y)->type == FISH) || (get_south(Player.x, Player.y)->type == FISH) || (get_here(Player.x, Player.y)->type == FISH)) {
                attack_speech("SCALES", "DROPPED!");
                if (get_east(Player.x, Player.y)->type == FISH) {
                    add_slain_enemy(Player.x + 1, Player.y);
                } else if (get_west(Player.x, Player.y)->type == FISH) {
                    add_slain_enemy(Player.x - 1, Player.y);
                } else if (get_north(Player.x, Player.y)->type == FISH) {
                    add_slain_enemy(Player.x, Player.y - 1);
                } else if (get_south(Player.x, Player.y)->type == FISH) {
                    add_slain_enemy(Player.x, Player.y + 1);
                }
                attack_speech("Attacked", "unassuming fish.");
                attack_speech("-5 points.", "");
                Player.points -= 5;
                Player.totalDefeated += 1;
            }

            if ((get_east(Player.x, Player.y)->type == BOSS) || (get_west(Player.x, Player.y)->type == BOSS) || (get_north(Player.x, Player.y)->type == BOSS) || (get_south(Player.x, Player.y)->type == BOSS)) {
                attack_speech("Attack not strong", "enough...");
                Player.points -= 5;
            }

            if (get_here(Player.x, Player.y)->type == TILE) {
                attack_speech("Attack not strong", "enough...");
                Player.points -= 5;
            }

            if (get_here(Player.x, Player.y)->type == BLAST) {
                Player.has_key = true;
                attack_speech("BUBBLE BLAST!", "INK DROPPED!");
                attack_speech("Sinister Squid", "is defeated!");
                add_slain_enemy(6, 8);
                Player.points += 50;
                Player.bossesDefeated += 1;
                Player.totalDefeated += 1;
            }

            if (get_here(Player.x, Player.y)->type == STRIKE && Player.learned_skill2 == true) {
                attack_speech("SEAWEED STRIKE!", "BOLT DROPPED!");
                attack_speech("Evil Eel is", "defeated!");
                add_slain_enemy(10, 12);
                Player.points += 30;
                Player.bossesDefeated += 1;
            } else if (get_here(Player.x, Player.y)->type == STRIKE && Player.learned_skill2 == false) {
                attack_speech("You must complete", "the side quest to");
                attack_speech("unlock this", "attack...");
            }

            return FULL_DRAW;
            break;

        //***********
        // Add more cases as needed
        //***********
        case MENU_BUTTON:
            // basic printf demo = 16 by 18 characters on screen
            playTimer.stop();
            uLCD.background_color(BLACK);
            uLCD.textbackground_color(BLACK);
            uLCD.color(0xBFFF00);
            uLCD.cls();
            uLCD.text_width(2); //2X size text
            uLCD.text_height(2);
            uLCD.locate(2, 0);
            uLCD.printf("MENU:");
            uLCD.text_width(1);
            uLCD.text_height(1);
            uLCD.locate(0, 4);
            uLCD.printf("Play Time: %3.1f", (playTimer.read()/60)); //CREATE TIMER PLS
            uLCD.locate(0, 5);
            uLCD.printf("Location: (%d, %d)", Player.x, Player.y);
            uLCD.locate(0, 6);
            uLCD.printf("Points: %d", Player.points);
            uLCD.locate(0, 7);
            uLCD.printf("Bosses Defeated: %d", Player.bossesDefeated);
            uLCD.locate(0, 8);
            uLCD.printf("Total Defeated: %d", Player.totalDefeated);
            uLCD.text_width(1); //2X size text
            uLCD.text_height(1);
            uLCD.locate(3, 12);
            uLCD.printf("PRESS ACTION");
            uLCD.locate(4, 13);
            uLCD.printf("BUTTON TO");
            uLCD.locate(3, 14);
            uLCD.printf("RESUME GAME");
            
            while (button1) {
                continue;
            }
            playTimer.start();
            uLCD.cls();
           
            return FULL_DRAW;
            break;
   
    return NO_RESULT;
    }
}



/////////////////////////
// Draw Game
/////////////////////////

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            //if (init && i == 0 && j == 0) // Only draw the player on init
            if (i == 0 && j == 0) // always draw the player
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
                else if (curr_item && curr_item->type == CLEAR)
                {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}







/////////////////////////
// Map Intialization
/////////////////////////

// Important locations for all maps
int cb_loc[2] = {5,20}; //Location of the center of the cave


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    //Initialize and sets current map to the first map
    Map* map = set_active_map(0);

    /////////////////////////
    //Initial Environmnet
    /////////////////////////

    //Adding random plants
    pc.printf("Adding Plants!\r\n");
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
    }

    //Adding random bubbles
    pc.printf("Adding Bubbles!\r\n");
    for(int i = map_width() + 3; i < map_area(); i += 43)
    {
        add_bubbles(i % map_width(), i / map_width());
    }

    //Adding wall borders 
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    
    //Adding extra chamber borders 
    pc.printf("Add extra chamber\r\n");
    add_wall(30, 0, VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39, 0, VERTICAL, 10);
    add_door(33, 10, HORIZONTAL, 4);

    //Adding extra cave to Buzz's evil lair
    pc.printf("Add cave\r\n");
    add_cave(cb_loc[0],cb_loc[1],1,1,5,5);      //Cave is set as a 4x4 block to be bigger
    add_cave(cb_loc[0]+1,cb_loc[1],2,1,5,5);
    add_cave(cb_loc[0],cb_loc[1]+1,3,1,5,5);
    add_cave(cb_loc[0]+1,cb_loc[1]+1,4,1,5,5);

    pc.printf("Add enemy\r\n");
    add_puffer(15, 10);

    pc.printf("Initial environment completed\r\n");

    /////////////////////////////////
    // Characters and Items for the map
    /////////////////////////////////

    // Add NPC
    add_npc(10, 5);  //NPC is initialized to (x,y) = 10, 5. Feel free to move him around
    add_npc2(20, 20);

    // Add random fish
    add_fish(15, 15);
    add_fish(30, 30);
    add_fish(7, 2);
    add_fish(20, 10);
    add_fish(40, 7);
    add_fish(20, 25);
    add_fish(48, 10);
    add_fish(27, 9);
    add_fish(8, 30);
    
    //***********************************
    // TODO: Implement As Needed
    //***********************************

    //Add any extra characters/items here for your project


    //Prints out map
    print_map();
}


void init_small_map()
{
    //Sets current map to the second map
    set_active_map(1);


    //***********************************
    // TODO: Implement 
    //***********************************

    // 1. Add walls to the smaller map.
    //    Set the dimensions to be 16x16  <-- you may change to your liking, but must be smaller than the main map
    //    Hint: See how walls are initialized in the main map
    pc.printf("Adding walls!\r\n");     // my way makes the walls close in more, but doesnt change the acutal map dimensions
    add_wall(0,                 0,                  HORIZONTAL, map_width());
    add_wall(0,                 map_height()-1,     HORIZONTAL, map_width());
    add_wall(0,                 0,                  VERTICAL,   map_height());
    add_wall(map_width()-1,     0,                  VERTICAL,   map_height());
    // 2. Add a way to access your special attacks either here or in update_game() or anywhere you feel would be the best
    pc.printf("Adding attack blocks!\r\n");
    add_bolt(3, 3);
    add_wave(6, 3);
    add_bubbleBlast(9, 3);
    add_seaweed(12, 3);
    // 3. Add Boss in the map
    pc.printf("Adding boss!\r\n");
    add_squid(6, 8);
    add_eel(10, 12);
    // You may add any extra characters/items here for your project
    uLCD.locate(0, 6);
    uLCD.printf("Time: %3.1f", (bossTimer.read()/60));

    // Add stairs back to main (map 0)
    add_stairs(4, 6, 0, cb_loc[0], cb_loc[1]);
    
}

void init_quiz_map()
{
    //Sets current map to the second map
    //set_active_map(2);


    //***********************************
    // TODO: Implement 
    //***********************************

    // 1. Add walls to the smaller map.
    //    Set the dimensions to be 16x16  <-- you may change to your liking, but must be smaller than the main map
    //    Hint: See how walls are initialized in the main map
    pc.printf("Adding walls!\r\n");     // my way makes the walls close in more, but doesnt change the acutal map dimensions
    add_wall(0,                 0,                  HORIZONTAL, map_width());
    add_wall(0,                 map_height()-1,     HORIZONTAL, map_width());
    add_wall(0,                 0,                  VERTICAL,   map_height());
    add_wall(map_width()-1,     0,                  VERTICAL,   map_height());
    //ADD INFO/RESTART ETC
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");


    //uLCD.filled_rectangle(64, 64, 74, 74, RED); //DELETE OR COMMENT THIS LINE  <-- It is a temporary indicator that your LCD is working before you implement your code


    // Initialize the maps
    maps_init();
    init_main_map();
    init_small_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.has_key = false;
    Player.game_solved = false;
    Player.talked_to_npc = false;
    Player.has_hook = false;
    Player.learned_skill = false;
    Player.talked_to_npc2 = false;
    Player.passed_quiz = false;
    Player.learned_skill2 = false;
    Player.points = 0;
    Player.bossesDefeated = 0;
    Player.totalDefeated = 0;

    //**************
    // START SCREEN
    //**************

    // NEED TO IMPLEMENT: WAY TO VIEW CONTROLS!!!! 

    uLCD.background_color(0x007382);
    uLCD.cls();
    uLCD.textbackground_color(0x007382);
    uLCD.text_width(1);
    uLCD.text_height(2);
    uLCD.color(0X800080);
    uLCD.text_bold(ON);
    uLCD.locate(4, 1);
    uLCD.printf("FISH QUEST");
    //uLCD.locate(2, 2);
    //uLCD.printf("QUEST");
    uLCD.text_bold(OFF);
    uLCD.text_width(1);
    uLCD.text_height(1);
    draw_start_fish(110, 80);
    draw_start_fish(90, 80);
    draw_start_fish(70, 80);
    draw_start_fish(50, 80);
    draw_start_fish(30, 80);
    draw_start_fish(10, 80);
    uLCD.locate(3, 12);
    uLCD.printf("PRESS ACTION");
    uLCD.locate(4, 13);
    uLCD.printf("BUTTON TO");
    uLCD.locate(3, 14);
    uLCD.printf("BEGIN QUEST");
    while (button1) {
        uLCD.filled_rectangle(0, 30, 200, 70, 0x007382);
        uLCD.locate(2, 5);
        uLCD.text_bold(ON);
        uLCD.printf("ACTION BUTTON:");
        uLCD.text_bold(OFF);
        uLCD.locate(2, 6);
        uLCD.printf("Talk to NPC,");
        uLCD.locate(0, 7);
        uLCD.printf("non-attack actions");
        wait(1);
        uLCD.filled_rectangle(0, 30, 200, 70, 0x007382);
        uLCD.locate(2, 5);
        uLCD.text_bold(ON);
        uLCD.printf("ATTACK BUTTON:");
        uLCD.locate(1, 6);
        uLCD.text_bold(OFF);
        uLCD.printf("Attack items on");
        uLCD.locate(4, 7);
        uLCD.printf("the map");
        wait(1);
        uLCD.filled_rectangle(0, 30, 200, 70, 0x007382);
        uLCD.locate(2, 5);
        uLCD.text_bold(ON);
        uLCD.printf("MENU BUTTON:");
        uLCD.locate(2, 6);
        uLCD.text_bold(OFF);
        uLCD.printf("Display menu");
        wait(1);
        continue;
    }
    uLCD.background_color(BLACK);
    uLCD.cls();

    // Initial drawing
    draw_game(true);

    // Main game loop
    while(1)
    {

        ////////////////////////////////
        // TODO: Implement 
        ////////////////////////////////

        // Timer to measure game update speed
        Timer t; t.start();
        playTimer.start();
        
        // Actually do the game update:
        // 1. Read inputs        
        GameInputs ips = read_inputs();
        // 2. Determine action (get_action)       
        int action = get_action(ips);
        // 3. Update game (update_game)
        int result = update_game(action);  // Set this variable "result" for the resulting state after update game

        // 3b. Check for game over based on update game result

        // 4. Draw screen to uLCD
        bool full_draw = false;
        if (result == FULL_DRAW) full_draw = true;
        draw_game(full_draw);
        
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);

        if(get_active_map_index() == 1) {
            lower_status_timer();
        }

    }

}




/////////////////////////////
//Advanced Features
/////////////////////////////

// Plays a wavfile
/*void playSound(char* wav)
{
    //open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");
    
    if(wave_file != NULL) 
    {
        printf("File opened successfully\n");

        //play wav file
        printf("Sound playing...\n");
        waver.play(wave_file);
    
        //close wav file
        printf("Sound stopped...\n");
        fclose(wave_file);
        return;
    }
    
    printf("Could not open file for reading - %s\n", wav);
    return;
} */

void gameOverScreen() {
    uLCD.background_color(BLACK);
    uLCD.cls();
    uLCD.text_width(2); //2X size text
    uLCD.text_height(2);
    uLCD.locate(3, 1);
    uLCD.printf("GAME");
    uLCD.locate(3, 3);
    uLCD.printf("OVER");
    uLCD.text_width(1);
    uLCD.text_height(1);
    uLCD.locate(3, 12);
    uLCD.printf("PRESS ACTION");
    uLCD.locate(4, 13);
    uLCD.printf("BUTTON TO");
    uLCD.locate(3, 14);
    uLCD.printf("RESTART GAME");
    while (button1) {
        continue;
    }
    // maps_init();
    init_main_map();
    init_small_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.has_key = false;
    Player.game_solved = false;
    Player.talked_to_npc = false;
    Player.has_hook = false;
    Player.learned_skill = false;
    Player.talked_to_npc2 = false;
    Player.passed_quiz = false;
    Player.learned_skill2 = false;
    Player.points = 0;
    Player.bossesDefeated = 0;
    Player.totalDefeated = 0;
    Player.usedKey = false;
    Player.inChamber = false;

    playTimer.reset();

    //**************
    // START SCREEN
    //**************

    // NEED TO IMPLEMENT: WAY TO VIEW CONTROLS!!!! 

    uLCD.background_color(0x007382);
    uLCD.cls();
    uLCD.textbackground_color(0x007382);
    uLCD.text_width(1);
    uLCD.text_height(2);
    uLCD.color(0X800080);
    uLCD.text_bold(ON);
    uLCD.locate(4, 1);
    uLCD.printf("FISH QUEST");
    //uLCD.locate(2, 2);
    //uLCD.printf("QUEST");
    uLCD.text_bold(OFF);
    uLCD.text_width(1);
    uLCD.text_height(1);
    draw_start_fish(110, 80);
    draw_start_fish(90, 80);
    draw_start_fish(70, 80);
    draw_start_fish(50, 80);
    draw_start_fish(30, 80);
    draw_start_fish(10, 80);
    uLCD.locate(3, 12);
    uLCD.printf("PRESS ACTION");
    uLCD.locate(4, 13);
    uLCD.printf("BUTTON TO");
    uLCD.locate(3, 14);
    uLCD.printf("BEGIN QUEST");
    while (button1) {
        uLCD.filled_rectangle(0, 30, 200, 70, 0x007382);
        uLCD.locate(2, 5);
        uLCD.text_bold(ON);
        uLCD.printf("ACTION BUTTON:");
        uLCD.text_bold(OFF);
        uLCD.locate(2, 6);
        uLCD.printf("Talk to NPC,");
        uLCD.locate(0, 7);
        uLCD.printf("non-attack actions");
        wait(1);
        uLCD.filled_rectangle(0, 30, 200, 70, 0x007382);
        uLCD.locate(2, 5);
        uLCD.text_bold(ON);
        uLCD.printf("ATTACK BUTTON:");
        uLCD.locate(1, 6);
        uLCD.text_bold(OFF);
        uLCD.printf("Attack items on");
        uLCD.locate(4, 7);
        uLCD.printf("the map");
        wait(1);
        uLCD.filled_rectangle(0, 30, 200, 70, 0x007382);
        uLCD.locate(2, 5);
        uLCD.text_bold(ON);
        uLCD.printf("MENU BUTTON:");
        uLCD.locate(2, 6);
        uLCD.text_bold(OFF);
        uLCD.printf("Display menu");
        wait(1);
        continue;
    }
    uLCD.background_color(BLACK);
    uLCD.cls();

    // Initial drawing
    draw_game(true);

    while(1)
    {

        ////////////////////////////////
        // TODO: Implement 
        ////////////////////////////////

        // Timer to measure game update speed
        Timer t; t.start();
        playTimer.start();
        
        // Actually do the game update:
        // 1. Read inputs        
        GameInputs ips = read_inputs();
        // 2. Determine action (get_action)       
        int action = get_action(ips);
        // 3. Update game (update_game)
        int result = update_game(action);  // Set this variable "result" for the resulting state after update game

        // 3b. Check for game over based on update game result

        // 4. Draw screen to uLCD
        bool full_draw = false;
        if (result == FULL_DRAW) full_draw = true;
        draw_game(full_draw);
        
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);

        if(get_active_map_index() == 1) {
            lower_status_timer();
        }

    }
}
