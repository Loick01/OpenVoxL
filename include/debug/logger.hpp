#pragma once

#include <string>
#include <vector>

enum class LogLevel
{   
    Level0, Level1, Level2, Level3
};

struct Log
{
    std::string message;
    LogLevel level; // TODO : Use level in DebugWindow
};

class Logger // Singleton
{
    private:
        std::vector<Log> m_logs;

    public:
        Logger();

        static Logger& GetInstance();

        const std::vector<Log>& GetLogs() const;
        void WriteLogsInFile() const; // TODO : For now, logs are not filtered according to their LogLevel

        void AddLog(const LogLevel level, const std::string& message);
        void Clear();
};