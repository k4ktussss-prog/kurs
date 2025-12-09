/**
 * @file connection.cpp
 * @author Клименко Г.А.
 * @version 1.0
 * @date 2025
 * @copyright ИБСТ ПГУ
 * @brief Реализация классов Communicator и DataReader для сетевого взаимодействия
 */

#include "connection.h"

string Communicator::salt = "0000SALTSALT"; ///< Инициализация статической переменной соли

/**
 * @brief Чтение данных из файла и передача через сокет
 * @param[in] p Указатель на параметры с именами файлов
 * @param[in] s Дескриптор сокета для передачи данных
 * @return 0 при успешном выполнении
 * @throw system_error при ошибках ввода-вывода или сетевого взаимодействия
 * @details Метод выполняет:
 * - Чтение количества векторов из файла
 * - Передачу размеров и элементов каждого вектора на сервер
 * - Получение результатов от сервера и запись в файл
 */
int DataReader::datareader(const Params* p, int s){
    //Открытие файла данных
    ifstream file(p->inFileName);
    //Проверка
    if(!file.is_open()){
        throw system_error(errno, generic_category());
        return 1;
    }

    // Отправка числа векторов
    uint32_t num_vect; ///< Количество векторов для обработки
    file >> num_vect;
    if (send(s, &num_vect, sizeof(num_vect), 0) == -1) {
        close(s);
        throw system_error(errno, generic_category());
    }
    
    //Открытие файла результата
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

        //Запись результатов
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
    return 0;
}

/**
 * @brief Установка соединения с сервером
 * @param[in] p Указатель на параметры соединения
 * @return 0 при успешном выполнении
 * @throw system_error при ошибках сетевого взаимодействия
 * @details Метод выполняет:
 * - Создание сокета и установку соединения
 * - Аутентификацию с использованием соли и хеширования пароля
 * - Вызов DataReader для передачи данных
 */
int Communicator::conn(const Params* p){
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
    string line;
    getline(fileData, line); // Читаем всю строку из файла

    // Ищем разделитель ":"
    size_t delimiterPos = line.find(':');
    if (delimiterPos == string::npos) {
        // Обработка ошибки: неправильный формат файла
        cerr << "Ошибка: неправильный формат данных в файле" << endl;
        return 1; // или выбросить исключение
    }

    // Разделяем строку на логин и пароль
    string login = line.substr(0, delimiterPos); ///< Логин пользователя
    string pass = line.substr(delimiterPos + 1); ///< Пароль пользователя
    
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

    // Передача данных через DataReader
    DataReader::datareader(p, s);
    
    close(s);
    return 0;
};