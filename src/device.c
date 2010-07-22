#include <stdio.h>
#include <libimobiledevice/afc.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/notification_proxy.h>
#include "device.h"

idevice_t device;
lockdownd_client_t client;
uint16_t port;

int set_up()
{
	int se=setUpDevice();
	int le=start_lockdownd();

	printf(se);
	printf(le);
}

int setUpDevice()
{
	char uuid[41];
	int count = 0;
	char **list = NULL;
	idevice_error_t device_error = 0;

	printf("INFO: Retriving device list\n");
	//idevice_set_debug_level(1);
	if (idevice_get_device_list(&list, &count) < 0) {
		fprintf(stderr, "ERROR: Cannot retrieve device list\n");
		return -1;
	}

	//memset(uuid, '\0', 41);
	//memcpy(uuid, list[0], 40);

	printf("INFO: Opening device\n");
	device_error = idevice_new(&device, list[0]);
	if(device_error != IDEVICE_E_SUCCESS) {
		if(device_error == IDEVICE_E_NO_DEVICE) {
			fprintf(stderr, "ERROR: No device found\n");
			return -1;
		} else {
			fprintf(stderr, "ERROR: Unable to open device, %d\n", device_error);
			return -1;
		}
		return -1;
	}

	idevice_device_list_free(list);

	return 0;

	//start_sync();
}

int start_lockdownd()
{
	lockdownd_error_t le=lockdownd_client_new_with_handshake(device, &client, "iDeviceUpdater");

	if (le==LOCKDOWN_E_SUCCESS)
	{
		printf("INFO: Started lockdownd\n");
		return 0;
	}

	else {
		printf("ERROR: Could not start lockdownd\n");
		return -1;
	}
}

int start_service(const char *service)
{
	start_lockdownd();

	// com.apple.afc
	lockdownd_error_t e=lockdownd_start_service(client, service, &port);

	if (e==LOCKDOWN_E_SUCCESS)
	{
		printf("INFO: Started %s\n", service);

		return 0;
	}
	else {
		printf("ERROR: Unable to start %s\n", service);

		return -1;
	}
}

int recovery_enter()
{
	printf("INFO: Entering recovery mode\n");
	lockdownd_enter_recovery(client);
	return 0;
}
