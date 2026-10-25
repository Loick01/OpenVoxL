#pragma once

#include <array>
#include <string>
#include <string_view>
#include <vector>

using Color = std::array<float, 3>;

enum class LogLevel
{   
    Level0, Level1, Level2, Level3
};

struct Log
{
    const std::string message;
    const LogLevel level;

    const std::string_view label;
    const Color labelColor;

    Log(const std::string& m, const LogLevel l):
        message(m), level(l), label(GetLevelLabel()), labelColor(GetLevelLabelColor())
    {}

    std::string_view GetLevelLabel() const;
    Color GetLevelLabelColor() const;
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