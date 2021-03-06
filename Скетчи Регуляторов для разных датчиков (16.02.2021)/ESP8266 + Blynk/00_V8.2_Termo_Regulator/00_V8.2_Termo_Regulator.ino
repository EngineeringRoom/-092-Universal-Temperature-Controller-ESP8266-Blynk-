/*****************************************************************************   
 *    00_V8.2_Termo_Regulator от 12 февраля 2021 года
 *  
 * BUG FIX
 * !!!Я ПАНТОРОГИЙ ОЛЕНЬ!!!
 *    исправленна работа таймеров на millis() на 50 сутки все зависало
 *    Извиняйте народ!!!
 *    
 *    Термореулятор. Сама логика очень простая, но меню навороченно.
 *    Микроконтроллер ESP8266, но ничего не мешает перенести на другие платформы.
 *    Работа с EEPROM сделана так что должно заработать на любой платформе.
 *    
 *    Код переделан для работы с ESP8266. Прикрученно приложение Blynk.
 *        
 *    Применяется экран LCD16x2.
 *    Датчик температуры может быть любой.  
 *    Сечас на выбор: 
 *    BME280
 *    DHT11
 *    DS18B20
 *    Три тактовых кнопки: Select, UP, DOWN. Их достаточно для навигации и настройки
 *    терморегулятора.
 *    
 *    Краткое описание меню:
 *    
 *    Главный экран (Выводится текущая темпратура от датчика, Утавка регулятора)
 *                  (Выход реле Включен/Выключен,Режим работы Автоматический/Ручной)
 *       Настройки <SETTINGS>
 *          Exit            (Выход из меню настроек)
 *          Temp setting    (Уставка температуры к которой стремиться регулятор)
 *          Hysteresis      (Гистерезис. Защита от дребезга реле)
 *          Mode            (Автоматический/Ручной)
 *          Regulator Type  (Тип регулятора HEATER/COOLER)
 *          Delay Light     (Время через которое подсветка экрана потухнет)
 *          RelayLogicLevel (Выбор Уровня Логики Реле)
 *          Reset Factory   (Сброс настроек на заводские)
 *          
 *    Если не нежимать не на какие кнопки то через 30 секунд экран потухнет.      
 *    При нажатиии на любую кнопку экран опять загориться.
 *    
 *    Если не нежимать не на какие кнопки то через 30 секунд кнопки 
 *    заблокируется(не касается подсветки).  Защита от случайных изменений и
 *    несанкционированного доступа.
 *    При долгом нажатии на кнопку SELECT, кнопки будут разблокированы.
 *    
 *    При разблокированных кнопках и режиме управления Ручной(Manual)
 *    можно управлять выходным реле нажатием кнопок UP/DOWN.
 *    
 *    Коротким нажатием SELECT вход в меню настроек.
 *    Выбор пункта меню осуществляется нажатием кнопок UP/DOWN.
 *    Вход в пункт меню осуществляется нажатием кнопки SELECT.
 *    
 *    Изменение настроек внутри пункта меню осуществляется нажатием кнопок UP/DOWN.
 *    При изменение Temp setting и Hysteresis можно менять величену на которую
 *    меняется уставка долгим нажатием кнопок UP/DOWN: 
 *    +/-10.0
 *    +/- 1.0
 *    +/- 0.1
 *    
 *    Сохранение любой настройки в пункте меню осуществляется нажатием кнопки SELECT.
 *    При этом данные сохраняются в EEPROM.
 *    
 *****************************************************************************    
 *    Автор скетча и видео: Обушенков Алексей Андреевич
 *    Инженерка Engineering room
 *    Группа в ВК https://vk.com/engineer24
 *    Телеграмм   https://t.me/Engineering_room24
 *    Канал на YouTube https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg
 *    
 *    
 *****************************************************************************
 * Библиотеки используемые в проекте:
 * "EEPROMAnything.h"     // http://playground.arduino.cc/Code/EEPROMWriteAnything
 * "OneButton.h"          // https://github.com/mathertel/OneButton.git
 * <LiquidCrystal_I2C.h>  // https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library.git
 *     Справка по библиотеке https://wiki.iarduino.ru/page/Working_with_character_LCD_displays
 *     
 * Библиотека Adafruit Unified Sensor Driver // https://github.com/adafruit/Adafruit_Sensor.git
 * "Adafruit_BME280.h"                // https://github.com/adafruit/Adafruit_BME280_Library.git
 * 
 *****************************************************************************
 * Модули в составе проекта
 * Датчики
 * GY-BME280-3.3 (3.6 $)http://ali.ski/oycpg
 * Датчики DHT (Линейка датчиков) ($0.63-3.92) http://ali.ski/iEIrd
 * Датчик DS18B20 ($0.99 - 1.43) http://ali.ski/PaxRmO
 * 
 * Arduino NANO берите Atmega328 ($1.95) http://ali.ski/dP2kK
 * Шилд для NANO качественней ($3.06) http://ali.ski/nwEnLA
 * Шилд для NANO как в видео ($1.11) http://ali.ski/g--NK2
 * 
 * LCD экран 
 * Берите с I2C расширителем или заказывайте отдельно ($0.67) http://ali.ski/b497kc 
 * LCD16x2 ($3.26) http://ali.ski/5FgenN
 * LCD16x2 (Поддерживает русский шрифт) ($6.98) http://ali.ski/1lz1J
 * 
 * 1-канальное реле с управлением
 * Высоким и Низким уровнем ($1.13) http://ali.ski/TkWXZj
 * 
 * Реле 1, 2, 4 или 8 каналов (низкий уровень) http://ali.ski/f_hxa
 * Реле твердотельное 1, 2, 4 или 8 каналов (высокий уровень) http://ali.ski/w1nJjv
 * 
 * Беспаечная Макетная плата http://ali.ski/nFecO0
 * или такая (побольше) http://ali.ski/7BdpR2
 * 
 * Блок питания для Макетки http://ali.ski/V_6j6
 * DuPont перемычки 120 шт. 20 см три вида по 40 щт. Папа-Папа; Папа-Мама; Мама-Мама ($6.62) http://ali.ski/htEzB
 * 
 * Блоки питания AC-DC разной мощности и вольтажа http://ali.ski/AMmrJr
 * 
 ***************************************************************************** 
 * Зачем вообще я сделал этот скетч? Для вас! Для тех кто хочет нучиться програмировать сам.
 * Если бы мне попался такой скетч раньше, я бы был очень благодарен.
 * Я посторался прокоментировать все, что бы было легче понять зачем та или иная закорючка.
 * Этот скетч. Это хорошая заготовка для более сложных и универсальных изделий.
 * 
 * Плюс этим устройством и скетчем можно вполне хорошо пользоваться да же невникая в суть кода.
 * Буду очень рад вашим фото терморегуляторов на основе моего у меня в группе
 * Вконтакте(https://vk.com/engineer24) и в Телеграмме (https://t.me/Engineering_room24).
 * Всем Добра! Всем Бобра!
 *****************************************************************************
 */


