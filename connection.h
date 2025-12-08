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
#define BUFFER_SIZE 1024

class Communicator{
private: 
    static string salt;
public:
    static int conn(const Params* p);
};

class DataReader{
public:
static int datareader(const Params* p, int s);
};