#include "interface.h"


UserInterface::UserInterface() : desc("Allowed options")
{
    // добавление параметров в парсер командной строки
    desc.add_options()
    ("help,h", "Show help")
    ("input,i", po::value<std::string>(&params.inFileName)->required(),"Set input file name")
    ("result,r", po::value<std::string>(&params.inFileResult)->required(),"Set output file name")
    ("data,d", po::value<std::string>(&params.inFileData)->required(),"Set data file name")
    ("port,t", po::value<int>(&params.Port)->required(), "Set port")
    ("address,a", po::value<string>(&params.Address)->required(), "Set address");

}


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


std::string UserInterface::getDescription()
{
    std::ostringstream ss;
    ss << desc;
    return ss.str();
}
