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

/* Stepper Motots:  steps/rotation  frequncies,1,2,3 etc (* indicates what Brian thinks is better to use)
 * 17HS19-2004S1    200             400*, 1000
 * 17HS4401S        200             600, 1000*
 *
 */

const int stepPinX = 4;        // Connect to the STEP pin of the driver
const int dirPinX = 3;         // Connect to the DIR pin of the driver
const int stepPinY = 6;        // Connect to the STEP pin of the driver
const int dirPinY = 5;         // Connect to the DIR pin of the driver
const int motorSpeed = 600;  
const int stepsPerInch = 633; // After deep calculations, it's 633 steps to move the gantry motor 1 inch linearly

// Constants for rotating stepper
const int stepPinRot = 8;  // Connect to the STEP pin of the rotation driver
const int dirPinRot = 7;   // Connect to the DIR pin of the rotation driver
const int stepsPerRotation = 200; // Steps required for one full 360-degree rotation

int startPositionX = 0;  // In Steps
int currentPositionX = 0; // In steps
int startPositionY = 0;  // In Steps
int currentPositionY = 0; // In steps
int xPos, yPos;
float currentRotation = 0.0; // Current rotation position in degrees

bool letsGo = true;           // Arduino programs run infinitely, this variable for testing allows me to end the loop with a delay.

// Setup function. Here we use pinmode which establishes a step and direction pin
// output setup. The direction pin is a simple high low signal, so if you can only
// move in one direction there is a good chance you have a bad wiring connection
// or wrong pin setup. The step pin allows the motor to count it's steps in a direction.
// Serial.begin allows for printable error messages in arduino IDE.
void setup(){
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);

  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);

  pinMode(stepPinRot, OUTPUT);
  pinMode(dirPinRot, OUTPUT);

  Serial.begin(9600);   // Initialize serial communication at 9600 baud rate
}

// Modified setCenter function to call setX and setY
void setCenter(int xPos, int yPos){
  setX(xPos); // Call setX with the X position
  setY(yPos); // Call setY with the Y position
}

// The setX() function wants to move the gantry to a set distance along it's axis in
// inches. It does this by keeping track of what position the gantry is at by tracking the
// step count so it can always reset back to the origin. Negative numbers are bad, so it resets to 0 instead.
void setX(int targetPositionInInches){
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
  int stepsToMove = targetPositionInInches * stepsPerInch - currentPositionX;
  if(stepsToMove > 0){
    forwardX(stepsToMove);
  } else {
    reverseX(-stepsToMove);
  }
  currentPositionX = targetPositionInInches * stepsPerInch;
}

// The setY() function wants to move the gantry to a set distance along it's axis in
// inches. It does this by keeping track of what position the gantry is at by tracking the
// step count so it can always reset back to the origin. Negative numbers are bad, so it resets to 0 instead.
void setY(int targetPositionInInches){
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
  int stepsToMove = targetPositionInInches * stepsPerInch - currentPositionX;
  if(stepsToMove > 0){
    forwardY(stepsToMove);
  } else {
    reverseY(-stepsToMove);
  }
  currentPositionY = targetPositionInInches * stepsPerInch;
}

void resetCenter(){
  resetX();
  resetY();
}

// Resets the gantry position all the way back to original start. You could also
// setX(0) alternatively, but I like reset functions.
void resetX(){
  setX(0); // Move back to the start position
}

// Resets the gantry position all the way back to original start. You could also
// setX(0) alternatively, but I like reset functions.
void resetY(){
  setY(0); // Move back to the start position
}

// Raw function that moves the motor forwards. Is called uopn by setX, or you can pass in steps
// manually with an integer. The function moves and also updates its new position once it has 
// been moved. Only for the forward (away from motor) direction.
void forwardX(int stepsToMove){
  digitalWrite(dirPinX, LOW);        // Sets the direction
  // This foorloop takes in the raw steps to move in the forward
  // direction. 
  for(int i = 0; i < stepsToMove; i++) {
    digitalWrite(stepPinX, HIGH);    // Writes direction and steps
    delayMicroseconds(motorSpeed); // Speed delay, less delay, faster movement
    digitalWrite(stepPinX, LOW);     
    delayMicroseconds(motorSpeed);
  }
  currentPositionX += stepsToMove;   // Update currentPosition after moving forward
}

// Raw function that moves the motor in reverse. Is called uopn by setX, or you can pass in steps
// manually with an integer. The function moves and also updates its new position once it has 
// been moved. Only for the reverse (towards the motor) direction.
void reverseX(int stepsToMove){
  digitalWrite(dirPinX, HIGH); // Changes the direction
  for(int i = 0; i < stepsToMove; i++) {
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(motorSpeed);
  }
  currentPositionX -= stepsToMove; // Update currentPosition after moving in reverse
}

