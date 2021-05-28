#include <Servo.h>

Servo clamp;
Servo rotate;
Servo up;
Servo forward;

#define STEPPER_MOTOR_LOWER_VALUE 20
#define STEPPER_MOTOR_HIGHER_VALUE 160
#define NUMBER_MOTORS 4
#define CLAMP '!'
#define ROTATE '@'
#define FORWARD '#'
#define UP '$'

const byte motorDegrees[NUMBER_MOTORS][2] = {
  {0, 180},     // Right arm - x-axis
  {0, 180},     // right arm - y-axis
  {40, 130},    // left arm - x-axis
  {30, 120}     // left arm - y-axis
  };

int pos = 0;
int counter = 0;

void setup()   
{
  Serial.begin(9600);
  clamp.attach(3);
  rotate.attach(5);
  forward.attach(6);
  up.attach(9);
}

float return_mapped_data(float lower_value, float upper_value, float input_value, byte motor) // allows smoother motor movement
{
    float output_value = (input_value - lower_value) * (motorDegrees[motor][1] - motorDegrees[motor][0]) / (upper_value - lower_value) + motorDegrees[motor][0]; // map function
    return output_value;
} 


float read_serial_data(int direction) // direction: 0-3
{
  char rcvdChars[14] = {0};
  unsigned int index = 0;
  char rc;
  
  while(Serial.available() <= 0);

  int i = 0;
  boolean goin = false;
  while(Serial.available() > 0){
    rc = Serial.read();
    switch(direction){
      case 0:
        if(rc == CLAMP){
        goin = true;
        if(Serial.available())
          rc = Serial.read();
        }
        break;

       case 1:
        if(rc == ROTATE){
          goin = true;
          if(Serial.available())
            rc = Serial.read();
        }
        break;

       case 2:
        if(rc == FORWARD){
          goin = true;
          if(Serial.available())
            rc = Serial.read();
        }
        break;

       case 3:
        if(rc == UP){
          goin = true;
          if(Serial.available())
            rc = Serial.read();
        }
        break;
    }
    if(goin && i < 14){
      delay(10);
      if(rc != '\n' && rc != '!'){
        rcvdChars[i] = rc;
        i++;
      }
      
     if(rc == '\n'){
      rcvdChars[i] = '\0';
      goin = false;
      i = 0;
      break;
    } 
    }
  }
    return atof(rcvdChars);
}


int output_array[NUMBER_MOTORS] = {0, 0, 0, 0}; // initial_value


float axis_array[NUMBER_MOTORS][2] = {
    {0, 0.7},       // right x axis
    {-0.6, 0.5},    // right y axis
    {-0.7, 0},      // left x axis
    {-0.6, 0.5}     // left y axis
};

void loop()   
{
    for (int index=0; index < NUMBER_MOTORS; index++)
    {
      float data = read_serial_data(index);
      if(index == 0){
        if(data <= 0.3){
          output_array[index] = 05;
        }else{
          output_array[index] = 175;
        }
      }else{
        output_array[index] = return_mapped_data(axis_array[index][0], axis_array[index][1], constrain(data, axis_array[index][0], axis_array[index][1]), index);
      }
    clamp.write(output_array[0]);
    
    rotate.write(output_array[1]);

    forward.write(output_array[2]);
    
    up.write(output_array[3]);
  }
}
