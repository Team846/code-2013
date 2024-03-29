/*---------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							 */
/* Open Source Software - may be modified and shared by FRC teams. The code  */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib. */
/*---------------------------------------------------------------------------*/

#ifndef UTILITY_H_
#define UTILITY_H_

#include "taskLib.h"
#include "NotSupportedException.h"
#include "TYPEDEF.h"

#define wpi_assert(condition) wpi_assert_impl(condition, #condition, NULL, __FILE__, __LINE__, __FUNCTION__)
#define wpi_assertWithMessage(condition, message) wpi_assert_impl(condition, #condition, message, __FILE__, __LINE__, __FUNCTION__)

#define wpi_assertEqual(a, b) wpi_assertEqual_impl(a, b, NULL, __FILE__, __LINE__, __FUNCTION__)
#define wpi_assertEqualWithMessage(a, b, message) wpi_assertEqual_impl(a, b, message, __FILE__, __LINE__, __FUNCTION__)

#define wpi_assertNotEqual(a, b) wpi_assertNotEqual_impl(a, b, NULL, __FILE__, __LINE__, __FUNCTION__)
#define wpi_assertNotEqualWithMessage(a, b, message) wpi_assertNotEqual_impl(a, b, message, __FILE__, __LINE__, __FUNCTION__)

bool wpi_assert_impl(bool conditionValue, const char *conditionText, const char *message, const char *fileName, UINT32 lineNumber, const char *funcName);
bool wpi_assertEqual_impl(int valueA, int valueB, const char *message, const char *fileName,UINT32 lineNumber, const char *funcName);
bool wpi_assertNotEqual_impl(int valueA, int valueB, const char *message, const char *fileName,UINT32 lineNumber, const char *funcName);

//char *wpi_getLabel(UINT addr, INT32 *found = NULL);
void wpi_selfTrace();
void wpi_suspendOnAssertEnabled(bool enabled);
void wpi_stackTraceOnAssertEnable(bool enabled);

UINT16 GetFPGAVersion();
UINT32 GetFPGARevision();
UINT32 GetFPGATime();
INT32 GetRIOUserSwitch();
void SetRIOUserLED(UINT32 state);
INT32 GetRIOUserLED();
INT32 ToggleRIOUserLED();
void SetRIO_FPGA_LED(UINT32 state);
INT32 GetRIO_FPGA_LED();
INT32 ToggleRIO_FPGA_LED();

#endif // UTILITY_H_
