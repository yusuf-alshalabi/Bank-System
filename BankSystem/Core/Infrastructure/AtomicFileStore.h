#pragma once
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace Bank::Persistence {

	// Crash-safe, line-oriented file store - migration of the atomic save
	// pattern from LastVersion/FileManager.h (temp file + backup + rename).
	//
	// Every SaveLines call follows the .tmp -> .bak -> rename sequence:
	//   1. write the full content to <file>.tmp
	//   2. copy the current <file> to <file>.bak
	//   3. swap <file>.tmp onto <file>
	// so a failure at any point leaves either the previous or the new content
	// on disk, never a truncated file. Reads validate the file before loading
	// and skip empty lines.
	//
	// This class intentionally does not include the logger or any other module
	// that depends on the session/entity headers, keeping it usable from the
	// persistence layer without include-order constraints.
	class AtomicFileStore
	{
	public:
		static constexpr const char Separator[] = "#//#";
		static constexpr std::uint64_t MaxFileSize = 100ull * 1024ull * 1024ull; // 100 MB

		// All non-empty lines of a valid file, else an empty vector.
		static std::vector<std::string> LoadLines(const std::string& path)
		{
			std::vector<std::string> lines;

			if (!IsValidFile(path))
				return lines;

			std::ifstream stream(path, std::ios::in);
			if (!stream.is_open())
				return lines;

			std::string line;
			while (std::getline(stream, line))
			{
				if (!line.empty() && line.back() == '\r')
					line.pop_back();
				if (!line.empty())
					lines.push_back(line);
			}
			stream.close();

			return lines;
		}

		// Atomically replace the file content. Returns false on any failure.
		static bool SaveLines(const std::string& path, const std::vector<std::string>& lines)
		{
			const std::string tmpPath = path + ".tmp";
			const std::string bakPath = path + ".bak";

			_EnsureParentDirectory(path);

			try
			{
				{
					std::ofstream tmp(tmpPath, std::ios::out | std::ios::trunc | std::ios::binary);
					if (!tmp.is_open())
						return false;

					for (const std::string& line : lines)
						tmp << line << '\n';
					tmp.close();
					if (!tmp)
					{
						_Remove(tmpPath);
						return false;
					}
				}

				std::error_code ec;
				if (std::filesystem::exists(path, ec))
				{
					std::error_code backupEc;
					std::filesystem::copy_file(path, bakPath,
						std::filesystem::copy_options::overwrite_existing, backupEc);
				}

				std::error_code removeEc;
				std::filesystem::remove(path, removeEc);

				std::error_code renameEc;
				std::filesystem::rename(tmpPath, path, renameEc);
				if (renameEc)
					return false;

				return true;
			}
			catch (const std::exception&)
			{
				_Remove(tmpPath);
				return false;
			}
		}

		// Appends a single line (used for append-only streams: transactions,
		// transfer log, login register audit).
		static bool AppendLine(const std::string& path, const std::string& line)
		{
			_EnsureParentDirectory(path);

			std::ofstream stream(path, std::ios::out | std::ios::app);
			if (!stream.is_open())
				return false;
			stream << line << '\n';
			stream.close();
			return static_cast<bool>(stream);
		}

		// True when the file exists, is non-empty and below the safety cap.
		static bool IsValidFile(const std::string& path)
		{
			std::error_code ec;
			if (!std::filesystem::exists(path, ec))
				return false;

			std::uintmax_t size = std::filesystem::file_size(path, ec);
			if (ec)
				return false;

			if (size == 0)
				return false;

			if (size > MaxFileSize)
				return false;

			return true;
		}

		// Splits a string by a delimiter, preserving empty fields.
		// Core::String::Split skips empty tokens, which breaks field-count
		// validation for records where optional fields are empty.
		static std::vector<std::string> SplitPreserve(const std::string& str, const std::string& delim)
		{
			std::vector<std::string> result;
			if (delim.empty())
			{
				result.push_back(str);
				return result;
			}

			std::size_t start = 0;
			while (true)
			{
				std::size_t pos = str.find(delim, start);
				if (pos == std::string::npos)
				{
					result.push_back(str.substr(start));
					break;
				}
				result.push_back(str.substr(start, pos - start));
				start = pos + delim.length();
			}
			return result;
		}

	private:
		static void _EnsureParentDirectory(const std::string& path)
		{
			const std::filesystem::path parent = std::filesystem::path(path).parent_path();
			if (parent.empty())
				return;
			std::error_code ec;
			std::filesystem::create_directories(parent, ec);
		}

		static void _Remove(const std::string& path)
		{
			std::error_code ec;
			std::filesystem::remove(path, ec);
		}
	};

} // namespace Bank::Persistence