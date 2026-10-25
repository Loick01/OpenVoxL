#include "debug/logger.hpp"

#include <fstream>

#include "core/file/path.hpp"

std::string_view Log::GetLevelLabel() const {
    switch (level) {
        case LogLevel::Level0:
            return "[DEBUG]";
        case LogLevel::Level1:
            return "[INFO]";
        case LogLevel::Level2: 
            return "[WARNING]";
        case LogLevel::Level3:
            return "[ERROR]";
        default:
            Logger::GetInstance().AddLog(LogLevel::Level1, "Unhandled LogLevel used in Log::GetLevelLabel");
            return "[ _ ]";
    }
}

Color Log::GetLevelLabelColor() const
{
    switch (level) {
        case LogLevel::Level0:
            return {0.23f, 0.68f, 0.95f};
        case LogLevel::Level1:
            return {0.16f, 0.65f, 0.07f};
        case LogLevel::Level2: 
            return {0.85f, 0.7f, 0.1f};
        case LogLevel::Level3:
            return {1.f, 0.25f, 0.25f};
        default:
            Logger::GetInstance().AddLog(LogLevel::Level1, "Unhandled LogLevel used in Log::GetLevelLabelColor");
            return {1.f, 1.f, 1.f};
    }
}

Logger::Logger()
{}

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

const std::vector<Log>& Logger::GetLogs() const
{
    return m_logs;
}

void Logger::WriteLogsInFile() const
{
    std::ofstream output(std::string(Path::LogFile) + std::string(FileExtension::Log));

    for (const Log& log : m_logs)
        output << log.label << " " << log.message << "\n";

    output.close();
}

void Logger::AddLog(const LogLevel level, const std::string& message)
{
    m_logs.emplace_back(message, level);
}

void Logger::Clear()
{
    m_logs.clear();
}