#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <memory>

int main(void)
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        std::cout << "Hello parent !" << std::endl;
    }   
    else
    {
        std::cerr << "error" << std::endl;
    }
    return (0);
}