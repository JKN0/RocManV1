/***************************************************************************

 soundata.h

 RocMan - PacMan for junk led matrix
 
 Controller: STC8A8K64D4 @ breakout
 
 8.1.2025  - First version

 ***************************************************************************/

/* ---------------------  Note values  ------------------------- */

#define NOTE_D6            (WORD)(-1596)    // D6 
#define NOTE_C6H           (WORD)(-1691)    // C#6/Db6 
#define NOTE_C6            (WORD)(-1792)    // C6
#define NOTE_B5            (WORD)(-1898)    // B5 
#define NOTE_A5H           (WORD)(-2011)    // A#5/Bb5 
#define NOTE_A5            (WORD)(-2131)    // A5 
#define NOTE_G5H           (WORD)(-2257)    // G#5/Ab5 
#define NOTE_G5            (WORD)(-2392)    // G5 
#define NOTE_F5H           (WORD)(-2534)    // F#5/Gb5 
#define NOTE_F5            (WORD)(-2684)    // F5 
#define NOTE_E5            (WORD)(-2844)    // E5 
#define NOTE_D5H           (WORD)(-3013)    // D#5/Eb5 
#define NOTE_D5            (WORD)(-3192)    // D5 
#define NOTE_C5H           (WORD)(-3382)    // C#5/Db5 
#define NOTE_C5            (WORD)(-3583)    // C5
#define NOTE_B4            (WORD)(-3796)    // B4 
#define NOTE_A4H           (WORD)(-4022)    // A#4/Bb4 
#define NOTE_A4            (WORD)(-4261)    // A4 A440 
#define NOTE_G4H           (WORD)(-4515)    // G#4/Ab4 
#define NOTE_G4            (WORD)(-4783)    // G4 
#define NOTE_F4H           (WORD)(-5068)    // F#4/Gb4 
#define NOTE_F4            (WORD)(-5369)    // F4 
#define NOTE_E4            (WORD)(-5688)    // E4 
#define NOTE_D4H           (WORD)(-6026)    // D#4/Eb4 
#define NOTE_D4            (WORD)(-6385)    // D4 
#define NOTE_C4H           (WORD)(-6764)    // C#4/Db4 
#define NOTE_C4            (WORD)(-7167)    // C4 -- Middle C 
#define NOTE_B3            (WORD)(-7593)    // B3 
#define NOTE_A3H           (WORD)(-8044)    // A#3/Bb3 
#define NOTE_A3            (WORD)(-8523)    // A3 
#define NOTE_G3H           (WORD)(-9030)    // G#3/Ab3 
#define NOTE_G3            (WORD)(-9566)    // G3 
#define NOTE_F3H           (WORD)(-10135)   // F#3/Gb3 
#define NOTE_F3            (WORD)(-10738)   // F3 

// Durations at 250 BPM
#define DUR_QUARTER        23               // 1/4 note 240 ms
#define DUR_EIGHTH_DOT     17               // dotted 1/8 note 180 ms
#define DUR_EIGHTH         11               // 1/8 note 120 ms
#define DUR_SIXTEENTH      5                // 1/16 note 60 ms
#define DUR_SIXT_TRIPLET   3                // 1/16 triplet note 40 ms

/* ---------------------  Sound sequences  ------------------------- */

SOUND_SEQ_T CODE seq_ready_jingle1[] = {
    { NOTE_C5,  DUR_SIXTEENTH,    FALSE },
    { PAUSE,    DUR_SIXTEENTH,    FALSE },
    { NOTE_C6,  DUR_EIGHTH,       FALSE },
    { NOTE_G5,  DUR_EIGHTH,       FALSE },
    { NOTE_E5,  DUR_EIGHTH,       FALSE },
    { NOTE_C6,  DUR_SIXTEENTH,    FALSE },
    { NOTE_G5,  DUR_SIXTEENTH,    FALSE }, 
    { PAUSE,    DUR_EIGHTH,       FALSE },
    { NOTE_E5,  DUR_EIGHTH,       FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },

    { NOTE_C5H, DUR_SIXTEENTH,    FALSE },
    { PAUSE,    DUR_SIXTEENTH,    FALSE },
    { NOTE_C6H, DUR_EIGHTH,       FALSE },
    { NOTE_G5H, DUR_EIGHTH,       FALSE },
    { NOTE_F5,  DUR_EIGHTH,       FALSE },
    { NOTE_C6H, DUR_SIXTEENTH,    FALSE },
    { NOTE_G5H, DUR_SIXTEENTH,    FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },
    { NOTE_F5,  DUR_EIGHTH,       FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },

    { NOTE_C5,  DUR_SIXTEENTH,    FALSE },
    { PAUSE,    DUR_SIXTEENTH,    FALSE },
    { NOTE_C6,  DUR_EIGHTH,       FALSE },
    { NOTE_G5,  DUR_EIGHTH,       FALSE },
    { NOTE_E5,  DUR_EIGHTH,       FALSE },
    { NOTE_G5,  DUR_EIGHTH,       FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },
    { NOTE_E5,  DUR_EIGHTH,       FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },
    
    { NOTE_D5,  DUR_SIXT_TRIPLET, FALSE },
    { NOTE_D5H, DUR_SIXT_TRIPLET, FALSE },
    { NOTE_E5,  DUR_SIXT_TRIPLET, FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },
    { NOTE_E5,  DUR_SIXT_TRIPLET, FALSE },
    { NOTE_F5,  DUR_SIXT_TRIPLET, FALSE },
    { NOTE_F5H, DUR_SIXT_TRIPLET, FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },
    { NOTE_F5,  DUR_SIXT_TRIPLET, FALSE },
    { NOTE_F5H, DUR_SIXT_TRIPLET, FALSE },
    { NOTE_G5,  DUR_SIXT_TRIPLET, FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },
    { NOTE_C6,  DUR_QUARTER,      FALSE },

    { EOS }
};

