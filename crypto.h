/**
 * @file crypto.h
 * @author Клименко Г.А.
 * @version 1.0
 * @date 2025
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для криптографических функций
 * @warning Используется библиотека CryptoPP
 */

#pragma once
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1 
#include <string>
#include <cryptopp/hex.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
using namespace std;
namespace CPP = CryptoPP;

/**
 * @brief Функция аутентификации с использованием хеширования
 * @param[in] salt Соль для хеширования
 * @param[in] pass Пароль пользователя
 * @return Хеш-строка в hex-формате
 * @details Использует алгоритм SHA224 для создания хеша соли и пароля
 */
string auth(string salt, string pass);