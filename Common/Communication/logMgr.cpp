/* Name: 		Gordon O'Neill
 * File Name: 	log_mgr.c
 * Purpose: 	instantiation of logging utility
 */

#include "logMgr.h"
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
 *  @return    OK (0) if write was successful or ERRROR (-1)
 */
int LogMgr::logEvent(eLevels l, const char *fmt, ...)
{
	if (fid_ == ERROR)
	{
		setLogfile("logfile");
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
			return ERROR;
			break;
		}

		char tmpString[MAX_MSG_SIZE];
		memset(tmpString, '\0', sizeof(tmpString));
		va_list args;
		va_start(args, fmt);

		if(vsprintf(tmpString, fmt, args) < OK)
		{
			return ERROR;
		}
		else
		{
			log += std::string(tmpString);
		}

		log += "\n";

		if (write(fid_, log.c_str(), log.length()) != (int) log.length())
		{
			return ERROR;
		}
		else
		{
			return OK;
		}
	}
	else
	{
		return ERROR;
	}
	return OK;
}

/*  @brief              Opens and sets new log file
 *  @param logfile_name Character pointer of new log file name
 *  @return             OK (0) if open was successful or ERRROR (-1)
 */
int LogMgr::setLogfile(const char *logfile_name)
{
	int tmpFid = open(logfile_name, O_APPEND|O_WRONLY|O_SYNC|O_CREAT, FULL_PERMISSION);
	if (tmpFid != ERROR)
	{
        closeLogfile();
		fid_ = tmpFid;
		return OK;
	}
	else
    {
        std::string buffer;
        buffer = "System call 'open' returned an error value of " + std::to_string(tmpFid);
        perror(buffer.c_str());
		return ERROR;
	}
}

/*  @brief  Closes an open file descriptor
 *  @param  none
 *  @return void
 */
void LogMgr::closeLogfile()
{
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
		}
	}
}
