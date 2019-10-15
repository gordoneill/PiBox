/* Name: 		Gordon O'Neill
 * File Name: 	log_mgr.c
 * Purpose: 	instantiation of logging utility
 */

#include "logMgr.h"

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
		char log[MAX_LOG_SIZE];
		memset(log, '\0', sizeof(log));

		time_t curTime;
		time(&curTime);
		strcpy(log, ctime(&curTime));
		log[strlen(log) - 1] = ':';

		switch (l)
		{
		  case INFO: 
			strcat(log, "INFO:");
			break;
		  case WARNING: 
			strcat(log, "WARNING:");
			break;
		  case FATAL:
			strcat(log, "FATAL:");
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
			strcat(log, tmpString);
		}

		strcat(log, "\n");

		if (write(fid_, log, strlen(log)) != strlen(log))
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
        char * buffer;
        sprintf(buffer, "System call 'open' returned an error value of %d", tmpFid);
        perror(buffer);
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
			char * buffer;
			sprintf(buffer, "System call 'close' returned an error value of %d", retval);
			perror(buffer);
		}
	}
}
