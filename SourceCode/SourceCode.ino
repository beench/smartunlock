#include <avr/io.h>
#include <util/delay.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);
#include <Servo.h>
Servo servo;

char password[4];
char RealPassword[] = "1111";
int count = 0, Repassword = 0, chtrue = 0, sec = 10, mins = 57, hr = 21, sensor = 0;
long count_1 = 0, count_2 = 0;

void init_peripheral() {

  DDRD = (DDRD | 0b01101011); //output
  PORTD = (PORTD | 0b01101011);
  DDRB = (DDRB & ~(0b00001111)); //input
  PORTB = (PORTB | 0b00001111); //open pull up(input)

  DDRC = (DDRC | 0b00000001); //output
  PORTC = (PORTC | 0b00000001);

}

void BeepTrue() {

  for (uint16_t i = 0; i < 2000; i++) {
    PORTD ^= (1 << PD6);
    _delay_us(200);
  }
  sec += 2;

}

void BeepFalse() {

  for (uint16_t i = 0; i < 1200; i++) {
    PORTD ^= (1 << PD6);
    _delay_us(1000);
  }
  sec += 2;

}

void Beep() {

  for (uint16_t i = 0; i < 700; i++) {
    PORTD ^= (1 << PD6);
    _delay_us(300);
  }
  sec += 0.7;

}

void BeepAlarm() {

  for (uint16_t i = 0; i < 15000; i++) {
    PORTD ^= (1 << PD6);
    _delay_us(150);
  }
  sec += 1;

}

int checkPassword(char *password, char *RealPassword) {

  for (int i = 0; i < 4; i++) {
    if (password[i] != RealPassword[i])
      return 0;
  }
  return 1;

}

int changePassword(char *password) {

  for (int i = 0; i < 4; i++) {
    if (password[i] == '#')
      return 1;
  }
  return 0;

}

void Pressed(int count, char *password) {

  while (count < 4) {
    for (int output = 0; output < 3; output++) { //col
      
      if (output == 2)
        PORTD &= ~(1 << 5);
      else
        PORTD &= ~(1 << output);
      _delay_ms(5);
      
      for (int input = 0; input < 4; input++) {

        if ((PINB & (1 << input)) == 0) {

          if (output == 0 && input == 0) {
            password[count] = '3';
          }
          else if (output == 1 && input == 0) {
            password[count] = '2';
          }
          else if (output == 2 && input == 0) {
            password[count] = '1';
          }
          else if (output == 0 && input == 1) {
            password[count] = '6';
          }
          else if (output == 1 && input == 1) {
            password[count] = '5';
          }
          else if (output == 2 && input == 1) {
            password[count] = '4';
          }
          else if (output == 0 && input == 2) {
            password[count] = '9';
          }
          else if (output == 1 && input == 2) {
            password[count] = '8';
          }
          else if (output == 2 && input == 2) {
            password[count] = '7';
          }
          else if (output == 0 && input == 3) {
            password[count] = '#';
          }
          else if (output == 1 && input == 3) {
            password[count] = '0';
          }
          else if (output == 2 && input == 3) {
            password[count] = '*';
          }
          Beep();
          count++;
          PressedKey(count);
        }
      }
      if (output == 2)
        PORTD |= (1 << 5);
      else
        PORTD |= (1 << output);
    }
  }
}

void Enter() {

  Clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
  lcd.print("_ _ _ _");

}

void Unlock() {

  Clear();
  lcd.setCursor(0, 0);
  lcd.print("     Unlock!");
  lcd.setCursor(0, 1);
  lcd.print("     Welcome");
  sec += 10;

}

void Error() {

  Clear();
  lcd.setCursor(0, 0);
  lcd.print("     Error!");
  lcd.setCursor(0, 1);
  lcd.print("  Please Retry  ");

}

void OldPassError() {

  Clear();
  lcd.setCursor(0, 0);
  lcd.print(" Old Password");
  lcd.setCursor(0, 1);
  lcd.print("     Error!  ");

}