SOUND_SEQ_T CODE seq_ready_jingle2[] = {
    { NOTE_G4,  DUR_EIGHTH,       FALSE },
    { PAUSE,    DUR_QUARTER,      FALSE },
    { NOTE_G4,  DUR_EIGHTH,       FALSE },
    { NOTE_C4,  DUR_EIGHTH,       FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },
    { NOTE_G4,  DUR_QUARTER,      FALSE },

    { NOTE_G4H, DUR_EIGHTH,       FALSE },
    { PAUSE,    DUR_QUARTER,      FALSE },
    { NOTE_G4H, DUR_EIGHTH,       FALSE },
    { NOTE_C4H, DUR_EIGHTH,       FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },
    { NOTE_G4H, DUR_QUARTER,      FALSE },

    { NOTE_G4,  DUR_EIGHTH,       FALSE },
    { PAUSE,    DUR_QUARTER,      FALSE },
    { NOTE_G4,  DUR_EIGHTH,       FALSE },
    { NOTE_C4,  DUR_EIGHTH,       FALSE },
    { PAUSE,    DUR_EIGHTH,       FALSE },
    { NOTE_G4,  DUR_QUARTER,      FALSE },

    { NOTE_G4,  DUR_EIGHTH_DOT,   FALSE },
    { PAUSE,    DUR_SIXTEENTH,    FALSE },
    { NOTE_A4,  DUR_EIGHTH_DOT,   FALSE },
    { PAUSE,    DUR_SIXTEENTH,    FALSE },
    { NOTE_B4,  DUR_EIGHTH_DOT,   FALSE },
    { PAUSE,    DUR_SIXTEENTH,    FALSE },
    { NOTE_C5,  DUR_QUARTER,      FALSE },

    { EOS }
};

// Normal backing sound
SOUND_SEQ_T CODE seq_run_a[] = {
    { (WORD)(-2678), 0, TRUE  },    // 700 Hz
    { (WORD)(-2741), 0, TRUE  },    // 684 Hz
    { (WORD)(-2806), 0, TRUE  },    // 668 Hz
    { (WORD)(-2875), 0, TRUE  },    // 652 Hz
    { (WORD)(-2948), 0, TRUE  },    // 636 Hz
    { (WORD)(-3024), 0, TRUE  },    // 620 Hz
    { (WORD)(-3104), 0, TRUE  },    // 604 Hz
    { (WORD)(-3188), 0, TRUE  },    // 588 Hz
    { (WORD)(-3277), 0, TRUE  },    // 572 Hz
    { (WORD)(-3372), 0, TRUE  },    // 556 Hz
    { (WORD)(-3472), 0, TRUE  },    // 540 Hz
    { (WORD)(-3578), 0, TRUE  },    // 524 Hz
    { (WORD)(-3690), 0, TRUE  },    // 508 Hz
    { (WORD)(-3810), 0, TRUE  },    // 492 Hz
    { (WORD)(-3939), 0, TRUE  },    // 476 Hz
    { (WORD)(-4076), 0, TRUE  },    // 460 Hz
    { (WORD)(-4222), 0, TRUE  },    // 444 Hz
    { (WORD)(-4380), 0, TRUE  },    // 428 Hz
    { (WORD)(-4550), 0, TRUE  },    // 412 Hz

    { (WORD)(-4687), 0, TRUE  },    // 400 Hz
    { (WORD)(-4507), 0, TRUE  },    // 416 Hz
    { (WORD)(-4340), 0, TRUE  },    // 432 Hz
    { (WORD)(-4185), 0, TRUE  },    // 448 Hz
    { (WORD)(-4040), 0, TRUE  },    // 464 Hz
    { (WORD)(-3906), 0, TRUE  },    // 480 Hz
    { (WORD)(-3780), 0, TRUE  },    // 496 Hz
    { (WORD)(-3662), 0, TRUE  },    // 512 Hz
    { (WORD)(-3551), 0, TRUE  },    // 528 Hz
    { (WORD)(-3446), 0, TRUE  },    // 544 Hz
    { (WORD)(-3348), 0, TRUE  },    // 560 Hz
    { (WORD)(-3255), 0, TRUE  },    // 576 Hz
    { (WORD)(-3167), 0, TRUE  },    // 592 Hz
    { (WORD)(-3083), 0, TRUE  },    // 608 Hz
    { (WORD)(-3004), 0, TRUE  },    // 624 Hz
    { (WORD)(-2929), 0, TRUE  },    // 640 Hz
    { (WORD)(-2858), 0, TRUE  },    // 656 Hz
    { (WORD)(-2790), 0, TRUE  },    // 672 Hz
    { (WORD)(-2725), 0, TRUE  },    // 688 Hz

    { LOOP }
};

