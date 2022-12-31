#define PIN_D 9
#define PIN_T 10
#define PIN_K 11

String msg;             // переменная для хранения сообщения
byte parseStart = 0;    // переменная запуска парсинга
bool powerLED = 0;      // флаг управления яркостью ленты
bool numLED = 0;      // флаг управления цветом ленты
int prevPower = 0;      // хранение предыдущего значения яркости

void setup() 
{
  pinMode(PIN_D, OUTPUT);
  pinMode(PIN_T, OUTPUT);
  pinMode(PIN_K, OUTPUT);
  Serial.begin(9600);
}

void loop() 
{
  if (Serial.available())                 //  если что-то пришло в Serial-порт
  {
    char in = Serial.read();              //  читаем один байт (символ)
    if (!(in == '\n' || in == '\r'))      //  отсеиваем символы возврата картеки и переноса строки
    {
        switch (in)       // в зависимости от принятого символа, делаем выбор
        {
            case '@': parseStart = 0; msg = ""; koza(); break;
            case ';': parseStart = 1; break;
            case '#': parseStart = 2;  powerLED = 1; break;
            case '$': parseStart = 2;  numLED = 1; break;
            case '^': parseStart = 0; msg = ""; like(); break;
        }

        // если парсинг запущен и это не символы начала или окончания посылки
        if ((parseStart == 2) && (in != '#') && (in != '$') && (in != '^') && (in != '@')) 
        {  
          msg += in;    // запоминаем переданное число (складываем по одному байту в общую строку)
        }
     }
  }
  
  if(parseStart == 1)
  {
      int message = msg.toInt();                          // преобразуем полученную строку в целое число
      if (message < 200) message = 200;                   // защита от выхода руки слева
      if (message > 800) message = 800;                   // защита от выхода руки справа
      message = map(message, 200, 800, 0, 255);           // преобразуем принятые координаты в значения яркости от 0 до 255

      if(powerLED)
      {
        for(int i=7;i<=11;i++){
          analogWrite(i,message);
          prevPower = message;
        }
      }
      if(numLED)  
      {
        if ((message<=80)&&(message>0)){
          analogWrite(PIN_D,prevPower);
          analogWrite(PIN_T,0);
          analogWrite(PIN_K,0);

        }
        if ((message<=160)&&(message>80)){
          analogWrite(PIN_D,0);
          analogWrite(PIN_T,prevPower);
          analogWrite(PIN_K,0);

        }
        if ((message<=255)&&(message>160)){
          analogWrite(PIN_D,0);
          analogWrite(PIN_T,0);
          analogWrite(PIN_K,prevPower);

        }
      }

      numLED = 0;
      powerLED = 0;
      parseStart = 0;
      msg = ""; 
    }   
}

void koza() 
{
  for(byte i=9; i<=11;i++){
    analogWrite(i,0);
  }
}

void like() 
{
    for(byte j=9;j<=11;j++){
      analogWrite(j,prevPower);
      delay(200);
    }
    delay(200);
    for(byte j=9;j<=11;j++){
      analogWrite(j,0);
      delay(200);
    }
    
}
