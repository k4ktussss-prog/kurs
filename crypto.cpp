/**
 * @file crypto.cpp
 * @author Кошкин Е.В.
 * @version 1.0
 * @date 2025
 * @copyright Ваша организация
 * @brief Реализация криптографических функций
 */

#include "crypto.h"

/**
 * @brief Функция аутентификации с использованием хеширования
 * @param[in] salt Соль для хеширования
 * @param[in] pass Пароль пользователя
 * @return Хеш-строка в hex-формате
 * @details Использует алгоритм SHA256 для создания хеша комбинации соли и пароля
 */
string auth(string salt, string pass){
    CPP::SHA256 sha256; ///< Объект для вычисления SHA256
    string hash; ///< Результирующий хеш
    
    CPP::StringSource(
        string(salt).append(pass), ///< Конкатенация соли и пароля
        true,
        new CPP::HashFilter(
            sha256,
            new CPP::HexEncoder(
                new CPP::StringSink(hash)))
    );

    return hash; ///< Возврат хеша в hex-формате
}