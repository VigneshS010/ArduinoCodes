#define USE_ARDUINO_INTERRUPTS true  // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>   // Includes the PulseSensorPlayground Library.

//  Variables
const int PulseWire = A0;  // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550;       // Determine which Signal to "count as a beat" and which to ignore.
                           // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                           // Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor;

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int bat = 2;
int buzzer = 3;

int myBPM;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  pinMode(bat, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }
}


void loop() {

  if (pulseSensor.sawStartOfBeat()) {              // Constantly test to see if "a beat happened".
    myBPM = pulseSensor.getBeatsPerMinute();       // Calls function on our pulseSensor object that returns BPM as an "int".
                                                   // "myBPM" hold this BPM value now.
    Serial.println("♥  A HeartBeat Happened ! ");  // If test is "true", print a message "a heartbeat happened".
    Serial.print("BPM: ");                         // Print phrase "BPM: "
    Serial.println(myBPM);                         // Print the value inside of myBPM.
  }

  lcd.setCursor(0, 0);
  lcd.print("BPM: ");
  lcd.print(myBPM);
  lcd.print("    ");

  if (myBPM != 0 && myBPM < 60) {
    lcd.setCursor(0, 1);
    lcd.print("    Abnormal    ");
    digitalWrite(bat, HIGH);
    digitalWrite(buzzer, HIGH);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("     NORMAL     ");
    digitalWrite(bat, LOW);
    digitalWrite(buzzer, LOW);
  }

  delay(20);  // considered best practice in a simple sketch.
}
