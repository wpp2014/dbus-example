/*
接收信号
*/

#include <iostream>
#include <dbus/dbus.h>
#include <unistd.h>

using namespace std;

int main()
{
	DBusMessage *msg;
	DBusMessageIter args;
	DBusConnection *conn;
	DBusError err;
	int ret;
	char *str;

	dbus_error_init(&err);

	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if(dbus_error_is_set(&err))
	{
		cout << err.message << endl;
		dbus_error_free(&err);
		return -1;
 	}
 	if(NULL == conn)
 	{
 		return -1;
 	}

 	ret = dbus_bus_request_name(conn, "org.dbus.example", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
 	if(dbus_error_is_set(&err))
 	{
 		cout << err.message << endl;
 		dbus_error_free(&err);
 		return -1;
 	}

 	dbus_bus_add_match(conn, "type='signal', interface='org.dbus.example'", &err);
 	if(dbus_error_is_set(&err))
 	{
 		cout << err.message << endl;
 		dbus_error_free(&err);
 		return -1;
 	}

 	while(true)
 	{
 		dbus_connection_read_write(conn, 0);
 		msg = dbus_connection_pop_message(conn);

 		if(NULL == msg)
 		{
 			usleep(10000);
 			continue;
 		}

 		if(dbus_message_is_signal(msg, "org.dbus.example", "Test"))
 		{
 			if(!dbus_message_iter_init(msg, &args))
 			{
 				cout << "Message Has no parameter" << endl;
 			}
 			else
 			{
 				if(DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
 				{
 					cout << "Argument is not string" << endl;
 				}
 				else
 				{
 					dbus_message_iter_get_basic(&args, &str);
 					cout << str << endl;
 				}
 			}
 		}

 		dbus_message_unref(msg);
 	}
}