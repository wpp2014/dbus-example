/*
调用系统总线方法
*/
#include <dbus/dbus.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	DBusMessage *msg;
	DBusMessageIter args;
	DBusConnection *conn;
	DBusError err;
	DBusPendingCall *pending;
	int ret;
	char *str;

	dbus_error_init(&err);

	//连接到系统总线，检查错误
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if(dbus_error_is_set(&err))
	{
		cout << err.message << endl;
		dbus_error_free(&err);
		return -1;
	}
	if(NULL == conn)
	{
		cout << "connect bus fail" << endl;
		return -1;
	}

	ret = dbus_bus_request_name(conn, "org.dbus.example", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
	if(dbus_error_is_set(&err))
	{
		cout << err.message << endl;
		dbus_error_free(&err);
		return -1;
	}
	if(-1 == ret)
	{
		cout << "request name fail" << endl;
		return -1;
	}

	//建立方法，检查错误
	msg = dbus_message_new_method_call("org.dbus.example",
											  "/org/dbus/example",
											  "org.dbus.example",
											  "Method");
	if(NULL == msg)
	{
		cout << "create new method fail" << endl;
		return -1;
	}

	//给方法添加对应的参数
	dbus_message_iter_init_append(msg, &args);
	if(!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &argv[1]))
	{
		cout << "out of memory" << endl;
		return -1;
	}
	//调用方法，接受返回值
	if(!dbus_connection_send_with_reply(conn, msg, &pending, -1))
	{
		cout << "out of memory" << endl;
		return -1;
	}

	if(NULL == pending)
	{
		cout << "pending call null" << endl;
		return -1;
	}

	dbus_connection_flush(conn);

	dbus_message_unref(msg);

	dbus_pending_call_block(pending);

	msg = dbus_pending_call_steal_reply(pending);
	if(NULL == msg)
	{
		cout << "reply null" << endl;
		return -1;
	}
	
	if(!dbus_message_iter_init(msg, &args))
	{
		cout << "message has no arguments" << endl;
	}
	else
	{
		if(DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
		{
			cout << "arguments is not string" << endl;
		}
		else
		{
			dbus_message_iter_get_basic(&args, &str);
			cout << str << endl;
		}
	}

	return 0;
}
