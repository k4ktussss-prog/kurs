/**
 * @file main.cpp
 * @author Кошкин Е.В.
 * @version 1.0
 * @date 2025
 * @copyright Ваша организация
 * @brief Главный файл приложения
 * @details Точка входа в программу, обработка параметров и запуск соединения
 */

#include "connection.h"
#include "interface.h"

/**
 * @brief Главная функция приложения
 * @param[in] argc Количество аргументов командной строки
 * @param[in] argv Массив аргументов командной строки
 * @return 0 при успешном выполнении, 1 при ошибке
 */
int main(int argc, const char** argv)
{
    UserInterface interface; ///< Объект пользовательского интерфейса
    
    // Парсинг аргументов командной строки
    if(!interface.Parser(argc, argv)){
        cout << interface.getDescription() << endl;
        return 1;
    }
    
    // Получение параметров и установка соединения
    Params params = interface.getParams();
    Connection::conn(&params);
    
    return 0;
}