#include "MDR1986VE1T.h"

void InitPORTE () {

  MDR_PORTE->OE =       // Режим работы вывода 0 порта ( 0 - вход, 1 - выход )
    ( 0 << 0 )           // Режим работы PORTE_PIN0. Никуда не подключен. Вход.
  | ( 1 << 1 )           // Режим работы PORTE_PIN1. Выход ЦАП, управляющий током в нагрузке. Выход.
  | ( 1 << 2 )           // Режим работы PORTE_PIN2. Выход ЦАП, управляющий током в нагрузке. Выход.
  | ( 1 << 3 )           // Режим работы PORTE_PIN3. TMR1_CH1, управляющий драйвером силового ключа. Выход.
  | ( 1 << 4 )           // Режим работы PORTE_PIN4. TMR1_CH2, управляющий драйвером силового ключа. Выход.
  | ( 0 << 5 )           // Режим работы PORTE_PIN5. Никуда не подключен. Вход.
  | ( 0 << 6 )           // Режим работы PORTE_PIN6. Никуда не подключен. Вход.
  | ( 0 << 7 )           // Режим работы PORTE_PIN7. Никуда не подключен. Вход.
  | ( 0 << 8 )           // Режим работы PORTE_PIN8. Никуда не подключен. Вход.
  | ( 0 << 9 )           // Режим работы PORTE_PIN9. Никуда не подключен. Вход.
  | ( 0 << 10 )         // Режим работы PORTE_PIN10. Никуда не подключен. Вход.
  | ( 0 << 11 )         // Режим работы PORTE_PIN11. Никуда не подключен. Вход.
  | ( 0 << 12 )         // Режим работы PORTE_PIN12. Никуда не подключен. Вход.
  | ( 0 << 13 )         // Режим работы PORTE_PIN13. Никуда не подключен. Вход.
  | ( 0 << 14 )         // Режим работы PORTE_PIN14. Вывод никуда не подключен. Вход.
  | ( 0 << 15 );        // Режим работы PORTE_PIN15. Вывод никуда не подключен. Вход.
  
  MDR_PORTE->FUNC =     // Функции выводов порта E. ( 0 - порт, 1 - основная функция, 2 - альтернативная, 3 - переопределённая )
  ( 0 << 0 )            // Функция вывода PORTE_PIN0. Вывод никуда не подключен. Порт.
  | ( 1 << 2 )          // Функция вывода PORTE_PIN1. Выход ЦАП, управляющий током в нагрузке. Основная функция (так требует спецификация).
  | ( 1 << 4 )          // Функция вывода PORTE_PIN2. Выход ЦАП, управляющий током в нагрузке. Основная функция (так требует спецификация).
  | ( 1 << 6 )          // Функция вывода PORTE_PIN3. Вывод TMR1_CH1, управляющий драйвером силового ключа. Основная функция.
  | ( 1 << 8 )          // Функция вывода PORTE_PIN4. Вывод TMR1_CH2, управляющий драйвером силового ключа. Основная функция.
  | ( 0 << 10 )         // Функция вывода PORTE_PIN5. Вывод никуда не подключен. Порт.
  | ( 0 << 12 )         // Функция вывода PORTE_PIN6. Вывод никуда не подключен. Порт.
  | ( 0 << 14 )         // Функция вывода PORTE_PIN7. Вывод никуда не подключен. Порт.
  | ( 0 << 16 )         // Функция вывода PORTE_PIN8. Вывод никуда не подключен. Порт.
  | ( 0 << 18 )         // Функция вывода PORTE_PIN9. Вывод никуда не подключен. Порт.
  | ( 0 << 20 )         // Функция вывода PORTE_PIN10. Вывод никуда не подключен. Порт.
  | ( 0 << 22 )         // Функция вывода PORTE_PIN11. Вывод никуда не подключен. Порт.
  | ( 0 << 24 )         // Функция вывода PORTE_PIN12. Вывод никуда не подключен. Порт.
  | ( 0 << 26 )         // Функция вывода PORTE_PIN13. Вывод никуда не подключен. Порт.
  | ( 0 << 28 )         // Функция вывода PORTE_PIN14. Вывод никуда не подключен. Порт.
  | ( 0 << 30 );        // Функция вывода PORTE_PIN15. Вывод никуда не подключен. Порт.
  
  MDR_PORTE->ANALOG =   // Режим работы вывода 0 порта ( 0 - аналоговый, 1 - цифровой )
    ( 1 << 0 )          // Режим работы PORTE_PIN0. Вывод никуда не подключен. Цифровой.
  | ( 0 << 1 )          // Режим работы PORTE_PIN1. Выход ЦАП, управляющий током в нагрузке. Аналоговый.
  | ( 0 << 2 )          // Режим работы PORTE_PIN2. Выход ЦАП, управляющий током в нагрузке. Аналоговый.
  | ( 1 << 3 )          // Режим работы PORTE_PIN3. Вывод TMR1_CH1. Цифровой.
  | ( 1 << 4 )          // Режим работы PORTE_PIN4. Вывод TMR1_CH2. Цифровой.
  | ( 1 << 5 )          // Режим работы PORTE_PIN5. Вывод никуда не подключен. Цифровой.
  | ( 1 << 6 )          // Режим работы PORTE_PIN6. Вывод никуда не подключен. Цифровой.
  | ( 1 << 7 )          // Режим работы PORTE_PIN7. Вывод никуда не подключен. Цифровой.
  | ( 1 << 8 )          // Режим работы PORTE_PIN8. Вывод никуда не подключен. Цифровой.
  | ( 1 << 9 )          // Режим работы PORTE_PIN9. Вывод никуда не подключен. Цифровой.
  | ( 1 << 10 )         // Режим работы PORTE_PIN10. Вывод никуда не подключен. Цифровой.
  | ( 1 << 11 )         // Режим работы PORTE_PIN11. Вывод никуда не подключен. Цифровой.
  | ( 1 << 12 )         // Режим работы PORTE_PIN12. Вывод никуда не подключен. Цифровой.
  | ( 1 << 13 )         // Режим работы PORTE_PIN13. Вывод никуда не подключен. Цифровой.
  | ( 1 << 14 )         // Режим работы PORTE_PIN14. Вывод никуда не подключен. Цифровой.
  | ( 1 << 15 );        // Режим работы PORTE_PIN15. Вывод никуда не подключен. Цифровой.
  
  MDR_PORTE->PULL =     // Разрешение подтяжки вниз вывода 0 порта ( 0 - подтяжка выключена, 1 - подтяжка включена )
  ( 0 << 0 )            // Подтяжка PORTE_PIN0 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 16 )         // Подтяжка PORTE_PIN0 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 1 )          // Подтяжка PORTE_PIN1 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 17 )         // Подтяжка PORTE_PIN1 к Vcc. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 2 )          // Подтяжка PORTE_PIN2 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 18 )         // Подтяжка PORTE_PIN2 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 3 )          // Подтяжка PORTE_PIN3 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 19 )         // Подтяжка PORTE_PIN3 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 4 )          // Подтяжка PORTE_PIN4 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 20 )         // Подтяжка PORTE_PIN4 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 5 )          // Подтяжка PORTE_PIN5 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 21 )         // Подтяжка PORTE_PIN5 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 6 )          // Подтяжка PORTE_PIN6 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 22 )         // Подтяжка PORTE_PIN6 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 7 )          // Подтяжка PORTE_PIN7 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 23 )         // Подтяжка PORTE_PIN7 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 8 )          // Подтяжка PORTE_PIN8 к GND. Вывод управляет полярностью тока. Подтяжки нет.
  | ( 0 << 24 )         // Подтяжка PORTE_PIN8 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 9 )          // Подтяжка PORTE_PIN9 к GND. Вывод управляет пуском/остановом контроллера ШИМ. Подтяжки нет.
  | ( 0 << 25 )         // Подтяжка PORTE_PIN9 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 10 )         // Подтяжка PORTE_PIN10 к GND. Вывод управляет шунтированием схемы плавного подзаряда. Подтяжки нет.
  | ( 0 << 26 )         // Подтяжка PORTE_PIN10 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 11 )         // Подтяжка PORTE_PIN11 к GND. CAN_RX. Основная функция вывода контроллера. Подтяжки нет.
  | ( 0 << 27 )         // Подтяжка PORTE_PIN11 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 12 )         // Подтяжка PORTE_PIN12 к GND. CAN_TX. Основная функция вывода контроллера. Подтяжки нет.
  | ( 0 << 28 )         // Подтяжка PORTE_PIN12 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 13 )         // Подтяжка PORTE_PIN13 к GND. Вывод управляет разрешением работы приёмопередатчика CAN. Подтяжки нет.
  | ( 0 << 29 )         // Подтяжка PORTE_PIN13 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 14 )         // Подтяжка PORTE_PIN14 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 30 )         // Подтяжка PORTE_PIN14 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  | ( 0 << 15 )         // Подтяжка PORTE_PIN15 к GND. Вывод никуда не подключен. Подтяжки нет.
  | ( 0 << 31 );        // Подтяжка PORTE_PIN15 к Vcc. Вывод никуда не подключен. Подтяжки нет.  
  
  
  MDR_PORTE->PD =        // Режим работы вывода порта ( 0 - управляемый драйвер, 1 - открытый сток )
  ( 0 << 0 )             // Функция вывода PORTE_PIN0. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 2 )           // Функция вывода PORTE_PIN1. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 4 )           // Функция вывода PORTE_PIN2. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 6 )           // Функция вывода PORTE_PIN3. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 8 )           // Функция вывода PORTE_PIN4. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 10 )         // Функция вывода PORTE_PIN5. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 12 )         // Функция вывода PORTE_PIN6. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 14 )         // Функция вывода PORTE_PIN7. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 16 )         // Функция вывода PORTE_PIN8. Вывод управляет полярностью тока. Управляемый драйвер.
  | ( 0 << 18 )         // Функция вывода PORTE_PIN9. Вывод управляет пуском/остановом контроллера ШИМ. Управляемый драйвер.
  | ( 0 << 20 )         // Функция вывода PORTE_PIN10. Вывод управляет шунтированием схемы плавного подзаряда. Управляемый драйвер.
  | ( 0 << 22 )         // Функция вывода PORTE_PIN11. CAN_RX. Основная функция вывода контроллера. Управляемый драйвер.
  | ( 0 << 24 )         // Функция вывода PORTE_PIN12. CAN_TX. Основная функция вывода контроллера. Управляемый драйвер.
  | ( 0 << 26 )         // Функция вывода PORTE_PIN13. Вывод управляет разрешением работы приёмопередатчика CAN. Управляемый драйвер.
  | ( 0 << 28 )         // Функция вывода PORTE_PIN14. Вывод никуда не подключен. Управляемый драйвер.
  | ( 0 << 30 );        // Функция вывода PORTE_PIN15. Вывод никуда не подключен. Управляемый драйвер.  
  
  MDR_PORTE->PWR =      // Режим работы вывода порта ( 0 - зарезервировано, 01 - медленный фронт, 10 - быстрый фронт, 11 - максимально быстрый фронт )
    ( 1 << 0 )           // Режим работы PORTE_PIN0. Вывод никуда не подключен. Медленный фронт.
  | ( 3 << 2 )           // Режим работы PORTE_PIN1. Выход ЦАП, управляющий током в нагрузке. Максимально быстрый фронт.
  | ( 3 << 4 )           // Режим работы PORTE_PIN2. Вывод никуда не подключен. Медленный фронт.
  | ( 3 << 6 )           // Режим работы PORTE_PIN3. Вывод TMR1_CH1. Максимально быстрый фронт.
  | ( 3 << 8 )           // Режим работы PORTE_PIN4. Вывод TMR1_CH2. Максимально быстрый фронт.
  | ( 1 << 10 )         // Режим работы PORTE_PIN5. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 12 )         // Режим работы PORTE_PIN6. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 14 )         // Режим работы PORTE_PIN7. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 16 )         // Режим работы PORTE_PIN8. Вывод управляет полярностью тока. Максимально быстрый фронт.
  | ( 1 << 18 )         // Режим работы PORTE_PIN9. Вывод управляет пуском/остановом контроллера ШИМ. Максимально быстрый фронт.
  | ( 1 << 20 )         // Режим работы PORTE_PIN10. Вывод управляет шунтированием схемы плавного подзаряда. Максимально быстрый фронт.
  | ( 1 << 22 )         // Режим работы PORTE_PIN11. CAN_RX. Основная функция вывода контроллера. Максимально быстрый фронт.
  | ( 1 << 24 )         // Режим работы PORTE_PIN12. CAN_TX. Основная функция вывода контроллера. Максимально быстрый фронт.
  | ( 1 << 26 )         // Режим работы PORTE_PIN13. Вывод управляет разрешением работы приёмопередатчика CAN. Максимально быстрый фронт.
  | ( 1 << 28 )         // Режим работы PORTE_PIN14. Вывод никуда не подключен. Медленный фронт.
  | ( 1 << 30 );        // Режим работы PORTE_PIN15. Вывод никуда не подключен. Медленный фронт.
  
  MDR_PORTE->GFEN = 
    ( 0 << 0 )           // Фильтрация PORTE_PIN0. Никуда не подключен. Фильтрации нет.
  | ( 0 << 1 )           // Фильтрация PORTE_PIN1. Никуда не подключен. Фильтрации нет.
  | ( 0 << 2 )           // Фильтрация PORTE_PIN2. Никуда не подключен. Фильтрации нет.
  | ( 0 << 3 )           // Фильтрация PORTE_PIN3. Никуда не подключен. Фильтрации нет.
  | ( 0 << 4 )           // Фильтрация PORTE_PIN4. Никуда не подключен. Фильтрации нет.
  | ( 0 << 5 )           // Фильтрация PORTE_PIN5. Никуда не подключен. Фильтрации нет.
  | ( 0 << 6 )           // Фильтрация PORTE_PIN6. Никуда не подключен. Фильтрации нет.
  | ( 0 << 7 )           // Фильтрация PORTE_PIN7. Никуда не подключен. Фильтрации нет.
  | ( 0 << 8 )           // Фильтрация PORTE_PIN8. Вывод управляет полярностью тока. Фильтрации нет.
  | ( 0 << 9 )           // Фильтрация PORTE_PIN9. Вывод управляет пуском/остановом контроллера ШИМ. Фильтрации нет.
  | ( 0 << 10 )         // Фильтрация PORTE_PIN10. Вывод управляет шунтированием схемы плавного подзаряда. Фильтрации нет.
  | ( 0 << 11 )         // Фильтрация PORTE_PIN11. CAN_RX. Основная функция вывода контроллера. Фильтрации нет.
  | ( 0 << 12 )         // Фильтрация PORTE_PIN12. CAN_TX. Основная функция вывода контроллера. Фильтрации нет.
  | ( 0 << 13 )         // Фильтрация PORTE_PIN13. Вывод управляет разрешением работы приёмопередатчика CAN. Фильтрации нет.
  | ( 0 << 14 )         // Фильтрация PORTE_PIN14. Вывод никуда не подключен. Фильтрации нет.
  | ( 0 << 15 );        // Фильтрация PORTE_PIN15. Вывод никуда не подключен. Фильтрации нет.
  
}
