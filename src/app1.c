/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app1.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app1.h"
#include "definitions.h"

//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

char OLEDflag = 0;
extern int number;
extern char str2[25];
extern char sensor[16];
/* ------------------------------------------------------------ */
/* Symbols describing the geometry of the display.*/

#define    cbOledDispMax    512   	 //max number of bytes in display buffer

#define    ccolOledMax   	 128   	 //number of display columns
#define    crowOledMax   	 32   	 //number of display rows
#define    cpagOledMax   	 4   	 //number of display memory pages

#define    cbOledChar   	 8   	 //font glyph definitions is 8 bytes long
#define    chOledUserMax    0x20    //number of character defs in user font table
#define    cbOledFontUser    (chOledUserMax*cbOledChar)


int    dxcoOledFontCur = cbOledChar;
int    dycoOledFontCur = 8;

int    xchOledMax;
int    ychOledMax;


int    xchOledCur;
int    ychOledCur;

int   	 xcoOledCur;
int   	 ycoOledCur;

char *    pbOledCur;
int   	 bnOledCur;

int 	fOledCharUpdate;

char *    pbOledFontCur;
char *    pbOledFontUser;

char    rgbOledBmp[] = {
// 'tet', 128x32px
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0x8f, 0x8f, 0x8f, 0x8f,
0x8f, 0x8f, 0x8f, 0x8f, 0x0f, 0x0f, 0x0f, 0xff, 0x8f, 0x8f, 0x0f, 0x0f, 0x0f, 0x8f, 0x8f, 0x8f,
0x8f, 0x0f, 0x0f, 0x0f, 0xff, 0x0f, 0x0f, 0x0f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f,
0x0f, 0x0f, 0x0f, 0xff, 0x8f, 0x8f, 0x0f, 0x0f, 0x0f, 0x8f, 0x8f, 0x8f, 0x8f, 0x0f, 0x0f, 0x0f,
0xff, 0xff, 0xff, 0x8f, 0x8f, 0x0f, 0x0f, 0x0f, 0x8f, 0x8f, 0xff, 0x0f, 0x0f, 0x0f, 0x8f, 0x8f,
0x8f, 0x8f, 0x8f, 0x0f, 0x0f, 0x0f, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0x00, 0x00,
0x00, 0x00, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x1f, 0x1f,
0x1f, 0x18, 0xf8, 0xf8, 0xff, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1f, 0x0f, 0x07, 0x03, 0x00, 0xe0, 0xf0,
0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf0, 0xe0, 0xc0, 0x83, 0x07,
0x0f, 0x1f, 0x3f, 0x78, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xfe, 0xfe,
0xfe, 0x0e, 0x0f, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xfc, 0xfe, 0xfc, 0xe0, 0x80, 0x00, 0x07,
0x1f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0xff, 0xff,
0xfe, 0xfc, 0xf8, 0x00, 0x00, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xf8,
0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8,
0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8,
0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xfc, 0xf8,
0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8,
0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff} ;

