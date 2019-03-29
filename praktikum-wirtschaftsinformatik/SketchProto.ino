#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

const int leftButton = 14;
const int middleButton = 12;
const int rightButton = 13;
const int ledPin = 2;

const char *projects[] = {
  "Rohrbruch Meyer", 
  "Baustelle A", 
  "Zaehler Gisela"
};
const int projectsCount = sizeof(projects) / sizeof(projects[0]);

int stopwatches[] = { 300, 3600, 18000};
int projectIndex = 1;
int leftButtonState, middleButtonState, rightButtonState;
boolean stopwatchRunning = false;

void handleMiddleButton() {
  Serial.println("middle button interrupt");
  if (stopwatchRunning) {
    stopwatchRunning = false;
    digitalWrite(ledPin, LOW);
    printProject();
  } else {
    stopwatchRunning = true;
    digitalWrite(ledPin, HIGH);
  }   
}

void setup() {
  pinMode(leftButton, INPUT);
  pinMode(middleButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(middleButton, handleMiddleButton, FALLING);
  digitalWrite(ledPin, LOW);
  
  Serial.begin(115200);
  Wire.begin(D2, D1);
  lcd.begin();
  printDisplay("Projektauswahl", "<              >");
}

void printDisplay(String firstLine, String secondLine) {
  lcd.clear();
  lcd.home();
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
}

void printProject() {
  lcd.clear();
  lcd.home();
  lcd.print(projects[projectIndex]);
  lcd.setCursor(0, 1);


  int hours = stopwatches[projectIndex] / 3600;
  int minutes = (stopwatches[projectIndex] - (hours * 3600)) / 60;
  int seconds = stopwatches[projectIndex] - (hours * 3600 ) - (minutes * 60);

  lcd.print("<   ");
  if (hours / 10 < 1) {
    lcd.print("0");
  }
  lcd.print(hours);
  lcd.print(":");
  if (minutes / 10 < 1) {
    lcd.print("0");
  }
  lcd.print(minutes);
  lcd.print(":");
  if (seconds / 10 < 1) {
    lcd.print("0");
  }
  lcd.print(seconds);  
  lcd.print("   >");
  
}

void printStopwatch() {
  lcd.clear();
  lcd.home();
  lcd.print(projects[projectIndex]);
  lcd.setCursor(0, 1);

  int hours = stopwatches[projectIndex] / 3600;
  int minutes = (stopwatches[projectIndex] - (hours * 3600)) / 60;
  int seconds = stopwatches[projectIndex] - (hours * 3600 ) - (minutes * 60);

  lcd.print("    ");
  if (hours / 10 < 1) {
    lcd.print("0");
  }
  lcd.print(hours);
  lcd.print(":");
  if (minutes / 10 < 1) {
    lcd.print("0");
  }
  lcd.print(minutes);
  lcd.print(":");
  if (seconds / 10 < 1) {
    lcd.print("0");
  }
  lcd.print(seconds);  
}


void loop() {
  leftButtonState = digitalRead(leftButton);
  middleButtonState = digitalRead(middleButton);
  rightButtonState = digitalRead(rightButton);

  if (leftButtonState == HIGH && !stopwatchRunning) {
    Serial.println("left");
    projectIndex = (projectIndex - 1 < 0) ? projectsCount-1 : projectIndex -1;
    printProject();
    delay(500);
  } else if (rightButtonState == HIGH && !stopwatchRunning) {
    Serial.println("right");
    projectIndex = (projectIndex + 1 >= projectsCount) ? 0 : projectIndex + 1;
    printProject();
    delay(500);
  }

  if (stopwatchRunning) {
    delay(1000);
    if (stopwatchRunning) {
      stopwatches[projectIndex]++;
      printStopwatch();
    }
  }
}

