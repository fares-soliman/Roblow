#include <StopWatch.h>
#include <Sabertooth.h>
#include <NewPing.h>

#define TRIGGER_PIN_LEFT 12
#define ECHO_PIN_LEFT 11
#define TRIGGER_PIN_RIGHT 14
#define ECHO_PIN_RIGHT 13
#define MAX_DISTANCE 200

//width in inches
#define WIDTH_OF_ROBOT 61
#define WIDTH_OF_DRIVEWAY 580


Sabertooth ST(128);


NewPing sonarLeft(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE);
NewPing sonarRight(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE); 

//using a timer for the return run (goigng up the driveway, it senses a car or the garage, but going down, how will it know when to stop? Timer!)
StopWatch MySW;

void setup() {
  SabertoothTXPinSerial.begin(9600);
  ST.autobaud();

}

void loop() {
  
  //right motor starts first (assuming snowblower is at the bottom right of the driveway on the edge of the driveway and lawn
  ST.motor(1,10);
  delay(2000);

  int drivewayLength = floor(WIDTH_OF_DRIVEWAY / WIDTH_OF_ROBOT);
  int singleRun = 1;
  
  while (singleRun <= drivewayLength) {
    MySW.start();
    
    //arbitrary initialization
    int cmLeft = 500;
    int cmRight = 500;
    
    while (cmLeft >= 100 && cmRight >= 100 && cmLeft != 0 && cmRight != 0) {
      ST.motor(1,10);
      ST.motor(1,10);
      cmLeft = sonarLeft.ping_cm();
      cmRight = sonarRight.ping_cm();
    }
    
    MySW.stop();
    int singleRunTime = MySW.elapsed();
    MySW.start();

    //reversing, cant make it turn incase it runs between two cars
    while (MySW.elapsed() <= singleRunTime) {
      ST.motor(1, -10);
      ST.motor(2, -10);
    }

    //adjusting itself for next run (turning, then moving a bit forward along the bottom of the driveway)
    ST.motor(1,-10);
    delay(2000);

    ST.motor(1,10);
    ST.motor(1,10);
    delay(1500);

    singleRun++;
  }

}