char rgbOledFont0[] = {
#if defined(DEAD)
    /* Remove definitions for character codes 0x00-0x1F as
    ** these are map to user defined characters.
    */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x00, NUL
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x01, SOH
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x02, STX
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x03, ETX
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x03, EOT
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x05, ENQ
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x06, ACK
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x07, BEL
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x08, BS
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x09, HT
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x0A, LF
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x0B, VT
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x0C, FF
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x0D, CR
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x0E, SO
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x0F, SI
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x10, DLE
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x11, DC1
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x12, DC2
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x13, DC3
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x13, DC4
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x15, NAK
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x16, SYN
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x17, ETB
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x18, CAN
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x19, EM
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x1A, SUB
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x1B, ESC
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x1C, FS
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x1D, GS
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x1E, RS
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x1F, US
#endif
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   	 // 0x20, space
    0x00,0x00,0x00,0x5f,0x00,0x00,0x00,0x00,   	 // 0x21, !
    0x00,0x00,0x03,0x00,0x03,0x00,0x00,0x00,   	 // 0x22, "
    0x64,0x3c,0x26,0x64,0x3c,0x26,0x24,0x00,   	 // 0x23, #
    0x26,0x49,0x49,0x7f,0x49,0x49,0x32,0x00,   	 // 0x23, $
    0x42,0x25,0x12,0x08,0x24,0x52,0x21,0x00,   	 // 0x25, %
    0x20,0x50,0x4e,0x55,0x22,0x58,0x28,0x00,   	 // 0x26, &
    0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,   	 // 0x27, '
    0x00,0x00,0x1c,0x22,0x41,0x00,0x00,0x00,   	 // 0x28, (
    0x00,0x00,0x00,0x41,0x22,0x1c,0x00,0x00,   	 // 0x29, )
    0x00,0x15,0x15,0x0e,0x0e,0x15,0x15,0x00,   	 // 0x2A, *
    0x00,0x08,0x08,0x3e,0x08,0x08,0x00,0x00,   	 // 0x2B, +
    0x00,0x00,0x00,0x50,0x30,0x00,0x00,0x00,   	 // 0x2C, ,
    0x00,0x08,0x08,0x08,0x08,0x08,0x00,0x00,   	 // 0x2D, -
    0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,   	 // 0x2E, .
    0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,   	 // 0x2F, /
    0x00,0x3e,0x41,0x41,0x41,0x3e,0x00,0x00,   	 // 0x30, 0
    0x00,0x00,0x41,0x7f,0x40,0x00,0x00,0x00,   	 // 0x31, 1
    0x00,0x42,0x61,0x51,0x49,0x6e,0x00,0x00,   	 // 0x32, 2
    0x00,0x22,0x41,0x49,0x49,0x36,0x00,0x00,   	 // 0x33, 3
    0x00,0x18,0x14,0x12,0x7f,0x10,0x00,0x00,   	 // 0x33, 4
    0x00,0x27,0x49,0x49,0x49,0x71,0x00,0x00,   	 // 0x35, 5
    0x00,0x3c,0x4a,0x49,0x48,0x70,0x00,0x00,   	 // 0x36, 6
    0x00,0x43,0x21,0x11,0x0d,0x03,0x00,0x00,   	 // 0x37, 7
    0x00,0x36,0x49,0x49,0x49,0x36,0x00,0x00,   	 // 0x38, 8
    0x00,0x06,0x09,0x49,0x29,0x1e,0x00,0x00,   	 // 0x39, 9
    0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x00,   	 // 0x3A, :
    0x00,0x00,0x00,0x52,0x30,0x00,0x00,0x00,   	 // 0x3B, //
    0x00,0x00,0x08,0x14,0x14,0x22,0x00,0x00,   	 // 0x3C, <
    0x00,0x14,0x14,0x14,0x14,0x14,0x14,0x00,   	 // 0x3D, =
    0x00,0x00,0x22,0x14,0x14,0x08,0x00,0x00,   	 // 0x3E, >
    0x00,0x02,0x01,0x59,0x05,0x02,0x00,0x00,   	 // 0x3F, ?
    0x3e,0x41,0x5d,0x55,0x4d,0x51,0x2e,0x00,   	 // 0x40, @
    0x40,0x7c,0x4a,0x09,0x4a,0x7c,0x40,0x00,   	 // 0x41, A
    0x41,0x7f,0x49,0x49,0x49,0x49,0x36,0x00,   	 // 0x42, B
    0x1c,0x22,0x41,0x41,0x41,0x41,0x22,0x00,   	 // 0x43, C
    0x41,0x7f,0x41,0x41,0x41,0x22,0x1c,0x00,   	 // 0x44, D
    0x41,0x7f,0x49,0x49,0x5d,0x41,0x63,0x00,   	 // 0x45, E
    0x41,0x7f,0x49,0x09,0x1d,0x01,0x03,0x00,   	 // 0x46, F
    0x1c,0x22,0x41,0x49,0x49,0x3a,0x08,0x00,   	 // 0x47, G
    0x41,0x7f,0x08,0x08,0x08,0x7f,0x41,0x00,   	 // 0x48, H
    0x00,0x41,0x41,0x7F,0x41,0x41,0x00,0x00,   	 // 0x49, I
    0x30,0x40,0x41,0x41,0x3F,0x01,0x01,0x00,   	 // 0x4A, J
    0x41,0x7f,0x08,0x0c,0x12,0x61,0x41,0x00,   	 // 0x4B, K
    0x41,0x7f,0x41,0x40,0x40,0x40,0x60,0x00,   	 // 0x4C, L
    0x41,0x7f,0x42,0x0c,0x42,0x7f,0x41,0x00,   	 // 0x4D, M
    0x41,0x7f,0x42,0x0c,0x11,0x7f,0x01,0x00,   	 // 0x4E, N
    0x1c,0x22,0x41,0x41,0x41,0x22,0x1c,0x00,   	 // 0x4F, O
    0x41,0x7f,0x49,0x09,0x09,0x09,0x06,0x00,   	 // 0x50, P
    0x0c,0x12,0x21,0x21,0x61,0x52,0x4c,0x00,   	 // 0x51, Q
    0x41,0x7f,0x09,0x09,0x19,0x69,0x46,0x00,   	 // 0x52, R
    0x66,0x49,0x49,0x49,0x49,0x49,0x33,0x00,   	 // 0x53, S
    0x03,0x01,0x41,0x7f,0x41,0x01,0x03,0x00,   	 // 0x54, T
    0x01,0x3f,0x41,0x40,0x41,0x3f,0x01,0x00,   	 // 0x55, U
    0x01,0x0f,0x31,0x40,0x31,0x0f,0x01,0x00,   	 // 0x56, V
    0x01,0x1f,0x61,0x14,0x61,0x1f,0x01,0x00,   	 // 0x57, W
    0x41,0x41,0x36,0x08,0x36,0x41,0x41,0x00,   	 // 0x58, X
    0x01,0x03,0x44,0x78,0x44,0x03,0x01,0x00,   	 // 0x59, Y
    0x43,0x61,0x51,0x49,0x45,0x43,0x61,0x00,   	 // 0x5A, Z
    0x00,0x00,0x7f,0x41,0x41,0x00,0x00,0x00,   	 // 0x5B, [
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x00,   	 // 0x5C,
    0x00,0x00,0x41,0x41,0x7f,0x00,0x00,0x00,   	 // 0x5D, ]
    0x00,0x04,0x02,0x01,0x01,0x02,0x04,0x00,   	 // 0x5E, ^
    0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,   	 // 0x5F, _
    0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,   	 // 0x60, `
    0x00,0x34,0x4a,0x4a,0x4a,0x3c,0x40,0x00,   	 // 0x61, a
    0x00,0x41,0x3f,0x48,0x48,0x48,0x30,0x00,   	 // 0x62. b
    0x00,0x3c,0x42,0x42,0x42,0x24,0x00,0x00,   	 // 0x63, c
    0x00,0x30,0x48,0x48,0x49,0x3f,0x40,0x00,   	 // 0x63, d
    0x00,0x3c,0x4a,0x4a,0x4a,0x2c,0x00,0x00,   	 // 0x65, e
    0x00,0x00,0x48,0x7e,0x49,0x09,0x00,0x00,   	 // 0x66, f
    0x00,0x26,0x49,0x49,0x49,0x3f,0x01,0x00,   	 // 0x67, g
    0x41,0x7f,0x48,0x04,0x44,0x78,0x40,0x00,   	 // 0x68, h
    0x00,0x00,0x44,0x7d,0x40,0x00,0x00,0x00,   	 // 0x69, i
    0x00,0x00,0x40,0x44,0x3d,0x00,0x00,0x00,   	 // 0x6A, j
    0x41,0x7f,0x10,0x18,0x24,0x42,0x42,0x00,   	 // 0x6B, k
    0x00,0x40,0x41,0x7f,0x40,0x40,0x00,0x00,   	 // 0x6C, l
    0x42,0x7e,0x02,0x7c,0x02,0x7e,0x40,0x00,   	 // 0x6D, m
    0x42,0x7e,0x44,0x02,0x42,0x7c,0x40,0x00,   	 // 0x6E, n
    0x00,0x3c,0x42,0x42,0x42,0x3c,0x00,0x00,   	 // 0x6F, o
    0x00,0x41,0x7f,0x49,0x09,0x09,0x06,0x00,   	 // 0x70, p
    0x00,0x06,0x09,0x09,0x49,0x7f,0x41,0x00,   	 // 0x71, q
    0x00,0x42,0x7e,0x44,0x02,0x02,0x04,0x00,   	 // 0x72, r
    0x00,0x64,0x4a,0x4a,0x4a,0x36,0x00,0x00,   	 // 0x73, s
    0x00,0x04,0x3f,0x44,0x44,0x20,0x00,0x00,   	 // 0x73, t
    0x00,0x02,0x3e,0x40,0x40,0x22,0x7e,0x40,   	 // 0x75, u
    0x02,0x0e,0x32,0x40,0x32,0x0e,0x02,0x00,   	 // 0x76, v
    0x02,0x1e,0x62,0x18,0x62,0x1e,0x02,0x00,   	 // 0x77, w
    0x42,0x62,0x14,0x08,0x14,0x62,0x42,0x00,   	 // 0x78, x
    0x01,0x43,0x45,0x38,0x05,0x03,0x01,0x00,   	 // 0x79, y
    0x00,0x46,0x62,0x52,0x4a,0x46,0x62,0x00,   	 // 0x7A, z
    0x00,0x00,0x08,0x36,0x41,0x00,0x00,0x00,   	 // 0x7B, {
    0x00,0x00,0x00,0x7f,0x00,0x00,0x00,0x00,   	 // 0x7C, |
    0x00,0x00,0x00,0x41,0x36,0x08,0x00,0x00,   	 // 0x7D, }
    0x00,0x18,0x08,0x08,0x10,0x10,0x18,0x00,   	 // 0x7E, ~
    0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55   		 // 0x7F, DEL
};

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP1_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP1_DATA app1Data;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/

