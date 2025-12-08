/**
 * @file connection.h
 * @author Кошкин Е.В.
 * @version 1.0
 * @date 2025
 * @copyright Ваша организация
 * @brief Заголовочный файл класса Connection для сетевого взаимодействия
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
 * @class Connection
 * @brief Класс для управления сетевым соединением с сервером
 * @details Обеспечивает установку соединения, аутентификацию и передачу данных
 */
class Connection{
private:
    static string salt; ///< Соль для хеширования пароля
    
public:
    /**
     * @brief Установка соединения и обмен данными с сервером
     * @param[in] p Указатель на параметры соединения
     * @return 0 при успешном выполнении
     * @throw system_error при ошибках сетевого взаимодействия
     */
    static int conn(const Params* p);
};