void PressedKey(int order) {

  lcd.setCursor(2 * order - 2, 1);
  lcd.print("* ");

}

void EnterNewPass() {

  Clear();
  lcd.setCursor(0, 0);
  lcd.print("New Password");
  lcd.setCursor(0, 1);
  lcd.print("_ _ _ _");
  Pressed(0, password);
  for (int i = 0; i < 4; i++) {
    RealPassword[i] = password[i];
  }

}

void EnterOldPass() {

  Clear();
  lcd.setCursor(0, 0);
  lcd.print("Old Password");
  lcd.setCursor(0, 1);
  lcd.print("_ _ _ _");

}

void Bye() {

  Clear();
  lcd.setCursor(0, 0);
  lcd.print("You can't unlock");
  lcd.setCursor(0, 1);
  lcd.print("   Bye Bye ^^ ");


}

void Clear() {

  lcd.setCursor (0, 0);
  for (int i = 0; i < 16; ++i){
    lcd.write(' ');
  }
  lcd.setCursor (0, 1);
  for (int i = 0; i < 16; ++i){
    lcd.write(' ');
  }

}

void updateTime() {

  count_2 = millis();
  if (sec >= 60) {
    sec = 0;
    mins += 1;
  }
  if (mins >= 60) {
    mins = 0;
    hr += 1;
  }
  if (mins == 60) {
    mins = 0;
  }
  if (hr == 24) {
    hr = 0;
  }
  if ((count_2 - count_1) >= 1000) {
    sec += 1;
    count_1 = count_2;
  }

}

void alarm() {

  if (sensor > 300) {
    Clear();
    lcd.setCursor(0, 0);
    lcd.print("   Calling...   ");
    lcd.setCursor(0, 1);
    lcd.print("      191");

    BeepAlarm();
    _delay_ms(2000);
  }

}

void checkTime() {

  Clear();
  lcd.setCursor(6, 0);
  lcd.print("TIME");
  lcd.setCursor(4, 1);
  lcd.print(hr);
  lcd.setCursor(6, 1);
  lcd.print(":");
  lcd.setCursor(7, 1);
  lcd.print(mins);
  lcd.setCursor(9, 1);
  lcd.print(":");
  lcd.setCursor(10, 1);
  lcd.print(sec);
  _delay_ms(100);
  count_2 -= 100;

  if (hr >= 22) {
    alarm();
  }

}

int start() { //pressed any key to start

  for (int output = 0; output < 3; output++) { //col
    if (output == 2)
      PORTD &= ~(1 << 5);
    else
      PORTD &= ~(1 << output);
    _delay_ms(5);
    for (int input = 0; input < 4; input++) {
      if ((PINB & (1 << input)) == 0) {
        delay(50);
        return 1;
      }
    }

  }
  return 0;

}

void setup() {

  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight
  init_peripheral();
  servo.attach(PIN_PC0);
  servo.write(180);
  count_1 = millis();

}

void loop() {

  sensor = analogRead(PC1);
  updateTime();
  checkTime();
  if (start() == 1) {
    if (Repassword == 3)return;
    Enter();
    Pressed(0, password);

    if (changePassword(password)) {
      EnterOldPass();
      Pressed(0, password);
      
      if (checkPassword(password, RealPassword) == 1) {
        EnterNewPass();
      }
      
      else {
        OldPassError();
      }
    }

    if (checkPassword(password, RealPassword) == 1) {
      _delay_ms(500);
      BeepTrue();
      Unlock();
      servo.write(30);
      delay(10000);
      servo.write(180);
      chtrue = 1;
    }
    
    else if (checkPassword(password, RealPassword) == 0 && Repassword < 3) {
      _delay_ms(500);
      Error();
      BeepFalse();
      _delay_ms(1500);
      Repassword++;
    }
    
    if (Repassword == 3) {
      Bye();
      for (int i = 0; i < 3; i++) { 
        _delay_ms(100)  ;
        BeepFalse();
        _delay_ms(100)  ;
      }
    }
  }

}
