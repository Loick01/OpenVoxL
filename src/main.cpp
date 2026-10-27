#include <exception>
#include <iostream>

#include "core/application.hpp"
#include "setting/logger.hpp"

int main()
{   
    try {
        Application app;
        bool isRunning = true;

        while(isRunning){
            isRunning = app.Run();
        }

        Logger::GetInstance().WriteLogsInFile(); // Logger::WriteLogsInFile is called no matter how the execution exits, and only once
    } catch (const std::exception& e) {
        std::cerr << "[Unhandled exception] " << e.what() << "\n";
        Logger::GetInstance().WriteLogsInFile();
        return EXIT_FAILURE;
    }
    
    return 0;
}