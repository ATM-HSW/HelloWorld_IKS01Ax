# X_NUCLEO_IKS01Ax(1,2 and 3)

Firmware Library for X-NUCLEO-IKS01Ax(1,2 and 3) (MEMS Inertial & Environmental Sensors) Expansion Boards

## Introduction

This firmware package includes Components Device Drivers and Board Support Package for STMicroelectronics' X-NUCLEO-IKS01Ax MEMS Inertial & Environmental Sensors Nucleo Expansion Board.

## Firmware Library
The classes X_NUCLEO_IKS01Ax are intended to represent the MEMS inertial & environmental sensors expansion board with the same name.

The expansion boards are basically featuring following IPs:

a HTS221 Relative Humidity and Temperature Sensor (IKS01A1, IKS01A2, IKS01A3)
a STTS751 MEMS Temperature Sensor (IKS01A3)
a LPS22HH MEMS Pressure and Temperature Sensor (IKS01A3)
a LPS22HB MEMS Pressure and Temperature Sensor (IKS01A2)
a LPS25H MEMS Pressure Sensor (IKS01A1)
a LIS2MDL 3-Axis Magnetometer (IKS01A3)
a LIS3MDL 3-Axis Magnetometer (IKS01A1)
a LIS2DW12 3-Axis Accelerometer (IKS01A3)
a LSM303AGR 3-Axis Magnetometer and 3-Axis Accelerometer (IKS01A2)
a LSM6DSO 3-Axis Accelerometer and 3-Axis Gyroscope (IKS01A1, IKS01A3)
a LSM6DSL 3-Axis Accelerometer and 3-Axis Gyroscope (IKS01A2)


The expansion board features also a DIL 24-pin socket which makes it possible to add further MEMS adapters and other sensors (e.g. UV index).

It is intentionally implemented as a singleton because only one X_NUCLEO_IKS01Ax at a time might be deployed in a HW component stack. In order to get the singleton instance you have to call class method `instance()`, e.g.:

```
// Sensors expansion board singleton instance
static X_NUCLEO_IKS01A1 *sensors_expansion_board = X_NUCLEO_IKS01A1::Instance(D14, D15, D4);
```

```
// Sensors expansion board singleton instance
static X_NUCLEO_IKS01A2 *mems_expansion_board = X_NUCLEO_IKS01A2::instance(D14, D15, D4, D5);
```

```
// Sensors expansion board singleton instance
static XNucleoIKS01A3 *mems_expansion_board = XNucleoIKS01A3::instance(D14, D15, D4, D5, A3, D6, A4);
```

Furthermore, library ST_INTERFACES contains all abstract classes which together constitute the common API to which all existing and future ST components will adhere to.