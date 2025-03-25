/***************************************************************************

 game.c
 
 RocMan - PacMan for junk led matrix

 Controller: STC8A8K64D4 @ breakout
 
 12.12.2024  - First version

 ***************************************************************************/

#define __STC8__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <datatypes.h>
#include "stc8a8.h"
#include "main.h"
#include "matrix.h"
#include "game.h"
#include "sound.h"
#include "ir_rec.h"
#include "eeprom.h"
#include "graphdata.h"
#include "font.h"

/* =====================================================================
------------------------ Constants & macros ------------------------- */

// Colors
#define BLANK           0
#define MAZE_COLOR      10
#define MAZE_BLUE       5
#define DOT_COLOR       11
#define DOOR_COLOR      12
#define TEXT_COLOR      3       // red
#define NUMBER_COLOR    2       // white

// Directions
#define DIR_UP          0x01
#define DIR_DOWN        0x02
#define DIR_RIGHT       0x04
#define DIR_LEFT        0x08

// Ghost modes
#define MODE_BOUNCE     0       // bouncing at home
#define MODE_LEAVE      1       // leaving home
#define MODE_OUT        2       // out of home, searching current target
#define MODE_EYES       3       // eyes returning home
#define MODE_ENTER      4       // eyes entering home
#define MODE_EATEN      5       // pacman just ate ghost

// Ghost flags
#define FLAG_SLOW       0x01    // ghost flag: slow speed
#define FLAG_BLUE       0x02    // ghost flag: blue mode
#define FLAG_FLASH      0x04    // ghost flag: flashing mode
#define FLAG_FRIGHTENED 0x06    // ghost flag: blue or flashing

// Game states
#define GAME_ATTRACT    0       // attract mode
#define GAME_START      1       // starting: READY-text, jingle
#define GAME_RUN        2       // game running
#define GAME_LEVEL_UP   3       // level done, change to next 
#define GAME_CAPTURE    4       // Pacman captured a blue ghost
#define GAME_DIE        5       // Pacman dies, ghost got it
#define GAME_RESTART    6       // short pause after DIE
#define GAME_OVER       7       // game over, all lives used

// Fruit states
#define FRUIT_INITIAL   0
#define FRUIT_SHOW      1
#define FRUIT_USED      2

// Dot types
#define DOT_NONE        0
#define DOT_NORM        1
#define DOT_PILL        2

// Ghosts frightening cycles
#define NOT_FRIGHTENED   0
#define FRIGHTENED_BLUE  1
#define FRIGHTENED_FLASH 2

#define DOT_COUNT       200     // does not include the 4 pills

// Masks for pills
#define PILL_UL         0x01    // upper left
#define PILL_UR         0x02    // upper right
#define PILL_LL         0x04    // lower left
#define PILL_LR         0x08    // lower right

// Timer intevals for each level
#define INTERV_FAST_0       (55/RTC_TIME)   // Fast update interval
#define INTERV_FAST_1       (50/RTC_TIME)
#define INTERV_FAST_2       (50/RTC_TIME)
#define INTERV_MEDIUM_0     (70/RTC_TIME)   // Medium update interval
#define INTERV_MEDIUM_1     (60/RTC_TIME)
#define INTERV_MEDIUM_2     (50/RTC_TIME)
#define INTERV_SLOW_0       (115/RTC_TIME)  // Slow update interval
#define INTERV_SLOW_1       (105/RTC_TIME)
#define INTERV_SLOW_2       (95/RTC_TIME)

#define START_STATE_TIME    (4500/RTC_TIME)  // Time in START state 
#define CAPTURE_STATE_TIME  (700/RTC_TIME)   // Time in CAPTURE state after ghost taken
#define DIE_STATE_TIME      (2000/RTC_TIME)  // Time in DIE state after Pacman eaten
#define RESTART_STATE_TIME  (1000/RTC_TIME)  // Time in RESTART state after DIE
#define OVER_STATE_TIME     (3000/RTC_TIME)  // Time in OVER state when game ends
#define L_UP_FLASH_TIME     (400/RTC_TIME)   // Time for one flash in LEVEL_UP state
#define L_UP_PAUSE_TIME     (1300/RTC_TIME)  // Pause time in LEVEL_UP state
#define FRUIT_SHOW_TIME     (15000/RTC_TIME) // Time for showing fruit

#define SCATTER_TIME        (7000/RTC_TIME)  // Scattering time
#define CHASE_TIME          (20000/RTC_TIME) // Chasing time
#define BLUE_TIME_0         (8000/RTC_TIME)  // Time being blue, level 0
#define BLUE_TIME_1         (6500/RTC_TIME)  // Time being blue, level 1
#define BLUE_TIME_2         (5000/RTC_TIME)  // Time being blue, level 2
#define FLASH_TIME          (2500/RTC_TIME)  // Time flashing

#define LEAVE_DELAY1        (1000/70)        // 1 s when counting medium rate ticks 
#define LEAVE_DELAY2        (5000/70)        // 5 s when counting medium rate ticks 

#define LIVES               3

#define SCATTER_TARGET_X0   78
#define SCATTER_TARGET_Y0   7
#define SCATTER_TARGET_X1   3
#define SCATTER_TARGET_Y1   7
#define SCATTER_TARGET_X2   78
#define SCATTER_TARGET_Y2   85
#define SCATTER_TARGET_X3   3
#define SCATTER_TARGET_Y3   85

#define DOOR_TARGET_X       39
#define DOOR_TARGET_Y       37

#define ABS(x)              ((x < 0) ? -x : x)

/* =======================================================================
------------------------ I/O ------------------------------------------ */

/* =====================================================================
------------------------ Structures --------------------------------- */

typedef struct pacman_t {
    BYTE x;
    BYTE y;
    BYTE dir;
    BYTE stopped;
    BYTE anim;
} PACMAN_T;

typedef struct ghost_t {
    BYTE x;
    BYTE y;
    BYTE dir;
    BYTE mode;
    BYTE flags;
    BYTE anim;
    BYTE anim_ctr;
    BYTE affinity;
} GHOST_T;

/* =====================================================================
------------------------  Global variables  ------------------------- */

PACMAN_T pacman;
GHOST_T IDATA ghost[4];

BYTE game_state = GAME_ATTRACT;

BYTE grid_x = 0;
BYTE grid_y = 0;

INT8 targ_x = 0;
INT8 targ_y = 0;

BYTE dots_eaten = 0;
BYTE pills = 0x0F;
BYTE ghosts_eaten = 0;
BYTE lives_left = LIVES;
BYTE leave_timer = 0;
BYTE frighten_phase = NOT_FRIGHTENED;
WORD score = 0;
BYTE cur_level = 3;
BYTE level_up_ctr = 0;
BYTE fruit_state = FRUIT_INITIAL;
BYTE aff_state = 0;

BIT scattering = TRUE;
BIT level_restarted = FALSE;
BIT pill_eaten = FALSE;
BIT reset_frighten = FALSE;
BIT level_done = FALSE;
BIT maze_flash = FALSE;
BIT maze_flash_synced = FALSE;

BIT pacman_got_ghost = FALSE;
BIT ghost_got_pacman = FALSE;
BIT pacman_got_fruit = FALSE;

BYTE CODE mask_tbl[] = { 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };

BYTE CODE eat_points[] = { 0,20,40,80,160 };
BYTE CODE fruit_points[] = { 10,30,50 };

BYTE PDATA numbuf[10];

BYTE XDATA dots[88][11];
BYTE XDATA sprites[88][44];

BYTE CODE interv_fast[] = { INTERV_FAST_0, INTERV_FAST_1, INTERV_FAST_2 };
BYTE CODE interv_medium[] = { INTERV_MEDIUM_0, INTERV_MEDIUM_1, INTERV_MEDIUM_2 };
BYTE CODE interv_slow[] = { INTERV_SLOW_0, INTERV_SLOW_1, INTERV_SLOW_2 };

WORD CODE blue_times[] = { BLUE_TIME_0, BLUE_TIME_1, BLUE_TIME_2 };

BYTE CODE * CODE pacman_up_phases[] = { 
    (BYTE CODE *)pacman_up1,
    (BYTE CODE *)pacman_up2,
    (BYTE CODE *)pacman_up1
};

BYTE CODE * CODE pacman_down_phases[] = { 
    (BYTE CODE *)pacman_down1,
    (BYTE CODE *)pacman_down2,
    (BYTE CODE *)pacman_down1
};

BYTE CODE * CODE pacman_left_phases[] = { 
    (BYTE CODE *)pacman_left1,
    (BYTE CODE *)pacman_left2,
    (BYTE CODE *)pacman_left1
};

BYTE CODE * CODE pacman_right_phases[] = { 
    (BYTE CODE *)pacman_right1,
    (BYTE CODE *)pacman_right2,
    (BYTE CODE *)pacman_right1
};

