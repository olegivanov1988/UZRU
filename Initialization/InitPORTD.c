#include "MDR1986VE1T.h"

void InitPORTD () {

  MDR_PORTD->OE =       // Режим работы вывода 0 порта ( 0 - вход, 1 - выход )
  ( 1 << 0 )           // Режим работы PORTD_PIN0. Сигнал CAN_EN разрешения работы приёмопередатчика CAN. Выход.
  | ( 1 << 1 )           // Режим работы PORTD_PIN1. Сигнал SHUNT шунтирования резисторов подзаряда. Выход.
  | ( 0 << 2 )           // Режим работы PORTD_PIN2. Сигнал блокировки нагрузки. Вход.
  | ( 0 << 3 )           // Режим работы PORTD_PIN3. Сигнал WORK_LED включения зелёного светодиода. Выход.
  | ( 1 << 4 )           // Режим работы PORTD_PIN4. Сигнал FAULT_LED включения красного светодиода. Выход.
  | ( 1 << 5 )           // Режим работы PORTD_PIN5. Сигнал GREEN_LED включения зелёного светодиода разъёма Ethernet. Выход.
  | ( 1 << 6 )           // Режим работы PORTD_PIN6. Сигнал YELLOW_LED включения желтого светодиода разъёма Ethernet. Выход.
  | ( 0 << 7 )           // Режим работы PORTD_PIN7. Никуда не подключен. Вход.
  | ( 0 << 8 )           // Режим работы PORTD_PIN8. Никуда не подключен. Вход.
  | ( 0 << 9 )           // Режим работы PORTD_PIN9. Сигнал . Вход.
  | ( 0 << 10 )         // Режим работы PORTD_PIN10. Никуда не подключен. Вход.
  | ( 0 << 11 )         // Режим работы PORTD_PIN11. Никуда не подключен. Вход.
  | ( 0 << 12 )         // Режим работы PORTD_PIN12. Никуда не подключен. Вход.
  | ( 0 << 13 )         // Режим работы PORTD_PIN13. Никуда не подключен. Вход.
  | ( 0 << 14 )         // Режим работы PORTD_PIN14. Никуда не подключен. Вход.
  | ( 0 << 15 )          // Режим работы PORTD_PIN15. Никуда не подключен. Вход.
  ;
  
  MDR_PORTD->FUNC =     // Функции выводов порта E. ( 0 - порт, 1 - основная функция, 2 - альтернативная, 3 - переопределённая )
  ( 0 << 0 )             // Функция вывода PORTD_PIN0. Вывод никуда не подключен. Порт.
  | ( 0 << 2 )           // Функция вывода PORTD_PIN1. Вывод никуда не подключен. Порт.
  | ( 0 << 4 )           // Функция вывода PORTD_PIN2. Вывод никуда не подключен. Порт.
  | ( 0 << 6 )           // Функция вывода PORTD_PIN3. Вывод никуда не подключен. Порт.
  | ( 0 << 8 )           // Функция вывода PORTD_PIN4. Вывод никуда не подключен. Порт.
  | ( 0 << 10 )         // Функция вывода PORTD_PIN5. Вывод никуда не подключен. Порт.
  | ( 0 << 12 )         // Функция вывода PORTD_PIN6. Вывод никуда не подключен. Порт.
  | ( 0 << 14 )         // Функция вывода PORTD_PIN7. Вывод никуда не подключен. Порт.
  | ( 0 << 16 )         // Функция вывода PORTD_PIN8. Вывод никуда не подключен. Порт.
  | ( 1 << 18 )         // Функция вывода PORTD_PIN9. Вывод никуда не подключен. Порт.
  | ( 1 << 20 )         // Функция вывода PORTD_PIN10. Вывод никуда не подключен. Порт.
  | ( 1 << 22 )         // Функция вывода PORTD_PIN11. Вывод никуда не подключен. Порт.
  | ( 1 << 24 )         // Функция вывода PORTD_PIN12. Вывод никуда не подключен. Порт.
  | ( 1 << 26 )         // Функция вывода PORTD_PIN13. Вывод никуда не подключен. Порт.
  | ( 1 << 28 )         // Функция вывода PORTD_PIN14. Вывод никуда не подключен. Порт.
  | ( 0 << 30 )          // Функция вывода PORTD_PIN15. Вывод никуда не подключен. Порт.
  ;
  
  MDR_PORTD->ANALOG =   // Режим работы вывода 0 порта ( 0 - аналоговый, 1 - цифровой )
    ( 1 << 0 )           // Режим работы PORTD_PIN0. Вывод никуда не подключен. Цифровой.
  | ( 1 << 1 )           // Режим работы PORTD_PIN1. Вывод никуда не подключен. Цифровой.
  | ( 1 << 2 )           // Режим работы PORTD_PIN2. Вывод никуда не подключен. Цифровой.
  | ( 1 << 3 )           // Режим работы PORTD_PIN3. Вывод никуда не подключен. Цифровой.
  | ( 1 << 4 )           // Режим работы PORTD_PIN4. Вывод никуда не подключен. Цифровой.
  | ( 1 << 5 )           // Режим работы PORTD_PIN5. Вывод никуда не подключен. Цифровой.
  | ( 1 << 6 )           // Режим работы PORTD_PIN6. Вывод никуда не подключен. Цифровой.
  | ( 1 << 7 )           // Режим работы PORTD_PIN7. Вывод никуда не подключен. Цифровой.
  | ( 1 << 8 )           // Режим работы PORTD_PIN8. Вывод никуда не подключен. Цифровой.
  | ( 0 << 9 )           // Режим работы PORTD_PIN9. Вывод никуда не подключен. Цифровой.
  | ( 0 << 10 )         // Режим работы PORTD_PIN10. Вход аналогового сигнала датчика положения электромагнита. Аналог.
  | ( 0 << 11 )         // Режим работы PORTD_PIN11. Вывод никуда не подключен. Цифровой.
  | ( 0 << 12 )         // Режим работы PORTD_PIN12. Вывод никуда не подключен. Цифровой.
  | ( 0 << 13 )         // Режим работы PORTD_PIN13. Вывод никуда не подключен. Цифровой.
  | ( 0 << 14 )         // Режим работы PORTD_PIN14. Вывод никуда не подключен. Цифровой.
  | ( 1 << 15 )          // Режим работы PORTD_PIN15. Вывод никуда не подключен. Цифровой.
  ;
  
  MDR_PORTD->PULL =     // Разрешение подтяжки вниз вывода 0 порта ( 0 - подтяжка выключена, 1 - подтяжка включена )
  ( 0 << 0 )             // Подтяжка PORTD_PIN0 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 16 )          // Подтяжка PORTD_PIN0 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 1 )           // Подтяжка PORTD_PIN1 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 17 )          // Подтяжка PORTD_PIN1 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 2 )           // Подтяжка PORTD_PIN2 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 18 )          // Подтяжка PORTD_PIN2 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 3 )           // Подтяжка PORTD_PIN3 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 19 )          // Подтяжка PORTD_PIN3 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 4 )           // Подтяжка PORTD_PIN4 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 20 )          // Подтяжка PORTD_PIN4 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 5 )           // Подтяжка PORTD_PIN5 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 21 )          // Подтяжка PORTD_PIN5 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 6 )           // Подтяжка PORTD_PIN6 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 22 )          // Подтяжка PORTD_PIN6 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 7 )           // Подтяжка PORTD_PIN7 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 23 )          // Подтяжка PORTD_PIN7 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 8 )           // Подтяжка PORTD_PIN8 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 24 )          // Подтяжка PORTD_PIN8 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 9 )           // Подтяжка PORTD_PIN9 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 25 )          // Подтяжка PORTD_PIN9 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 10 )         // Подтяжка PORTD_PIN10 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 26 )          // Подтяжка PORTD_PIN10 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 11 )         // Подтяжка PORTD_PIN11 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 27 )          // Подтяжка PORTD_PIN11 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 12 )         // Подтяжка PORTD_PIN12 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 28 )          // Подтяжка PORTD_PIN12 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 13 )         // Подтяжка PORTD_PIN13 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 29 )          // Подтяжка PORTD_PIN13 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 14 )         // Подтяжка PORTD_PIN14 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 30 )          // Подтяжка PORTD_PIN14 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 15 )          // Подтяжка PORTD_PIN15 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 31 )          // Подтяжка PORTD_PIN15 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  ;
  
  MDR_PORTD->PD =        // Режим работы вывода порта ( 0 - управляемый драйвер, 1 - открытый сток )
  ( 0 << 0 )             // Функция вывода PORTD_PIN0. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 2 )           // Функция вывода PORTD_PIN1. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 4 )           // Функция вывода PORTD_PIN2. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 6 )           // Функция вывода PORTD_PIN3. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 8 )           // Функция вывода PORTD_PIN4. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 10 )         // Функция вывода PORTD_PIN5. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 12 )         // Функция вывода PORTD_PIN6. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 14 )         // Функция вывода PORTD_PIN7. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 16 )         // Функция вывода PORTD_PIN8. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 18 )         // Функция вывода PORTD_PIN9. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 20 )         // Функция вывода PORTD_PIN10. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 22 )         // Функция вывода PORTD_PIN11. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 24 )         // Функция вывода PORTD_PIN12. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 26 )         // Функция вывода PORTD_PIN13. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 28 )         // Функция вывода PORTD_PIN14. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 30 )          // Функция вывода PORTD_PIN15. Вывод никуда не подключен. Управляемый драйвер.
  ;
  
  MDR_PORTD->PWR =      // Режим работы вывода порта ( 0 - зарезервировано, 01 - медленный фронт, 10 - быстрый фронт, 11 - максимально быстрый фронт )
    ( 1 << 0 )           // Режим работы PORTD_PIN0. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 2 )           // Режим работы PORTD_PIN1. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 4 )           // Режим работы PORTD_PIN2. Вывод никуда не подключен. Медленный фронт.
  | ( 3 << 6 )           // Режим работы PORTD_PIN3. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 8 )           // Режим работы PORTD_PIN4. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 10 )         // Режим работы PORTD_PIN5. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 12 )         // Режим работы PORTD_PIN6. Вывод никуда не подключен. Медленный фронт.
  | ( 3 << 14 )         // Режим работы PORTD_PIN7. Вывод никуда не подключен. Медленный фронт.
  | ( 3 << 16 )         // Режим работы PORTD_PIN8. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 18 )         // Режим работы PORTD_PIN9. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 20 )         // Режим работы PORTD_PIN10. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 22 )         // Режим работы PORTD_PIN11. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 24 )         // Режим работы PORTD_PIN12. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 26 )         // Режим работы PORTD_PIN13. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 28 )         // Режим работы PORTD_PIN14. Вывод никуда не подключен. Медленный фронт.
  | ( 3 << 30 )          // Режим работы PORTD_PIN15. Вывод никуда не подключен. Медленный фронт.
  ;
  
  MDR_PORTD->GFEN = 
    ( 0 << 0 )           // Фильтрация PORTD_PIN0. Никуда не подключен. Фильтрации нет.
  | ( 0 << 1 )           // Фильтрация PORTD_PIN1. Никуда не подключен. Фильтрации нет.
  | ( 0 << 2 )           // Фильтрация PORTD_PIN2. Никуда не подключен. Фильтрации нет.
  | ( 0 << 3 )           // Фильтрация PORTD_PIN3. Никуда не подключен. Фильтрации нет.
  | ( 0 << 4 )           // Фильтрация PORTD_PIN4. Никуда не подключен. Фильтрации нет.
  | ( 0 << 5 )           // Фильтрация PORTD_PIN5. Никуда не подключен. Фильтрации нет.
  | ( 0 << 6 )           // Фильтрация PORTD_PIN6. Никуда не подключен. Фильтрации нет.
  | ( 0 << 7 )           // Фильтрация PORTD_PIN7. Никуда не подключен. Фильтрации нет.
  | ( 0 << 8 )           // Фильтрация PORTD_PIN8. Никуда не подключен. Фильтрации нет.
  | ( 0 << 9 )           // Фильтрация PORTD_PIN9. Никуда не подключен. Фильтрации нет.
  | ( 0 << 10 )         // Фильтрация PORTD_PIN10. Никуда не подключен. Фильтрации нет.
  | ( 0 << 11 )         // Фильтрация PORTD_PIN11. Никуда не подключен. Фильтрации нет.
  | ( 0 << 12 )         // Фильтрация PORTD_PIN12. Никуда не подключен. Фильтрации нет.
  | ( 0 << 13 )         // Фильтрация PORTD_PIN13. Никуда не подключен. Фильтрации нет.
  | ( 0 << 14 )         // Фильтрация PORTD_PIN14. Никуда не подключен. Фильтрации нет.
  | ( 0 << 15 );        // Фильтрация PORTD_PIN15. Никуда не подключен. Фильтрации нет.
  
  
}