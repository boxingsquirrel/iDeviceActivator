#include <gtk/gtk.h>
#include "ui.h"

// This is a REALLY DIRTY HACK to set the button text: set it twice!
void set_button_label(const gchar *label)
{
	gtk_button_set_label(button, label);
	gtk_button_set_label(button, label);
}

void set_progress(const gchar *label)
{
	gtk_label_set_text(GTK_LABEL(pL), label);
	gtk_main_iteration();
}

// Displays a little GTK+ info dialog with the specified text and title
void info(const char *text, const char *title)
{
		// Create the dialog
		GtkWidget *d=gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, text);

		// Set the title
		gtk_window_set_title(GTK_WINDOW(d), title);

		g_signal_connect_swapped (d, "response", G_CALLBACK (gtk_widget_destroy), d);

		// Display the dialog and return the result (not that we care ;P)
		gint result = gtk_dialog_run(GTK_DIALOG(d));

		// Kill it.
		gtk_widget_destroy(d);
}

// Displays a GTK+ Yes/No dialog and returns 0 for no and 1 for yes
int confirm(const char *q, const char *title)
{
	// Create it
	GtkWidget *d=gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, q);

	// Set the title
	gtk_window_set_title(GTK_WINDOW(d), title);

	// Show the dialog and return the result
	gint result = gtk_dialog_run(GTK_DIALOG(d)); 

	// No was clicked
	if (result==GTK_RESPONSE_NO)
	{
		// Kill the dialog
		gtk_widget_destroy(d);

		// Return 0
		return 0;
	}

	// Yes was clicked
	else if (result==GTK_RESPONSE_YES) {
		// Kill the dialog
		gtk_widget_destroy(d);

		// Return 1
		return 1;
	}

	// WTF?
	else {
		// Kill the dialog
		gtk_widget_destroy(d);

		// Return -1
		return -1;
	}

}

// Displays a GTK+ Yes/No dialog and returns 0 for no and 1 for yes
void null_dlg(const char *q, const char *title)
{
	// Create it
	GtkWidget *d=gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, q);

	// Set the title
	gtk_window_set_title(GTK_WINDOW(d), title);

	// Show the dialog and return the result
	//gint result = gtk_dialog_run(GTK_DIALOG(d)); 
}