// Faster backing sound
SOUND_SEQ_T CODE seq_run_b[] = {
    { (WORD)(-2500), 0, TRUE  },    // 750 Hz
    { (WORD)(-2561), 0, TRUE  },    // 732 Hz
    { (WORD)(-2626), 0, TRUE  },    // 714 Hz
    { (WORD)(-2693), 0, TRUE  },    // 696 Hz
    { (WORD)(-2765), 0, TRUE  },    // 678 Hz
    { (WORD)(-2840), 0, TRUE  },    // 660 Hz
    { (WORD)(-2920), 0, TRUE  },    // 642 Hz
    { (WORD)(-3004), 0, TRUE  },    // 624 Hz
    { (WORD)(-3094), 0, TRUE  },    // 606 Hz
    { (WORD)(-3188), 0, TRUE  },    // 588 Hz
    { (WORD)(-3289), 0, TRUE  },    // 570 Hz
    { (WORD)(-3396), 0, TRUE  },    // 552 Hz
    { (WORD)(-3511), 0, TRUE  },    // 534 Hz
    { (WORD)(-3633), 0, TRUE  },    // 516 Hz
    { (WORD)(-3765), 0, TRUE  },    // 498 Hz
    { (WORD)(-3906), 0, TRUE  },    // 480 Hz
    { (WORD)(-4058), 0, TRUE  },    // 462 Hz

    { (WORD)(-4166), 0, TRUE  },    // 450 Hz
    { (WORD)(-4006), 0, TRUE  },    // 468 Hz
    { (WORD)(-3858), 0, TRUE  },    // 486 Hz
    { (WORD)(-3720), 0, TRUE  },    // 504 Hz
    { (WORD)(-3591), 0, TRUE  },    // 522 Hz
    { (WORD)(-3472), 0, TRUE  },    // 540 Hz
    { (WORD)(-3360), 0, TRUE  },    // 558 Hz
    { (WORD)(-3255), 0, TRUE  },    // 576 Hz
    { (WORD)(-3156), 0, TRUE  },    // 594 Hz
    { (WORD)(-3063), 0, TRUE  },    // 612 Hz
    { (WORD)(-2976), 0, TRUE  },    // 630 Hz
    { (WORD)(-2893), 0, TRUE  },    // 648 Hz
    { (WORD)(-2815), 0, TRUE  },    // 666 Hz
    { (WORD)(-2741), 0, TRUE  },    // 684 Hz
    { (WORD)(-2670), 0, TRUE  },    // 702 Hz
    { (WORD)(-2604), 0, TRUE  },    // 720 Hz
    { (WORD)(-2540), 0, TRUE  },    // 738 Hz

    { LOOP }
};

// Backing sound in frightened mode
SOUND_SEQ_T CODE seq_frighten[] = {
    { (WORD)(-12500), 0, TRUE },
    { (WORD)(-10838), 0, TRUE },
    { (WORD)( -9566), 0, TRUE },
    { (WORD)( -8562), 0, TRUE },
    { (WORD)( -7748), 0, TRUE },
    { (WORD)( -7075), 0, TRUE },
    { (WORD)( -6510), 0, TRUE },
    { (WORD)( -6029), 0, TRUE },
    { (WORD)( -5614), 0, TRUE },
    { (WORD)( -5252), 0, TRUE },
    { (WORD)( -4934), 0, TRUE },
    { (WORD)( -4653), 0, TRUE },
    { (WORD)( -4401), 0, TRUE },
    { (WORD)( -4176), 0, TRUE },
    
    { LOOP }
};

// Pacman eats a dot
SOUND_SEQ_T CODE seq_chomp[] = {
    { (WORD)(-3125), 0, TRUE  },    // 600 Hz
    { (WORD)(-3485), 0, TRUE  },    // 538 Hz
    { (WORD)(-3939), 0, TRUE  },    // 476 Hz
    { (WORD)(-4528), 1, TRUE  },    // 414 Hz
    { (WORD)(-5326), 1, TRUE  },    // 352 Hz
    { (WORD)(-4528), 1, TRUE  },    // 414 Hz
    { (WORD)(-3939), 0, TRUE  },    // 476 Hz
    { (WORD)(-3485), 0, TRUE  },    // 538 Hz
    { (WORD)(-3125), 0, TRUE  },    // 600 Hz

    { EOS }
};

