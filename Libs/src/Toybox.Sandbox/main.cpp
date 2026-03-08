#include "Toybox/Common.h"
#include "Toybox/Server.h"

TOYBOX_NAMESPACE_BEGIN

void sandbox() {
    Server s(8080);
    s.run();
    std::cout << "finised" << std::endl;
    // Add sandbox code
}

TOYBOX_NAMESPACE_END

int main() {
    toybox::sandbox();
    return 0;
}
