/* Name: 		Gordon O'Neill
 * Course: 		System Development Unix Environment Fall 2018
 * File Name: 	log_mgr.h
 * Purpose: 	Declaration of logging methods and variables
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define ERROR -1
#define OK    0

#define DEFAULT_LOG_FILE "logfile"
#define FULL_PERMISSION  0777
#define MAX_LOG_SIZE     100
#define MAX_MSG_SIZE     80

typedef enum {
	INFO, 
	WARNING, 
	FATAL
} eLevels;

class logMgr
{
public:
	void logMgr();
	void ~logMgr();

	/*  @brief     Writes logs to file specified by set_logfile, else a default file
	 *  @param l   Severity level of log
	 *  @param fmt Format of the log message
	 *  @param ... Additional arguments (i.e. message string pointer)
	 *  @return    OK (0) if write was successful or ERRROR (-1)
	 */
	int logEvent (eLevels l, const char *fmt, ...);

	/*  @brief              Opens and sets new log file
	 *  @param logfile_name Character pointer of new log file name
	 *  @return             OK (0) if open was successful or ERRROR (-1)
	 */
	int setLogfile (const char *logfile_name);

	/*  @brief  Closes an open file descriptor
	 *  @param  none
	 *  @return void
	 */
	void closeLogfile (void);

private:
	int Fid;
}