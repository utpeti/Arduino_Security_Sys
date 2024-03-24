#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
#include <Servo.h>
#include <Keypad.h>
#include <Wire.h> 

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

LiquidCrystal_I2C lcd(0x3F,20,4);
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
Servo servo;

int led_2 = 11;
int led_3 = 12;
int led_4 = 13;

void setup()
{
  Serial.begin(9600);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  servo.attach(10);
  lcd.begin();
  lcd.backlight();
}

int generateCombination()
{
  int range = 55 - 48 + 1;
  return rand() % range + 48;
}

bool goodAnswer(int in_number, char generated_number)
{
  return in_number == generated_number;
}

void displayCombination(int generated_number)
{
  int generated_int = generated_number - 48;

  //First LED
  Serial.print("First LED:");
  Serial.println(generated_int);
  if(generated_int % 2)
  {
    digitalWrite(led_2, HIGH);
  }
  else
  {
    digitalWrite(led_2, LOW);
  }
  generated_int /= 2;

  //Second LED
  Serial.print("Second LED:");
  Serial.println(generated_int);
  if(generated_int % 2)
  {
    digitalWrite(led_3, HIGH);
  }
  else
  {
    digitalWrite(led_3, LOW);
  }
  generated_int /= 2;

  //Third LED
  Serial.print("Third LED:");
  Serial.println(generated_int);
  if(generated_int % 2)
  {
    digitalWrite(led_4, HIGH);
  }
  else
  {
    digitalWrite(led_4, LOW);
  }
  generated_int /= 2;
}

void displayDelete()
{
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
}

void displayGood()
{
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Yaay :)");
  lcd.setCursor(5,1);
  lcd.print("Proceed!");

  for(int i = 0; i < 30; ++i)
  {
  digitalWrite(led_2, HIGH);
  digitalWrite(led_3, HIGH);
  digitalWrite(led_4, HIGH);
  delay(500);
  
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
  delay(500);
  }

  lcd.clear();
}

void displayWrong()
{
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("WRONG :(");

  for(int i = 0; i < 10; ++i)
  {
  digitalWrite(led_2, HIGH);
  digitalWrite(led_3, HIGH);
  digitalWrite(led_4, HIGH);
  delay(75);
  
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
  delay(75);
  }

  lcd.clear();
}

void displayIdle()
{
  lcd.setCursor(1, 0);
  lcd.print("ENTER PASSWORD!");
}

void loop()
{
  int good_answers = 0;
  while(good_answers < 4)
  {
    displayIdle();
    int generated_number = generateCombination();
    displayCombination(generated_number);
    char in_number = customKeypad.waitForKey();
    Serial.println(generated_number - 48);
    if(goodAnswer(in_number, generated_number))
    {
      ++good_answers;
      Serial.println("+1");
    }
    else
    {
      good_answers = 0;
      displayWrong();
    }
    displayDelete();
  }
  Serial.println("Good answer!");

  //Unlocks door
  int pos;
  for (pos = 0; pos <= 120; ++pos)
  {
    servo.write(pos);
    delay(5);
  }
  delay(1000);
  displayGood();
  for (pos = 120; pos >= 0; --pos)
  {
    servo.write(pos);
    //delay(5);                       
  }
}