BYTE CODE * CODE ghost_phases_a[] = { 
    (BYTE CODE *)ghost1a,
    (BYTE CODE *)ghost2a,
    (BYTE CODE *)ghost3a,
    (BYTE CODE *)ghost4a
};

BYTE CODE * CODE ghost_phases_b[] = { 
    (BYTE CODE *)ghost1b,
    (BYTE CODE *)ghost2b,
    (BYTE CODE *)ghost3b,
    (BYTE CODE *)ghost4b
};

BYTE CODE * CODE ghost_explosion[] = { 
    NULL,
    (BYTE CODE *)ghost_explosion_1,
    (BYTE CODE *)ghost_explosion_2,
    (BYTE CODE *)ghost_explosion_3,
    (BYTE CODE *)ghost_explosion_4,
};

BYTE CODE * CODE fruits[] = { 
    (BYTE CODE *)fruit0,
    (BYTE CODE *)fruit1,
    (BYTE CODE *)fruit2,
};

/* =====================================================================
------------------------ Function prototypes ------------------------ */

void game_logic( void );
void draw_display( void );
void manage_game_states( void );
void manage_ghost_modes( void );
void check_hi_score_reset( void );
void init_pacman( void );
void init_ghosts( void );
void update_affinities( BIT reset );
void update_pacman( void );
void update_ghost( BYTE g );
void move_sprite( GHOST_T IDATA *sp );

BYTE ghost_control( BYTE g );
BYTE ghost_pixel_control( BYTE g );
BYTE ghost_ctrl_out( BYTE g );
BYTE ghost_ctrl_frightened( BYTE g );
BYTE ghost_ctrl_eyes( BYTE g );
BYTE ghost_ctrl_bounce( BYTE g );
BYTE ghost_ctrl_leave( BYTE g );
BYTE ghost_ctrl_enter( BYTE g );
BYTE cur_backing_sound( void );
BYTE count_mode( BYTE mode );
BIT at_ghost_door( BYTE g );
void set_target( BYTE g );
void set_chase_target1( void );
void set_chase_target2( void );
void set_chase_target3( void );
BYTE get_opposite_dir( BYTE dir );
BYTE get_random_dir( BYTE curdir );
BYTE get_preferred_dir( GHOST_T IDATA *gp );
BYTE get_next_dir( BYTE dir );
BIT is_gridpoint( BYTE x, BYTE y );
WORD get_nearest_gridpoint( BYTE x, BYTE y );
void check_collisions( void );
void seed_random( void );
BYTE random_nr( BYTE max );
void word2asc( WORD n, BYTE PDATA *bufp );

BYTE get_dot_type( BYTE x, BYTE y );
void eat_dot( BYTE x, BYTE y );
void init_dots( void );

void clear_sprites( void );
void draw_pacman( void );
void draw_spare_pacman( BYTE pnr );
void draw_fruit( BYTE fnr );
void draw_fruit_right( BYTE fnr );
void draw_ghost( BYTE g );
void draw_text( BYTE x, BYTE y, BYTE color, BYTE *text );
void draw_char( BYTE x, BYTE y, BYTE color, BYTE ch );
void draw_logo( void );
void set_sprite_pixel( BYTE x, BYTE y, BYTE c );
void invert_pills( void );

BYTE get_maze_pixel( BYTE x, BYTE y );
BYTE get_dots_pixel( BYTE x, BYTE y ); 
BYTE get_sprite_pixel( BYTE x, BYTE y );

/* =====================================================================
--------------------------------------------------------------------- */

void init_game_task( void )
{
    init_dots();
    clear_sprites();
    
    init_pacman();
    init_ghosts();

    scattering = TRUE;
    set_timeout3(SCATTER_TIME);
    
    set_rep_timeout1(INTERV_FAST_0);
    set_rep_timeout2(INTERV_MEDIUM_0);
    set_rep_timeout3(INTERV_SLOW_0);
    
    if (!params_valid())
        set_default_params();
}

/* =====================================================================
Game task
--------------------------------------------------------------------- */

void game_task( void )
{
    static BIT frame_toggle = FALSE;
    
    TP2 = 1;
        
    // Run game logic always
    game_logic();

    // Do display operations only between frames
    if (frame_sync)
    {
        frame_sync = FALSE;

        // Sync maze flashing to frame
        maze_flash_synced = maze_flash;

        // Pills are turned on/off every second frame
        frame_toggle = !frame_toggle;
        if (frame_toggle)
            invert_pills();
        
        // Draw sprites etc. to display buffer
        draw_display();
    }
    TP2 = 0;
}

/* =====================================================================
Run game logic
This only updates variables, it does not draw anything on screen
--------------------------------------------------------------------- */

void game_logic( void )
{
    // --- Game management
    
    // Manage game states always
    manage_game_states();

    // When game running, manage ghost modes and 
    // check collisions between sprites
    if (game_state == GAME_RUN)
    {
        manage_ghost_modes();
        check_collisions();
    }

    // Update ghost affinities
    update_affinities(FALSE);
    
    // Check for hi score reset
    check_hi_score_reset();
    
    // --- Update sprites at one of the 3 tick rates
    
    // - Fast rate: pacman
    if (rep_timeout1)
    {
        rep_timeout1 = FALSE;
        
        if (game_state == GAME_RUN)
            update_pacman();
    }
    
    // - Medium rate: ghosts at normal speed
    if (rep_timeout2)
    {
        rep_timeout2 = FALSE;
        
        // Timer for ghosts leaving home
        // this makes timer automatically dependent on ghost speed
        if (leave_timer != 0)
            leave_timer--;
        
        // Update ghosts in states where they are visible and moving
        if (game_state == GAME_ATTRACT || game_state == GAME_RUN || game_state == GAME_DIE)
        {
            for (BYTE g = 0; g < 4; g++)
            {
                // Update ghosts in EYES-mode twice to get double speed
                if (ghost[g].mode == MODE_EYES)
                {
                    update_ghost(g);
                    // Don't re-run if already at target
                    if (!at_ghost_door(g))
                        update_ghost(g);
                }
                else if (!(ghost[g].flags & FLAG_SLOW)) // update only non-slow ghosts
                    update_ghost(g);
            }
        }
        else if (game_state == GAME_CAPTURE)
        {        
            // During CAPTURE, only ghosts in EYES mode are moving
            for (BYTE g = 0; g < 4; g++)
            {
                // Update ghosts in EYES-mode twice to get double speed
                if (ghost[g].mode == MODE_EYES)
                {
                    update_ghost(g);
                    // Don't re-run if already at target
                    if (!at_ghost_door(g))
                        update_ghost(g);
                }
            }
        }
    }

    // - Slow rate: ghosts at slow speed
    if (rep_timeout3)
    {
        rep_timeout3 = FALSE;
        
        // Ghosts can be slow in ATTRACT/RUN/DIE
        if (game_state == GAME_ATTRACT || game_state == GAME_RUN || game_state == GAME_DIE)
        {
            for (BYTE g = 0; g < 4; g++)
            {
                if (ghost[g].flags & FLAG_SLOW)      // update only slow ghosts
                    update_ghost(g);
            }
        }
    }
}

/* =====================================================================
Draw things in sprite buffer: sprites, texts, etc.
--------------------------------------------------------------------- */

void draw_display( void )
{
    // Start by clearing buffer
    clear_sprites();
    
    // Draw EYES-mode ghosts first --> Pacman or any ghost will then be on top of them
    // Draw only in states where eyes are shown
    if (!(game_state == GAME_RESTART || game_state == GAME_OVER || game_state == GAME_LEVEL_UP))
    {
        for (BYTE g = 0; g < 4; g++)
            if (ghost[g].mode == MODE_EYES)
                draw_ghost(g);
    }
    
    // Draw Pacman in states where it is shown
    if (game_state == GAME_START || game_state == GAME_RUN || game_state == GAME_DIE)
        draw_pacman();
    
    // Draw rest of the ghosts in states where they are shown
    if (!(game_state == GAME_RESTART || game_state == GAME_OVER || game_state == GAME_LEVEL_UP))
    {
        // Draw ghosts in opposite order if they might overlap (last drawn gets on top)
        // Don't draw eyes, they were already drawn
        for (INT8 g = 3; g >= 0; g--)
            if (ghost[g].mode != MODE_EYES)
                draw_ghost(g);
    }

    // Fruit at the center
    if (fruit_state == FRUIT_SHOW)
        draw_fruit(cur_level);
    
    // Spare Pacmans and fruits at right
    if (game_state == GAME_ATTRACT)
    {
        // ATTRACT: draw all 3 Pacmans and fruits
        lives_left = 4;
        cur_level = 2;
    }
    
    if (lives_left != 0)
    {
        for (BYTE n = 0; n < (lives_left-1); n++)
            draw_spare_pacman(n);
    }

    for (BYTE n = 0; n <= cur_level; n++)
        draw_fruit_right(n);
    
    // Score display at top
    word2asc(score,numbuf);
    draw_text(5,1,NUMBER_COLOR,numbuf);
    word2asc(params.hi_score,numbuf);
    draw_text(58,1,NUMBER_COLOR,numbuf);
    
    // Logos and texts
    switch (game_state)
    {
    case GAME_ATTRACT:
        draw_logo();
        break;
        
    case GAME_START:
        draw_text(30,50,TEXT_COLOR,"READY!");
        break;
        
    case GAME_OVER:
        draw_text(25,35,TEXT_COLOR,"GAME");
        draw_text(42,35,TEXT_COLOR,"OVER");
        break;
    }
}