// Pacman dies
SOUND_SEQ_T CODE seq_die[] = {
    { (WORD)(-2678), 0, TRUE  },    // 700 Hz
    { (WORD)(-2709), 0, TRUE  },    // 692 Hz
    { (WORD)(-2741), 0, TRUE  },    // 684 Hz
    { (WORD)(-2773), 0, TRUE  },    // 676 Hz
    { (WORD)(-2806), 0, TRUE  },    // 668 Hz
    { (WORD)(-2840), 0, TRUE  },    // 660 Hz
    { (WORD)(-2875), 0, TRUE  },    // 652 Hz
    { (WORD)(-2911), 0, TRUE  },    // 644 Hz
    { (WORD)(-2948), 0, TRUE  },    // 636 Hz
    { (WORD)(-2985), 0, TRUE  },    // 628 Hz
    { (WORD)(-3024), 0, TRUE  },    // 620 Hz
    { (WORD)(-3063), 0, TRUE  },    // 612 Hz
    { (WORD)(-3104), 0, TRUE  },    // 604 Hz
    { (WORD)(-3145), 0, TRUE  },    // 596 Hz
    { (WORD)(-3188), 0, TRUE  },    // 588 Hz
    { (WORD)(-3232), 0, TRUE  },    // 580 Hz
    { (WORD)(-3277), 0, TRUE  },    // 572 Hz
    { (WORD)(-3324), 0, TRUE  },    // 564 Hz
    { (WORD)(-3372), 0, TRUE  },    // 556 Hz

    { (WORD)(-3409), 0, TRUE  },    // 550 Hz
    { (WORD)(-3312), 0, TRUE  },    // 566 Hz
    { (WORD)(-3221), 0, TRUE  },    // 582 Hz
    { (WORD)(-3135), 0, TRUE  },    // 598 Hz
    { (WORD)(-3053), 0, TRUE  },    // 614 Hz
    { (WORD)(-2976), 0, TRUE  },    // 630 Hz
    { (WORD)(-2902), 0, TRUE  },    // 646 Hz

    { (WORD)(-2884), 0, TRUE  },    // 650 Hz
    { (WORD)(-2920), 0, TRUE  },    // 642 Hz
    { (WORD)(-2957), 0, TRUE  },    // 634 Hz
    { (WORD)(-2995), 0, TRUE  },    // 626 Hz
    { (WORD)(-3033), 0, TRUE  },    // 618 Hz
    { (WORD)(-3073), 0, TRUE  },    // 610 Hz
    { (WORD)(-3114), 0, TRUE  },    // 602 Hz
    { (WORD)(-3156), 0, TRUE  },    // 594 Hz
    { (WORD)(-3199), 0, TRUE  },    // 586 Hz
    { (WORD)(-3243), 0, TRUE  },    // 578 Hz
    { (WORD)(-3289), 0, TRUE  },    // 570 Hz
    { (WORD)(-3336), 0, TRUE  },    // 562 Hz
    { (WORD)(-3384), 0, TRUE  },    // 554 Hz
    { (WORD)(-3434), 0, TRUE  },    // 546 Hz
    { (WORD)(-3485), 0, TRUE  },    // 538 Hz
    { (WORD)(-3537), 0, TRUE  },    // 530 Hz
    { (WORD)(-3591), 0, TRUE  },    // 522 Hz
    { (WORD)(-3647), 0, TRUE  },    // 514 Hz
    { (WORD)(-3705), 0, TRUE  },    // 506 Hz

    { (WORD)(-3750), 0, TRUE  },    // 500 Hz
    { (WORD)(-3633), 0, TRUE  },    // 516 Hz
    { (WORD)(-3524), 0, TRUE  },    // 532 Hz
    { (WORD)(-3421), 0, TRUE  },    // 548 Hz
    { (WORD)(-3324), 0, TRUE  },    // 564 Hz
    { (WORD)(-3232), 0, TRUE  },    // 580 Hz
    { (WORD)(-3145), 0, TRUE  },    // 596 Hz

    { (WORD)(-3125), 0, TRUE  },    // 600 Hz
    { (WORD)(-3167), 0, TRUE  },    // 592 Hz
    { (WORD)(-3210), 0, TRUE  },    // 584 Hz
    { (WORD)(-3255), 0, TRUE  },    // 576 Hz
    { (WORD)(-3301), 0, TRUE  },    // 568 Hz
    { (WORD)(-3348), 0, TRUE  },    // 560 Hz
    { (WORD)(-3396), 0, TRUE  },    // 552 Hz
    { (WORD)(-3446), 0, TRUE  },    // 544 Hz
    { (WORD)(-3498), 0, TRUE  },    // 536 Hz
    { (WORD)(-3551), 0, TRUE  },    // 528 Hz
    { (WORD)(-3605), 0, TRUE  },    // 520 Hz
    { (WORD)(-3662), 0, TRUE  },    // 512 Hz
    { (WORD)(-3720), 0, TRUE  },    // 504 Hz
    { (WORD)(-3780), 0, TRUE  },    // 496 Hz
    { (WORD)(-3842), 0, TRUE  },    // 488 Hz
    { (WORD)(-3906), 0, TRUE  },    // 480 Hz
    { (WORD)(-3972), 0, TRUE  },    // 472 Hz
    { (WORD)(-4040), 0, TRUE  },    // 464 Hz
    { (WORD)(-4111), 0, TRUE  },    // 456 Hz

    { (WORD)(-4166), 0, TRUE  },    // 450 Hz
    { (WORD)(-4023), 0, TRUE  },    // 466 Hz
    { (WORD)(-3890), 0, TRUE  },    // 482 Hz
    { (WORD)(-3765), 0, TRUE  },    // 498 Hz
    { (WORD)(-3647), 0, TRUE  },    // 514 Hz
    { (WORD)(-3537), 0, TRUE  },    // 530 Hz
    { (WORD)(-3434), 0, TRUE  },    // 546 Hz

    { (WORD)(-3409), 0, TRUE  },    // 550 Hz
    { (WORD)(-3459), 0, TRUE  },    // 542 Hz
    { (WORD)(-3511), 0, TRUE  },    // 534 Hz
    { (WORD)(-3564), 0, TRUE  },    // 526 Hz
    { (WORD)(-3619), 0, TRUE  },    // 518 Hz
    { (WORD)(-3676), 0, TRUE  },    // 510 Hz
    { (WORD)(-3735), 0, TRUE  },    // 502 Hz
    { (WORD)(-3795), 0, TRUE  },    // 494 Hz
    { (WORD)(-3858), 0, TRUE  },    // 486 Hz
    { (WORD)(-3922), 0, TRUE  },    // 478 Hz
    { (WORD)(-3989), 0, TRUE  },    // 470 Hz
    { (WORD)(-4058), 0, TRUE  },    // 462 Hz
    { (WORD)(-4129), 0, TRUE  },    // 454 Hz
    { (WORD)(-4204), 0, TRUE  },    // 446 Hz
    { (WORD)(-4280), 0, TRUE  },    // 438 Hz
    { (WORD)(-4360), 0, TRUE  },    // 430 Hz
    { (WORD)(-4443), 0, TRUE  },    // 422 Hz
    { (WORD)(-4528), 0, TRUE  },    // 414 Hz
    { (WORD)(-4618), 0, TRUE  },    // 406 Hz

    { (WORD)(-4687), 0, TRUE  },    // 400 Hz
    { (WORD)(-4507), 0, TRUE  },    // 416 Hz
    { (WORD)(-4340), 0, TRUE  },    // 432 Hz
    { (WORD)(-4185), 0, TRUE  },    // 448 Hz
    { (WORD)(-4040), 0, TRUE  },    // 464 Hz
    { (WORD)(-3906), 0, TRUE  },    // 480 Hz
    { (WORD)(-3780), 0, TRUE  },    // 496 Hz

    { (WORD)(-3750), 0, TRUE  },    // 500 Hz
    { (WORD)(-3810), 0, TRUE  },    // 492 Hz
    { (WORD)(-3873), 0, TRUE  },    // 484 Hz
    { (WORD)(-3939), 0, TRUE  },    // 476 Hz
    { (WORD)(-4006), 0, TRUE  },    // 468 Hz
    { (WORD)(-4076), 0, TRUE  },    // 460 Hz
    { (WORD)(-4148), 0, TRUE  },    // 452 Hz
    { (WORD)(-4222), 0, TRUE  },    // 444 Hz
    { (WORD)(-4300), 0, TRUE  },    // 436 Hz
    { (WORD)(-4380), 0, TRUE  },    // 428 Hz
    { (WORD)(-4464), 0, TRUE  },    // 420 Hz
    { (WORD)(-4550), 0, TRUE  },    // 412 Hz
    { (WORD)(-4641), 0, TRUE  },    // 404 Hz
    { (WORD)(-4734), 0, TRUE  },    // 396 Hz
    { (WORD)(-4832), 0, TRUE  },    // 388 Hz
    { (WORD)(-4934), 0, TRUE  },    // 380 Hz
    { (WORD)(-5040), 0, TRUE  },    // 372 Hz
    { (WORD)(-5151), 0, TRUE  },    // 364 Hz
    { (WORD)(-5266), 0, TRUE  },    // 356 Hz

    { (WORD)(-5357), 0, TRUE  },    // 350 Hz
    { (WORD)(-5122), 0, TRUE  },    // 366 Hz
    { (WORD)(-4908), 0, TRUE  },    // 382 Hz
    { (WORD)(-4711), 0, TRUE  },    // 398 Hz
    { (WORD)(-4528), 0, TRUE  },    // 414 Hz
    { (WORD)(-4360), 0, TRUE  },    // 430 Hz
    { (WORD)(-4204), 0, TRUE  },    // 446 Hz

    { (WORD)(-4166), 0, TRUE  },    // 450 Hz
    { (WORD)(-4242), 0, TRUE  },    // 442 Hz
    { (WORD)(-4320), 0, TRUE  },    // 434 Hz
    { (WORD)(-4401), 0, TRUE  },    // 426 Hz
    { (WORD)(-4485), 0, TRUE  },    // 418 Hz
    { (WORD)(-4573), 0, TRUE  },    // 410 Hz
    { (WORD)(-4664), 0, TRUE  },    // 402 Hz
    { (WORD)(-4758), 0, TRUE  },    // 394 Hz
    { (WORD)(-4857), 0, TRUE  },    // 386 Hz
    { (WORD)(-4960), 0, TRUE  },    // 378 Hz
    { (WORD)(-5067), 0, TRUE  },    // 370 Hz
    { (WORD)(-5179), 0, TRUE  },    // 362 Hz
    { (WORD)(-5296), 0, TRUE  },    // 354 Hz
    { (WORD)(-5419), 0, TRUE  },    // 346 Hz
    { (WORD)(-5547), 0, TRUE  },    // 338 Hz
    { (WORD)(-5681), 0, TRUE  },    // 330 Hz
    { (WORD)(-5822), 0, TRUE  },    // 322 Hz
    { (WORD)(-5971), 0, TRUE  },    // 314 Hz
    { (WORD)(-6127), 1, FALSE },    // 306 Hz

    { PAUSE,         8, FALSE },

    { (WORD)(-9375), 0, TRUE  },    // 200 Hz
    { (WORD)(-4934), 0, TRUE  },    // 380 Hz
    { (WORD)(-3348), 0, TRUE  },    // 560 Hz
    { (WORD)(-2533), 0, TRUE  },    // 740 Hz
    { (WORD)(-2038), 0, TRUE  },    // 920 Hz
    { (WORD)(-1704), 0, TRUE  },    // 1100 Hz
    { (WORD)(-1464), 0, TRUE  },    // 1280 Hz
    { (WORD)(-1284), 0, TRUE  },    // 1460 Hz
    { (WORD)(-1143), 0, TRUE  },    // 1640 Hz
    { (WORD)(-1030), 1, FALSE },    // 1820 Hz

    { PAUSE,         1, FALSE },
    
    { (WORD)(-9375), 0, TRUE  },    // 200 Hz
    { (WORD)(-4934), 0, TRUE  },    // 380 Hz
    { (WORD)(-3348), 0, TRUE  },    // 560 Hz
    { (WORD)(-2533), 0, TRUE  },    // 740 Hz
    { (WORD)(-2038), 0, TRUE  },    // 920 Hz
    { (WORD)(-1704), 0, TRUE  },    // 1100 Hz
    { (WORD)(-1464), 0, TRUE  },    // 1280 Hz
    { (WORD)(-1284), 0, TRUE  },    // 1460 Hz
    { (WORD)(-1143), 0, TRUE  },    // 1640 Hz
    { (WORD)(-1030), 1, FALSE },    // 1820 Hz
    
    { EOS }
};

