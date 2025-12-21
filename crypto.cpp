/**
 * @file crypto.cpp
 * @author Клименко Г.А.
 * @version 1.0
 * @date 2025
 * @copyright ИБСТ ПГУ
 * @brief Реализация криптографических функций
 * @warning Используется библиотека CryptoPP
 */

#include "crypto.h"

/**
 * @brief Функция аутентификации с использованием хеширования
 * @param[in] salt Соль для хеширования
 * @param[in] pass Пароль пользователя
 * @return Хеш-строка в hex-формате
 * @details Использует алгоритм SHA224 для создания хеша комбинации соли и пароля
 */
string auth(string salt, string pass){
    CPP::SHA224 sha224; ///< Объект для вычисления SHA224
    string hash; ///< Результирующий хеш
    
    CPP::StringSource(
        string(salt).append(pass), ///< Конкатенация соли и пароля
        true,
        new CPP::HashFilter(
            sha224,
            new CPP::HexEncoder(
                new CPP::StringSink(hash)))
    );

    return hash; ///< Возврат хеша в hex-формате
}