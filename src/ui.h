#include <gtk/gtk.h>

extern GtkWidget *window;
extern GtkWidget *button;
extern GtkWidget *devImg;
extern GtkWidget *dName;
extern GtkWidget *fV;
extern GtkWidget *pL;

extern void set_button_label(const gchar *label);
extern void set_progress(const gchar *label);
extern void info(const char *text, const char *title);
extern int confirm(const char *q, const char *title);
extern void null_dlg(const char *q, const char *title);
