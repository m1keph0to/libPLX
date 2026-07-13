/*
 * displayinfo.c
 *
 *  Created on: 19 Oct 2016
 *      Author: Mike
 */

#include "libPLX/displayinfo.h"

PLX_DisplayInfo const DisplayInfo[] =
    {
        // Displayed, Units, min, max, scaleticks, decimal places
        [PLX_AFR] = {"AFR", "", 1, 10, 20, 1, 1},                 // 10 to 20 , tick every 1 unit   Wideband Air/Fuel
        [PLX_EGT] = {"EGT", "°C", 0, 0, 1000, 100, 0},            // EGT
        [PLX_FluidTemp] = {"Temp", "°C", 0, 0, 200, 20, 0},       // Fluid Temp
        [PLX_Vac] = {"VAC", "", 0, -100, 0, 10, 0},               // Vac
        [PLX_Boost] = {"Boost", "", 0, 0, 200, 20, 0},            // Boost
        [PLX_AIT] = {"AIT", "°C", 0, 0, 150, 20, 0},              // AIT
        [PLX_RPM] = {"RPM", "", 0, 0, 6000, 1000, 0},             // RPM
        [PLX_Speed] = {"Speed", "mph", 0, 0, 100, 10, 0},         // Speed
        [PLX_TPS] = {"TPS", "%", 0, 0, 100, 10, 0},               // Throttle Position Sensor
        [PLX_Load] = {"Load", "%", 0, 0, 100, 10, 0},             // Engine Load
        [PLX_FluidPressure] = {"Press", "PSI", 0, 0, 100, 10, 0}, // Fluid Pressure
        [PLX_Timing] = {"Timing", "°", 0, 0, 60, 10, 1},          // Engine timing
        [PLX_MAP] = {"MAP", "mb", 0, 0, 1100, 100, 0},            // MAP
        [PLX_MAF] = {"MAF", "kg", 0, 0, 10, 1, 0},                // MAF
        [PLX_ShortFuel] = {"ShFu", "%", 0, 0, 100, 1, 0},         // Short term fuel trim
        [PLX_LongFuel] = {"LoFu", "%", 0, 0, 100, 1, 0},          // Long term fuel trim
        [PLX_NBO2] = {"NBO2", "", 0, 0, 1, 1, 2},                 // Narrowband O2 sensor
        [PLX_Knock] = {"Knock", "V", 0, 0, 5, 1, 1},              // Knock
        [PLX_Fuel] = {"Fuel", "%", 0, 0, 100, 10, 0},             // Fuel
        [PLX_Duty] = {"Duty", "%", 0, 0, 100, 10, 0},             // Duty cycle
        [PLX_Volts] = {"Volts", "V", 0, 0, 20, 5, 2},             // Volts
        [PLX_X_CHT] = {"CHT", "°C", 0, 0, 200, 20, 0},            // Cylinder head temperature
};
