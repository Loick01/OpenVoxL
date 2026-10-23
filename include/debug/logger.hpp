#pragma once

#include <string>
#include <vector>

enum class LogLevel
{   
    Level1, Level2
};

struct Log
{
    std::string message;
    LogLevel level;
};

class Logger // Singleton
{
    private:
        std::vector<Log> m_logs;

    public:
        Logger();

        static Logger& GetInstance();

        const std::vector<Log>& GetLogs() const;

        void AddLog(const LogLevel level, const std::string& message);
        void Clear();
};