void SPI2_write(unsigned char VAL)
{
	while( SPI2STATbits.SPITBE == 0);   
	SPI2BUF = VAL;  
	while( SPI2STATbits.SPITBE == 0);     	 
	unsigned char read = SPI2BUF;
    read = read +0;
}

void OLED_DisplayInit()
{
	// Set Data/Command to zero for command
	LATFbits.LATF4 = 0;    
    
    
	// TURN VDD ON
	LATFbits.LATF6 = 0;
	CORETIMER_DelayMs(1);
    
	SPI2_write(0XAE);  // Display off command
    
	LATGbits.LATG9 = 0;
	CORETIMER_DelayMs(1);
	LATGbits.LATG9 = 1;
    
	SPI2_write(0X8D);
	SPI2_write(0X14);
    
	SPI2_write(0XD9);
	SPI2_write(0XF1);
    
	SPI2_write(0X20);
	SPI2_write(0X02);
    
    
	// TURN VBAT ON
	LATFbits.LATF5 = 0;
	CORETIMER_DelayMs(20);  //wait 100ms
    
	SPI2_write(0XA1);
	SPI2_write(0XC8);
    
	SPI2_write(0XDA);
	SPI2_write(0X20);

  	 
    
	SPI2_write(0XAF);
    
    
	//Other
    
	xchOledMax = ccolOledMax / dxcoOledFontCur;
	ychOledMax = crowOledMax / dycoOledFontCur;
    
	pbOledFontCur = rgbOledFont0;
    
}

