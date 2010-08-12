/*
 * ideviceactivate.c
 * A simple utility to handle the activation process for iPhones.
 *
 * Copyright (c) 2010 Joshua Hill and boxingsquirrel. All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <plist/plist.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/libimobiledevice.h>

#include "activate.h"
#include "cache.h"
#include "util.h"
#include "idevice.h"

char* cachedir = NULL;
int use_cache=0;
int backup_to_cache=0;

idevice_t device = NULL;
lockdownd_client_t client = NULL;

int activate_thread(int mmle) {
	int opt = 0;
	int debug = 0;
	char* uuid = NULL;
	char* file = NULL;

	char* cust_imei=NULL;
	char* cust_imsi=NULL;
	char* cust_iccid=NULL;
	char* cust_serial_num=NULL;

	int deactivate = 0;

	if (mmle==1)
	{
		char* dev_type=(char*)lockdownd_get_string_value(client, "ProductType");
		char* model_num=(char*)lockdownd_get_string_value(client, "ModelNumber");

		char *cache_path[512];
		snprintf(cache_path, 512, "%s_%s", dev_type, model_num);

		if (cache("SeralNumber", "")!=0)
		{
			mkdir((const char *)cache_path, 0777);
			cachedir = (char*)cache_path;
			backup_to_cache=1;
		}

		else {
			cachedir = (char*)cache_path;
			use_cache=1;
		}
	}

	init_lockdownd(uuid);

	if (use_cache==1)
	{
		if (check_cache(client)!=0)
		{
			error("The selected cache does not match this device :(");
			free_up();
			return -1;
		}
	}

	plist_t activation_record = NULL;

	printf("Creating activation request\n");
	if(activate_fetch_record(client, &activation_record, cust_imei, cust_imsi, cust_iccid, cust_serial_num) < 0) {
		error("Unable to fetch activation request");
		free_up();
		return -1;
	}

	if (do_activation(client, activation_record)!=0)
	{
		free_up();
		return -1;
	}

	free_up();
	return 0;
}

void init_lockdownd(char* uuid)
{
	idevice_error_t device_error = idevice_new(&device, uuid);
	if (device_error != IDEVICE_E_SUCCESS) {
		error("No device found, is it plugged in?");
		exit(-1);
	}

	lockdownd_error_t client_error = lockdownd_client_new_with_handshake(device, &client, "ideviceactivate");
	if (client_error != LOCKDOWN_E_SUCCESS) {
		error("Unable to connect to lockdownd");
		free_up();
		exit(-1);
	}
}

void free_up()
{
	if (device!=NULL)
	{
		idevice_free(device);
	}

	if (client!=NULL)
	{
		lockdownd_client_free(client);
	}
}
