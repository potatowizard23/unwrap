/*  ⠀⠀⢀⣴⠚⠛⠉⠉⠉⠳⢦⡀⠀⠀⠀⠀⠀
  ⠀⠀⠀⣴⣿⠁⠀⠀⠀⠀⠀⠀⠀⢹⡄⠀⠀⠀⠀
  ⠀⠀⣼⣟⣿⠀⠀⠀⠀⠀⠀⠀⠀⠈⣷⠀⠀⠀⠀▗▄▄▖                          ▄   ▄  █                    ▗▖ ▄▄▖  ▄▄▖
  ⠀⢸⡟⠘⠉⠱⣿⠀⠀⠀⠀⢠⣶⡄⣿⠀⠀⠀⠀▐▛▀▜▖      ▐▌        ▐▌       █   █  ▀                    ▐▌▐▀▀█▖▐▀▀█▖
  ⠀⣿⠃⠀⠀⠀⠀⠀⠉⠙⠃⠀⠀⠀⢸⠀⠀⠀⠀▐▌ ▐▌ ▟█▙ ▐███  ▟██▖▐███  ▟█▙ ▜▖█▗▛ ██  ▐███▌ ▟██▖ █▟█▌ ▟█▟▌   ▐▌   ▟▌
  ⢸⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡇⠀⠀⠀▐██▛ ▐▛ ▜▌ ▐▌   ▘▄▟▌ ▐▌  ▐▛ ▜▌▐▌█▐▌  █    ▗▛  ▘▄▟▌ █▘  ▐▛ ▜▌  ▗▛  ▐██
  ⣾⣤⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣷⠀⠀⠀▐▌   ▐▌ ▐▌ ▐▌  ▗█▀▜▌ ▐▌  ▐▌ ▐▌▐█▀█▌  █   ▗▛  ▗█▀▜▌ █   ▐▌ ▐▌ ▗▛     ▜▌
  ⣿⣤⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠀▐▌   ▝█▄█▘ ▐▙▄ ▐▙▄█▌ ▐▙▄ ▝█▄█▘▐█ █▌▗▄█▄▖▗█▄▄▖▐▙▄█▌ █   ▝█▄█▌▗█▄▄▖▐▄▄█▘
  ⢿⣿⠀⠶⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⠇⠀⠀⠀▝▘    ▝▀▘   ▀▀  ▀▀▝▘  ▀▀  ▝▀▘ ▝▀ ▀▘▝▀▀▀▘▝▀▀▀▘ ▀▀▝▘ ▀    ▝▀▝▘▝▀▀▀▘ ▀▀▘
  ⠈⠻⣶⣇⣰⡄⠀⠀⠀⠀⢀⣠⣾⡿⠏⠀⢤⡀⠀
  ⠀⠀⠈⠛⠛⠿⠿⠿⠿⡿⢿⡛⡩⠤⠴⠒⠛⠁⠀
  unwrap/app.c | PotatoWizard23 | GNU GPL-3 (see LICENSE file) */

#include "app.h"
#include <adwaita.h>
#include <stdbool.h>
#include <stdlib.h>
#include "config.h"
#include "gresource.h"

static void activate_cb(AdwApplication* adw, gpointer data);

struct _UnwrapApp {
    AdwApplication* adw;
    GResource* gresource;

    int argc;
    char** argv;
};

UnwrapApp* unwrap_app_init(int argc, char** argv) {
    UnwrapApp* app = malloc(sizeof(UnwrapApp));
    app->argc = argc;
    app->argv = argv;
    app->adw = adw_application_new(APP_ID, G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app->adw, "activate", G_CALLBACK(activate_cb), app);

    return app;
}

static void activate_cb(AdwApplication* adw, gpointer data) {
    (void)adw;
    UnwrapApp* app = (UnwrapApp*)data;

    app->gresource = io_get_resource();
    g_resources_register(app->gresource);

    GtkBuilder* builder = gtk_builder_new();
    GError* err = NULL;

    gtk_builder_add_from_resource(builder, "/io/github/potatowizard23/unwrap/builder.ui",
                                  &err);

    if (err != NULL)
        printf("%s\n", err->message);

    GObject* window = gtk_builder_get_object(builder, "window");

    if (window == NULL)
        printf("AdwWindow with id 'window' not found.\n");

    gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app->adw));
    gtk_widget_set_visible(GTK_WIDGET(window), true);
    gtk_window_present(GTK_WINDOW(window));

    g_object_unref(builder);
}

void unwrap_app_run(UnwrapApp* app) {
    g_application_run(G_APPLICATION(app->adw), app->argc, app->argv);
}

void unwrap_app_destroy(UnwrapApp* app) {
    g_resources_unregister(app->gresource);
    g_object_unref(app->gresource);

    g_object_unref(app->adw);
    free(app);
}
