// #include <iostream>
// #include "Client.h"

// int main(){
//     std::cout << "Client here" << std::endl;
//     Client c;
//     return 0;
// }

#include "UtilsLib/Logger.h"
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    auto&              logger = Logger::instance();
    int                s = 0, bytes = 0;
    struct sockaddr_in sa;
    char               buffer[BUFSIZ + 1];
    
    logger.log(Logger::LogLevel::INFO, "BUFSIZ is ", BUFSIZ);

    logger.log(Logger::LogLevel::INFO, "Start");
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        logger.log(Logger::LogLevel::ERROR, "socket return: ", s);
        return 1;
    }
    logger.log(Logger::LogLevel::INFO, "socket return: ", s);

    memset(&sa, '\0', sizeof(sa));

    sa.sin_family      = AF_INET;
    sa.sin_port        = htons(13);
    
    // time.nist.gov
    sa.sin_addr.s_addr = htonl((((((128 << 8) | 138) << 8) | 141) << 8) | 172);
    // \todo handle timeout
    logger.log(Logger::LogLevel::INFO, "Connecting to the address");
    if (connect(s, (struct sockaddr*)&sa, sizeof sa) < 0) {
        logger.log(Logger::LogLevel::ERROR, "Error while connecting to the address");
        close(s);
        return 2;
    }
    logger.log(Logger::LogLevel::INFO, "Connected");
    while ((bytes = read(s, buffer, BUFSIZ)) > 0) {
        logger.log(Logger::LogLevel::INFO, "Writing to buffer");
        if (write(1, buffer, bytes) == -1) {
            logger.log(Logger::LogLevel::INFO, "Error while writing to buffer");
            break;
        }
    }
    close(s);
    logger.log(Logger::LogLevel::INFO, "Bytes ", bytes);
    return 0;
}