#pragma once
#include <fstream>
#include <mutex>
#include <string>

#include "SessionManager.h"
#include "../../../Libs/Cpp-Library-Collection/Lib/Date.h"

namespace Bank::Diagnostics {

	enum class LogLevel { Info, Warning, Error, Critical };

	// Thread-safe leveled logger writing to SystemLog.txt - migration of
	// LastVersion/Logger.h. The acting user is resolved from the active session
	// and degrades to "SYSTEM" when no session is running.
	class Logger
	{
	public:
		static Logger& Instance()
		{
			static Logger instance;
			return instance;
		}

		void Log(LogLevel level, const std::string& message)
		{
			std::lock_guard<std::mutex> lock(_Mutex);

			std::ofstream stream(_LogFile, std::ios::app);
			if (!stream.is_open())
				return;

			stream << "[" << Core::Date::GetSystemDateTime() << "] "
				<< "[" << _ToString(level) << "] "
				<< "[User: " << _CurrentUserName() << "] "
				<< message << '\n';
		}

		void LogLoginAttempt(const std::string& userName, bool succeeded)
		{
			Log(succeeded ? LogLevel::Info : LogLevel::Warning,
				"Login attempt for user '" + userName + "' - " + (succeeded ? "SUCCESS" : "FAILED"));
		}

		void LogUserAction(const std::string& action, const std::string& details = "")
		{
			Log(LogLevel::Info, "Action: " + action + (details.empty() ? "" : " - Details: " + details));
		}

		void LogTransaction(const std::string& transactionId, const std::string& type,
			double amount, double fees, const std::string& source, const std::string& destination)
		{
			Log(LogLevel::Info, "Transaction " + transactionId + " - Type: " + type +
				" - Amount: " + std::to_string(amount) +
				" - Fees: " + std::to_string(fees) +
				" - Source: " + source +
				" - Destination: " + destination);
		}

	private:
		Logger() = default;
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		static const char* _ToString(LogLevel level)
		{
			switch (level)
			{
			case LogLevel::Info:     return "INFO";
			case LogLevel::Warning:  return "WARNING";
			case LogLevel::Error:    return "ERROR";
			case LogLevel::Critical: return "CRITICAL";
			}
			return "UNKNOWN";
		}

		std::string _CurrentUserName() const
		{
			return Bank::Security::SessionManager::Instance().CurrentUserName();
		}

		std::string _LogFile = "SystemLog.txt";
		std::mutex _Mutex;
	};

} // namespace Bank::Diagnostics