/* =====================================================================
Manage game states
--------------------------------------------------------------------- */

void manage_game_states( void )
{
    switch (game_state)
    {
    case GAME_ATTRACT:
        // Wait for player's OK press
        if (ir_cmd == IRCMD_OK)
        {
            ir_cmd = IRCMD_NONE;        // Consume command
            
            game_state = GAME_START;
            
            seed_random();
            init_pacman();
            init_ghosts();
            cur_level = 0;
            aff_state = 0;
            level_restarted = FALSE;
            pill_eaten = FALSE;
            pacman_got_ghost = FALSE;
            ghost_got_pacman = FALSE;
            pacman_got_fruit = FALSE;
            lives_left = LIVES;
            score = 0;
            fruit_state = FRUIT_INITIAL;
            sound_cmd = SND_READY_JINGLE;
            
            set_timeout4(START_STATE_TIME);
        }
        break;

    case GAME_START:
        // Time for start jingle
        if (timeout4)
        {
            // Set timer intervals
            set_rep_timeout1(interv_fast[cur_level]);
            set_rep_timeout2(interv_medium[cur_level]);
            set_rep_timeout3(interv_slow[cur_level]);
            
            reset_frighten = TRUE;      // --> manage_ghost_modes()
            ir_cmd = IRCMD_NONE;        // remove possible waiting IR command
            sound_cmd = (aff_state == 2) ? SND_RUN_B : SND_RUN_A;
            game_state = GAME_RUN;
        }
        break;

    case GAME_RUN:
        // Game running: monitor the flags set in check_collisions()
        
        // Pacman got frightened ghost: to CAPTURE state
        if (pacman_got_ghost)
        {
            pacman_got_ghost = FALSE;
            
            game_state = GAME_CAPTURE;
            ghosts_eaten++;
            
            score += eat_points[ghosts_eaten];

            sound_cmd = SND_CAPTURE | SND_STOP2;  // also stop background sound
            set_timeout4(CAPTURE_STATE_TIME);
        }
        
        // Ghost got Pacman: to DIE state
        if (ghost_got_pacman)
        {
            ghost_got_pacman = FALSE;
            
            game_state = GAME_DIE;
            sound_cmd = SND_DIE | SND_STOP2;    // also stop background sound
            set_timeout4(DIE_STATE_TIME);
        }

        // Pacman got fruit: no state change
        if (pacman_got_fruit)
        {
            pacman_got_fruit = FALSE;
            
            sound_cmd = SND_FRUIT;
            fruit_state = FRUIT_USED;           // remove fruit
            score += fruit_points[cur_level];
        }
        
        // If level is done (all dots eaten) start next level
        // The flag level_done is set in update_pacman()
        if (level_done)
        {
            level_done = FALSE;
            
            set_timeout4(L_UP_FLASH_TIME);
            level_up_ctr = 0;
            sound_cmd = SND_STOP2;            // stop background sound
            game_state = GAME_LEVEL_UP;
        }
        
        // Show fruit after 70 eaten dots and remove after 15 sec
        if (dots_eaten > 70 && fruit_state == FRUIT_INITIAL)
        {
            fruit_state = FRUIT_SHOW;
            set_timeout4(FRUIT_SHOW_TIME);
        }
        
        if (timeout4 && fruit_state == FRUIT_SHOW)
            fruit_state = FRUIT_USED;
        break;

    case GAME_LEVEL_UP:
        // Next level: flash maze and keep short pause
        if (timeout4)
        {
            level_up_ctr++;
            
            if (level_up_ctr > 6)
            {
                // Pause done, prepare for next level
                game_state = GAME_START;
               
                init_pacman();
                init_ghosts();
                init_dots();
                aff_state = 0;
                level_restarted = FALSE;
                pill_eaten = FALSE;
                pacman_got_ghost = FALSE;
                ghost_got_pacman = FALSE;
                pacman_got_fruit = FALSE;
                fruit_state = FRUIT_INITIAL;
                sound_cmd = SND_READY_JINGLE;
                set_timeout4(START_STATE_TIME);
                
                if (cur_level < 2)
                    cur_level++;
            }
            else if (level_up_ctr == 6)
            {
                // Start new level pre-pause
                maze_flash = FALSE;
                set_timeout4(L_UP_PAUSE_TIME);
            }
            else // level_up_ctr < 6
            {
                // Flash maze with sound
                maze_flash = !maze_flash;
                if (maze_flash)
                    sound_cmd = SND_LEVEL;
                    
                set_timeout4(L_UP_FLASH_TIME);
            }
        }
        break;

    case GAME_CAPTURE:
        // Time for Pacman eating ghost
        if (timeout4)
        {
            // State ended: change the eaten ghost to EYES mode
            for (BYTE g = 0; g < 4; g++)
            {
                if (ghost[g].mode == MODE_EATEN)
                {
                    ghost[g].mode = MODE_EYES;
                    ghost[g].flags &= ~FLAG_SLOW;   // eyes always at normal speed
                }
            }
            
            sound_cmd = SND_EYES;
            game_state = GAME_RUN;
        }
        break;

    case GAME_DIE:
        // Time for ghost eating Pacman
        if (timeout4)
        {
            // State ended: go to OVER or RESTART, depending on 
            // remaining lives
            lives_left--;
            if (lives_left == 0)
            {
                game_state = GAME_OVER;
                set_timeout4(OVER_STATE_TIME);
                
                if (score > params.hi_score)
                    save_hi_score(score);
            }
            else
            {
                game_state = GAME_RESTART;
                set_timeout4(RESTART_STATE_TIME);
            }

            sound_cmd = SND_STOP;
        }
        break;

    case GAME_RESTART:
        // Short pause before restart with new Pacman
        if (timeout4)
        {
            // State ended: back to start
            game_state = GAME_START;
            
            init_pacman();
            init_ghosts();
            pill_eaten = FALSE;
            pacman_got_ghost = FALSE;
            ghost_got_pacman = FALSE;
            pacman_got_fruit = FALSE;
            level_restarted = TRUE;
            leave_timer = LEAVE_DELAY1;
            fruit_state = FRUIT_USED;
            sound_cmd = SND_READY_JINGLE;
            set_timeout4(START_STATE_TIME);
        }
        break;
        
    case GAME_OVER:
        // Time for showing GAME OVER
        if (timeout4)
        {
            // State ended: back to attract
            game_state = GAME_ATTRACT;
            init_ghosts();
            init_dots();
            set_rep_timeout1(INTERV_FAST_0);
            set_rep_timeout2(INTERV_MEDIUM_0);
            set_rep_timeout3(INTERV_SLOW_0);
        }
        break;
    }
}

/* =====================================================================
Manage ghost modes
--------------------------------------------------------------------- */

