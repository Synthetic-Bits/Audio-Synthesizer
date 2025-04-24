/**
 ******************************************************************************
 * @file           : notes.h
 * @brief          : Common Note Definitions
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>

/* ========================================================================== */
/*                                                                            */
/*    Notes Definitions                                                       */
/*                                                                            */
/* ========================================================================== */

#ifndef _NOTES_H_
#define _NOTES_H_

#define TWO_SEMITONE_SCALAR 575 // 2 ^ (2/12) * 512

// FREQUENCY DEFINES--------------------------------------------------------------------
#define FREQ_DEFAULT 480
#define FREQ_C2 65   // 0  C
#define FREQ_Db2 69  // 1  Db
#define FREQ_D2 73   // 2  D
#define FREQ_Eb2 78  // 3  Eb
#define FREQ_E2 82   // 4  E
#define FREQ_F2 87   // 5  F
#define FREQ_Gb2 93  // 6  Gb
#define FREQ_G2 98   // 7  G
#define FREQ_Ab2 104 // 8  Ab
#define FREQ_A2 110  // 9  A
#define FREQ_Bb2 117 // 10 Bb
#define FREQ_B2 123  // 11 B

#endif /* _NOTES_H_ */