// Pacman eats a ghost
SOUND_SEQ_T CODE seq_capture[] = {
    { (WORD)(-9375), 0, TRUE  },    // 200 Hz
    { (WORD)(-7944), 0, TRUE  },    // 236 Hz
    { (WORD)(-6893), 0, TRUE  },    // 272 Hz
    { (WORD)(-6087), 0, TRUE  },    // 308 Hz
    { (WORD)(-5450), 0, TRUE  },    // 344 Hz
    { (WORD)(-4934), 0, TRUE  },    // 380 Hz
    { (WORD)(-4507), 0, TRUE  },    // 416 Hz
    { (WORD)(-4148), 0, TRUE  },    // 452 Hz
    { (WORD)(-3842), 0, TRUE  },    // 488 Hz
    { (WORD)(-3578), 0, TRUE  },    // 524 Hz
    { (WORD)(-3348), 0, TRUE  },    // 560 Hz
    { (WORD)(-3145), 0, TRUE  },    // 596 Hz
    { (WORD)(-2966), 0, TRUE  },    // 632 Hz
    { (WORD)(-2806), 0, TRUE  },    // 668 Hz
    { (WORD)(-2663), 0, TRUE  },    // 704 Hz
    { (WORD)(-2533), 0, TRUE  },    // 740 Hz
    { (WORD)(-2416), 0, TRUE  },    // 776 Hz
    { (WORD)(-2309), 0, TRUE  },    // 812 Hz
    { (WORD)(-2211), 0, TRUE  },    // 848 Hz
    { (WORD)(-2121), 0, TRUE  },    // 884 Hz
    { (WORD)(-2038), 0, TRUE  },    // 920 Hz
    { (WORD)(-1961), 0, TRUE  },    // 956 Hz
    { (WORD)(-1890), 0, TRUE  },    // 992 Hz
    { (WORD)(-1823), 0, TRUE  },    // 1028 Hz
    { (WORD)(-1762), 0, TRUE  },    // 1064 Hz
    { (WORD)(-1704), 0, TRUE  },    // 1100 Hz
    { (WORD)(-1650), 0, TRUE  },    // 1136 Hz
    { (WORD)(-1599), 0, TRUE  },    // 1172 Hz
    { (WORD)(-1552), 0, TRUE  },    // 1208 Hz
    { (WORD)(-1507), 0, TRUE  },    // 1244 Hz
    { (WORD)(-1464), 0, TRUE  },    // 1280 Hz

    { (WORD)(-1442), 0, TRUE  },    // 1300 Hz
    { (WORD)(-1428), 0, TRUE  },    // 1313 Hz
    { (WORD)(-1414), 0, TRUE  },    // 1326 Hz
    { (WORD)(-1400), 0, TRUE  },    // 1339 Hz
    { (WORD)(-1386), 0, TRUE  },    // 1352 Hz
    { (WORD)(-1373), 0, TRUE  },    // 1365 Hz
    { (WORD)(-1360), 0, TRUE  },    // 1378 Hz
    { (WORD)(-1347), 0, TRUE  },    // 1391 Hz
    { (WORD)(-1335), 0, TRUE  },    // 1404 Hz
    { (WORD)(-1323), 0, TRUE  },    // 1417 Hz
    { (WORD)(-1311), 0, TRUE  },    // 1430 Hz
    { (WORD)(-1299), 0, TRUE  },    // 1443 Hz
    { (WORD)(-1287), 0, TRUE  },    // 1456 Hz
    { (WORD)(-1276), 0, TRUE  },    // 1469 Hz
    { (WORD)(-1265), 0, TRUE  },    // 1482 Hz
    { (WORD)(-1254), 0, TRUE  },    // 1495 Hz

    { EOS }
};

