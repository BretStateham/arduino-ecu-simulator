/*
  pids.h

  PID Definitions
  Based on https://en.wikipedia.org/wiki/OBD-II_PIDs

 */

                                                        // Note in the formulas below A = 1st Byte (MSB), B = 2nd Byte, C = 3rd Byte, D = 4th Byte (LSB) from left to right

                                                        //          RAW    RAW   CALC      CALC   CALC  
                                                        // BYTES    MIN    MAX    MIN       MAX  UNITS  FORMULA
                                                        // -----  -----  -----  -----  --------  -----  -------
#define M1_00_PIDS_SUPPORTED_01_20                0x00  //     4                                        Bit encoded [A7..D0] == [PID $01..PID $20] (https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_00)
#define M1_01_MONITOR_STATUS_SINCE_DTSC_CLEARED   0x01  //     4                                        Bit encoded (https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_01)
#define M1_02_FREEZE_DTC                          0x02  //     2  
#define M1_03_FUEL_SYSTEM_STATUS                  0x03  //     2                                        Bit encoded (https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_03)
#define M1_04_CALCULATED_ENGINE_LOAD              0x04  //     1      0    255      0       100      %  (100/255)*A 
#define M1_05_ENGINE_COOLANT_TEMPERATURE          0x05  //     1      0    255    -40       215     °C  A-40
#define M1_06_SHORT_TERM_FUEL_TRIM_BANK_1         0x06  //     1      0    255   -100      99.2         ((100/128)*A) - 100
#define M1_07_LONG_TERM_FUEL_TRIM_BANK_1          0x07  //     1      0    255   -100      99.2         ((100/128)*A) - 100
#define M1_08_SHORT_TERM_FUEL_TRIM_BANK_2         0x08  //     1      0    255   -100      99.2         ((100/128)*A) - 100
#define M1_09_LONG_TERM_FUEL_TRIM_BANK_2          0x09  //     1      0    255   -100      99.2         ((100/128)*A) - 100
#define M1_0A_FUEL_PRESSURE                       0x0A  //     1      0    255      0       765    kPa  3*A
#define M1_0B_INTAKE_MANIFOLD_ABSOLUTE_PRESSURE   0x0B  //     1      0    255      0       255    kPa  A
#define M1_0C_ENGINE_RPM                          0x0C  //     2      0  65535      0  16383.75    rpm  ((256*A)+B)/4
#define M1_0D_VEHICLE_SPEED                       0x0D  //     1      0    255      0       255   km/h  A
#define M1_0E_TIMING_ADVANCE                      0x0E  //     1      0    255    -64      63.5      °  (A/2)-64
#define M1_0F_INTAKE_AIR_TEMPERATURE              0x0F  //     1      0    255    -40       215     °C  A-40
#define M1_10_MAF_AIR_FLOW_RATE                   0x10  //     2      0  65535      0    653.35   gm/s  ((256*A)+B)/100  grams/second  (((256*A)+B)/100)*60 gm/min ((((256*A)+B)/100)*60)/453.592 lb/min
#define M1_11_THROTTLE_POSITION                   0x11  //     1      0    255      0       100      %  (100/255)*A
#define M1_COMMANDED_SECONDARY_AIR_STATUS         0x12  //     1                                        Bit encoded (https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_12) 
#define M1_13_OXYGEN_SENSORS_PRESENT_2_BANKS      0x13  //     1                                        [A0..A3] == Bank 1, Sensors 1-4. [A4..A7] == Bank 2... 
#define M1_14_OXYGEN_SENSOR_1                     0x14  //     2                                        Takes in two bytes (A & B) and returns two bytes, A' & B', defined as follows:
//        A': Voltage                                   //            0    255      0     1.275         A/200
//        B': Short term fuel trim                      //            0    255   -100      99.2         ((100/128)*B)-100  (if B==0xFF, sensor is not used in trim calculation)
#define M1_15_OXYGEN_SENSOR_2                     0x15  //     2                                        Takes in two bytes (A & B) and returns two bytes, A' & B', defined as follows:
//        A': Voltage                                   //            0    255      0     1.275         A/200
//        B': Short term fuel trim                      //            0    255   -100      99.2         ((100/128)*B)-100  (if B==0xFF, sensor is not used in trim calculation)
#define M1_16_OXYGEN_SENSOR_3                     0x16  //     2                                        Takes in two bytes (A & B) and returns two bytes, A' & B', defined as follows:
//        A': Voltage                                   //            0    255      0     1.275         A/200
//        B': Short term fuel trim                      //            0    255   -100      99.2         ((100/128)*B)-100  (if B==0xFF, sensor is not used in trim calculation)
#define M1_17_OXYGEN_SENSOR_4                     0x17  //     2                                        Takes in two bytes (A & B) and returns two bytes, A' & B', defined as follows:
//        A': Voltage                                   //            0    255      0     1.275         A/200
//        B': Short term fuel trim                      //            0    255   -100      99.2         ((100/128)*B)-100  (if B==0xFF, sensor is not used in trim calculation)
#define M1_18_OXYGEN_SENSOR_5                     0x18  //     2                                        Takes in two bytes (A & B) and returns two bytes, A' & B', defined as follows:
//        A': Voltage                                   //            0    255      0     1.275         A/200
//        B': Short term fuel trim                      //            0    255   -100      99.2         ((100/128)*B)-100  (if B==0xFF, sensor is not used in trim calculation)
#define M1_19_OXYGEN_SENSOR_6                     0x19  //     2                                        Takes in two bytes (A & B) and returns two bytes, A' & B', defined as follows:
//        A': Voltage                                   //            0    255      0     1.275         A/200
//        B': Short term fuel trim                      //            0    255   -100      99.2         ((100/128)*B)-100  (if B==0xFF, sensor is not used in trim calculation)
#define M1_1A_OXYGEN_SENSOR_7                     0x1A  //     2                                        Takes in two bytes (A & B) and returns two bytes, A' & B', defined as follows:
//        A': Voltage                                   //            0    255      0     1.275         A/200
//        B': Short term fuel trim                      //            0    255   -100      99.2         ((100/128)*B)-100  (if B==0xFF, sensor is not used in trim calculation)
#define M1_1B_OXYGEN_SENSOR_8                     0x1B  //     2                                        Takes in two bytes (A & B) and returns two bytes, A' & B', defined as follows:
//        A': Voltage                                   //            0    255      0     1.275         A/200
//        B': Short term fuel trim                      //            0    255   -100      99.2         ((100/128)*B)-100  (if B==0xFF, sensor is not used in trim calculation)
#define M1_1C_OBD_STANDARDS_CONFORMED_TO          0x1C  //     1                                        Bit encoded (https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_1C)
#define M1_1D_OXYGEN_SENSORS_PRESENT_4_BANKS      0x1D  //     1                                        Similar to PID 13, but [A0..A7] == [B1S1, B1S2, B2S1, B2S2, B3S1, B3S2, B4S1, B4S2] 
#define M1_1E_AUXILIARY_INPUT_STATUS              0x1E  //     1                                        A0 == Power Take Off (PTO) status (1 == active) [A1..A7] not used 
#define M1_1F_RUN_TIME_SINCE_ENGINE_START         0x1F  //     2      0  65535      0     65535   secs  (256*A) + B seconds