void manage_ghost_modes( void )
{
    // --- Control BOUNCE --> LEAVE transitions
    
    // Ghost 0 will always leave home immediately
    // This may happen only after revival, originally ghost 0 is 
    // not in bounce mode at all
    if (ghost[0].mode == MODE_BOUNCE && count_mode(MODE_LEAVE) == 0)
        ghost[0].mode = MODE_LEAVE;

    if (!level_restarted)
    {
        // --- Level has not been restarted (no lives lost yet at this level)
        
        // Ghost 1 will leave home very early, after 5 eaten dots
        if (ghost[1].mode == MODE_BOUNCE && dots_eaten > 5 && count_mode(MODE_LEAVE) == 0)
            ghost[1].mode = MODE_LEAVE;

        // Ghost 2 will leave home after 30 eaten dots
        if (ghost[2].mode == MODE_BOUNCE && dots_eaten > 30 && count_mode(MODE_LEAVE) == 0)
            ghost[2].mode = MODE_LEAVE;

        // Ghost 3 will leave home after 60 eaten dots
        if (ghost[3].mode == MODE_BOUNCE && dots_eaten > 60 && count_mode(MODE_LEAVE) == 0)
            ghost[3].mode = MODE_LEAVE;
    }
    else
    {
        // -- Level has been restarted after Pacman death
        
        // Ghost 1 will leave home after LEAVE_DELAY1 from level restart
        if (ghost[1].mode == MODE_BOUNCE && leave_timer == 0 && count_mode(MODE_LEAVE) == 0)
        {
            ghost[1].mode = MODE_LEAVE;
            leave_timer = LEAVE_DELAY2;
        }

        // Ghost 2 will leave home after LEAVE_DELAY2 from ghost 1
        if (ghost[2].mode == MODE_BOUNCE && leave_timer == 0 && count_mode(MODE_LEAVE) == 0)
        {
            ghost[2].mode = MODE_LEAVE;
            leave_timer = LEAVE_DELAY2;
        }

        // Ghost 3 will leave home after LEAVE_DELAY2 from ghost 2
        if (ghost[3].mode == MODE_BOUNCE && leave_timer == 0 && count_mode(MODE_LEAVE) == 0)
            ghost[3].mode = MODE_LEAVE;
    }
    
    // --- Eyes returning home and reached door --> ENTER-mode
    
    for (BYTE g = 0; g < 4; g++)
    {
        if (ghost[g].mode == MODE_EYES)
        {
            if (at_ghost_door(g))
            {
                ghost[g].mode = MODE_ENTER;
                ghost[g].dir = DIR_DOWN;
            }
        }
    }
    
    // --- Pill eaten --> frightened
    
    if (pill_eaten) // update_pacman() sets this when eaten dot is a pill
    {
        pill_eaten = FALSE;
        
        // Start frightened cycle
        frighten_phase = FRIGHTENED_BLUE;   
        set_timeout3(blue_times[cur_level]);
        ghosts_eaten = 0;
        sound_cmd = SND_FRIGHTEN;
        
        // All ghosts: turn blue, slow speed
        // All ghosts outside: to opposite direction
        for (BYTE g = 0; g < 4; g++)
        {
            ghost[g].flags = FLAG_BLUE | FLAG_SLOW; // resets FLAG_FLASH
            
            if (ghost[g].mode == MODE_OUT)
                ghost[g].dir = get_opposite_dir(ghost[g].dir);
        }
    }

    // --- State machine controlling ghosts frightening cycle
    //     normal -> blue -> flash -> normal
    
    // Reset state machine, if requested
    if (reset_frighten)
    {
        reset_frighten = FALSE;
        
        frighten_phase = NOT_FRIGHTENED;
        scattering = TRUE;
        set_timeout3(SCATTER_TIME);
    }
    
    // State machine
    switch (frighten_phase)
    {
    case NOT_FRIGHTENED:
        // Normal state: alternate between scatter/chase
        if (timeout3)
        {
            scattering = !scattering;
            
            if (scattering)
                set_timeout3(SCATTER_TIME);
            else
                set_timeout3(CHASE_TIME);
        }
        break;

    case FRIGHTENED_BLUE:
        // Frightened, phase 1 (blue)
        if (timeout3)
        {
            // Stop blue mode, start flash mode
            frighten_phase = FRIGHTENED_FLASH;
            set_timeout3(FLASH_TIME);
            
            for (BYTE g = 0; g < 4; g++)
            {
                // All blue ghosts start flashing
                if (ghost[g].flags & FLAG_BLUE)
                {
                    ghost[g].flags |= FLAG_FLASH;
                    ghost[g].flags &= ~FLAG_BLUE;
                    ghost[g].anim = 0;          // sync flashing
                    ghost[g].anim_ctr = 0;
                }
            }
        }
        break;
        
    case FRIGHTENED_FLASH:
        // Frightened, phase 2 (flash)
        if (timeout3)
        {
             // Stop flash mode, back to normal
            frighten_phase = NOT_FRIGHTENED;
            set_timeout3(SCATTER_TIME);
            scattering = TRUE;              // restart in scatter mode
            sound_cmd = cur_backing_sound();

            for (BYTE g = 0; g < 4; g++)
            {
                // All flashing ghosts back to normal
                // Ghosts at home stay slow
                if (ghost[g].flags & FLAG_FLASH)
                {
                    if (ghost[g].mode == MODE_BOUNCE || ghost[g].mode == MODE_LEAVE)
                        ghost[g].flags = FLAG_SLOW;
                    else
                        ghost[g].flags = 0;
                }
            }
        }
        break;
    }
}

/* =====================================================================
Check if user wants to reset hi score
This is done by pressing button '0' over 3 seconds
--------------------------------------------------------------------- */

void check_hi_score_reset( void )
{
    static BYTE tick_timer = 20;
    static BYTE cmd_ctr = 0;
    
    if (!tick) return;

    if (--tick_timer != 0) return;

    tick_timer = 20;
    
    // Executed at 200 ms intervals
    if (ir_cmd == IRCMD_0)
    {
        ir_cmd = IRCMD_NONE;
        
        cmd_ctr++;
        if (cmd_ctr > 15)
        {
            save_hi_score(0);
            cmd_ctr = 0;
        }
    }
    else
        cmd_ctr = 0;
}

/* =====================================================================
Set pacman to initial position
--------------------------------------------------------------------- */

void init_pacman( void )
{
    pacman.x = 41;
    pacman.y = 61;
    pacman.dir = DIR_LEFT;
    pacman.stopped = FALSE;
    pacman.anim = 0;
}

/* =====================================================================
Set ghosts to initial positions
--------------------------------------------------------------------- */

void init_ghosts( void )
{
    if (game_state == GAME_ATTRACT)
    {
        // In attract mode, all ghosts are immediately out 
        ghost[1].x = 39; 
        ghost[1].y = 37; 
        ghost[1].mode = MODE_OUT;
        ghost[1].dir = DIR_RIGHT;
        ghost[1].flags = 0;
        
        ghost[2].x = 39; 
        ghost[2].y = 37; 
        ghost[2].mode = MODE_OUT;
        ghost[2].dir = DIR_LEFT;
        ghost[2].flags = 0;
        
        ghost[3].x = 39; 
        ghost[3].y = 37; 
        ghost[3].mode = MODE_OUT;
        ghost[3].dir = DIR_RIGHT;
        ghost[3].flags = 0;

        // Set initial affinities
        update_affinities(TRUE);
    }
    else
    {
        // In normal mode, ghosts 1...3 are bouncing at home
        ghost[1].x = 40; 
        ghost[1].y = 43; 
        ghost[1].mode = MODE_BOUNCE;
        ghost[1].dir = DIR_DOWN;
        ghost[1].flags = FLAG_SLOW;
        
        ghost[2].x = 34; 
        ghost[2].y = 46; 
        ghost[2].mode = MODE_BOUNCE;
        ghost[2].dir = DIR_UP;
        ghost[2].flags = FLAG_SLOW;

        ghost[3].x = 46; 
        ghost[3].y = 46; 
        ghost[3].mode = MODE_BOUNCE;
        ghost[3].dir = DIR_DOWN;
        ghost[3].flags = FLAG_SLOW;
    }

    // Ghost 0 is always out in the beginning
    ghost[0].x = 39; 
    ghost[0].y = 37;
    ghost[0].mode = MODE_OUT;
    ghost[0].dir = DIR_LEFT;
    ghost[0].flags = 0;
    
    // Reset animations
    ghost[0].anim = 0;
    ghost[0].anim_ctr = 0;
    ghost[1].anim = 0;
    ghost[1].anim_ctr = 0;
    ghost[2].anim = 0;
    ghost[2].anim_ctr = 0;
    ghost[3].anim = 0;
    ghost[3].anim_ctr = 0;
}

/* =====================================================================
Update ghost affinities according to eaten dots
If reset==TRUE, set initial affinities
--------------------------------------------------------------------- */

void update_affinities( BIT reset )
{
    if (reset)
    {
        // Set initial values
        ghost[0].affinity = (BYTE)(0.3*256); 
        ghost[1].affinity = (BYTE)(0.3*256); 
        ghost[2].affinity = (BYTE)(0.5*256); 
        ghost[3].affinity = (BYTE)(0.8*256); 
        return;
    }
    
    // Increment according to eaten dots
    switch (aff_state)
    {
    case 0:
        if (dots_eaten > 50)
        {
            // At 50 dots, slight rise
            ghost[0].affinity = (BYTE)(0.4*256); 
            ghost[1].affinity = (BYTE)(0.4*256); 
            ghost[2].affinity = (BYTE)(0.6*256);
            aff_state = 1;
        }
        break;
        
    case 1:
        if (dots_eaten > 170)
        {
            // At 170 dots, serious rise
            ghost[0].affinity = (BYTE)(0.8*256); 
            ghost[1].affinity = (BYTE)(0.5*256); 
            ghost[2].affinity = (BYTE)(0.6*256);
            aff_state = 2;
            sound_cmd = SND_RUN_B;      // change backing sound
        }
        break;
        
    default:
        break;
    }
}

