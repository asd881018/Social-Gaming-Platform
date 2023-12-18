#include "clientHelper.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage : \n  " << argv[0] << " <ip address> <port>\n"
                  << "  e.g. " << argv[0] << " localhost 4002\n";
        return 1;
    }
    networking::Client client{argv[1], argv[2]};
    runClient(client);

    return 0;
}
