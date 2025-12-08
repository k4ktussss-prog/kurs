/**
 * @file connection.cpp
 * @author Кошкин Е.В.
 * @version 1.0
 * @date 2025
 * @copyright Ваша организация
 * @brief Реализация класса Connection для сетевого взаимодействия
 */

#include "connection.h"

string Connection::salt = "1234567812345678"; ///< Инициализация статической переменной соли

/**
 * @brief Установка соединения и обмен данными с сервером
 * @param[in] p Указатель на параметры соединения
 * @return 0 при успешном выполнении
 * @throw system_error при ошибках сетевого взаимодействия
 * @details Метод выполняет:
 * - Создание сокета и установку соединения
 * - Аутентификацию с использованием соли и хеширования пароля
 * - Чтение векторов из файла и передачу на сервер
 * - Получение результатов от сервера и запись в файл
 */
int Connection::conn(const Params* p){
    // Создание сокета
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) 
        throw system_error(errno, generic_category()); 
    
    // Настройка локального адреса
    unique_ptr <sockaddr_in> self_addr(new sockaddr_in); 
    self_addr->sin_family = AF_INET; ///< Семейство адресов IPv4
    self_addr->sin_port = 0; ///< Автоматический выбор порта
    self_addr->sin_addr.s_addr = 0; ///< Автоматический выбор адреса

    // Настройка адреса сервера
    unique_ptr <sockaddr_in> serv_addr(new sockaddr_in);
    serv_addr->sin_family = AF_INET; ///< Семейство адресов IPv4
    serv_addr->sin_port = htons(p->Port); ///< Порт сервера
    serv_addr->sin_addr.s_addr = inet_addr(p->Address.c_str()); ///< IP-адрес сервера

    // Установка соединения
    if (connect(s, (sockaddr*) serv_addr.get(), sizeof(sockaddr_in)) == -1)
        throw system_error(errno, generic_category()); 
    
    // Аутентификация
    ifstream fileData(p->inFileData);
    string login;
    fileData >> login; ///< Чтение логина из файла
    string pass;
    fileData >> pass; ///< Чтение пароля из файла
    
    // Подготовка и отправка данных аутентификации
    salt = string(16 - salt.size(), '0') + salt;
    login.append(salt + auth(salt, pass));
    send(s, login.c_str(), login.length(), 0);
    
    // Получение подтверждения от сервера
    char buffer[BUFFER_SIZE];
    ssize_t received_bytes = recv(s, buffer, BUFFER_SIZE - 1, 0);
    if (received_bytes == -1) 
        throw system_error(errno, generic_category());
    buffer[received_bytes] = '\0';

    // Обработка данных векторов
    ifstream file(p->inFileName);
    if(!file.is_open()){
        throw system_error(errno, generic_category());
        return 1;
    }

    // Отправка количества векторов
    uint32_t num_vect; ///< Количество векторов для обработки
    file >> num_vect;
    if (send(s, &num_vect, sizeof(num_vect), 0) == -1) {
        close(s);
        throw system_error(errno, generic_category());
    }

    // Открытие файла для результатов
    ofstream fileResult(p->inFileResult);

    // Обработка каждого вектора
    for (uint32_t i = 0; i < num_vect; i++){
        uint32_t size_vect; ///< Размер текущего вектора
        file >> size_vect;
        if (send(s, &size_vect, sizeof(size_vect), 0) == -1) {
            close(s);
            throw system_error(errno, generic_category());
        }

        vector<double> num_vectt; ///< Вектор числовых значений
        for (uint32_t i = 0; i < size_vect; i++)
        {
            double v;
            file >> v;
            num_vectt.push_back(v);
        }
        
        // Отправка элементов вектора
        for (double num : num_vectt) {
            double network_num = num; ///< Число в сетевом порядке байт
            if (send(s, &network_num, sizeof(network_num), 0) == -1) {
                close(s);
                throw system_error(errno, generic_category());
            }
        }

        // Получение и запись результата
        double result; ///< Результат обработки от сервера
        ssize_t result_received = recv(s, &result, sizeof(result), 0);
        if (result_received == sizeof(result)){
            cout << "Результат от сервера: " << result << endl;
            fileResult << result << endl;
        }else if (result_received == -1){
            close(s);
            throw system_error(errno, generic_category());
        }
    }
    close(s);
    return 0;
};