// Raw function that moves the motor forwards. Is called uopn by setX, or you can pass in steps
// manually with an integer. The function moves and also updates its new position once it has 
// been moved. Only for the forward (away from motor) direction.
void forwardY(int stepsToMove){
  digitalWrite(dirPinY, LOW);        // Sets the direction
  // This foorloop takes in the raw steps to move in the forward
  // direction. 
  for(int i = 0; i < stepsToMove; i++) {
    digitalWrite(stepPinY, HIGH);    // Writes direction and steps
    delayMicroseconds(motorSpeed); // Speed delay, less delay, faster movement
    digitalWrite(stepPinY, LOW);     
    delayMicroseconds(motorSpeed);
  }
  currentPositionY += stepsToMove;   // Update currentPosition after moving forward
}

// Raw function that moves the motor in reverse. Is called uopn by setX, or you can pass in steps
// manually with an integer. The function moves and also updates its new position once it has 
// been moved. Only for the reverse (towards the motor) direction.
void reverseY(int stepsToMove){
  digitalWrite(dirPinY, HIGH); // Changes the direction
  for(int i = 0; i < stepsToMove; i++) {
    digitalWrite(stepPinY, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(stepPinY, LOW);
    delayMicroseconds(motorSpeed);
  }
  currentPositionY -= stepsToMove; // Update currentPosition after moving in reverse
}

// Function to reset the rotation to the initial position (0 degrees)
void resetRotation(){
  setRotation(0); // Call setRotation with 0 to reset to the initial position
}

// Function to rotate to a specified angle
void setRotation(float targetAngle){
  float rotationDifference = targetAngle - currentRotation;
  
  // Normalize the rotation difference to the range [-360, 360]
  rotationDifference = fmod(rotationDifference, 360);
  if (rotationDifference > 180) {
    rotationDifference -= 360;
  } else if (rotationDifference < -180) {
    rotationDifference += 360;
  }

  // Check if rotation exceeds 400 degrees limit
  if (abs(currentRotation + rotationDifference) > 400) {
    Serial.println("Rotation exceeds 400-degree limit. Adjusting rotation direction.");
    if (rotationDifference > 0) {
      // Rotate in the negative direction
      rotationDifference -= 360;
    } else {
      // Rotate in the positive direction
      rotationDifference += 360;
    }
  }

  // Calculate steps to move and set direction
  int stepsToMove = round(rotationDifference / 360.0 * stepsPerRotation);
  digitalWrite(dirPinRot, stepsToMove > 0 ? LOW : HIGH);

  // Move the rotation stepper
  for(int i = 0; i < abs(stepsToMove); i++) {
    digitalWrite(stepPinRot, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(stepPinRot, LOW);
    delayMicroseconds(motorSpeed);
  }

  // Update current rotation position
  currentRotation += rotationDifference;
  // Normalize currentRotation to the range [0, 360)
  currentRotation = fmod(currentRotation, 360);
  if (currentRotation < 0) {
    currentRotation += 360;
  }
}



// Currently all example code to demonstrate greatness
void loop() {
  if (letsGo) {
    // To demonstrate functions forwardX take in raw steps, and reset position
    // always goes back to 0.
    forwardX(stepsPerRotation);
    delay(500);
    reverseX(stepsPerRotation);
    delay(500);
    forwardX(100);
    delay(500);
    resetX(); // Move gantry back to starting position
    delay(2000);

    forwardY(1000);
    delay(500);
    reverseY(800);
    delay(500);
    forwardY(100);
    delay(500);
    resetY(); // Move gantry back to starting position
    delay(2000);

    // Code to demonstrate setX() function and resetX() functions
    setX(2); 
    delay(500);
    setX(5.678); 
    delay(500);
    setX(3.25); 
    delay(500);
    setX(7.252345342645); 
    delay(500);
    resetX(); // Move gantry back to starting position
    delay(2000);

    setY(2); 
    delay(500);
    setY(5.678); 
    delay(500);
    setY(3.25); 
    delay(500);
    setY(7.252345342645); 
    delay(500);
    resetY(); // Move gantry back to starting position
    delay(2000);

    // Code to prevent gantry from breaking itself. There is a safety to prevent reach
    // from negative numbers, as well as above 8 inches.
    setX(100);
    delay(500);
    setX(-5);
    delay(500);
    resetX(); // Move gantry back to starting position
    delay(2000);

    setY(100);
    delay(500);
    setY(-5);
    delay(500);
    resetY(); // Move gantry back to starting position
    delay(2000);


    setCenter(3,1);
    setCenter(0,7);
    setCenter(5,4);
    resetCenter();

    
    letsGo = false; // Prevent further movement, Arduino code runs in permanent loop and this exits operation
  }
  
  delay(500); // Wait for a half a second, effectively stops the program with an infinite delay loop
}
