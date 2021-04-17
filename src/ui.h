#ifndef __RESOURCE_ui_H__
#define __RESOURCE_ui_H__

#include <gio/gio.h>

extern GResource *ui_get_resource (void);

extern void ui_register_resource (void);
extern void ui_unregister_resource (void);

#endif
