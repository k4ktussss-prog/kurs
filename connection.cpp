#include "connection.h"
string Communicator::salt = "0000SALTSALT";

int DataReader::datareader(const Params* p, int s){
    //Открытие файла данных
    ifstream file(p->inFileName);
    //Проверка
    if(!file.is_open()){
        throw system_error(errno, generic_category());
        return 1;
    }

        // Отправка числа векторов
    uint32_t num_vect; // Число векторов
    file >> num_vect;
    if (send(s, &num_vect, sizeof(num_vect), 0) == -1) {
        close(s);
        throw system_error(errno, generic_category());
    }
    
    //Открытие файла результата
    ofstream fileResult(p->inFileResult);

    for (uint32_t i = 0; i < num_vect; i++){
        uint32_t size_vect; // Размер вектора
        file >> size_vect;
        if (send(s, &size_vect, sizeof(size_vect), 0) == -1) {
            close(s);
            throw system_error(errno, generic_category());
        }

        vector<double> num_vectt; //Значения вектора
        for (uint32_t i = 0; i < size_vect; i++)
        {
            double v;
            file >> v;
            num_vectt.push_back(v);
        }
        
        // Отправка элементов вектора
        for (double num : num_vectt) {
            double network_num = num; // Преобразование в сетевой порядок байт
            if (send(s, &network_num, sizeof(network_num), 0) == -1) {
                close(s);
                throw system_error(errno, generic_category());
            }
        }

        //Запись результатов
        double result;
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


int Communicator::conn(const Params* p){
    
    int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == -1) 
            throw system_error(errno, generic_category()); 
    
        
    unique_ptr <sockaddr_in> self_addr(new sockaddr_in); 
            self_addr->sin_family = AF_INET; // всегда так
            self_addr->sin_port = 0; // автовыбор
            self_addr->sin_addr.s_addr = 0; // автовыбор


    unique_ptr <sockaddr_in> serv_addr(new sockaddr_in); // умный указатель
        serv_addr->sin_family = AF_INET; // всегда так
        serv_addr->sin_port = htons(p->Port); // конкретное значение
        serv_addr->sin_addr.s_addr = inet_addr(p->Address.c_str());//конкретное значение


    if (connect(s, (sockaddr*) serv_addr.get(), sizeof(sockaddr_in)) == -1)
        throw system_error(errno, generic_category()); 
    
    //Чтение данных из файла
    ifstream fileData(p->inFileData);
    //Получение логина
    string login;
    fileData >> login;
    //Получение пароля
    string pass;
    fileData >> pass;
    //Отправка логина на сервер
    salt = string(16 - salt.size(), '0') + salt;
    login.append(salt + auth(salt, pass));
    
    send(s, login.c_str(), login.length(), 0);
    
    //Прием ответа сервера (ОК)
    char buffer[BUFFER_SIZE];
    ssize_t received_bytes = recv(s, buffer, BUFFER_SIZE - 1, 0);
    if (received_bytes == -1) 
        throw system_error(errno, generic_category());
    buffer[received_bytes] = '\0';

    DataReader::datareader(p, s);
    
    close(s);
return 0;
};



