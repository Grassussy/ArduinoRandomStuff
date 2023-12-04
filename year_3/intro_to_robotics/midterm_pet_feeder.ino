#include <Wire.h>
#include <HX711_ADC.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// pins
int pin_dt = 4;
int pin_sck = 5;
int pin_servo1 = 6;
int pin_servo2 = 7;
int pin_btn1 = 8;
int pin_btn2 = 9;
int pin_btn3 = 10;

// load cell
float calibration_factor = 765;

// lcd_i2c
int address_i2c = 0x27;
int lcd_columns = 16;
int lcd_rows = 2;

// servo positions
int servo1_pos_start = 180;
int servo1_pos_end = 0;
int servo2_pos_start = 90;
int servo2_pos_end = 150;

// counters
int btn1_counter = 0;
int btn2_counter = 0;
int btn3_counter = 0;
int weight_counter = 10;

// misc
HX711_ADC LoadCell(pin_dt, pin_sck);
LiquidCrystal_I2C lcd(address_i2c, lcd_columns, lcd_rows);
Servo myservo1;
Servo myservo2;

void setup() {
  // load cell
  LoadCell.begin();
  LoadCell.start(5000);
  LoadCell.setCalFactor(calibration_factor);

  // lcd_i2c
  lcd.init();
  lcd.backlight();

  // servo
  myservo1.attach(pin_servo1);
  myservo2.attach(pin_servo2);
  myservo1.write(servo1_pos_start);
  myservo2.write(servo2_pos_start);

  // button
  pinMode(pin_btn1, INPUT_PULLUP);
  pinMode(pin_btn2, INPUT_PULLUP);
  pinMode(pin_btn3, INPUT_PULLUP);
}

void loop() {
  LoadCell.update();

  float i = LoadCell.getData();

  lcd.setCursor(0, 0);
  lcd.print("Weight[g]");

  lcd.setCursor(14, 0);
  lcd.print("  ");
  lcd.print(weight_counter);

  lcd.setCursor(0, 1);
  lcd.print("                ");
  if (i < 0)
    lcd.print("0");
  else
    lcd.print(i, 0);

  btn1OnPress(i);
  btn2OnPress();
  btn3OnPress();

  delay(25);
}

void btn1OnPress(float weight) {
  if (btn1_counter == 0) {
    if (digitalRead(pin_btn1) == LOW) {
      myservo1.write(servo1_pos_end);

      btn1_counter = 1;
    }
  } else if (weight >= weight_counter - 0.05) {
    btn1_counter = 0;

    lcd.setCursor(0, 0);
    lcd.print("Unloading...    ");
    lcd.setCursor(0, 1);
    lcd.print("                ");

    myservo1.write(servo1_pos_start);

    delay(1000);

    for (int i = servo2_pos_start + 1; i <= servo2_pos_end; i++) {
      myservo2.write(i);
      delay(100);
    }

    delay(2500);

    for (int i = servo2_pos_end - 1; i >= servo2_pos_start; i--) {
      myservo2.write(i);
      delay(100);
    }

    lcd.setCursor(0, 0);
    lcd.print("                ");
  }
}

void btn2OnPress() {
  if (btn2_counter == 0) {
    if (digitalRead(pin_btn2) == LOW && weight_counter > 1) {
      weight_counter--;
      btn2_counter = 1;
      delay(50);
    }
  } else
    btn2_counter = 0;
}

void btn3OnPress() {
  if (btn3_counter == 0) {
    if (digitalRead(pin_btn3) == LOW) {
      weight_counter++;
      btn3_counter = 1;
      delay(50);
    }
  } else
    btn3_counter = 0;
}
