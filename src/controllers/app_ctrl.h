/*
 * Created by french on 2/4/24.
*/

#ifndef MINI_APP_APP_CTRL_H
#define MINI_APP_APP_CTRL_H
#include <stdbool.h>

/**
 * @brief initialize the application, parsing input arguments and setting application state prior
 * to application start
 * @param argc the number of input args we were passed
 * @param argv the input arg list
 */
void app_init(int argc, char **argv);

/**
 * @brief An function for finalizing the application.
 */
void app_finalize(void);

#endif  /* MINI_APP_APP_CTRL_H */
