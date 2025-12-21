/**
 * @file main.cpp
 * @author Клименко Г.А.
 * @version 1.0
 * @date 2025
 * @copyright ИБСТ ПГУ
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
    UserInterface userinterface; ///< Объект пользовательского интерфейса
    
    // Парсинг аргументов командной строки
    if(!userinterface.Parser(argc, argv)){
        cout << userinterface.getDescription() << endl;
        return 1;
    }
    
    // Получение параметров и установка соединения
    Params params = userinterface.getParams();
    Communicator::conn(&params);
    
    return 0;
}