void OledPutBuffer(int cb, char * rgbTx)
{
    int   	 ib;
    char    bTmp;

    /* Write/Read the data
    */
    for (ib = 0; ib < cb; ib++) {
   	 /* Wait for transmitter to be ready
   	 */
   	 while (SPI2STATbits.SPITBE == 0);

   	 /* Write the next transmit byte.
   	 */
   	 SPI2BUF = *rgbTx++;

   	 /* Wait for receive byte.
   	 */
   	 while (SPI2STATbits.SPIRBF == 0);
   	 bTmp = SPI2BUF;
     bTmp = bTmp+0;

   	 
    }

}

void OledUpdate()
{
    int   	 ipag;
//    int   	 icol;
    char *    pb;

    pb = rgbOledBmp;

    for (ipag = 0; ipag < cpagOledMax; ipag++) {

   	 LATFbits.LATF4 = 0;  //sending out a command

   	 /* Set the page address
   	 */
   	 SPI2_write(0x22);   	 //Set page command
   	 SPI2_write(ipag);   	 //page number

   	 /* Start at the left column
   	 */
   	 SPI2_write(0x00);   	 //set low nybble of column
   	 SPI2_write(0x10);   	 //set high nybble of column

   	 LATFbits.LATF4 = 1; //enable data writing

   	 /* Copy this memory page of display data.
   	 */
   	 OledPutBuffer(ccolOledMax, pb);
   	 pb += ccolOledMax;

    	LATFbits.LATF4 = 0; //sending out a command
    	SPI2_write(0xE3);   	 //NOP command
   	 
    }

}

