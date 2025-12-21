/**
 * @file connection.h
 * @author Клименко Г.А.
 * @version 1.0
 * @date 2025
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл классов Communicator и DataReader для сетевого взаимодействия
 */

#pragma once
#include "errno.h"
#include "crypto.h"
#include "interface.h"
#include <system_error>
#include <netinet/in.h>
#include <memory>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <fstream>
using namespace std;

#define BUFFER_SIZE 1024 ///< Размер буфера для сетевого обмена

/**
 * @class Communicator
 * @brief Класс для управления сетевым соединением с сервером
 * @details Обеспечивает установку соединения и аутентификацию
 */
class Communicator{
private: 
    static string salt; ///< Соль для хеширования пароля
    
public:
    /**
     * @brief Установка соединения с сервером
     * @param[in] p Указатель на параметры соединения
     * @return 0 при успешном выполнении
     * @throw system_error при ошибках сетевого взаимодействия
     */
    static int conn(const Params* p);
};

/**
 * @class DataReader
 * @brief Класс для чтения и передачи данных через сетевое соединение
 * @details Обрабатывает файлы с векторами и передает их на сервер
 */
class DataReader{
public:
    /**
     * @brief Чтение данных из файла и передача через сокет
     * @param[in] p Указатель на параметры с именами файлов
     * @param[in] s Дескриптор сокета для передачи данных
     * @return 0 при успешном выполнении
     * @throw system_error при ошибках ввода-вывода или сетевого взаимодействия
     */
    static int datareader(const Params* p, int s);
};