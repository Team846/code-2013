#ifndef INCLUDES_H_
#define INCLUDES_H_

#define USE_WPILIB

#ifdef USE_WPILIB
#include <WPILib.h>
#else
#endif

#ifdef __VXWORKS__
#define TIME_IN_SECONDS() Timer::GetFPGATimestamp();
#else
#define TIME_IN_SECONDS() clock() / (double) CLOCKS_PER_SEC;
#endif

#include "typedef.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>

#endif
