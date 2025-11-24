#include "connection.h"
#include "interface.h"

int main(int argc, const char** argv)
{
    UserInterface userinterface;
    if(!userinterface.Parser(argc, argv)){
        cout << userinterface.getDescription() << endl;
        return 1;
    }
    Params params = userinterface.getParams();
    Communicator::conn(&params);
    return 0;
}