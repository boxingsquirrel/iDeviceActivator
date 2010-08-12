/* Test */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <plist/plist.h>
#include <libimobiledevice/afc.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/notification_proxy.h>
#include <gtk/gtk.h>
#include "device.h"
#include "file.h"
#include "ui.h"
#include "activate.h"


GtkWidget *window;
GtkWidget *button;
GtkWidget *button1;
GtkWidget *devImg;
GtkWidget *dName;
GtkWidget *fV;
GtkWidget *pL;
GtkWidget *mmle_check;

char latest_plist[BUFSIZE];
char current_plist[BUFSIZE];
char img_plist[BUFSIZE];

#define iPhone2G "iPhone1,1"

#define iPhone3G "iPhone1,2"

#define iPhone3GS "iPhone2,1"

#define iPhone4 "iPhone3,1"

#define ipt1g "iPod1,1"

#define ipt2g "iPod2,1"

#define ipt3g "iPod3,1"

#define iPad1G "iPad1,1"

int conf_up=0;

static void activate(GtkWidget *widget, gpointer data)
{
	// Let the main thread update...
	gtk_main_iteration();

	gboolean make_my_life_easier=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(mmle_check));

	if (make_my_life_easier)
	{
		// Activate!
		activate_thread(1);
	}
	else {
		if (confirm("Are you sure you don't want you life to be easier should you have to reactivate?", "Are you sure?")!=1)
		{
			gtk_main_iteration();

			// Activate!
			activate_thread(1);
		}
		else {
			gtk_main_iteration();

			// Activate!
			activate_thread(0);
		}
	}

	// Fill in the info again (to get new device name)
	//fill_in_info();
}

static void deactivate(GtkWidget *widget, gpointer data)
{
	gtk_main_iteration();

	deactivate_device(client);
}

static void destroy(GtkWidget *widget, gpointer data)
{
	if (widget==window)
	{
		int e=finish();
		gtk_main_quit();
	}
}

int main(int argc, char *argv[])
{
	gtk_init (&argc, &argv);

	GtkBuilder *builder;
	GError* error = NULL;

	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, "/usr/local/share/iDeviceActivator/res/ui.xml", &error))
	{
		g_warning ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	gtk_builder_connect_signals (builder, NULL);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
	button=GTK_WIDGET(gtk_builder_get_object(builder, "uBut"));
	button1=GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
	devImg=GTK_WIDGET(gtk_builder_get_object(builder, "devImg"));
	dName=GTK_WIDGET(gtk_builder_get_object(builder, "dName"));
	fV=GTK_WIDGET(gtk_builder_get_object(builder, "fV"));
	pL=GTK_WIDGET(gtk_builder_get_object(builder, "pL"));
	mmle_check=GTK_WIDGET(gtk_builder_get_object(builder, "make_my_life_easier"));

	g_signal_connect (G_OBJECT (button), "released",
		      G_CALLBACK (activate), NULL);
	g_signal_connect (G_OBJECT (button1), "released",
		      G_CALLBACK (deactivate), NULL);
        g_signal_connect (G_OBJECT (window), "destroy",
		      G_CALLBACK (destroy), NULL);

	set_up();

	read_in_plists();

	fill_in_info();

	g_object_unref (builder);

	gtk_widget_show (window);

	gtk_main ();

	return 0;
}

int fill_in_info()
{
	plist_t *node=NULL;
	char* type=NULL;
	char* version=NULL;
	char* name=NULL;

	char* img=NULL;

	lockdownd_get_device_name(client, &name);

	lockdownd_get_value(client, NULL, "ProductType", &node);
	plist_get_string_val(node, &type);
	plist_free(node);
	node=NULL;

	lockdownd_get_value(client, NULL, "ProductVersion", &node);
	plist_get_string_val(node, &version);
	plist_free(node);
	node=NULL;

	plist_from_xml(img_plist, strlen(img_plist), &node);
	plist_t devNode=plist_dict_get_item(node, type);
	if (!devNode)
	{
		printf("ERROR: Unknown device!\n");
	}
	plist_get_string_val(devNode, &img);
	plist_free(node);
	node=NULL;
	strcpy(data, "");

	gtk_image_set_from_file(devImg, img);

	//gtk_label_set_use_underline(dName, TRUE);

	char devL[512]="";

	snprintf(devL, 512, "<b>%s</b>", name);

	gtk_label_set_text(dName, name);
	gtk_label_set_markup(dName, devL);
	gtk_label_set_text(fV, version);

	return 0;
}

int read_in_plists()
{
	//r_file("/usr/local/share/iDeviceActivator/data/latest.plist");
	strcpy(latest_plist, data);

	r_file("/usr/local/share/iDeviceActivator/data/img.plist");
	strcpy(img_plist, data);

	//r_file("/usr/local/share/iDeviceActivator/data/current.plist");
	strcpy(current_plist, data);

	return 0;
}