// Backing sound while eyes are returning home
SOUND_SEQ_T CODE seq_eyes[] = {
    { (WORD)(-721), 0, TRUE  },    // 2600 Hz
    { (WORD)(-744), 0, TRUE  },    // 2520 Hz
    { (WORD)(-768), 0, TRUE  },    // 2440 Hz
    { (WORD)(-794), 0, TRUE  },    // 2360 Hz
    { (WORD)(-822), 0, TRUE  },    // 2280 Hz
    { (WORD)(-852), 0, TRUE  },    // 2200 Hz
    { (WORD)(-884), 0, TRUE  },    // 2120 Hz
    { (WORD)(-919), 0, TRUE  },    // 2040 Hz
    { (WORD)(-956), 0, TRUE  },    // 1960 Hz
    { (WORD)(-997), 0, TRUE  },    // 1880 Hz
    { (WORD)(-1041), 0, TRUE  },    // 1800 Hz
    { (WORD)(-1090), 0, TRUE  },    // 1720 Hz
    { (WORD)(-1143), 0, TRUE  },    // 1640 Hz
    { (WORD)(-1201), 0, TRUE  },    // 1560 Hz
    { (WORD)(-1266), 0, TRUE  },    // 1480 Hz
    { (WORD)(-1339), 0, TRUE  },    // 1400 Hz
    { (WORD)(-1420), 0, TRUE  },    // 1320 Hz
    { (WORD)(-1512), 0, TRUE  },    // 1240 Hz
    { (WORD)(-1616), 0, TRUE  },    // 1160 Hz
    { (WORD)(-1736), 0, TRUE  },    // 1080 Hz

    { (WORD)(-1875), 0, TRUE  },    // 1000 Hz
    { (WORD)(-1913), 0, TRUE  },    // 980 Hz
    { (WORD)(-1953), 0, TRUE  },    // 960 Hz
    { (WORD)(-1994), 0, TRUE  },    // 940 Hz
    { (WORD)(-2038), 0, TRUE  },    // 920 Hz
    { (WORD)(-2083), 0, TRUE  },    // 900 Hz
    { (WORD)(-2130), 0, TRUE  },    // 880 Hz
    { (WORD)(-2180), 0, TRUE  },    // 860 Hz
    { (WORD)(-2232), 0, TRUE  },    // 840 Hz
    { (WORD)(-2286), 4, TRUE  },    // 820 Hz
    
    { LOOP }
};