/* =====================================================================
Update Pacman variables. 
--------------------------------------------------------------------- */

void update_pacman( void )
{
    static BIT anim_toggle = FALSE;
    
    // *** Things to do at each gridpoint
    
    if (is_gridpoint(pacman.x,pacman.y))
    {
        // --- Change direction according to IR commands
        
        if (ir_cmd != IRCMD_NONE)
        {
            BYTE newdir;
            
            // Command -> direction
            // Don't consume other commands than directions
            switch (ir_cmd)
            {
            case IRCMD_UP:    newdir = DIR_UP;    ir_cmd = IRCMD_NONE; break;
            case IRCMD_DOWN:  newdir = DIR_DOWN;  ir_cmd = IRCMD_NONE; break;
            case IRCMD_LEFT:  newdir = DIR_LEFT;  ir_cmd = IRCMD_NONE; break;
            case IRCMD_RIGHT: newdir = DIR_RIGHT; ir_cmd = IRCMD_NONE; break;
            default:          newdir = 0;         break;    // don't accept other commands
            }

            // Is new direction allowed?
            if ((newdir & allowed_dirs[grid_y][grid_x]) != 0)
            {
                // Yes: accept new direction as current direction
                pacman.dir = newdir;
                pacman.stopped = FALSE;
            }            
        }
        
        // If current direction is blocked, stop
        if ((pacman.dir & allowed_dirs[grid_y][grid_x]) == 0)
            pacman.stopped = TRUE;
        
        // --- Eat dots (all dots are at gridpoint)
        
        // If current pixel contains a dot, eat it
        BYTE cur_dot = get_dot_type(pacman.x,pacman.y);
        if (cur_dot == DOT_NORM)
        {
            eat_dot(pacman.x,pacman.y);
            sound_cmd = SND_CHOMP;
            dots_eaten++;
        }
        else if (cur_dot == DOT_PILL)
        {
            // Power pill eaten
            eat_dot(pacman.x,pacman.y);
            pill_eaten = TRUE;          // ---> manage_ghost_modes()
        }
        
        if (dots_eaten == DOT_COUNT && pills == 0x00)
        {
            // Level ready
            level_done = TRUE;          // ---> manage_game_states()
        }
    }
    
    // *** Things to do at each pixel

    // Move and animate Pacman
    if (pacman.stopped)
    {
        pacman.anim = 0;
    }
    else
    {   // Not stopped: advance one pixel position
        move_sprite((GHOST_T IDATA *)&pacman);  // use same function as for ghosts

        // Animate Pacman
        anim_toggle = !anim_toggle;
        if (anim_toggle)
        {
            pacman.anim++;
            if (pacman.anim > 2)
                pacman.anim = 0;
        }
    }
}

/* =====================================================================
Update one ghost's variables. 
  g = ghost index, 0...3
--------------------------------------------------------------------- */

void update_ghost( BYTE g )
{
    GHOST_T IDATA *gp = &ghost[g];
    
    // *** Things to do at each gridpoint
    
    if (is_gridpoint(gp->x,gp->y))
    {
        // Ghost is at gridpoint: calculate new direction
         gp->dir = ghost_control(g);

        // Change speed when entering or leaving tunnel
        // Not done for frightened ghosts
        if (!(gp->flags & FLAG_FRIGHTENED))
        {
            if (grid_y == 11)
            {
                if (grid_x == 3)
                {
                    if (gp->dir == DIR_LEFT)
                        gp->flags |= FLAG_SLOW;     // enter tunnel at left
                    else
                        gp->flags &= ~FLAG_SLOW;    // leave tunnel at left
                }
                else if (grid_x == 20)
                {
                    if (gp->dir == DIR_RIGHT)
                        gp->flags |= FLAG_SLOW;      // enter tunnel at right
                    else
                        gp->flags &= ~FLAG_SLOW;     // leave tunnel at right
                }
            }
        }
    }

    // *** Things to do at each pixel
    
    gp->dir = ghost_pixel_control(g);   // home related modes need this

    // Don't move ghost in DIE state
    if (game_state != GAME_DIE)
        move_sprite(gp);

    // Animate ghost
    gp->anim_ctr++;
    if (gp->anim_ctr > 3)
    {
        gp->anim_ctr = 0;
        gp->anim ^= 0x01;
    }
}

/* =====================================================================
Advance sprite (pacman or ghost) one pixel to current direction.
Execute possible teleportation also.
--------------------------------------------------------------------- */

void move_sprite( GHOST_T IDATA *sp )
{
    // This is used also for moving pacman:
    // we rely on the x,y,dir being the first three fields 
    // for both GHOST_T and PACMAN_T

    switch (sp->dir)
    {
    case DIR_UP:
        if (sp->y > 10) 
            sp->y--; 
        break;
        
    case DIR_DOWN:
        if (sp->y < 82)
            sp->y++;
        break;
        
    case DIR_LEFT:
        if (sp->y == 43)
        {
            // Teleport tunnel
            sp->x--;
            if (sp->x == 0)
                sp->x = 81;  // teleport to right
        }
        else if (sp->x > 6)
            sp->x--;
        break;
        
    case DIR_RIGHT:
        if (sp->y == 43)
        {
            // Teleport tunnel
            sp->x++;
            if (sp->x == 81)
                sp->x = 0;  // teleport to left
        }
        else if (sp->x < 75)
            sp->x++;
        break;
    }
}

/* =====================================================================
Main function to decide direction to go while ghost at gridpoint
Calls mode dependent subfunctions
--------------------------------------------------------------------- */

BYTE ghost_control( BYTE g )
{
    switch (ghost[g].mode)
    {
    case MODE_OUT:   
        if (ghost[g].flags & FLAG_FRIGHTENED)
            return ghost_ctrl_frightened(g);
        else
            return ghost_ctrl_out(g);
        
    case MODE_EYES:   
        return ghost_ctrl_eyes(g);
        
    default: 
        return ghost[g].dir;    // default to current direction
    }
}

/* =====================================================================
Main function to decide direction to go at pixel level
Calls mode dependent subfuntions
This is serving "home modes" (BOUNCE,LEAVE,ENTER) where direction
needs to be changed outside gridpoints
--------------------------------------------------------------------- */

BYTE ghost_pixel_control( BYTE g )
{
    switch (ghost[g].mode)
    {
    case MODE_BOUNCE: 
        return ghost_ctrl_bounce(g);
        
    case MODE_LEAVE:  
        return ghost_ctrl_leave(g);
        
    case MODE_ENTER:  
        return ghost_ctrl_enter(g);
        
    default: 
        return ghost[g].dir;    // default to current direction
    }
}

/* =====================================================================
OUT mode:
Return best direction to reach this ghost's target.
--------------------------------------------------------------------- */

BYTE ghost_ctrl_out( BYTE g )
{
    // Set target for this ghost
    set_target(g);
    
    GHOST_T IDATA *gp = &ghost[g];
    
    // Allowed directions in this gridpoint
    BYTE adirs = allowed_dirs[grid_y][grid_x];

    // If this is not an intersection, we must continue to current direction
    if (adirs == (DIR_UP|DIR_DOWN) || adirs == (DIR_LEFT|DIR_RIGHT))
        return gp->dir;
    
    // Get direction to target
    BYTE prefdir = get_preferred_dir(gp);
    
    // Is preferred direction available?
    if ((prefdir & adirs) != 0)
    {
        // Available: depending on ghost's affinity, go to
        // preferred direction or not
        // Never change to opposite direction (reduces back and forth movement)
        if ((prefdir != get_opposite_dir(gp->dir)) && ((rand() & 0xFF) < gp->affinity))
            return prefdir;
    }
    
    // If we did not return anything yet, return random available direction
    return get_random_dir(gp->dir);
}

/* =====================================================================
Frightened mode:
Return random direction
--------------------------------------------------------------------- */

BYTE ghost_ctrl_frightened( BYTE g )
{
    // Return random direction 
    return get_random_dir(ghost[g].dir);
}

/* =====================================================================
EYES mode:
Return best direction to reach ghost door
--------------------------------------------------------------------- */

BYTE ghost_ctrl_eyes( BYTE g )
{
    GHOST_T IDATA *gp = &ghost[g];
    
    // Target is always at ghost door (39,37)
    targ_x = DOOR_TARGET_X;
    targ_y = DOOR_TARGET_Y;
    
    // Allowed directions in this gridpoint
    BYTE adirs = allowed_dirs[grid_y][grid_x];

    // If this is not an intersection, we must continue to current direction
    if (adirs == (DIR_UP|DIR_DOWN) || adirs == (DIR_LEFT|DIR_RIGHT))
        return gp->dir;
    
    // Get direction to target
    BYTE prefdir = get_preferred_dir(gp);
    
    // Is preferred direction available?
    if ((prefdir & adirs) != 0)
    {
        // Available: go there
        // This may cause sudden changes in direction, 
        // but we allow this for eyes
        return prefdir;
    }
    
    // If we did not return anything yet, return random available direction
    return get_random_dir(gp->dir);
}

