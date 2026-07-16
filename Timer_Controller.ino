#include <TM1637Display.h>
#include <ctype.h>
int row1, row2, row3, row4; //ROW NUMBERS
int column1, column2, column3, column4; //COLUMN NUMBERS
int CLK, DIO; //DISPLAY CONTROLS
int LED = 13; //LED CONTROL PIN
int time_num[4] = {0, 0, 0, 0}; //TIMER VALS
char in; //INPUT CHARACTER
bool pause = false;
TM1637Display display(CLK, DIO);
void setup() {
  row1 = 1;
  row2 = 2;
  row3 = 3;
  row4 = 4;
  column1 = 5;
  column2 = 6;
  column3 = 8;
  column4 = 9;
  CLK = 10;
  DIO = 12;
  pinMode(row1, OUTPUT);
  pinMode(row2, OUTPUT);
  pinMode(row3, OUTPUT);
  pinMode(row4, OUTPUT);
  pinMode(column1, INPUT_PULLDOWN);
  pinMode(column2, INPUT_PULLDOWN);
  pinMode(column3, INPUT_PULLDOWN);
  pinMode(column4, INPUT_PULLDOWN);
  pinMode(LED, OUTPUT);
  display.setBrightness(7);
}

char keyboard() {       //KEYBOARD READ
  while(true)
  {
    //SCAN ROW1
    digitalWrite(row1, HIGH);
    delay(10);
    if(digitalRead(column1)) { digitalWrite(row1, LOW); return '1'; }
    if(digitalRead(column2)) { digitalWrite(row1, LOW); return '2'; }
    if(digitalRead(column3)) { digitalWrite(row1, LOW); return '3'; }
    if(digitalRead(column4)) { digitalWrite(row1, LOW); return 'A'; }
    digitalWrite(row1, LOW);
    //SCAN ROW2
    digitalWrite(row2, HIGH);
    delay(10);
    if(digitalRead(column1)) { digitalWrite(row2, LOW); return '4'; }
    if(digitalRead(column2)) { digitalWrite(row2, LOW); return '5'; }
    if(digitalRead(column3)) { digitalWrite(row2, LOW); return '6'; }
    if(digitalRead(column4)) { digitalWrite(row2, LOW); return 'B'; }
    digitalWrite(row2, LOW);
    //SCAN ROW3
    digitalWrite(row3, HIGH);
    delay(10);
    if(digitalRead(column1)) { digitalWrite(row3, LOW); return '7'; }
    if(digitalRead(column2)) { digitalWrite(row3, LOW); return '8'; }
    if(digitalRead(column3)) { digitalWrite(row3, LOW); return '9'; }
    if(digitalRead(column4)) { digitalWrite(row3, LOW); return 'C'; }
    digitalWrite(row3, LOW);
    //SCAN ROW4
    digitalWrite(row4, HIGH);
    delay(10);
    if(digitalRead(column1)) { digitalWrite(row4, LOW); return '*'; }
    if(digitalRead(column2)) { digitalWrite(row4, LOW); return '0'; }
    if(digitalRead(column3)) { digitalWrite(row4, LOW); return '#'; }
    if(digitalRead(column4)) { digitalWrite(row4, LOW); return 'D'; }
    digitalWrite(row4, LOW);
  }

}

void showTime(int time_val[]) {       //DISPLAY TIME
  int value = (time_val[0] * 1000) + (time_val[1] * 100) + (time_val[2] * 10) + time_val[3];
  display.showNumberDecEx(value, 0b01000000, true, 4, 0);
}

void val_ent()        //ENTER FOUR DIGIT VALUE
{
  for(int i = 0;i < 4;i++)
  {
    in = keyboard();
    if(isdigit(in))
    {
      time_num[i] = in - '0';
      showTime(time_num);
    }
    else
    {
      i = i - 1;
    }
  }
}

void setInit(int a, int b, int c, int d)
{
  time_num[0] = a;
  time_num[1] = b;
  time_num[2] = c;
  time_num[3] = d;
}
void loop()
{
  showTime(time_num);
  in = keyboard();
  switch(in)
  {
    case 'A': setInit(1, 0, 0, 0); break;
    case 'B': setInit(2, 0, 0, 0); break;
    case 'C': setInit(3, 0, 0, 0); break;
    case 'D': setInit(4, 0, 0, 0); break;
    default: val_ent();
  }
  int sec_val = (((time_num[0] * 10) + (time_num[1])) * 60) + (time_num[2] * 10) + time_num[3];
  digitalWrite(LED, HIGH);
  digitalWrite(row4, HIGH);
  while(true)
  {
    delay(1000);
    if(sec_val == 0)
    {
      digitalWrite(LED, LOW);
      break;
    }
    if(!pause)
    {
      sec_val--;
      time_num[0] = (sec_val / 60) / 10;
      time_num[1] = (sec_val / 60) % 10;
      time_num[2] = (sec_val % 60) / 10;
      time_num[3] = (sec_val % 60) % 10;
      showTime(time_num);
    }
    else
    {
      delay(5000);
    }
    if(digitalRead(column1))
    {
      pause = !pause;
      digitalWrite(LED, pause ? LOW : HIGH);
      delay(5000);
      if(pause == false)
      {
        sec_val -= 5;
      }
    }
  }
  digitalWrite(row4, LOW);
}