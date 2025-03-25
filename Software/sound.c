/***************************************************************************

 sound.c
 
 RocMan - PacMan for junk led matrix
 
 Controller: STC8A8K64D4 @ breakout
 
 8.1.2025  - First version

 ***************************************************************************/

#define __STC8__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <datatypes.h>
#include "stc8a8.h"
#include "main.h"
#include "sound.h"

/* =====================================================================
------------------------ Constants & macros ------------------------- */

#define START_SOUND1(f)    do { TMR3 = f; T4T3M |= T3R; } while(0)
#define STOP_SOUND1        T4T3M &= ~T3R
#define START_SOUND2(f)    do { TMR4 = f; T4T3M |= T4R; } while(0)
#define STOP_SOUND2        T4T3M &= ~T4R

#define START_TIMER1(t)    tick_timer1 = t
#define TIMEOUT1           (tick_timer1 == 0)
#define START_TIMER2(t)    tick_timer2 = t
#define TIMEOUT2           (tick_timer2 == 0)

#define IDLE               0
#define NEXT_STEP          1
#define SOUND_ON           2
#define WAIT_FSM1          3

#define EOS                0
#define PAUSE              1
#define LOOP               2

/* =======================================================================
------------------------ I/O ------------------------------------------ */

/* =====================================================================
------------------------ Structures --------------------------------- */

typedef struct sound_seq_t {
    WORD freq;      // frequency (timer reload value)
    BYTE dur;       // duration (10 ms)
    BYTE legato;    // stop note before next?
} SOUND_SEQ_T;

/* =====================================================================
------------------------  Global variables  ------------------------- */

// --- Sound sequences

#include "soundata.h"

// Mapping tables from SND_XXXX -> seq_xxxxx

// CH 1
SOUND_SEQ_T CODE * CODE seq_tbl1[] = {
    NULL,                   // SND_NONE
    seq_ready_jingle1,      // SND_READY_JINGLE
    NULL,                   // SND_RUN_A
    NULL,                   // SND_RUN_B
    NULL,                   // SND_RUN_C    -- not used
    NULL,                   // SND_FRIGHTEN
    seq_chomp,              // SND_CHOMP
    seq_die,                // SND_DIE
    seq_capture,            // SND_CAPTURE
    NULL,                   // SND_EYES
    seq_fruit,              // SND_FRUIT
    NULL,                   // SND_LEVEL
};

// CH 2
SOUND_SEQ_T CODE * CODE seq_tbl2[] = {
    NULL,                   // SND_NONE
    seq_ready_jingle2,      // SND_READY_JINGLE
    seq_run_a,              // SND_RUN_A
    seq_run_b,              // SND_RUN_B
    seq_run_b,              // SND_RUN_C    -- not used
    seq_frighten,           // SND_FRIGHTEN
    NULL,                   // SND_CHOMP
    NULL,                   // SND_DIE
    NULL,                   // SND_CAPTURE
    seq_eyes,               // SND_EYES
    NULL,                   // SND_FRUIT
    seq_level,              // SND_LEVEL
};


BYTE tick_timer1 = 0;
BYTE tick_timer2 = 0;
BYTE sound_cmd = SND_NONE;

/* =====================================================================
------------------------ Function prototypes ------------------------ */

void run_sound_ch1( void );
void run_sound_ch2( void );

/* =====================================================================
Sound task
--------------------------------------------------------------------- */

void sound_task( void )
{
    if (!tick) return;      // run at 10 ms intervals

    if (tick_timer1 != 0)
        tick_timer1--;

    if (tick_timer2 != 0)
        tick_timer2--;

    run_sound_ch1();
    run_sound_ch2();
    
    sound_cmd = SND_NONE;
}

/* =====================================================================
Run sound channel 1 (hw timer 3)
--------------------------------------------------------------------- */