/* =====================================================================
BOUNCE mode
Return direction during bouncing at home
--------------------------------------------------------------------- */

BYTE ghost_ctrl_bounce( BYTE g )
{
    GHOST_T IDATA *gp = &ghost[g];

    // Change direction UP<->DOWN at limits
    if (gp->y == 43)
        return DIR_DOWN;
    else if (gp->y == 46)
        return DIR_UP;
    
    return gp->dir;
}

/* =====================================================================
LEAVE mode
Return direction during leaving from home
--------------------------------------------------------------------- */

BYTE ghost_ctrl_leave( BYTE g )
{
    GHOST_T IDATA *gp = &ghost[g];

    if (g == 2)
    {
        // Ghost 2 is initally at left
        if (gp->x < 39)
            return DIR_RIGHT;       // move to right
    }
    else
    {
        // Ghosts 1 and 3 are initally at right
        // Ghost 0 leaves only after revival, it is at right x already
        if (gp->x > 39)
            return DIR_LEFT;        // move to left
    }

    // Move up until out
    if (gp->y > 37)
        return DIR_UP;
    else
    {   // Moved to passage, enter normal OUT mode
        gp->mode = MODE_OUT;
        if (!(gp->flags & FLAG_FRIGHTENED))  // keep slow if frightened
            gp->flags &= ~FLAG_SLOW;
        return DIR_LEFT;
    }
}

/* =====================================================================
ENTER mode:
Return direction to get inside home from door

This leaves all ghosts home in BOUNCE-mode. manage_ghost_modes() will
release them out at appropriate moment.
--------------------------------------------------------------------- */

BYTE ghost_ctrl_enter( BYTE g )
{
    GHOST_T IDATA *gp = &ghost[g];

    // All ghosts move first down
    if (gp->y < 46)
        return DIR_DOWN;
    
    switch (g)
    {
    case 2:
        // Ghost 2 goes left
        if (gp->x > 34)
            return DIR_LEFT;
        break;

    case 3:
        // Ghost 3 goes right
        if (gp->x < 46)
            return DIR_RIGHT;
        break;
    }

    gp->mode = MODE_BOUNCE;
    sound_cmd = cur_backing_sound();

    return DIR_UP;
}

/* =====================================================================
Find out currently needed backing sound and return corresponding 
SND_XXX command
--------------------------------------------------------------------- */

BYTE cur_backing_sound( void )
{
    // If at least one ghost in EYES-mode --> eyes-sound
    if (count_mode(MODE_EYES) != 0)
        return SND_EYES;
    
    // If ghosts frightened --> frighten-sound
    if (frighten_phase != NOT_FRIGHTENED)
        return SND_FRIGHTEN;
    
    // Normal case
    return (aff_state == 2) ? SND_RUN_B : SND_RUN_A;
}

/* =====================================================================
Count how many ghosts are in given mode
--------------------------------------------------------------------- */

BYTE count_mode( BYTE mode )
{
    BYTE cnt = 0;
    
    for (BYTE g = 0; g < 4; g++)
        if (ghost[g].mode == mode)
            cnt++;
    
    return cnt;
}

/* =====================================================================
Return TRUE, if ghost g is at ghost door
--------------------------------------------------------------------- */

BIT at_ghost_door( BYTE g )
{
    if (ghost[g].x == DOOR_TARGET_X && ghost[g].y == DOOR_TARGET_Y)
        return TRUE;
    
    return FALSE;
}

/* =====================================================================
Set global (targ_x,targ_y) to ghost's current target
Depending on scatter/chase-mode, target is either fixed per ghost or 
calculated by ghost specific algorithm
--------------------------------------------------------------------- */

void set_target( BYTE g )
{
    static BIT random_generated = FALSE;
    
    // In ATTRACT state, position Pacman randomly each time when 
    // beginning chase mode (Pacman is not visible)
    if (game_state == GAME_ATTRACT)
    {
        if (scattering)
            random_generated = FALSE;
        else if (!random_generated)
        {
            pacman.x = random_nr(85);
            pacman.y = random_nr(85);
            random_generated = TRUE;
        }
    }
    
    switch (g)
    {
    case 0:
        if (scattering)
        {
            targ_x = SCATTER_TARGET_X0;
            targ_y = SCATTER_TARGET_Y0;
        }
        else
        {   // Ghost 0 chase target is Pacman directly
            targ_x = pacman.x;
            targ_y = pacman.y;
        }
        break;
        
    case 1:
        if (scattering)
        {
            targ_x = SCATTER_TARGET_X1;
            targ_y = SCATTER_TARGET_Y1;
        }
        else
            set_chase_target1();
        break;

    case 2:
        if (scattering)
        {
            targ_x = SCATTER_TARGET_X2;
            targ_y = SCATTER_TARGET_Y2;
        }
        else
            set_chase_target2();
        break;
        
    case 3:
        if (scattering)
        {
            targ_x = SCATTER_TARGET_X3;
            targ_y = SCATTER_TARGET_Y3;
        }
        else
            set_chase_target3();
        break;
    }
}

/* =====================================================================
Ghost 1's chase target is 5 tiles ahead of Pacman, regardless of walls
--------------------------------------------------------------------- */

#define G1_TARG_DIST  12

void set_chase_target1( void )
{
    switch (pacman.dir)
    {
    case DIR_LEFT:
        targ_x = pacman.x - G1_TARG_DIST;
        targ_y = pacman.y;
        break;
        
    case DIR_RIGHT:
        targ_x = pacman.x + G1_TARG_DIST;
        targ_y = pacman.y;
        break;
        
    case DIR_DOWN:
        targ_x = pacman.x;
        targ_y = pacman.y + G1_TARG_DIST;
        break;
        
    case DIR_UP:
        // Not reproducing the UP bug in original game
        targ_x = pacman.x;
        targ_y = pacman.y - G1_TARG_DIST;
        break;
    }
}

/* =====================================================================
Ghost 2
First, calculate pivot point, which is 3 tiles ahead of Pacman, regardless 
of walls
The target is ghost 0's position mirrored through pivot point
--------------------------------------------------------------------- */

#define G2_PIV_DIST  6
#define G2_MAX_VAL  85

void set_chase_target2( void )
{
    INT8 piv_x = 0;
    INT8 piv_y = 0;
    
    // Pivot point: ahead of pacman
    switch (pacman.dir)
    {
    case DIR_LEFT:
        piv_x = pacman.x - G2_PIV_DIST;
        piv_y = pacman.y;
        break;
        
    case DIR_RIGHT:
        piv_x = pacman.x + G2_PIV_DIST;
        piv_y = pacman.y;
        break;
        
    case DIR_DOWN:
        piv_x = pacman.x;
        piv_y = pacman.y + G2_PIV_DIST;
        break;
        
    case DIR_UP:
        // Not reproducing the UP bug in original game
        piv_x = pacman.x;
        piv_y = pacman.y - G2_PIV_DIST;
        break;
    }

    // Distance from pivot: d = piv - ghost[0]
    // Mirror: targ = piv + d 
    // ==>  targ = 2*piv - ghost[0]
    
    // More obvious would be  targ = ghost[0] + 2*d, 
    // but this saves two 16-bit variables and the result is same

    int tx = 2*piv_x - ghost[0].x;
    int ty = 2*piv_y - ghost[0].y;
    
    // Truncate values to valid coordinates (0...85)
    if (tx > G2_MAX_VAL) tx = G2_MAX_VAL;
    else if (tx < 0) tx = 0;
    
    if (ty > G2_MAX_VAL) ty = G2_MAX_VAL;
    else if (ty < 0) ty = 0;
    
    // To target
    targ_x = tx;
    targ_y = ty;
}

/* =====================================================================
Ghost 3
First, calculate distance to Pacman, regardless of walls. 
If distance is over G3_LIMIT_DIST, the target is Pacman. Otherwise it is
lower left corner.
--------------------------------------------------------------------- */

#define G3_LIMIT_DIST    576      // 576 = 24^2, radius = 24 pixels = 8 tiles

void set_chase_target3( void )
{
    INT8 dx = pacman.x - ghost[3].x;
    INT8 dy = pacman.y - ghost[3].y;
    
    // Euclidean distance (circle)
    // Compare directly to squared limit, to save square root
    if ((dx*dx + dy*dy) > G3_LIMIT_DIST)
    {
        targ_x = pacman.x;
        targ_y = pacman.y;
    }
    else
    {
        targ_x = SCATTER_TARGET_X3;
        targ_y = SCATTER_TARGET_Y3;
    }
}

