#ifndef __LOGGER_H_
#define __LOGGER_H_

const char *logger_init(const char *prefix);

/*!
 @brief Add current data to logs
 */
void logger_update(void);

/*!
 @brief Write log to disk
 
 This should only be called by other modules when closing down the log
 */
void logger_sync(void);

void logger_close(void);

#endif
