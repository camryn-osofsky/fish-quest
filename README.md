# 🐟 Fish Quest

An underwater RPG built for an embedded system using the **ARM mbed LPC1768** microcontroller. Explore an ocean world, talk to NPCs, learn special attacks, defeat bosses, and reach the Chamber of Champions to win!

## 🎥 Demo

[![Fish Quest Demo](https://img.youtube.com/vi/edxA4Bwaslc/0.jpg)](https://youtu.be/edxA4Bwaslc)

## 🕹️ Gameplay Overview

Fish Quest is a top-down adventure game rendered on a uLCD screen. The player navigates an underwater map, interacts with characters, and fights enemies to collect a golden key and unlock the Chamber of Champions.

### Storyline
- Talk to the **first NPC** to receive a quest: defeat Pointy Pufferfish and bring back his hook
- Defeat **Pointy Pufferfish** to earn the hook and learn the **Bubble Blast** attack
- Complete the **side quest** with the second NPC (answer a trivia question!) to learn the **Seaweed Strike**
- Enter **Sinister Squid's lair** through the cave and defeat both bosses
- Collect the **golden key** and unlock the **Chamber of Champions** to win

### Enemies
| Enemy | Location | Points |
|-------|----------|--------|
| Pointy Pufferfish | Main map | +20 |
| Random Fish | Main map | -5 (penalty!) |
| Sinister Squid | Boss lair | +50 |
| Evil Eel | Boss lair | +30 |

> ⚠️ You have **2 minutes** to defeat the bosses once inside the lair!

## 🔧 Hardware Requirements

| Component | Details |
|-----------|---------|
| Microcontroller | mbed LPC1768 |
| Display | uLCD-4DGL LCD screen |
| Input | Navigation switch (5-way) |
| Input | 3 push buttons |

## 🎮 Controls

| Input | Action |
|-------|--------|
| Navigation Switch | Move up / down / left / right |
| Nav Switch Center | Teleport 5 tiles forward |
| Action Button (B1) | Talk to NPCs, interact with objects, unlock door |
| Attack Button (B3) | Attack nearby enemies and bosses |
| Menu Button (B2) | Open menu (shows stats, location, play time) |

## 📁 Project Structure

```
fish-quest/
├── main.cpp          # Game loop, player logic, map initialization
├── graphics.cpp/.h   # Drawing functions for all map tiles and characters
├── map.cpp/.h        # Map data structure and item placement
├── hardware.cpp/.h   # Hardware initialization and input reading
├── speech.cpp/.h     # Speech bubble display functions
├── hash_table.cpp/.h # Hash table used for map storage
├── Nav_Switch.cpp/.h # Navigation switch driver
├── wave_player.cpp/.h# Audio wave file player
├── globals.h         # Global hardware objects and constants
├── mbed_config.h     # mbed configuration
└── P2-2.bin          # Compiled binary for flashing to mbed
```

## 🚀 Running the Game

1. Connect the mbed LPC1768 with the uLCD screen and nav switch wired up
2. Flash `P2-2.bin` to the mbed by dragging it onto the mbed USB drive
3. Press reset on the mbed — the title screen will appear on the uLCD
4. Press the **Action Button** to begin your quest!

> To recompile from source, import the project into the [mbed online compiler](https://os.mbed.com/compiler/) and add the required libraries (`uLCD-4DGL`, `Nav_Switch`, `mbed`).

## 📊 Menu Stats

Press the **Menu Button** mid-game to view:
- Current location (x, y)
- Play time (in minutes)
- Points scored
- Bosses defeated
- Total enemies defeated

Press the **Action Button** to resume.

## Built With

- [mbed OS](https://os.mbed.com/) — embedded systems platform
- [uLCD-4DGL](https://os.mbed.com/users/4180_1/code/uLCD-4DGL-Smart-LCD/) — LCD display library
- C++