/* =====================================================================
Return opposite direction to dir
--------------------------------------------------------------------- */

BYTE get_opposite_dir( BYTE dir )
{
    if (dir < 3)
        return dir ^ 0x03;    // UP/DOWN
    else
        return dir ^ 0x0C;    // RIGHT/LEFT
}

/* =====================================================================
Return next direction to dir, rotating clockwise
--------------------------------------------------------------------- */

BYTE get_next_dir( BYTE dir )
{
    switch (dir)
    {
    case DIR_UP:    return DIR_RIGHT;
    case DIR_RIGHT: return DIR_DOWN;
    case DIR_DOWN:  return DIR_LEFT;
    case DIR_LEFT:  return DIR_UP;
    }
    
    return 0;   // Never comes here but keeps SDCC happy
}

/* =====================================================================
Return random available direction, but not opposite to current direction
to reduce back and forth movement.

Don't call this for ghosts at home! This will never return.
--------------------------------------------------------------------- */

BYTE get_random_dir( BYTE curdir )
{
    BYTE oppdir = get_opposite_dir(curdir);
    BYTE adirs = allowed_dirs[grid_y][grid_x];
    BYTE newdir;

    do {
        newdir = 1 << (rand() & 0x03);
    }
    while (newdir == oppdir || (newdir & adirs) == 0);
    
    return newdir;
}

/* =====================================================================
Calculate in which quadrant target (targ_x,targ_y) is in relation 
to ghost.
This differs significantly from original game's algorithm.
--------------------------------------------------------------------- */

BYTE get_preferred_dir( GHOST_T IDATA *gp )
{
    INT8 dx = targ_x - gp->x;
    INT8 dy = gp->y - targ_y;      // y coord increases down
    BYTE adx = ABS(dx);
    BYTE ady = ABS(dy);
        
    if (adx > ady)
    {
        if (dx > 0)
            return DIR_RIGHT;
        else
            return DIR_LEFT;
    }
    else
    {
        if (dy > 0)
            return DIR_UP;
        else
            return DIR_DOWN;
    }
}

/* =====================================================================
Return TRUE, if we are on 3x3 tile grid. 
Update tile coordinates grid_x, grid_y 
--------------------------------------------------------------------- */

BIT is_gridpoint( BYTE x, BYTE y )
{
    // Return FALSE if x not in grid area (teleports)
    if (x < 6 || x > 75)
        return FALSE;
    
    BYTE gx = x - 6;    // offsets between pixel grid and tile grid
    BYTE gy = y - 10;
    
    if ((gx % (BYTE)3) != 0 || (gy % (BYTE)3) != 0)
        return FALSE;
    
    grid_x = gx / (BYTE)3;
    grid_y = gy / (BYTE)3;
    
    return TRUE;
}

/* =====================================================================
Get nearest gridpoint for (x,y)
Result is returned as WORD: y = hi byte, x = lo byte
--------------------------------------------------------------------- */

WORD get_nearest_gridpoint( BYTE x, BYTE y )
{
    // Force x to grid area, if not already (teleports)
    if (x < 6) x = 6;
    if (x > 76) x = 76;
    
    x -= 6;    // offsets between pixel grid and tile grid
    y -= 10;

    BYTE gx = x / (BYTE)3;
    BYTE gy = y / (BYTE)3;
    
    // Round up
    if ((x % (BYTE)3) == 2) gx++;
    if ((y % (BYTE)3) == 2) gy++;
    
    return (gy << 8) | gx;
}

/* =====================================================================
Check collisions between Pacman/ghosts and Pacman/fruit
Set flags pacman_got_ghost, pacman_got_fruit, ghost_got_pacman accordingly
--------------------------------------------------------------------- */

void check_collisions( void )
{
    WORD pm_gridp,gh_gridp;
    
    pm_gridp = get_nearest_gridpoint(pacman.x,pacman.y);
    
    for (BYTE g = 0; g < 4; g++)
    {
        gh_gridp = get_nearest_gridpoint(ghost[g].x,ghost[g].y);
        
        if (pm_gridp == gh_gridp)
        {
            // We have collision: interpret it
            if (ghost[g].flags & FLAG_FRIGHTENED)
            {
                // Pacman got ghost
                pacman_got_ghost = TRUE;  // --> manage_game_states()
                
                ghost[g].flags = 0;
                ghost[g].mode = MODE_EATEN;
            }
            else if (ghost[g].mode == MODE_OUT || ghost[g].mode == MODE_LEAVE)
            {
                // Ghost got Pacman
                ghost_got_pacman = TRUE;  // --> manage_game_states()
            }
            // else EYES,ENTER or EATEN: don't detect collision
        }
    }
    
    if (fruit_state == FRUIT_SHOW && pacman.x == 40 && pacman.y == 52)
        pacman_got_fruit = TRUE;        // --> manage_game_states()
}

/* =====================================================================
Seend random number generator from timer 0
--------------------------------------------------------------------- */

void seed_random( void )
{
    srand(TMR0);
}

/* =====================================================================
Return random number 0...max-1
--------------------------------------------------------------------- */

BYTE random_nr( BYTE max )
{
    return rand() % max;
}

/* =======================================================================
Convert n to 5 ASCII numbers, with 0 to end.
n=1234 --> buf = "12340"
----------------------------------------------------------------------- */

void word2asc( WORD n, BYTE PDATA *bufp )
{
    if (n > 9999) n = 9999;  // handle only 4 digits
    
    for (INT8 i = 3; i >= 0; i--)
    {
        bufp[i] = (n % 10) | 0x30;
        n /= 10;
    }

    // '0' to end + end nul
    bufp[4] = '0';
    bufp[5] = '\0';
}

/* =====================================================================
Return dot type at point (x,y): NONE/NORM/PILL
--------------------------------------------------------------------- */

BYTE get_dot_type( BYTE x, BYTE y )
{
    // Pills
    if (y == 16)
    {        
        if (x == 6)
            return (pills & PILL_UL) ? DOT_PILL : DOT_NONE;
        else if (x == 75)
            return (pills & PILL_UR) ? DOT_PILL : DOT_NONE;
    }
    else if (y == 67)
    {        
        if (x == 6)
            return (pills & PILL_LL) ? DOT_PILL : DOT_NONE;
        else if (x == 75)
            return (pills & PILL_LR) ? DOT_PILL : DOT_NONE;
    }

    // Normal dots
    BYTE mask = mask_tbl[x & (BYTE)0x07];
    
    if (dots[y][x >> 3] & mask)
        return DOT_NORM;

    return DOT_NONE;
}

/* =====================================================================
Remove dot at point (x,y)
Update also score: 10 points for normal dot, 50 for a pill
--------------------------------------------------------------------- */

void eat_dot( BYTE x, BYTE y )
{
    // Power pills
    if (y == 16)
    {        
        if (x == 6)
        {
            // Upper left
            dots[15][0] &= ~0x02;
            dots[16][0] &= ~0x07;
            dots[17][0] &= ~0x02;
            pills &= ~PILL_UL;
            score += 5;
            return;
        }
        else if (x == 75)
        {
            // Upper right
            dots[15][9] &= ~0x10;
            dots[16][9] &= ~0x38;
            dots[17][9] &= ~0x10;
            pills &= ~PILL_UR;
            score += 5;
            return;
        }
    }
    else if (y == 67)
    {        
        if (x == 6)
        {
            // Lower left
            dots[66][0] &= ~0x02;
            dots[67][0] &= ~0x07;
            dots[68][0] &= ~0x02;
            pills &= ~PILL_LL;
            score += 5;
            return;
        }
        else if (x == 75)
        {
            // Lower right
            dots[66][9] &= ~0x10;
            dots[67][9] &= ~0x38;
            dots[68][9] &= ~0x10;
            pills &= ~PILL_LR;
            score += 5;
            return;
        }
    }
    
    // Normal dots
    BYTE mask = mask_tbl[x & (BYTE)0x07];
    dots[y][x >> 3] &= ~mask;
    score++;
}

/* =====================================================================
Turn remaining power pills on or off
--------------------------------------------------------------------- */

void invert_pills( void )
{
    // Upper left
    if (pills & PILL_UL)
    {
        dots[15][0] ^= 0x02;
        dots[16][0] ^= 0x07;
        dots[17][0] ^= 0x02;
    }
    
    // Upper right
    if (pills & PILL_UR)
    {
        dots[15][9] ^= 0x10;
        dots[16][9] ^= 0x38;
        dots[17][9] ^= 0x10;
    }
    
    // Lower left
    if (pills & PILL_LL)
    {
        dots[66][0] ^= 0x02;
        dots[67][0] ^= 0x07;
        dots[68][0] ^= 0x02;
    }
    
    // Lower right
    if (pills & PILL_LR)
    {
        dots[66][9] ^= 0x10;
        dots[67][9] ^= 0x38;
        dots[68][9] ^= 0x10;
    }
}