#include "Variables.h"  // Подключаем вкладку с переменными
#include "timers.h"     // Подключаем вкладку с функцией таймера
#include "Regulator.h"  // Подключаем вкладку с кодом Термо регулятора
#include "setEEPROM.h"  // Подключаем вкладку с кодом EEPROM
#include "LCD16x2.h"    // Подключаем вкладку с кодом LCD экрана
#include "Blynk.h"      // Подключаем вкладку с кодом Blynk
#include "Buttons.h"    // Подключаем вкладку с кодом кнопок

// Выбираем сенсор с которым будем работать
//#define USE_BME280
//#define USE_DHT11
#define USE_DS18B20

#if defined(USE_BME280)

  #warning "Sensor_BME280 selected" 
  #include "Sensor_BME280.h"     // Подключаем вкладку с кодом датчика BME280
  
#elif defined(USE_DHT11)

  #warning "Sensor_DHT11 selected"  
  #include "Sensor_DHT11.h"     // Подключаем вкладку с кодом датчика DHT11

#elif defined(USE_DS18B20)

  #warning "Sensor_DS18B20 selected"  
  #include "Sensor_DS18B20.h"     // Подключаем вкладку с кодом датчика DHT11
  
#else  
  #error "No Sensor selected"
#endif
 

void ErrorScreen(){
  Serial.print("ErrorScreen... ");
    
  lcd.backlight(); // Включаем подсветку.
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("Error");
  lcd.setCursor(0,1);
  lcd.print("Sensor");
  Serial.print("Sensor... ");

  ModeAuto=LOW;
  StateRelay1=LOW;
  Regulator(RegulatorType, ModeAuto, TempIn1, Ts, D, StateRelay1, GPIO_Relay1, RLL);
    Blynk.virtualWrite(VPIN_TempIn1, "Error Sensor"); // Температура от датчика
    if (StateRelay1) {led1.on();} else {led1.off();}  // Состояние Реле
  delay(1000);
  
  Serial.println("  Done!");   
}

void setup() {
// Настраиваем GPIO
  pinMode(GPIO_Relay1,   OUTPUT);

// Подключаем монитор порта
  Serial.begin(9600);
  Serial.println("Termo_Regulator!");
  Serial.println("Termo_Regulator!");  
  Serial.println("Termo_Regulator!");

// Подключаем EEPROM
  setupEEPROM();

// Подключаем кнопки
  setupButtons();

// Подключаем датчик температуры
  setupSensor();
  
// Подключаем экран  
  setupLCD16x2();

// Настраиваем все связанное с Blynk
  setupBlynk();
}

void loop() {
  if (Error==false){
      ms = millis();        // Текущее время с начала работы МК
    
    // Если долго не нажимали не одну кнопку, то выключим подсветку экрана
    // Если долго не нажимали не одну кнопку, то заблокируем их
      checkButtonLock(); 
         
      loopTempIn1(TempIn1); // Чтение Температуры от датчика в переменную TemIn1
      loopButtons();        // Опрос кнопок
      Menu();               // Меню
      
    // Функция регулятора температуры  
      Regulator(RegulatorType, ModeAuto, TempIn1, Ts, D, StateRelay1, GPIO_Relay1, RLL);
      
      ButtonsLOW(); // Здесь обнуляются флаги который могли быть подняты при нажатии кнопки в предидущем цикле

  }
  else{
      ErrorScreen();
  }
      loopBlynk();
}