// Pacman eats fruit
SOUND_SEQ_T CODE seq_fruit[] = {
    { (WORD)(-4687), 0, TRUE  },    // 400 Hz
    { (WORD)(-4882), 0, TRUE  },    // 384 Hz
    { (WORD)(-5095), 0, TRUE  },    // 368 Hz
    { (WORD)(-5326), 0, TRUE  },    // 352 Hz
    { (WORD)(-5580), 0, TRUE  },    // 336 Hz
    { (WORD)(-5859), 0, TRUE  },    // 320 Hz
    { (WORD)(-6167), 0, TRUE  },    // 304 Hz
    { (WORD)(-6510), 0, TRUE  },    // 288 Hz
    { (WORD)(-6893), 0, TRUE  },    // 272 Hz
    { (WORD)(-7324), 0, TRUE  },    // 256 Hz
    { (WORD)(-7812), 0, TRUE  },    // 240 Hz
    { (WORD)(-8370), 0, TRUE  },    // 224 Hz
    { (WORD)(-9014), 0, TRUE  },    // 208 Hz
    { (WORD)(-9765), 0, TRUE  },    // 192 Hz
    { (WORD)(-10653), 0, TRUE  },    // 176 Hz
    { (WORD)(-11718), 0, TRUE  },    // 160 Hz
    { (WORD)(-13020), 0, TRUE  },    // 144 Hz
    { (WORD)(-14648), 0, TRUE  },    // 128 Hz
    { (WORD)(-16741), 0, TRUE  },    // 112 Hz
    
    { (WORD)(-18750), 0, TRUE  },    // 100 Hz
    { (WORD)(-14763), 0, TRUE  },    // 127 Hz
    { (WORD)(-12175), 0, TRUE  },    // 154 Hz
    { (WORD)(-10359), 0, TRUE  },    // 181 Hz
    { (WORD)(-9014), 0, TRUE  },    // 208 Hz
    { (WORD)(-7978), 0, TRUE  },    // 235 Hz
    { (WORD)(-7156), 0, TRUE  },    // 262 Hz
    { (WORD)(-6487), 0, TRUE  },    // 289 Hz
    { (WORD)(-5933), 0, TRUE  },    // 316 Hz
    { (WORD)(-5466), 0, TRUE  },    // 343 Hz
    { (WORD)(-5067), 0, TRUE  },    // 370 Hz
    { (WORD)(-4722), 0, TRUE  },    // 397 Hz
    { (WORD)(-4422), 0, TRUE  },    // 424 Hz
    { (WORD)(-4157), 0, TRUE  },    // 451 Hz
    { (WORD)(-3922), 0, TRUE  },    // 478 Hz
    { (WORD)(-3712), 0, TRUE  },    // 505 Hz
    { (WORD)(-3524), 0, TRUE  },    // 532 Hz
    { (WORD)(-3354), 0, TRUE  },    // 559 Hz
    { (WORD)(-3199), 0, TRUE  },    // 586 Hz

    { EOS }
};

