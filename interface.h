/**
 * @file interface.h
 * @author Кошкин Е.В.
 * @version 1.0
 * @date 2025
 * @copyright Ваша организация
 * @brief Заголовочный файл пользовательского интерфейса
 */

#pragma once
#include <boost/program_options.hpp>
#include <string>
#include <sstream>
using namespace std;
namespace po = boost::program_options;

/**
 * @struct Params
 * @brief Структура для хранения параметров командной строки
 */
struct Params {
    string inFileName;   ///< Имя входного файла с данными
    string inFileResult; ///< Имя файла для результатов
    string inFileData;   ///< Имя файла с данными аутентификации
    int Port;           ///< Порт сервера
    string Address;     ///< Адрес сервера
};

/**
 * @class UserInterface
 * @brief Класс для обработки параметров командной строки
 * @details Использует boost::program_options для парсинга аргументов
 */
class UserInterface {
private:
    po::options_description desc; ///< Описание поддерживаемых опций
    po::variables_map vm;         ///< Переменные для хранения распарсенных значений
    Params params;               ///< Структура с параметрами
    
public:
    /**
     * @brief Конструктор класса UserInterface
     */
    UserInterface();
    
    /**
     * @brief Парсинг аргументов командной строки
     * @param[in] argc Количество аргументов
     * @param[in] argv Массив аргументов
     * @return true если парсинг успешен, false если требуется показать справку
     */
    bool Parser(int argc, const char** argv);
    
    /**
     * @brief Получение описания параметров
     * @return Строка с описанием поддерживаемых опций
     */
    string getDescription();
    
    /**
     * @brief Получение параметров
     * @return Структура Params с распарсенными значениями
     */
    Params getParams() {
        return params;
    };
};