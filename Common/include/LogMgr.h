/* Name: 		Gordon O'Neill
 * File Name: 	LogMgr.h
 * Purpose: 	Declaration of logging utility
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
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

class LogMgr
{
public:
    LogMgr();
    ~LogMgr();

	/*  @brief     Writes logs to file specified by set_logfile, else a default file
	 *  @param l   Severity level of log
	 *  @param fmt Format of the log message
	 *  @param ... Additional arguments (i.e. message string pointer)
	 *  @return    bool true if successful, false otherwise
	 */
	bool logEvent (eLevels l, const char *fmt, ...);

	/*  @brief              Opens and sets new log file
	 *  @param logfile_name Character pointer of new log file name
	 *  @return             bool true if successful, false otherwise
	 */
	bool setLogfile (const char *logfile_name);

	/*  @brief  Closes an open file descriptor
	 *  @param  none
	 *  @return bool true if successful, false otherwise
	 */
	bool closeLogfile (void);

private:
	int fid_;
};