/* =====================================================================
Initialize dots[][] table from ROM.
--------------------------------------------------------------------- */

void init_dots( void )
{
    memcpy((BYTE *)dots, (BYTE *)all_dots, sizeof(dots));
    
    dots_eaten = 0;
    pills = 0x0F;
}

/* =====================================================================
Clear sprites[][] table.
--------------------------------------------------------------------- */

void clear_sprites( void )
{
    memset((BYTE *)sprites, 0, sizeof(sprites));
}

/* =================================================================== */
/* ===================      DRAWING ROUTINES       =================== */
/* =================================================================== */

/* These functions draw various objects to sprites framebuffer.
*/

/* =====================================================================
Draw Pacman
--------------------------------------------------------------------- */

void draw_pacman( void )
{
    BYTE CODE *p = NULL;
    
    switch (pacman.dir)
    {
    case DIR_UP:    p = pacman_up_phases[pacman.anim];    break;
    case DIR_DOWN:  p = pacman_down_phases[pacman.anim];  break;
    case DIR_LEFT:  p = pacman_left_phases[pacman.anim];  break;
    case DIR_RIGHT: p = pacman_right_phases[pacman.anim]; break;
    }
    
    for (INT8 y = -2; y <= 2; y++)
    {
        for (INT8 x = -2; x <= 2; x++)
        {
            // Show pixels only inside maze area
            INT8 tx = pacman.x + x;
            if (tx >= 3 && tx <= 78)
                set_sprite_pixel(tx, pacman.y + y, *p++);
            else
                p++;
        }   
    }
}

/* =====================================================================
Draw spare Pacman at the right of maze
--------------------------------------------------------------------- */

void draw_spare_pacman( BYTE pnr )
{
    BYTE CODE *p = (BYTE CODE *)pacman_right1;
    BYTE pmy = 6*pnr + 10;

    // Don't try to draw more than 5 spare Pacmans 
    // (might be more when testing with extra lives)
    if (pnr > 4)
        return;
    
    for (INT8 y = -2; y <= 2; y++)
    {
        for (INT8 x = -2; x <= 2; x++)
        {
            set_sprite_pixel(83 + x, pmy + y, *p++);
        }   
    }
}

/* =====================================================================
Draw fruit at the center of maze. Fixed position.
--------------------------------------------------------------------- */

void draw_fruit( BYTE fnr )
{
    BYTE CODE *p = (BYTE CODE *)fruits[fnr];

    for (INT8 y = -2; y <= 2; y++)
    {
        for (INT8 x = -2; x <= 2; x++)
        {
            set_sprite_pixel(40 + x, 52 + y, *p++);
        }   
    }
}

/* =====================================================================
Draw fruit at the right of maze. Position depends on fnr.
--------------------------------------------------------------------- */

void draw_fruit_right( BYTE fnr )
{
    BYTE CODE *p = (BYTE CODE *)fruits[fnr];
    BYTE fmy = 81 - 7*fnr;

    for (INT8 y = -2; y <= 2; y++)
    {
        for (INT8 x = -2; x <= 2; x++)
        {
            set_sprite_pixel(83 + x, fmy + y, *p++);
        }   
    }
}

/* =====================================================================
Draw a ghost
--------------------------------------------------------------------- */

void draw_ghost( BYTE g )
{
    BYTE CODE *p;

    if (ghost[g].flags & FLAG_BLUE)
        p = ghost[g].anim ? (BYTE CODE *)ghost_blue_b : (BYTE CODE *)ghost_blue_a;
    else if (ghost[g].flags & FLAG_FLASH)
        p = ghost[g].anim ? (BYTE CODE *)ghost_blue_b : (BYTE CODE *)ghost_white_a;
    else
    {
        switch (ghost[g].mode)
        {
        case MODE_EATEN:
            p = (BYTE CODE *)ghost_explosion[ghosts_eaten];
            break;

        case MODE_EYES:
        case MODE_ENTER:
            p = (BYTE CODE *)ghost_eyes;
            break;

        default:
            p = ghost[g].anim ? ghost_phases_b[g] : ghost_phases_a[g];
            break;
        }
    }
    
    for (INT8 y = -2; y <= 2; y++)
    {
        for (INT8 x = -2; x <= 2; x++)
        {
            // Show pixels only inside maze area
            INT8 tx = ghost[g].x + x;
            if (tx >= 3 && tx <= 78)
                set_sprite_pixel(tx, ghost[g].y + y, *p++);
            else
                p++;
        }   
    }
}

/* =====================================================================
Draw text on display starting at (x,y) using given color
--------------------------------------------------------------------- */

void draw_text( BYTE x, BYTE y, BYTE color, BYTE *text )
{
    while (*text != '\0')
    {
        draw_char(x,y,color,*text);
        text++;
        x += 4;
    }
}

/* =====================================================================
Draw a char on display at (x,y) using given color
--------------------------------------------------------------------- */

void draw_char( BYTE x, BYTE y, BYTE color, BYTE ch )
{
    ch -= 0x20;     // Font starts from ' '
    
    for (BYTE yoffs = 0; yoffs < 5; yoffs++)
    {
        BYTE fontrow = font[ch][yoffs];
        BYTE mask = 0x04;
        
        for (BYTE xoffs = 0; xoffs < 3; xoffs++)
        {
            if (fontrow & mask)
                set_sprite_pixel(x+xoffs,y+yoffs,color);
            
            mask >>= 1;
        }
    }
}

/* =====================================================================
Draw Pacman logo on display
--------------------------------------------------------------------- */

void draw_logo( void )
{
    memcpy(&sprites[32][0],(BYTE *)logo,sizeof(logo));
}

/* =====================================================================
Set color c to sprite plane position (x,y)
--------------------------------------------------------------------- */

void set_sprite_pixel( BYTE x, BYTE y, BYTE c )
{
    if (x & 0x01)
    {
        sprites[y][x >> 1] &= 0xF0;
        sprites[y][x >> 1] |= c;
    }
    else
    {
        sprites[y][x >> 1] &= 0x0F;
        sprites[y][x >> 1] |= c << 4;
    }
}

/* =================================================================== */
/* ===================        PIXEL ROUTINES       =================== */
/* =================================================================== */

/* These functions are called from routine get_chain_data() feeding data 
   to matrix.
   They are called for each pixel and each frame, they must be optimized.
   
   get_pixel() is the main entry called from matrix.c/get_chain_data(). 
*/

/* =====================================================================
Return pixel color at (x,y)

Color comes from 3 sources:
- sprite framebuffer (XRAM)
- dots framebuffer (XRAM)
- maze (ROM)
--------------------------------------------------------------------- */

BYTE get_pixel( BYTE x, BYTE y ) 
{
    BYTE color;
    
    // Sprites are over everything; if they return non-blank,
    // return it directly
    color = get_sprite_pixel(x,y);
    if (color != BLANK)
        return color;
    
    // Dots and maze are non-overlapping, they can be ORed
    color = get_dots_pixel(x,y);
    color |= get_maze_pixel(x,y);

    return color;
}

/* =====================================================================
Get color for maze
This is 1-bit (8 pix/byte) bitmap in CODE, color is defined here
--------------------------------------------------------------------- */

BYTE get_maze_pixel( BYTE x, BYTE y ) 
{
    // Ghost door is different color
    if (y == 40 && x > 36 && x < 42)
        return DOOR_COLOR;

    BYTE mask = mask_tbl[x & (BYTE)0x07];
    
    // Return blue or normal maze color depending on maze flashing
    if (maze[y][x >> 3] & mask)
        return maze_flash_synced ? MAZE_BLUE : MAZE_COLOR;

    return BLANK;
}

/* =====================================================================
Get color for dots
This is 1-bit (8 pix/byte) bitmap in XRAM, color is fixed
--------------------------------------------------------------------- */

BYTE get_dots_pixel( BYTE x, BYTE y ) 
{
    BYTE mask = mask_tbl[x & (BYTE)0x07];
    
    if (dots[y][x >> 3] & mask)
        return DOT_COLOR;

    return BLANK;
}

/* =====================================================================
Get color for sprites
This is 4-bit (2 pix/byte) bitmap in XRAM, color is in the bitmap.
--------------------------------------------------------------------- */

BYTE get_sprite_pixel( BYTE x, BYTE y ) 
{
    BYTE sbyte = sprites[y][x >> 1];
    
    if (x & 0x01)
        return sbyte & 0x0F;
    else
        return sbyte >> 4;
}

/* ============================ EOF ====================================== */
