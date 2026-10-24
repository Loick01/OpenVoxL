#include "debug/logger.hpp"

#include <fstream>

#include "core/file/path.hpp"

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
        output << log.message << "\n";

    output.close();
}

void Logger::AddLog(const LogLevel level, const std::string& message)
{
    m_logs.push_back(Log{message, level});
}

void Logger::Clear()
{
    m_logs.clear();
}