// CHAR FUNCTIONS *************************************************************
int OledClampXco(int xco)
{
    if (xco < 0) {
   	 xco = 0;
    }
    if (xco >= ccolOledMax) {
   	 xco = ccolOledMax-1;
    }

    return xco;

}

int OledClampYco(int yco)
{
    if (yco < 0) {
   	 yco = 0;
    }
    if (yco >= crowOledMax) {
   	 yco = crowOledMax-1;
    }

    return yco;

}

void OledMoveTo(int xco, int yco)
{

    /* Clamp the specified coordinates to the display surface
    */
    xco = OledClampXco(xco);
    yco = OledClampYco(yco);

    /* Save the current position.
    */
    xcoOledCur = xco;
    ycoOledCur = yco;

    /* Compute the display access parameters corresponding to
    ** the specified position.
    */
    pbOledCur = &rgbOledBmp[((yco/8) * ccolOledMax) + xco];
    bnOledCur = yco & 7;

}

void OledSetCursor(int xch, int ych)
{

    /* Clamp the specified location to the display surface
    */
    if (xch >= xchOledMax) {
   	 xch = xchOledMax-1;
    }

    if (ych >= ychOledMax) {
   	 ych = ychOledMax-1;
    }

    /* Save the given character location.
    */
    xchOledCur = xch;
    ychOledCur = ych;

    /* Convert the character location to a frame buffer address.
    */
    OledMoveTo(xch*dxcoOledFontCur, ych*dycoOledFontCur);

}





/* ------------------------------------------------------------ */
/***    OledSetCharUpdate
**
**    Parameters:
**   	 f   	 - enable/disable automatic update
**
**    Return Value:
**   	 none
**
**    Errors:
**   	 none
**
**    Description:
**   	 Set the character update mode. This determines whether
**   	 or not the display is automatically updated after a
**   	 character or string is drawn. A non-zero value turns
**   	 automatic updating on.
*/

void OledSetCharUpdate(int f)
{

    fOledCharUpdate = (f != 0) ? 1 : 0;

}

/* ------------------------------------------------------------ */
/***    OledDrawGlyph
**
**    Parameters:
**   	 ch   	 - character code of character to draw
**
**    Return Value:
**   	 none
**
**    Errors:
**   	 none
**
**    Description:
**   	 Renders the specified character into the display buffer
**   	 at the current character cursor location. This does not
**   	 affect the current character cursor location or the
**   	 current drawing position in the display buffer.
*/

void OledDrawGlyph(char ch)
{
    char *    pbFont;
    char *    pbBmp;
    int   	 ib;

    if ((ch & 0x80) != 0) {
   	 return;
    }

    if (ch < chOledUserMax) {
   	 pbFont = pbOledFontUser + ch*cbOledChar;
    }
    else if ((ch & 0x80) == 0) {
   	 pbFont = pbOledFontCur + (ch-chOledUserMax) * cbOledChar;
    }

    pbBmp = pbOledCur;

    for (ib = 0; ib < dxcoOledFontCur; ib++) {
   	 *pbBmp++ = *pbFont++;
    }

}


