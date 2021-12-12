#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SW 4
#define SWL 12
#define SWR 13

int16_t r=10; 
int16_t x, y;
int score=0, high=0, counter=20;
//int dots[];
long randx, randy;
unsigned long startMillis, currentMillis, scoreMillis, counterMillis;
bool scoreable = true;
  
void setup() {
  Serial.begin(115200); // initialize serial communication at 115200 bits per second:

  pinMode(SW,INPUT);
  digitalWrite(SW,HIGH);
  pinMode(SWL,INPUT);
  digitalWrite(SWL,LOW);
  pinMode(SWR,INPUT);
  digitalWrite(SWR,LOW);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Hello, world!");
  display.display();
  delay(1000);

  x = display.width()/2;
  y = display.height()/2;

  randomSeed(analogRead(0));
}

void loop() {
  Serial.print("VRX_value=");
  int X_value=analogRead(A3);
  Serial.print(X_value);
  Serial.print("\t");

  Serial.print("VRY_value=");
  int Y_value=analogRead(A2);
  Serial.print(Y_value);
  Serial.print("\t");

  Serial.print("Switch_value=");
  int switch_value=digitalRead(SW);
  Serial.print(switch_value);
  Serial.print("\t");

  Serial.print("Switch_value=");
  int swl_value=digitalRead(SWL);
  Serial.print(switch_value);
  Serial.print("\t");

  Serial.print("Switch_value=");
  int swr_value=digitalRead(SWR);
  Serial.print(switch_value);
  Serial.print("\t");

  int disp=score;
  
//  delay(1000);

  // Clear the buffer
  display.clearDisplay();
  delay(1);
  
  // Draw a single pixel in white
//  display.drawPixel(60, 30, SSD1306_WHITE);
//  delay(1);

  if (X_value > 550 && x < display.width()) {
    x+=5;
  }
  if (X_value < 500 && x > 0) {
    x-=5;
  }
  if (Y_value > 550 && y < display.height()) {
    y+=5;
  }
  if (Y_value < 500 && y > 0) {
    y-=5;
  }
  if (swl_value==HIGH) {
    score=0;
  }
  if (swr_value==HIGH) {
    disp=high;
  }
  Serial.print("x=");
  Serial.print(x);
  Serial.print("\t");
  Serial.print("y=");
  Serial.print(y);
  Serial.print("\t");
  Serial.print("r=");
  Serial.print(r);
  Serial.println("\t");

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= 2000)  //test whether the period has elapsed
  {
    randx = random(127);
    randy = random(63);
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
  display.fillCircle(randx, randy, 5, SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print(disp);
  if (millis() - scoreMillis > 1000) {
    scoreable=true;
  }
  display.setCursor(115,0);
  display.print(counter);
  if (millis() - counterMillis > 1000) {
    counter--;
    if (counter < 0) {
      counter=10;
      score=0;
    }
    counterMillis=millis();
  }
  if (switch_value==0) {
    display.drawRect(x-r, y-r, 2*r, 2*r, SSD1306_WHITE);
    if (randx > x-r && randx < x+r && randy > y-r && randy < y+r && scoreable) {
      score++;
      scoreable=false;
      scoreMillis=millis();
      if (score>high) {
        high=score;
      }
    }
  } else {
    display.drawCircle(x, y, r, SSD1306_WHITE);
  }
//  display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
//  display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
//  display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(1);
}
