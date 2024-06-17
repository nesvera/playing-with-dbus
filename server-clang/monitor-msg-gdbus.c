#include <gio/gio.h>
#include <string.h>

static void
on_signal(GDBusProxy *proxy,
          char       *sender_name,
          char       *signal_name,
          GVariant   *parameters,
          gpointer    user_data)
{
    guint32 new_state;

    /* We are only interested in "StateChanged" signal */
    if (strcmp(signal_name, "Clock") == 0) {
        GVariant *tmp = g_variant_get_child_value(parameters, 0);
        new_state     = g_variant_get_int32(tmp);
        g_variant_unref(tmp);
        g_print("Clock: %d\n", new_state);
    }
}

int
main(int argc, char *argv[])
{
    GMainLoop      *loop;
    GError         *error = NULL;
    GDBusProxyFlags flags;
    GDBusProxy     *proxy;

    /* Monitor 'StateChanged' signal on 'org.example.test' interface */
    g_print("Monitor other service state\n");
    g_print("==============================\n");

    flags = G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES | G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START;
    proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION,
                                          flags,
                                          NULL, /* GDBusInterfaceInfo */
                                          "org.example.test",
                                          "/",
                                          "org.example.interface",
                                          NULL, /* GCancellable */
                                          &error);

    if (proxy == NULL) {
        g_dbus_error_strip_remote_error(error);
        g_printerr("Error creating D-Bus proxy: %s\n", error->message);
        g_error_free(error);
        return -1;
    }

    /* Connect to g-signal to receive signals from proxy (remote object) */
    g_signal_connect(proxy, "g-signal", G_CALLBACK(on_signal), NULL);

    /* Run main loop */
    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    g_object_unref(proxy);

    return 0;
}