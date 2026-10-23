#include "debug/logger.hpp"

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

void Logger::AddLog(const LogLevel level, const std::string& message)
{
    m_logs.push_back(Log{message, level});
}

void Logger::Clear()
{
    m_logs.clear();
}