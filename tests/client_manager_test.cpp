#include <iostream>
#include "ClientManager.hpp"

int main(void)
{
    ClientManager   clientManager;
    Client*         client;

    clientManager.addClient(4);
    clientManager.addClient(5);

    client = clientManager.getByFd(4);
    if (!client)
        return(1);
    
    client->setNickname("Leon");
    if (!clientManager.nicknameExists("Leon"))
    {
        std::cout << "nickname test failed \n";
        return (1);
    }

    client->appendInput("NICK le");
    if (client->hasCompleteLine())
    {
        std::cout << "partial line test failed\n";
        return (1);
    }

    client->appendInput("on\r\nUser leon 0 * :Leon\r\n");
    
    if (client->popLine() != "NICK leon")
    {
        std::cout << "first popLine failed\n";
        return (1);

    }
    if (client->popLine() != "User leon 0 * :Leon")
	{
		std::cout << "second popLine failed" << std::endl;
		return (1);
	}

    //test appendOutput
    client->appendOutput("Hello world");
    client->consumeOutput(6); //"H e l l o _"

    if (client->getOutputBuffer() != "world") {
        std::cout << "consumeOutput failed\n";
        return (1);
    }
    std::cout << "Client and ClientManager tests passed\n";
    return (0);

}