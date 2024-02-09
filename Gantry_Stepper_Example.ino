/* Gantry Stepper Exmaple 
 * Code here is us learning how to write code for a stepper motor that the
 * teensy can control. For this particular code we used a stepper motor loaned
 * from the PSC so some things like motor speed and steps per inch may need to
 * be changed, as well as the driving mechanism be taken into account for 
 * (Gantry in this case)
 * For this code, it was designed to have the gantry start as close to the rail
 * of the stepper motor as possible. Then it counts and keeps track of steps as
 * the code operates, as well as is able to receive a command to go to a set
 * position and update itself accurately.
 */

const int stepPin = 4;        // Connect to the STEP pin of the driver
const int dirPin = 3;         // Connect to the DIR pin of the driver
const int motor_speed = 600;  // Fastest speed seems minimum 300, smoothest at 500 to 600. For more power 1000 works.
const int stepsPerInch = 633; // After deep calculations, it's 633 steps to move the gantry motor 1 inch linearly
bool letsGo = true;           // Arduino programs run infinitely, this variable for testing allows me to end the loop with a delay.

int startPosition = 0;  // In Steps
int currentPosition = 0; // In steps

// Setup function. Here we use pinmode which establishes a step and direction pin
// output setup. The direction pin is a simple high low signal, so if you can only
// move in one direction there is a good chance you have a bad wiring connection
// or wrong pin setup. The step pin allows the motor to count it's steps in a direction.
// Serial.begin allows for printable error messages in arduino IDE.
void setup(){
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(9600);   // Initialize serial communication at 9600 baud rate
}

// The setPosition() function wants to move the gantry to a set distance along it's axis in
// inches. It does this by keeping track of what position the gantry is at by tracking the
// step count so it can always reset back to the origin. Negative numbers are bad, so it resets to 0 instead.
void setPosition(int targetPositionInInches){
  // Limit the target position to a maximum of 8 inches
  if (targetPositionInInches > 8) {
    Serial.println("Target position exceeds maximum range of 8 inches. Will limit reach to 8 inches.");
    targetPositionInInches = 8; // Limit the target position to 8 inches
  }
  else if(targetPositionInInches < 0) {
    Serial.println("Target position invalid, no negative numbers. Will reset gantry position to 0.");
    targetPositionInInches = 0;

  }

  // After checking position, calls to move either forward or reverse.
  int stepsToMove = targetPositionInInches * stepsPerInch - currentPosition;
  if(stepsToMove > 0){
    gantryForward(stepsToMove);
  } else {
    gantryReverse(-stepsToMove);
  }
  currentPosition = targetPositionInInches * stepsPerInch;
}

// Resets the gantry position all the way back to original start. You could also
// setPosition(0) alternatively, but I like reset functions.
void resetPosition(){
  setPosition(0); // Move back to the start position
}

// Raw function that moves the motor forwards. Is called uopn by setPosition, or you can pass in steps
// manually with an integer. The function moves and also updates its new position once it has 
// been moved. Only for the forward (away from motor) direction.
void gantryForward(int stepsToMove){
  digitalWrite(dirPin, LOW);        // Sets the direction
  // This foorloop takes in the raw steps to move in the forward
  // direction. 
  for(int i = 0; i < stepsToMove; i++) {
    digitalWrite(stepPin, HIGH);    // Writes direction and steps
    delayMicroseconds(motor_speed); // Speed delay, less delay, faster movement
    digitalWrite(stepPin, LOW);     
    delayMicroseconds(motor_speed);
  }
  currentPosition += stepsToMove;   // Update currentPosition after moving forward
}

// Raw function that moves the motor in reverse. Is called uopn by setPosition, or you can pass in steps
// manually with an integer. The function moves and also updates its new position once it has 
// been moved. Only for the reverse (towards the motor) direction.
void gantryReverse(int stepsToMove){
  digitalWrite(dirPin, HIGH); // Changes the direction
  for(int i = 0; i < stepsToMove; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(motor_speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(motor_speed);
  }
  currentPosition -= stepsToMove; // Update currentPosition after moving in reverse
}
// Currently all example code to demonstrate greatness
void loop() {
  if (letsGo) {
    // To demonstrate functions gantryForward take in raw steps, and reset position
    // always goes back to 0.
    gantryForward(1000);
    delay(500);
    gantryReverse(800);
    delay(500);
    gantryForward(100);
    delay(500);
    resetPosition(); // Move gantry back to starting position
    delay(2000);

    // Code to demonstrate setPosition() function and resetPosition() functions
    setPosition(2); 
    delay(500);
    setPosition(5.678); 
    delay(500);
    setPosition(3.25); 
    delay(500);
    setPosition(7.252345342645); 
    delay(500);
    resetPosition(); // Move gantry back to starting position
    delay(2000);

    // Code to prevent gantry from breaking itself. There is a safety to prevent reach
    // from negative numbers, as well as above 8 inches.
    setPosition(100);
    delay(500);
    setPosition(-5);
    delay(500);
    resetPosition(); // Move gantry back to starting position
    delay(2000);
    letsGo = false; // Prevent further movement, Arduino code runs in permanent loop and this exits operation

  }
  
  delay(500); // Wait for a half a second, effectively stops the program with an infinite delay loop
}
