#pragma once
#include <string>
#include <sstream>
#include <mutex>
#include <memory>
#include <fstream>

// log message levels
enum Level	{ Finest, Finer, Fine, Config, Info, Warning, Severe };
class logger;

class logstream : public std::ostringstream
{
public:
	logstream(logger& oLogger, Level nLevel);
	logstream(const logstream& ls);
	~logstream();

private:
	logger& m_oLogger;
	Level m_nLevel;
};

class logger
{
public:
	logger(std::string filename);
	virtual ~logger();

	void log(Level nLevel, std::string oMessage);

	logstream operator()();
	logstream operator()(Level nLevel);

private:
	const tm* getLocalTime();

private:
	std::mutex		m_oMutex;
	std::ofstream	m_oFile;

	tm				m_oLocalTime;
};

