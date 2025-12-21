/**
 * @file interface.h
 * @author Клименко Г.А.
 * @version 1.0
 * @date 2025
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл пользовательского интерфейса
 * @details Использует boost::program_options для парсинга аргументов командной строки
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
 * @details Обеспечивает парсинг и валидацию входных параметров
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