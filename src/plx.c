// This code is derived from a PLX Devices Application note
// which provided ConveriMFDRaw2Data(). I created the inverse
// function to map data the other way.

#include "libPLX/plx.h"
#if defined __cplusplus
extern "C"
{
#endif


	const char *PLX_Obs_Names[] =
		{"AFR", "EGT", "FluidTemp", "Vac", "Boost", "AIT", "RPM", "Speed", "TPS",
		 "Load", "FluidPressure", "Timing", "MAP", "MAF", "ShortFuel", "LongFuel",
		 "NBO2", "Fuel", "Volts", "Knock", "Duty", "CHT"};

	float
	ConveriMFDRaw2Data(enum PLX_Observations sensor, int units, int raw)
	{
		double data = raw; // default assignment
		switch (sensor)
		{
		case PLX_AFR: // Wideband Air/Fuel
			switch (units)
			{
			case 0: // Lambda
				data = (raw / 3.75 + 68) / 100;
				break;
			case 1: // Gasoline 14.7
				data = (raw / 2.55 + 100) / 10;
				break;
			case 2: // Diesel 14.6
				data = (raw / 2.58 + 100) / 10;
				break;
			case 3: // Methanol 6.4
				data = (raw / 5.856 + 43.5) / 10;
				break;
			case 4: // Ethanol 9.0
				data = (raw / 4.167 + 61.7) / 10;
				break;
			case 5: // LPG 15.5
				data = (raw / 2.417 + 105.6) / 10;
				break;
			case 6: // CNG 17.2
				data = (raw / 2.18 + 117) / 10;
				break;
			}
			break;

		case PLX_X_CHT: // CHT extended
		case PLX_AIT:	// AIT
		case PLX_EGT:	// EGT
			switch (units)
			{
			case 0: // Degrees Celsius
				data = raw;
				break;
			case 1: // Degrees Fahrenheit
				data = (raw / .555 + 32);
				break;
			}
			break;

		case PLX_FluidTemp: // Fluid Temp
			switch (units)
			{
			case 0: // Degrees Celsius Water
			case 2: // Degrees Celsius Oil
				data = raw;
				break;
			case 1: // Degrees Fahrenheit Water
			case 3: // Degrees Fahrenheit Oil
				data = (raw / .555 + 32);
				break;
			}
			break;

		case PLX_Vac: // Vac
			switch (units)
			{
			case 0: // in/Hg (inch Mercury)
				data = -(raw / 11.39 - 29.93);
				break;
			case 1: // mm/Hg (millimeters Mercury)
				data = -(raw * 2.23 + 760.4);
				break;
			}
			break;

		case PLX_Boost: // Boost
			switch (units)
			{
			case 0: // 0-30 PSI
				data = raw / 22.73;
				break;
			case 1: // 0-2 kg/cm^2
				data = raw / 329.47;
				break;
			case 2: // 0-15 PSI
				data = raw / 22.73;
				break;
			case 3: // 0-1 kg/cm^2
				data = raw / 329.47;
				break;
			case 4: // 0-60 PSI
				data = raw / 22.73;
				break;
			case 5: // 0-4 kg/cm^2
				data = raw / 329.47;
				break;
			}
			break;

		case PLX_RPM:			// RPM
			data = raw * 19.55; // RPM
			break;
		case PLX_Speed: // Speed
			switch (units)
			{
			case 0: // MPH
				data = raw / 6.39;
				break;
			case 1: // KMH
				data = raw / 3.97;
				break;
			}
			break;
		case PLX_TPS:	// TPS
			data = raw; // Throttle Position %
			break;
		case PLX_Load:	// Engine Load
			data = raw; // Engine Load %
			break;
		case PLX_FluidPressure: // Fluid Pressure
			switch (units)
			{
			case 0: // PSI Fuel
			case 3: // PSI Oil
				data = raw / 5.115;
				break;
			case 1: // kg/cm^2 Fuel
			case 4: // kg/cm^2 Oil
				data = raw / 72.73;
				break;
			case 2: // Bar Fuel
			case 5: // Bar Oil
				data = raw / 74.22;
				break;
			}
			break;
		case PLX_Timing:	 // Engine timing
			data = raw - 64; // Degree Timing
			break;
		case PLX_MAP: // MAP
			switch (units)
			{
			case 0: // kPa
				data = raw;
				break;
			case 1: // inHg
				data = raw / 3.386;
				break;
			}
			break;
		case PLX_MAF: // MAF
			switch (units)
			{
			case 0: // g/s (grams per second)
				data = raw;
				break;
			case 1: // lb/min (pounds per minute)
				data = raw / 7.54;
				break;
			}
			break;
		case PLX_ShortFuel:	  // Short term fuel trim
			data = raw - 100; // Fuel trim %
			break;
		case PLX_LongFuel:	  // Long term fuel trim
			data = raw - 100; // Fuel trim %
			break;
		case PLX_NBO2: // Narrowband O2 sensor
			switch (units)
			{
			case 0: // Percent
				data = raw;
				break;
			case 1: // Volts
				data = raw / 78.43;
				break;
			}
			break;
		case PLX_Fuel:	// Fuel level
			data = raw; // Fuel Level %
			break;
		case PLX_Volts:			// Volts
			data = raw / 51.15; // Volt Meter Volts
			break;
		case PLX_Knock:			// Knock
			data = raw / 204.6; // Knock volts 0-5
			break;
		case PLX_Duty: // Duty cycle
			switch (units)
			{
			case 0: // Positive Duty
				data = raw / 10.23;
				break;
			case 1: // Negative Duty
				data = 100 - (raw / 10.23);
				break;
			}
			break;
		default:
			data = raw;
		}
		return data;
	}

	int
	ConveriMFDData2Raw(enum PLX_Observations sensor, int units, float MFDData)
	{
		float data = MFDData; // default assignment
		int raw = 0;
		switch (sensor)
		{
		case PLX_AFR: // Wideband Air/Fuel
			switch (units)
			{
			case 0: // Lambda
				raw = ((data * 100) - 68) * 3.75;
				break;
			case 1: // Gasoline 14.7
				raw = ((data * 10) - 100) * 2.55;
				break;
			case 2: // Diesel 14.6
				raw = ((data * 10) - 100) * 2.58;
				break;
			case 3: // Methanol 6.4
				raw = ((data * 10) - 43.5) * 5.856;
				break;
			case 4: // Ethanol 9.0
				raw = ((data * 10) - 61.7) * 4.167;
				break;
			case 5: // LPG 15.5
				raw = ((data * 10) - 105.6) * 2.417;
				break;
			case 6: // CNG 17.2
				raw = ((data * 10) - 117) * 2.18;
				break;
			}
			break;

		case PLX_X_CHT: // CHT extended
		case PLX_AIT:	// AIT
		case PLX_EGT:	// EGT
			switch (units)
			{
			case 0: // Degrees Celsius
				raw = data;
				break;
			case 1: // Degrees Fahrenheit
				raw = (data - 32) * 0.555;
				break;
			}
			break;

		case PLX_FluidTemp: // Fluid Temp
			switch (units)
			{
			case 0: // Degrees Celsius Water
			case 2: // Degrees Celsius Oil
				raw = data;
				break;
			case 1: // Degrees Fahrenheit Water
			case 3: // Degrees Fahrenheit Oil
				raw = (data - 32) * 0.555;
				break;
			}
			break;

		case PLX_Vac: // Vac
			switch (units)
			{
			case 0: // in/Hg (inch Mercury)
				raw = ((-data) + 29.93) * 11.39;
				break;
			case 1: // mm/Hg (millimeters Mercury)
				raw = ((-data) - 760.4) / 2.23;
				break;
			}
			break;

		case PLX_Boost: // Boost
			switch (units)
			{
			case 0: // 0-30 PSI
				raw = data * 22.73;
				break;
			case 1: // 0-2 kg/cm^2
				raw = data * 329.47;
				break;
			case 2: // 0-15 PSI
				raw = data * 22.73;
				break;
			case 3: // 0-1 kg/cm^2
				raw = data * 329.47;
				break;
			case 4: // 0-60 PSI
				raw = data * 22.73;
				break;
			case 5: // 0-4 kg/cm^2
				raw = data * 329.47;
				break;
			}
			break;

		case PLX_RPM:			// RPM
			raw = data / 19.55; // RPM
			break;
		case PLX_Speed: // Speed
			switch (units)
			{
			case 0: // MPH
				raw = data * 6.39;
				break;
			case 1: // KMH
				raw = data * 3.97;
				break;
			}
			break;
		case PLX_TPS:	// TPS
			raw = data; // Throttle Position %
			break;
		case PLX_Load:	// Engine Load
			raw = data; // Engine Load %
			break;
		case PLX_FluidPressure: // Fluid Pressure
			switch (units)
			{
			case 0: // PSI Fuel
			case 3: // PSI Oil
				raw = data * 5.115;
				break;
			case 1: // kg/cm^2 Fuel
			case 4: // kg/cm^2 Oil
				raw = data * 72.73;
				break;
			case 2: // Bar Fuel
			case 5: // Bar Oil
				raw = data * 74.22;
				break;
			}
			break;
		case PLX_Timing:	 // Engine timing
			raw = data + 64; // Degree Timing
			break;
		case PLX_MAP: // MAP
			switch (units)
			{
			case 0: // kPa
				raw = data;
				break;
			case 1: // inHg
				raw = data * 3.386;
				break;
			}
			break;
		case PLX_MAF: // MAF
			switch (units)
			{
			case 0: // g/s (grams per second)
				raw = data;
				break;
			case 1: // lb/min (pounds per minute)
				raw = data * 7.54;
				break;
			}
			break;
		case PLX_ShortFuel:	  // Short term fuel trim
			raw = data + 100; // Fuel trim %
			break;
		case PLX_LongFuel:	  // Long term fuel trim
			raw = data + 100; // Fuel trim %
			break;
		case PLX_NBO2: // Narrowband O2 sensor
			switch (units)
			{
			case 0: // Percent
				raw = data;
				break;
			case 1: // Volts
				raw = data * 78.43;
				break;
			}
			break;
		case PLX_Fuel:	// Fuel level
			raw = data; // Fuel Level %
			break;
		case PLX_Volts:			// Volts
			raw = data * 51.15; // Volt Meter Volts
			break;
		case PLX_Knock:			// Knock
			raw = data * 204.6; // Knock volts 0-5
			break;
		case PLX_Duty: // Duty cycle
			switch (units)
			{
			case 0: // Positive Duty
				raw = data * 10.23;
				break;
			case 1: // Negative Duty
				raw = (100 - data) * 10.23;
				break;
			}
			break;
		default:
			raw = data;
		}
		return raw;
	}

#if defined __cplusplus
}
#endif