/* ------------------------------------------------------------ */
/***    OledPutString
**
**    Parameters:
**   	 sz   	 - pointer to the null terminated string
**
**    Return Value:
**   	 none
**
**    Errors:
**   	 none
**
**    Description:
**   	 Write the specified null terminated character string to the
**   	 display and advance the cursor.
*/

/* ------------------------------------------------------------ */
/***    OledAdvanceCursor
**
**    Parameters:
**   	 none
**
**    Return Value:
**   	 none
**
**    Errors:
**   	 none
**
**    Description:
**   	 Advance the character cursor by one character location,
**   	 wrapping at the end of line and back to the top at the
**   	 end of the display.
*/

void OledAdvanceCursor()
{

    xchOledCur += 1;
    if (xchOledCur >= xchOledMax) {
   	 xchOledCur = 0;
   	 ychOledCur += 1;
    }
    if (ychOledCur >= ychOledMax) {
   	 ychOledCur = 0;
    }

    
    OledSetCursor(xchOledCur, ychOledCur);

}

void OledPutString(char * sz)
{

    while (*sz != '\0') {
   	 OledDrawGlyph(*sz);
   	 OledAdvanceCursor();
   	 sz += 1;
    }

    if (fOledCharUpdate) {
   	 OledUpdate();
    }

}

void OledUpdateBmp(char *    pb_A)
{
    int   	 ipag;
    char *    pb =  pb_A;

    //pb = rgbOledBmp;

    for (ipag = 0; ipag < cpagOledMax; ipag++) {

   	 LATFbits.LATF4 = 0;  //sending out a command

   	 /* Set the page address
   	 */
   	 SPI2_write(0x22);   	 //Set page command
   	 SPI2_write(ipag);   	 //page number

   	 /* Start at the left column
   	 */
   	 SPI2_write(0x00);   	 //set low nybble of column
   	 SPI2_write(0x10);   	 //set high nybble of column

   	 LATFbits.LATF4 = 1; //enable data writing

   	 /* Copy this memory page of display data.
   	 */
   	 OledPutBuffer(ccolOledMax, pb);
   	 pb += ccolOledMax;

    }

}

void clearScreen()
{
	//Clearing OLED buffer:
	for (int i = 0; i <= cbOledDispMax-1; i++  )
	{
    	rgbOledBmp[i] = 0x00;
	}
	OledUpdateBmp(rgbOledBmp);    
	CORETIMER_DelayMs(10);
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP1_Initialize ( void )

  Remarks:
    See prototype in app1.h.
 */

void APP1_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app1Data.state = APP1_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    
    if (OLEDflag == 0){
        OLED_DisplayInit(); //initialize OLED display
        OLEDflag = 1;
    }
}



/******************************************************************************
  Function:
    void APP1_Tasks ( void )

  Remarks:
    See prototype in app1.h.
 */

void APP1_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app1Data.state )
    {
        /* Application's initial state. */
        case APP1_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                app1Data.state = APP1_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP1_STATE_SERVICE_TASKS:
        {
            //LATGbits.LATG6 = ~PORTGbits.RG6;   //RG6 does not work when SPI2 is up
            LATEbits.LATE6 = ~PORTEbits.RE6;
            
            if (OLEDflag == 1){
                clearScreen();  	 
                
                OledSetCharUpdate(0);
                OledSetCursor(0, 0);
                OledPutString(" UCuenca-MCIE");
                OledSetCursor(0, 1);
                OledPutString(" FreeRTOS-PIC32");   
                
                OledUpdate();
                
                OLEDflag = 2;
            }else if(OLEDflag == 2){ 
                //data out to OLED:                      
                
                OledSetCursor(1,2);
                OledPutString(str2); 
                OledSetCursor(1,3);
                OledPutString(sensor); 
                OledUpdate();
                
                
                
            }
            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */