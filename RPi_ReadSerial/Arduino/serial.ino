/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}


/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13 through 220 ohm resistor
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInput
*/

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
  // stop the program for <sensorValue> milliseconds:
  delay(sensorValue);
  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
  // stop the program for for <sensorValue> milliseconds:
  delay(sensorValue);
}



/*
mine
*/

// defines
constexpr int BAUD_RATE = 9600;
constexpr double MAX_POTI_VAL = 10000.0;

// values
constexpr double MIN_BRIGHTNESS = 0.0;
constexpr double MAX_BRIGHTNESS = 255.0;

constexpr double MIN_FFT_VAL = 2.0;
constexpr double MAX_FFT_VAL = 20.0;

constexpr double MIN_DB_VAL = 5.0;
constexpr double MAX_DB_VAL = 30.0;

constexpr double MIN_BIN_VAL = 20.0;
constexpr double MAX_BIN_VAL = 300.0;

constexpr double MIN_COLOR = 20.0;
constexpr double MAX_COLOR = 300.0;

template<typename T> 
T ScaleForOutput(const double& in, const double& min, const double& max)
{
    return static_cast<T>(in * (max - min) + min);
}

// message types
enum Messages : uint8_t
{
    Brightness = 1,     // change brighness
    FFTScale = 2,       // update FFTScale
    DBScale = 3,        // update DB Scale
    MusicMode = 4,      // change to music mode
    AmbientMode = 5,    // change to ambient
    Red = 6,            // change r component of ambient
    Green = 7,          // change g component of ambient
    Blue = 8,           // change b component of ambient
    FreqPerBin = 9      // for placing bins
};

// state
enum States : uint8_t
{
    MusicMode,
    AmbientMode
};

// buttons
constexpr int BUTTON1 = 10;

// Potis
constexpr int POTI1 = A0; 
constexpr int POTI2 = A1; 
constexpr int POTI3 = A2; 
constexpr int POTI4 = A3; 

// values
double poti1_val = 0.0;
double poti2_val = 0.0;
double poti3_val = 0.0;
double poti4_val = 0.0;

// Mode
States state = States::AmbientMode;

void setup() 
{
    Serial.begin(BAUD_RATE);
    pinMode(BUTTON1, INPUT_PULLUP);
}

template <typename T>
void send_message(Messages msg, T value)
{
    Serial.print(msg);
    Serial.print(" ");
    Serial.println(value);
}

int buttonStatus = 0;

void loop()
{

    double sensorValue;
    
    // read 1. poti
    sensorValue = analogRead(POTI1) / MAX_POTI_VAL;
    if (sensorValue != poti1_val)
    {
        poti1_val = sensorValue;
        send_message<int>(Messages::Brightness, ScaleForOutput<int>(poti1_val, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
    }

    // read 2. poti
    sensorValue = analogRead(POTI2) / MAX_POTI_VAL;
    if (sensorValue != poti2_val)
    {
        poti2_val = sensorValue;
        if (state == States::AmbientMode)
            send_message<float>(Messages::Red, ScaleForOutput<float>(poti2_val, MIN_COLOR, MAX_COLOR));
        else if (state == States::MusicMode)
            send_message<float>(Messages::FFTScale, ScaleForOutput<float>(poti2_val, MIN_FFT_VAL, MAX_FFT_VAL));
    }

    // read 3. poti
    sensorValue = analogRead(POTI3) / MAX_POTI_VAL;
    if (sensorValue != poti3_val)
    {
        poti3_val = sensorValue;
        if (state == States::AmbientMode)
            send_message<float>(Messages::Green, ScaleForOutput<float>(poti2_val, MIN_COLOR, MAX_COLOR));
        else if (state == States::MusicMode)
            send_message<float>(Messages::DBScale, ScaleForOutput<float>(poti3_val, MIN_DB_VAL, MAX_DB_VAL));
    }

    // read 4. poti
    sensorValue = analogRead(POTI4) / MAX_POTI_VAL;
    if (sensorValue != poti4_val)
    {
        poti4_val = sensorValue;
        if (state == States::AmbientMode)
            send_message<float>(Messages::Blue, ScaleForOutput<float>(poti2_val, MIN_COLOR, MAX_COLOR));
        else if (state == States::MusicMode)
            send_message<float>(Messages::FreqPerBin, ScaleForOutput<float>(poti4_val, MIN_BIN_VAL, MAX_BIN_VAL));
    }

    // read Mode Switch
    int pinValue = digitalRead(BUTTON1);
    if (buttonStatus != pinValue)
    {
        buttonStatus = pinValue;
        Serial.println("pressed");

        if (state == States::MusicMode)
        {
            send_message<int>(Messages::AmbientMode, 1);
            state = States::AmbientMode;
        }
        else if (state == States::AmbientMode)
        {
            send_message<int>(Messages::MusicMode, 1);
            state = States::MusicMode;
        }
    }   

    delay(50);

}

  