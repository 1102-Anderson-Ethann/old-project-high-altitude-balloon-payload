# High-Altitude Balloon Payload

An Arduino-based sensor payload built to collect atmospheric and positional 
data during a high-altitude balloon flight, developed as part of the Spring 2022 
MSE 102 course project at the University of Nevada, Reno.

## Project Overview

The payload was designed to log environmental and GPS data continuously 
throughout a balloon flight. All sensor readings were recorded to an onboard 
data logger at timed intervals, with data surviving the full flight for 
post-mission analysis.

The payload itself was designed to protect the arduino and sensors, while still exposing 
them to the atmosphere. The payload was made out of styrofoam, epoxy, and wood. 

## Sensors & Hardware

| Component | Function |
|---|---|
| SparkFun MS8607 | Temperature, pressure, and humidity |
| SparkFun VEML6030 | Ambient light (lux) |
| SparkFun I2C GPS + TinyGPS++ | Location, altitude, satellite count, HDOP |
| SparkFun Qwiic OpenLog | SD card data logging |
| Qwiic Button | Physical push-to-start trigger |

## Data Logged

Each loop iteration recorded the following columns to a CSV-formatted log file:

`Mission Time | # Satellites | GPS Time | Latitude | Longitude | Altitude | HDOP | Temp (°C) | Pressure (mbar) | Humidity (%RH) | Light (lux)`


## How It Worked

1. On power-up, the payload initialized all sensors and waited for a physical 
button press before beginning data collection — allowing the team to confirm 
everything was connected before launch.
2. Once started, the main loop read all sensors and wrote a complete data row 
to the log every ~15 seconds.
3. `syncFile()` was called each cycle to protect against data loss in the event 
of a power interruption during flight.
4. An LED blink confirmed each successful logging cycle.

## File Structure

| File | Description |
|---|---|
| `Payload_Code_V5.ino` | Final flight version |
| `Payload_code__Data__WX__AmbLight_.ino` | Mid-development integration of weather and light sensors |
| `Data_storage_integration.ino` | Early work integrating the Qwiic OpenLog data logger |
| `Code_review_2` through `Code_review_5` | Iterative revisions across team code reviews |

## Built With

- [Arduino](https://www.arduino.cc/)
- [SparkFun Qwiic ecosystem](https://www.sparkfun.com/qwiic)
- [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus)
