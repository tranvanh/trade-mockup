#include "Toybox/Common.h"
#include "Toybox/Client.h"

TOYBOX_NAMESPACE_BEGIN

void sandbox() {
    Client client;
    client.connect("127.0.0.1", 8080);
    client.sendMessage("hello world");
    client.run();
    std::cout << "finised" << std::endl;
    // Add sandbox code
}

TOYBOX_NAMESPACE_END

int main() {
    toybox::sandbox();
    return 0;
}