void run_sound_ch1( void )
{
    static SOUND_SEQ_T CODE *cur_seq = NULL;
    static BYTE state = IDLE;
    static BYTE seq_idx = 0;
    
    if (sound_cmd != SND_NONE)
    {
        if (sound_cmd & SND_MASK_CH1)
        {
            // Start sequence
            cur_seq = seq_tbl1[sound_cmd & SND_MASK_SEQNR];
            if (cur_seq != NULL)
            {
                seq_idx = 0;
                state = NEXT_STEP;
            }
        }
        
        if (sound_cmd & SND_MASK_STOP1)
        {
            // Stop sequence
            STOP_SOUND1;
            state = IDLE;
        }
    }
        
    switch (state)
    {
    case IDLE:
        // Do nothing
        break;

    case NEXT_STEP:
        // Execute next step in the sequence
        switch (cur_seq[seq_idx].freq)
        {
        case PAUSE:
            START_TIMER1(cur_seq[seq_idx].dur);
            state = SOUND_ON;
            break;
            
        case EOS:
            // Stop sequence
            STOP_SOUND1;
            state = IDLE;
            break;

        case LOOP:
            // Restart sequence
            seq_idx = 0;
            break;

        default:
            // Normal case: feed frequency to hw timer
            START_SOUND1(cur_seq[seq_idx].freq);

            // If duration == 0, go directly to next step
            if (cur_seq[seq_idx].dur == 0)
                seq_idx++;
            else
            {
                START_TIMER1(cur_seq[seq_idx].dur);
                state = SOUND_ON;
            }
            break;
        }
        break;

    case SOUND_ON:
        // Wait while the sound is on
        if (TIMEOUT1)
        {
            if (!(cur_seq[seq_idx].legato || cur_seq[seq_idx].freq == PAUSE))
                STOP_SOUND1;

            seq_idx++;
            state = NEXT_STEP;
        }
    }
}

/* =====================================================================
Run sound channel 2 (hw timer 4)
--------------------------------------------------------------------- */

void run_sound_ch2( void )
{
    static SOUND_SEQ_T CODE *cur_seq = NULL;
    static BYTE state = IDLE;
    static BYTE seq_idx = 0;
    
    if (sound_cmd != SND_NONE)
    {
        if (sound_cmd & SND_MASK_CH2)
        {
            // Start sequence
            cur_seq = seq_tbl2[sound_cmd & SND_MASK_SEQNR];
            if (cur_seq != NULL)
            {
                seq_idx = 0;
                state = NEXT_STEP;
            }
        }
        
        if (sound_cmd & SND_MASK_STOP2)
        {
            // Stop sequence
            STOP_SOUND2;
            state = IDLE;
        }
    }
        
    switch (state)
    {
    case IDLE:
        // Do nothing
        break;

    case NEXT_STEP:
        // Execute next step in the sequence
        switch (cur_seq[seq_idx].freq)
        {
        case PAUSE:
            START_TIMER2(cur_seq[seq_idx].dur);
            state = SOUND_ON;
            break;
            
        case EOS:
            // Stop sequence
            STOP_SOUND2;
            state = IDLE;
            break;

        case LOOP:
            // Restart sequence
            seq_idx = 0;
            break;

        default:
            // Normal case: feed frequency to hw timer
            START_SOUND2(cur_seq[seq_idx].freq);
            
            // If duration == 0, go directly to next step
            if (cur_seq[seq_idx].dur == 0)
                seq_idx++;
            else
            {
                START_TIMER2(cur_seq[seq_idx].dur);
                state = SOUND_ON;
            }
            break;
        }
        break;

    case SOUND_ON:
        // Wait while the sound is on
        if (TIMEOUT2)
        {
            if (!(cur_seq[seq_idx].legato || cur_seq[seq_idx].freq == PAUSE))
                STOP_SOUND2;

            seq_idx++;
            state = NEXT_STEP;
        }
    }
}

/* ============================ EOF ====================================== */

