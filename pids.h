/*
  pids.h

  PID Definitions
  Based on https://en.wikipedia.org/wiki/OBD-II_PIDs

 */

                                                  // Note in the formulas below A = 1st Byte (MSB), B = 2nd Byte, C = 3rd Byte, D = 4th Byte (LSB) from left to right

                                                  //          RAW    RAW   CALC      CALC   CALC  
                                                  // BYTES    MIN    MAX    MIN       MAX  UNITS  FORMULA
                                                  // -----  -----  -----  -----  --------  -----  -------
#define M1_00_PIDS_SUPPORTED_01_20 0x00           //     4                                        Bit encoded [A7..D0] == [PID $01..PID $20] (https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_00)
#define M1_01_MONITOR_STATUS_SINCE_DTSC_CLEARED   //     4                                        Bit encoded (https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_01)
#define M1_02_FREEZE_DTC                          //     2  
#define M1_03_FUEL_SYSTEM_STATUS                  //     2                                        Bit encoded (https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_03)
#define M1_04_CALCULATED_ENGINE_LOAD              //     1      0    255      0       100      %  (100/255)*A 
#define M1_05_ENGINE_COOLANT_TEMPERATURE          //     1      0    255    -40       215     °C  A-40
#define M1_06_SHORT_TERM_FUEL_TRIM_BANK_1         //     1      0    255   -100      99.2         ((100/128)*A) - 100
#define M1_07_LONG_TERM_FUEL_TRIM_BANK_1          //     1      0    255   -100      99.2         ((100/128)*A) - 100
#define M1_08_SHORT_TERM_FUEL_TRIM_BANK_2         //     1      0    255   -100      99.2         ((100/128)*A) - 100
#define M1_09_LONG_TERM_FUEL_TRIM_BANK_2          //     1      0    255   -100      99.2         ((100/128)*A) - 100
#define M1_0A_FUEL_PRESSURE                       //     1      0    255      0       765    kPa  3*A
#define M1_0B_INTAKE_MANIFOLD_ABSOLUTE_PRESSURE   //     1      0    255      0       255    kPa  A
#define M1_0C_ENGINE_RPM                          //     2      0  65535      0  16383.75    rpm  ((256*A)+B)/4
#define M1_0D_VEHICLE_SPEED                       //     1      0    255      0       255   km/h  A
#define M1_0E_TIMING_ADVANCE                      //     1      0    255    -64      63.5      °  (A/2)-64
#define M1_0F_INTAKE_AIR_TEMPERATURE              //     1      0    255    -40       215     °C  A-40
#define M1_10_MAF_AIR_FLOW_RATE                   // 
#define M1_11_THROTTLE_POSITION                   // 
#define M1_COMMANDED_SECONDARY_AIR_STATUS         // 
#define M1_13_OXYGEN_SENSORS_PRESENT_2_BANKS      // 
#define M1_14_OXYGEN_SENSOR_1                     // 
#define M1_15_OXYGEN_SENSOR_2                     // 
#define M1_16_OXYGEN_SENSOR_3                     // 
#define M1_17_OXYGEN_SENSOR_4                     // 
#define M1_18_OXYGEN_SENSOR_5                     // 
#define M1_19_OXYGEN_SENSOR_6                     // 
#define M1_1A_OXYGEN_SENSOR_7                     // 
#define M1_1B_OXYGEN_SENSOR_8                     // 
#define M1_1C_OBD_STANDARDS_CONFORMED_TO          // 
#define M1_1D_OXYGEN_SENSORS_PRESENT_4_BANKS      // 
#define M1_1E_AUXILIARY_INPUT_STATUS              // 
#define M1_1F_RUN_TIME_SINCE_ENGINE_START         // 
