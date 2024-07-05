//
// Created by french on 2/3/24.
//
#ifndef MINI_APP_VIEW_BUILDER_H
#define MINI_APP_VIEW_BUILDER_H
#include "comm.h"

G_BEGIN_DECLS // C++ guard, from gmacros.h

/**
 * @brief The main application window build
 * @return an app_widget_ref_struct with the application widgets mapped
 * to their respective builder-constructed template objects
 */
app_widget_ref_struct *app_builder(void);

G_END_DECLS
#endif  // MINI_APP_VIEW_BUILDER_H
