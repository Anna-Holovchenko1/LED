
 #include "FastLED.h"
 
 #define NUM_STRIP 1                 // количество лент
 #define NUM_LEDS 60                  // количество светодиодов в каждой ленте
 #define NUM_METEORS 15               // длина МЕТЕОРА
 #define INCREASE_BRIGHTNESS (256/NUM_METEORS)
 #define TIME_MIN 5                   // максимальная скорость метеора
 #define TIME_MAX 95                  // минимальная скорость метеора
 
 #define DATA_PIN1 2                  // ноги по АРДУИНО ПРО МИНИ для каждой ленты
 #define DATA_PIN2 3                  // подключать через резистор 470 Ом
 #define DATA_PIN3 4                  // питание лент от отдельного стабилизированного
 #define DATA_PIN4 5                  // источника постоянного напряжения 5V
 #define DATA_PIN5 6                  // Земля общая с блоком питания Ардуино
 #define DATA_PIN6 7                  //
 #define DATA_PIN7 8                  // ВНИМАНИЕ: потребление каждого кусочка ленты
 #define DATA_PIN8 9                  // из 20 светтодиодов = 6 Ватт (ток 1,2А на канал)
 #define DATA_PIN9 10                 // из 12 лент = 72 Ватт, что равно току почти 15А
 #define DATA_PIN10 11                // если предполагается одновременное включение всех
 #define DATA_PIN11 12                // практика показала что максимальное потребление
 #define DATA_PIN12 13                // одного отрезка ленты в режиме сосульки не более 200мА

 
 CRGB leds1[NUM_LEDS];                // первая лента
 CRGB leds2[NUM_LEDS];                // вторая лента
 CRGB leds3[NUM_LEDS];                // третья лента
 CRGB leds4[NUM_LEDS];                // четвертая лента
 CRGB leds5[NUM_LEDS];                // пятая лента
 CRGB leds6[NUM_LEDS];                // шестая лента
 CRGB leds7[NUM_LEDS];                // седьмая лента
 CRGB leds8[NUM_LEDS];                // восьмая лента
 CRGB leds9[NUM_LEDS];                // девятая лента
 CRGB leds10[NUM_LEDS];               // десятая лента
 CRGB leds11[NUM_LEDS];               // одиннадцатая лента
 CRGB leds12[NUM_LEDS];               // двенадцатая лента
  
 CRGB meteors[NUM_METEORS];           // метеор
 CRGB etalone[NUM_METEORS+NUM_LEDS];  // эталонный массив метеор+лента
 CRGB rabochiy[NUM_METEORS+NUM_LEDS]; // рабочий массив для расчета лент
 byte LENTA[NUM_STRIP];               // массив признаков работы лент (1-выполняется метеор, 0-выкл)
 byte PHASE[NUM_STRIP];               // массив значений состояния сдвига для каждой ленты 0 - NUM_METEORS+NUM_LEDS
 byte PAUSE[NUM_STRIP];               // массив значений циклов задержки между метеорами для каждой ленты
 byte COLOR[NUM_STRIP];               // массив значений цвета лент
 
 int i,j,n = 0;
 int HSV, STAY_TIME;                  // цвет и скорость метеора если число + то скорость -
 
