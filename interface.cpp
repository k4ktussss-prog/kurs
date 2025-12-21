/**
 * @file interface.cpp
 * @author Клименко Г.А.
 * @version 1.0
 * @date 2025
 * @copyright ИБСТ ПГУ
 * @brief Реализация пользовательского интерфейса
 */

#include "interface.h"

/**
 * @brief Конструктор класса UserInterface
 * @details Инициализирует опции командной строки с обязательными параметрами
 */
UserInterface::UserInterface() : desc("Allowed options")
{
    // добавление параметров в парсер командной строки
    desc.add_options()
    ("help,h", "Show help") ///< Опция помощи
    ("input,i", po::value<std::string>(&params.inFileName)->required(),"Set input file name") ///< Входной файл
    ("result,r", po::value<std::string>(&params.inFileResult)->required(),"Set output file name") ///< Файл результатов
    ("data,d", po::value<std::string>(&params.inFileData)->required(),"Set data file name") ///< Файл с данными аутентификации
    ("port,t", po::value<int>(&params.Port)->required(), "Set port") ///< Порт сервера
    ("address,a", po::value<string>(&params.Address)->required(), "Set address"); ///< Адрес сервера
}

/**
 * @brief Парсинг аргументов командной строки
 * @param[in] argc Количество аргументов
 * @param[in] argv Массив аргументов
 * @return true если парсинг успешен, false если требуется показать справку
 */
bool UserInterface::Parser(int argc, const char** argv)
{
    // Если нет аргументов или только --help, показываем справку
    if (argc == 1) {
        return false;
    }
    // парсинг
    po::store(po::parse_command_line(argc, argv, desc), vm);
    // обрабатываем --help до вызова notify
    if (vm.count("help"))
    return false;
    // присвоение значений по умолчанию и возбуждение исключений
    po::notify(vm);
    return true;
}

/**
 * @brief Получение описания параметров
 * @return Строка с описанием поддерживаемых опций
 */
std::string UserInterface::getDescription()
{
    std::ostringstream ss;
    ss << desc;
    return ss.str();
}