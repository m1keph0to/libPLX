/*
 * displayinfo.h
 *
 *  Created on: 19 Oct 2016
 *      Author: Mike
 */

#ifndef INC_DISPLAYINFO_H_
#define INC_DISPLAYINFO_H_

#if defined __cplusplus
extern "C" {
#endif
#include "plx.h"
typedef struct
{
	char name[8];   // Label given
	char suffix[3]; // mph / �C etc
	int  Units; // units to use in display (passed to PLX decoder )
	int  Low; // low value to use on dial display
	int  High; // high value to use on dial display
	int  TickScale; // Scale values by this to give tick spacing
	  // so a dial with 0.. 1300 and TickScale=100 would display 14 major ticks on the dial face
	int DP; // Number of decimal places to display
} PLX_DisplayInfo;


extern const PLX_DisplayInfo DisplayInfo[];

#if defined __cplusplus
}
#endif

#endif /* INC_DISPLAYINFO_H_ */
