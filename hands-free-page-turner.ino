#include <ESP32Servo.h>
#include <BluetoothSerial.h>

const int servoPin1 = 25; // Adjust pin numbers as needed for your ESP32
const int servoPin2 = 26;
const int servoPin3 = 27;
const int servoPin4 = 14;
const int servoPin5 = 32;
const int servoPin6 = 33;
const int buttonforward= 34;
const int buttonbackward= 35;
int buttonState=0;
int buttonState2=0;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

BluetoothSerial SerialBT;
String incomingData;        // String to store received data from Bluetooth

int pageTurnAngle = 90; // Adjust based on your servo motor range and desired page turn extent
int turnDelay = 1500;   // Adjust delay between page turns (in milliseconds)
int rev=180;

int bookmark = 0;
bool shouldCountBookmark = false;  // Variable to store the bookmarked page number

void setup() {
  Serial.begin(115200);   // Initialize serial communication for debugging (optional)
  pinMode(buttonforward,INPUT);
  pinMode(buttonbackward,INPUT);

  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
  servo5.attach(servoPin5);
  servo6.attach(servoPin6);

  servo1.write(0); 
  servo2.write(90);
  servo3.write(0); 
  servo4.write(180); 
  servo5.write(200); 
  servo6.write(0); 
  SerialBT.begin("ESP32_Relay_Control");  // Replace with your desired name

}
// Flag to indicate whether bookmark count should increase

void loop() {
  buttonState = digitalRead(buttonforward);
  buttonState2 = digitalRead(buttonbackward);

  if (SerialBT.available()) {
    incomingData = "";  // Clear the incoming data string before reading

    // Read data until newline character is received for complete sentence
    while (SerialBT.available() && SerialBT.peek() != '\n') {
      incomingData += (char)SerialBT.read();
    }

    // Remove the newline character (optional)
    incomingData.trim();
    incomingData.toLowerCase();

    if (incomingData=="next page") {
      turnPageForward();
      delay(turnDelay);
      if (shouldCountBookmark) { // Increment bookmark count if flag is true
        updateBookmark(); // Update bookmark when turning to the next page
      }
    } else if (incomingData=="previous page") {
      turnPageBackward();
      delay(turnDelay);
      if (shouldCountBookmark) { // Increment bookmark count if flag is true
        updateBookmark(); // Update bookmark when turning to the previous page
      }
    } else if (incomingData=="add bookmark") {
      shouldCountBookmark = true; // Set flag to true when "add bookmark" command is received
      bookmark = 0; // Reset bookmark count
    } else if (incomingData=="go to bookmark") {
      goToBookmark();
    }
  }

  if (buttonState == HIGH ) { 
    turnPageForward();
    delay(turnDelay);   
    if (shouldCountBookmark) { // Increment bookmark count if flag is true
      updateBookmark(); // Update bookmark when turning to the next page
    }
  } else if(buttonState2 == HIGH) {
    turnPageBackward();
    delay(turnDelay);
    if (shouldCountBookmark) { // Increment bookmark count if flag is true
      updateBookmark(); // Update bookmark when turning to the previous page
    }
  }
}


void turnPageForward() {
  servo5.write(80);
  servo6.write(100);
  delay(300);
  servo1.write(180);
  delay(turnDelay);
  servo2.write(0);
  delay(turnDelay);  
  
  servo1.write(0); 
  delay(turnDelay);     
  servo3.write(180);  
   delay(300);
   servo2.write(90); 
  delay(300);

    servo3.write(0);
   delay(1000);
  servo2.write(90); 
  delay(turnDelay);

  servo5.write(200);
  servo6.write(0);
}

void turnPageBackward() {
  servo5.write(80);
  servo6.write(100);
  delay(300);

  servo1.write(0); 
  delay(turnDelay); 
  servo2.write(180);
  delay(turnDelay); 

  servo1.write(180); 
  delay(turnDelay); 

  servo4.write(0); 
  delay(1000);  
   servo2.write(90);
     delay(300);

  servo4.write(180);
  delay(300); 
  
  
  servo5.write(200);
  servo6.write(0);
      
}

void goToBookmark() {
  for (int i = 0; i < bookmark; i++) {
    turnPageBackward(); // Go back bookmark times
    delay(turnDelay);
  }

  bookmark=0;
}

void updateBookmark() {
  bookmark++; // Increment the bookmark when turning pages
}
