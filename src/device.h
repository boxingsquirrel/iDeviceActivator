#ifndef device_h
#define device_h
extern idevice_t device;
extern lockdownd_client_t client;
extern uint16_t port;
#endif

extern int set_up();
extern int setUpDevice();
extern int start_lockdownd();
extern int start_service(const char *service);
extern int recovery_enter();
extern int finish();