// Game changes to next level 
SOUND_SEQ_T CODE seq_level[] = {
    { (WORD)(-18750), 1, TRUE  },    // 100 Hz
    { (WORD)(-17688), 1, TRUE  },    // 106 Hz
    { (WORD)(-16741), 1, TRUE  },    // 112 Hz
    { (WORD)(-15889), 1, TRUE  },    // 118 Hz
    { (WORD)(-15120), 1, TRUE  },    // 124 Hz
    { (WORD)(-14423), 1, TRUE  },    // 130 Hz
    { (WORD)(-13786), 1, TRUE  },    // 136 Hz
    { (WORD)(-13204), 1, TRUE  },    // 142 Hz
    { (WORD)(-12668), 1, TRUE  },    // 148 Hz
    { (WORD)(-12175), 1, TRUE  },    // 154 Hz
    { (WORD)(-11718), 1, TRUE  },    // 160 Hz
    { (WORD)(-11295), 1, TRUE  },    // 166 Hz
    { (WORD)(-10901), 1, TRUE  },    // 172 Hz
    { (WORD)(-10533), 1, TRUE  },    // 178 Hz
    { (WORD)(-10190), 1, TRUE  },    // 184 Hz
    { (WORD)(-9868), 1, TRUE  },    // 190 Hz
    { (WORD)(-9566), 1, TRUE  },    // 196 Hz
    { (WORD)(-9282), 1, TRUE  },    // 202 Hz
    { (WORD)(-9014), 1, TRUE  },    // 208 Hz
    { (WORD)(-8761), 1, TRUE  },    // 214 Hz
    { (WORD)(-8522), 1, TRUE  },    // 220 Hz
    { (WORD)(-8296), 1, TRUE  },    // 226 Hz
    { (WORD)(-8081), 1, TRUE  },    // 232 Hz
    { (WORD)(-7878), 1, TRUE  },    // 238 Hz
    { (WORD)(-7684), 1, TRUE  },    // 244 Hz
    { (WORD)(-7500), 1, TRUE  },    // 250 Hz
    { (WORD)(-7324), 1, TRUE  },    // 256 Hz
    { (WORD)(-7156), 1, TRUE  },    // 262 Hz
    { (WORD)(-6996), 1, TRUE  },    // 268 Hz
    { (WORD)(-6843), 1, TRUE  },    // 274 Hz
    { (WORD)(-6696), 1, TRUE  },    // 280 Hz
    { (WORD)(-6555), 1, TRUE  },    // 286 Hz
    { (WORD)(-6421), 1, TRUE  },    // 292 Hz
    { (WORD)(-6291), 1, TRUE  },    // 298 Hz
    { (WORD)(-6167), 1, TRUE  },    // 304 Hz
    { (WORD)(-6048), 1, TRUE  },    // 310 Hz
    { (WORD)(-5933), 1, TRUE  },    // 316 Hz
    { (WORD)(-5822), 1, TRUE  },    // 322 Hz
    { (WORD)(-5716), 1, TRUE  },    // 328 Hz
    { (WORD)(-5613), 1, TRUE  },    // 334 Hz
    { (WORD)(-5514), 1, TRUE  },    // 340 Hz
    { (WORD)(-5419), 1, TRUE  },    // 346 Hz
    { (WORD)(-5326), 1, TRUE  },    // 352 Hz
    { (WORD)(-5237), 1, TRUE  },    // 358 Hz
    { (WORD)(-5151), 1, TRUE  },    // 364 Hz
    { (WORD)(-5067), 1, TRUE  },    // 370 Hz
    { (WORD)(-4986), 1, TRUE  },    // 376 Hz
    { (WORD)(-4908), 1, TRUE  },    // 382 Hz
    { (WORD)(-4832), 1, TRUE  },    // 388 Hz
    { (WORD)(-4758), 1, TRUE  },    // 394 Hz

    { EOS }
};
