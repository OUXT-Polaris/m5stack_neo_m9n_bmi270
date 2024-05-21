/// See also https://www.hackster.io/ptschulik/simple-gps-tracker-d3500e
/// See also https://qiita.com/ryokurta256/items/dafba366a9c16626e7d2

#include <Arduino.h>

#include <M5Stack.h>   /* Include M5 Stack library */
#include <TinyGPS++.h> /* Include Tiny GPS++ library */
#include <Wire.h>

#include <gpsdata.pb.h>

/* Constants */
/// Delay between two reads: 10 = 100ms
const byte DELAY_READ = 10;
/// Delay to check if any data received 100 * 10 = 1 sec
const byte DELAY_CHECK_CONST = 100;
/// Minumum number of satellites before fix is accepted
const byte MINUMUM_SAT = 4;
/// Time difference from UTC to CET
const byte UTC_CET = 2;

/* Variables */
/// GPS format 0 = Decimal degree, 1= Degree, Minutes, Seconds
byte GPS_FORMAT = 0;
/// GPS status: 0 = No GPS receiver detected, 1 = No valid GPS fix, 2 = Valid
byte GPS_STATUS;

/// Delay to check if any data received
byte DELAY_CHECK = 0;

/// Number of satellites
unsigned int NO_OF_SAT;
/// Number of procesed chars
unsigned int CHARS_PROCESSED = 0;
/// Number of procesed chars
unsigned int OLD_CHARS_PROCESSED = 1;

/// Reference the TinyGPS++ object
TinyGPSPlus gps;
/// By default, GPS is connected with M5Core through UART2
HardwareSerial GPSRaw(2);

void setup() {
  M5.begin();
  GPSRaw.begin(9600);
  /// 2000ms init delay
  delay(2000);
}

void loop() {
  while (GPSRaw.available() > 0) {
    gps.encode(GPSRaw.read());
  }

  /// Read processed chars
  CHARS_PROCESSED = gps.charsProcessed();

  /* No chars processed or no more data received --> Checked every 10ms ? */
  if ((CHARS_PROCESSED < 10) || (CHARS_PROCESSED == OLD_CHARS_PROCESSED)) {
    if (DELAY_CHECK < 230) /* Prevent variable from overflow */
    {
      DELAY_CHECK =
          DELAY_CHECK +
          10; /* Increase delay check if any data received each 10ms by 10 */
    }
  } else /* No chars received ? */
  {
    DELAY_CHECK = 0; /* Reset delay check if any data received */
  }
}

// put function definitions here:
int myFunction(int x, int y) { return x + y; }