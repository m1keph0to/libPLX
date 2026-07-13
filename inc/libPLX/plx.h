// this file defines various constants

#if !defined __INC_PLX_H
#define __INC_PLX_H

#if defined __cplusplus
extern "C"
{
#endif

  // This data is sent at 19200 baud 8,n,1. TTL level.
  // No data value is greater than 0x3F.
  // Only start and stop bytes can have bit 6 or 7 set
  // 12 bit constants are sent in two bytes
  typedef enum
  {
    PLX_Start = 0x80u,
    PLX_Stop = 0x40u,
  } PLX_Header;

  // theoretically the value is up to PLX_MAX_INST : enums are constants
  enum
  {
    PLX_MAX_INST = 0x3F
  };
  // practically the value is limited to 0..3 : enums are constants
  enum
  {
    PLX_MAX_INST_LIMIT = 4
  };

  extern const char *PLX_Obs_Names[];
  enum PLX_Observations
  {
    PLX_AFR = 0,            // Wideband Air/Fuel
    PLX_EGT = 1,            // Exhaust Gas Temperature
    PLX_FluidTemp = 2,      // Fluid Temp
    PLX_Vac = 3,            // Vacuum
    PLX_Boost = 4,          // Boost pressure
    PLX_AIT = 5,            // Air Intake Temperature
    PLX_RPM = 6,            // RPM
    PLX_Speed = 7,          // Speed
    PLX_TPS = 8,            // TPS
    PLX_Load = 9,           // Engine Load
    PLX_FluidPressure = 10, // Fluid Pressure
    PLX_Timing = 11,        // Engine timing
    PLX_MAP = 12,           // MAP
    PLX_MAF = 13,           // MAF
    PLX_ShortFuel = 14,     // Short term fuel trim
    PLX_LongFuel = 15,      // Long term fuel trim
    PLX_NBO2 = 16,          // Narrowband O2 sensor
    PLX_Fuel = 17,          // Fuel level
    PLX_Volts = 18,         // Volts
    PLX_Knock = 19,         // Knock
    PLX_Duty = 20,          // Duty cycle
    PLX_X_CHT = 21,         // Extended observations for aircooled engine
    PLX_MAX_OBS
  };

  enum PLX_AFR_Units
  {
    AFR_Lambda,
    AFR_Gasoline,
    AFR_Diesel,
    AFR_Methanol,
    AFR_Ethanol,
    AFR_LPG,
    AFR_CNG

  };

  enum PLX_TEMP_Units
  {
    TEMP_Celsius,
    TEMP_Fahrenheit
  };

  enum PLX_PRESSURE_Units
  {
    PRESSURE_PSI_Fuel,
    PRESSURE_KGCM2_Fuel,
    PRESSURE_Bar_Fuel,
    PRESSURE_PSI_Oil,
    PRESSURE_KGCM2_Oil,
    PRESSURE_Bar_Oil
  };

  enum PLX_MAP_Units
  {
    PRESSURE_kPa,
    PRESSURE_InHg
  };

  /// @brief Convert MFD data to float observation
  /// @param  sensor Observation index
  /// @param units what units the result is expected in
  /// @param raw the raw PLX data
  /// @return floating point observation
  extern float
  ConveriMFDRaw2Data(enum PLX_Observations sensor, int units, int raw);

  /// @brief Convert sensor data to MFD Data
  /// @param sensor observation index
  /// @param units  what units the data is provided in
  /// @param MFDData the observation
  /// @return integer coded data
  extern int
  ConveriMFDData2Raw(enum PLX_Observations sensor, int units, float MFDData);

#pragma pack(push, 1)
  /// @brief Structure representing on the wire view of data structure
  typedef union
  {
    struct
    {
      char AddrH;    ///< High byte of sensor index
      char AddrL;    ///< Low byte of sensor index
      char Instance; ///< Instance number of sensor in system
      char ReadingH; ///< High byte encoded value
      char ReadingL; ///< Low byte encoded value
    };
    unsigned char bytes[5];
  } PLX_SensorInfo;
#pragma pack(pop)

  /// @brief Convert from two bytes PLX data to integer
  /// @param H high byte
  /// @param L low byte
  /// @return integer value
  static inline int
  ConvPLX(char H, char L)
  {
    return ((H & 0x3F) << 6) | (L & 0x3F);
  }

  /// @brief Get the address from the SensorInfo
  /// @param src Pointer to data structure
  /// @return value
  static inline int
  ConvPLXAddr(PLX_SensorInfo *src)
  {
    return ConvPLX(src->AddrH, src->AddrL);
  }

  /// @brief Get the reading from the SensorInfo
  /// @param src Pointer to data structure
  /// @return value
  static inline int
  ConvPLXReading(PLX_SensorInfo *src)
  {
    return ConvPLX(src->ReadingH, src->ReadingL);
  }

  /// @brief Convert to PLX Data bytes
  /// @param val value to convert
  /// @param dest Data structure
  static inline void
  ConvToPLXReading(int val, PLX_SensorInfo *dest)
  {
    dest->ReadingH = (val >> 6) & 0x3F;
    dest->ReadingL = (val & 0x3F);
  }

  /// @brief Convert to PLX Address bytes
  /// @param val value to convert
  /// @param dest Data structure
  static inline void ConvToPLXAddr(int val, PLX_SensorInfo *dest)
  {
    dest->AddrH = (val >> 6) & 0x3F;
    dest->AddrL = (val & 0x3F);
  }

  /// @brief Convert to PLX Instance bytes
  /// @param instance value to convert
  /// @param dest Data structure
  static inline void ConvToPLXInstance(int val, PLX_SensorInfo *dest)
  {
    dest->Instance = (val & 0x3F);
  }

#if defined __cplusplus
}
#endif

#endif
