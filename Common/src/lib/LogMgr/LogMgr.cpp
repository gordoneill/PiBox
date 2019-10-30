/* Name: 		Gordon O'Neill
 * File Name: 	LogMgr.c
 * Purpose: 	instantiation of logging utility
 */

#include "LogMgr.h"
#include <string>
#include <cstring>

LogMgr::LogMgr() :
	fid_(ERROR)
{

}

LogMgr::~LogMgr()
{
	closeLogfile();
}

/*  @brief     Writes logs to file specified by set_logfile, else a default file
 *  @param l   Severity level of log
 *  @param fmt Format of the log message
 *  @param ... Additional arguments (i.e. message string pointer)
 *  @return    bool true if successful, false otherwise
 */
bool LogMgr::logEvent(eLevels l, const char *fmt, ...)
{
	bool okay = true;
	if (fid_ == ERROR)
	{
		okay = okay && setLogfile("logfile");
	}
	if (fid_ != ERROR)
	{
		std::string log;

		time_t curTime;
		time(&curTime);
		log = ctime(&curTime);
		log.pop_back();
		log += ':';

		switch (l)
		{
		  case INFO: 
			log += "INFO:";
			break;
		  case WARNING: 
			log += "WARNING:";
			break;
		  case FATAL:
			log += "FATAL:";
			break;
		  default:
			okay = false;
			break;
		}

		char tmpString[MAX_MSG_SIZE];
		memset(tmpString, '\0', sizeof(tmpString));
		va_list args;
		va_start(args, fmt);

		if(vsprintf(tmpString, fmt, args) < OK)
		{
			okay = false;
		}
		else
		{
			log += std::string(tmpString);
		}

		log += "\n";

		okay = okay && (write(fid_, log.c_str(), log.length()) == (int) log.length());
	}
	else
	{
		okay = false;
	}

	return okay;
}

/*  @brief              Opens and sets new log file
 *  @param logfile_name Character pointer of new log file name
 *  @return             bool true if successful, false otherwise
 */
bool LogMgr::setLogfile(const char *logfile_name)
{
	bool okay = true;
	int tmpFid = open(logfile_name, O_APPEND|O_WRONLY|O_SYNC|O_CREAT, FULL_PERMISSION);
	if (tmpFid != ERROR)
	{
        closeLogfile();
		fid_ = tmpFid;
	}
	else
    {
        std::string buffer;
        buffer = "System call 'open' returned an error value of " + std::to_string(tmpFid);
        perror(buffer.c_str());
		okay = false;
	}
	return okay;
}

/*  @brief  Closes an open file descriptor
 *  @param  none
 *  @return bool true if successful, false otherwise
 */
bool LogMgr::closeLogfile()
{
	bool okay = true;
	if (fid_ != ERROR)
	{
		int retval = 0;
		retval = close(fid_);
		if(retval != ERROR)
		{
			fid_ = -1;
		}
		else
		{
			std::string buffer;
			buffer = "System call 'close' returned an error value of " + std::to_string(retval);
			perror(buffer.c_str());
			okay = false;
		}
	}
	return okay;
}