void setup() { 
   
   // В случае управление питанием чрез клю нужен этот кусок кода, чтобы включить ленту
   // и дождаться окончания переходных процессов
   //  pinMode( POWER_PIN, OUTPUT );
   //  digitalWrite( A0, HIGH );
   //  delay(2000);  
   
   // инициализируем ленты
   FastLED.addLeds<WS2812B, DATA_PIN1>(leds1, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN2>(leds2, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN3>(leds3, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN4>(leds4, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN5>(leds5, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN6>(leds6, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN7>(leds7, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN8>(leds8, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN9>(leds9, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN10>(leds10, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN11>(leds11, NUM_LEDS);
   FastLED.addLeds<WS2812B, DATA_PIN12>(leds12, NUM_LEDS);
   
   // включение аналогового входа настройка диапазона цветов метеора
   pinMode( A0, INPUT);
   pinMode( A1, INPUT);
   HSV = GETCOLOR();
   STAY_TIME = map(analogRead(A1), 0, 1023, TIME_MIN, TIME_MAX);
   
   // стартовая инициализация всех массивов
   for ( i = 0; i < NUM_STRIP; i++) {
   LENTA[i] = 1;
   PHASE[i] = 0;
   PAUSE[i] = 0;
   COLOR[i] = HSV;}
   
}
 
 
void loop() {
   for ( n = 0; n < NUM_STRIP; n++) {               // поочередно перебираем все ленты

   if ( LENTA[n] == 1) {                            // если на ленте есть МЕТЕОР то обрабатываем
     if ( PHASE[n] == (NUM_LEDS+NUM_METEORS)){      // если МЕТЕОР "вылетел" то
     PHASE[n] = 0;                                  // сброс ленты на НОЛЬ
     LENTA[n] = 0;                                  // выключение ленты   
     HSV = GETCOLOR();                              // подготовка нового цвета к следующему включению
     COLOR[n] = random(HSV-5,HSV+5);                // и установка коридора флюктуации цвета
     PAUSE[n] = random(10,50);}                     // и задание паузы до следующего включения (в циклах)
   SHIFT(n,PHASE[n]);                               // если нормальный цикл обработки, то засвечиваем Метеор на ленте
   PHASE[n]++;}                                     // и сдвигаем фазу
   
   if (LENTA[n]==0) {                               // Если лента выключена
      if (PAUSE[n]>0) PAUSE[n]--;                   // то считаем длительность паузы
        if (PAUSE[n]==0) LENTA[n]=1;}               // и по окончании включаем ленту снова
  }

   FastLED.show();                                  // засветка лвсех лент
   STAY_TIME = map(analogRead(A1), 0, 1023, TIME_MIN, TIME_MAX); // снятие показателя задержки
   delay(STAY_TIME);                                // задержка по уровню скорости МЕТЕОРА
}


// Подпрограмма настройки диапазона цвета метеора по регулятору, подключенному к A0
int GETCOLOR()
   {
   int val = map(analogRead(A0), 0, 1023, 5, 250);  // цветовой ряд HSV описывается в диапазоне от 0 до 255
   return val;                                      // поскольку корридор +-45 то масштабирование аналогового входа
   }                                                // делается в диапазоне от 45 до 315


// подпрограмма формирование метеора заданной длины и цвета и заливка его в эталонный массив
void METEOR(int color)
   {
   i,j = 0;
   for( i = INCREASE_BRIGHTNESS - 1; i < 256; i += INCREASE_BRIGHTNESS )     // создание МЕТЕОРА
   meteors[j++].setHSV( color, 255, i );                                     // заданного цвета
   for( i = 0; i < NUM_METEORS; i++)                                         // и сброс его в эталонный массив
   etalone[i] = meteors[i];
   for( i = NUM_METEORS; i < (NUM_LEDS + NUM_METEORS); i ++ )                // остальное заполняем "черным"
   etalone[i] = CRGB::Black;
   }
   

// подпрограмма копирования эталонного массива со сдвигом в рабочую область   
void SHIFT(int LED, int shift)
   {
   // подготовка своего цвета для каждой ленты
   if (LED==0) METEOR(COLOR[0]);        
   if (LED==1) METEOR(COLOR[1]);
   if (LED==2) METEOR(COLOR[2]);
   if (LED==3) METEOR(COLOR[3]);
   if (LED==4) METEOR(COLOR[4]);
   if (LED==5) METEOR(COLOR[5]);
   if (LED==6) METEOR(COLOR[6]);
   if (LED==7) METEOR(COLOR[7]);
   if (LED==8) METEOR(COLOR[8]);
   if (LED==9) METEOR(COLOR[9]);
   if (LED==10) METEOR(COLOR[10]);
   if (LED==11) METEOR(COLOR[11]);
    
   // переписываем эталон с указанным сдвигом в рабочий массив      
   for( i = (NUM_LEDS+NUM_METEORS-1); i > shift; i-- ) 
   rabochiy[i] = etalone[i-shift];
   for ( i = shift; i > 0; i--)
   rabochiy[i] = CRGB::Black;
   
   // и заливаем его на соответствующую ленту
   if (LED==0)   for( i = 0; i < NUM_LEDS; i++ ) leds1[i] = rabochiy[i+NUM_METEORS];  
   if (LED==1)   for( i = 0; i < NUM_LEDS; i++ ) leds2[i] = rabochiy[i+NUM_METEORS]; 
   if (LED==2)   for( i = 0; i < NUM_LEDS; i++ ) leds3[i] = rabochiy[i+NUM_METEORS]; 
   if (LED==3)   for( i = 0; i < NUM_LEDS; i++ ) leds4[i] = rabochiy[i+NUM_METEORS]; 
   if (LED==4)   for( i = 0; i < NUM_LEDS; i++ ) leds5[i] = rabochiy[i+NUM_METEORS]; 
   if (LED==5)   for( i = 0; i < NUM_LEDS; i++ ) leds6[i] = rabochiy[i+NUM_METEORS]; 
   if (LED==6)   for( i = 0; i < NUM_LEDS; i++ ) leds7[i] = rabochiy[i+NUM_METEORS]; 
   if (LED==7)   for( i = 0; i < NUM_LEDS; i++ ) leds8[i] = rabochiy[i+NUM_METEORS]; 
   if (LED==8)   for( i = 0; i < NUM_LEDS; i++ ) leds9[i] = rabochiy[i+NUM_METEORS]; 
   if (LED==9)   for( i = 0; i < NUM_LEDS; i++ ) leds10[i] = rabochiy[i+NUM_METEORS]; 
   if (LED==10)  for( i = 0; i < NUM_LEDS; i++ ) leds11[i] = rabochiy[i+NUM_METEORS]; 
   if (LED==11)  for( i = 0; i < NUM_LEDS; i++ ) leds12[i] = rabochiy[i+NUM_METEORS]; 
   }
