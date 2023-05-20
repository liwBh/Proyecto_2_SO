//
// Created by liwbh on 19/05/23.
//

#ifndef PROYECTO_2_SO_VISTARENDIMIENTO_H
#define PROYECTO_2_SO_VISTARENDIMIENTO_H
#include <gtk/gtk.h>
#include <stdio.h>
#include "../Archivos/Archivo.h"
#include "../Sonidos//Sonido.h"
#define MAX_VENTANAS 10
GtkWidget* ventanas_abiertas[MAX_VENTANAS];
int num_ventanas_abiertas = 0;


GtkWidget *botones[7];
;

GtkWidget* ventana_principal = NULL;
char *obtenerRutaRelativa(char* ruta) {
    char *currentDir = g_get_current_dir(); //ruta del proyecto
    char newFilePath[200]; //variable para depurar la ruta del proyecto
    memset(newFilePath, '\0', sizeof(newFilePath));
    char *subStr = "/cmake-build-debug"; //cadena a eliminar
    char *pos = strstr(currentDir, subStr);

    //manejo de errores al depurar la cadena de la ruta
    if (pos != NULL) {
        strncpy(newFilePath, currentDir, pos - currentDir);
        newFilePath[pos - currentDir] = '\0';
        strcat(newFilePath, pos + strlen(subStr));
    } else {
        strcpy(newFilePath, currentDir);
    }

    //armando la ruta absoluta
    char *rutaAbsoluta = malloc(strlen(newFilePath) + strlen("/Archivos/") + strlen(ruta) + 1);
    strcpy(rutaAbsoluta, newFilePath);
    strcat(rutaAbsoluta, "/Archivos/");
    strcat(rutaAbsoluta, ruta);


    return rutaAbsoluta;
}

void cerrarVentanas() {
    for (int i = 0; i < num_ventanas_abiertas; i++) {
        gtk_widget_destroy(ventanas_abiertas[i]);
    }
    num_ventanas_abiertas = 0;
}

// Función para leer el archivo de texto
void leerArchivo1(GtkTextBuffer *buffer, const char *prueba) {
    FILE *archivo;
    char linea[256];

    // Abrir el archivo en modo lectura
    archivo = fopen(prueba, "r");
    if (archivo == NULL) {
        g_print("Error al abrir el archivo.\n");
        return;
    }

    // Leer el archivo línea por línea
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Agregar la línea al buffer de texto de GTK
        gtk_text_buffer_insert_at_cursor(buffer, linea, -1);
    }

    // Cerrar el archivo
    fclose(archivo);
}

GtkWidget* crearVentana(GtkWidget *widget, gpointer data, char *ruta) {
    // Crear la ventana principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    ventana_principal = window;

    gtk_window_set_title(GTK_WINDOW(window), "Archivo de texto");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
    gtk_window_set_decorated(GTK_WINDOW(window), TRUE);  // Mostrar los botones de la barra de título

    // Crear un GtkScrolledWindow
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    // Crear el widget de texto
    GtkWidget *textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);

    // Obtener el buffer de texto
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

    // Leer el archivo de texto y mostrarlo en el widget
    leerArchivo1(buffer, ruta);

    // Agregar el GtkTextView al GtkScrolledWindow
    gtk_container_add(GTK_CONTAINER(scrolled_window), textview);

    // Agregar el GtkScrolledWindow a un GtkViewport
    GtkWidget *viewport = gtk_viewport_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(viewport), scrolled_window);

    // Agregar el GtkViewport a la ventana principal
    gtk_container_add(GTK_CONTAINER(window), viewport);

    // Mostrar la ventana y sus contenidos
    gtk_widget_show_all(window);

    return window;
}




// Función para manejar el evento de ingreso al botón
gboolean on_button_enter(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    GdkWindow *window = gtk_widget_get_window(widget);
    GdkDisplay *display = gdk_window_get_display(window);
    GdkCursor *hand_cursor = gdk_cursor_new_for_display(display, GDK_HAND1);
    gdk_window_set_cursor(window, hand_cursor);
    g_object_unref(hand_cursor);
    return TRUE;
}


// Función para manejar el evento de salida del botón
gboolean on_button_leave(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    GdkWindow *window = gtk_widget_get_window(widget);
    GdkDisplay *display = gdk_window_get_display(window);
    GdkCursor *default_cursor = gdk_cursor_new_for_display(display, GDK_LEFT_PTR);
    gdk_window_set_cursor(window, default_cursor);
    g_object_unref(default_cursor);
    return TRUE;
}

void establecerEstiloLabel(GtkWidget *label) {
    const gchar *css_label = "label { color: white; font-size: 28px; }";
    GtkCssProvider *provider_label = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider_label, css_label, -1, NULL);
    GtkStyleContext *context_label = gtk_widget_get_style_context(label);
    gtk_style_context_add_provider(context_label,
                                   GTK_STYLE_PROVIDER(provider_label),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void establecerEstiloBoton(GtkWidget *button) {
    const gchar *css_button = "button { color: black;   }";
    GtkCssProvider *provider_button = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider_button, css_button, -1, NULL);
    GtkStyleContext *context_button = gtk_widget_get_style_context(button);
    gtk_style_context_add_provider(context_button,
                                   GTK_STYLE_PROVIDER(provider_button),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}
gboolean on_child_window_closed(GtkWidget *widget, GdkEvent *event, gpointer data) {
    // Ocultar la ventana secundaria en lugar de cerrarla
    gtk_widget_hide(widget);
}


void on_button_clicked(GtkWidget *widget, gpointer data) {
    // Cerrar todas las ventanas abiertas excepto la ventana principal
    cerrarVentanas();

    // Declarar la variable nueva_ventana fuera del bloque if
    GtkWidget* nueva_ventana = NULL;

    // Crear la nueva ventana y agregarla a la lista de ventanas abiertas
    if (widget == botones[0]) {
        reproducirSonido("/Sonidos/button1.mp3");
        nueva_ventana = crearVentana(widget, data, obtenerRutaRelativa("prueba.txt"));
        ventanas_abiertas[num_ventanas_abiertas++] = nueva_ventana;
    } else if (widget == botones[1]) {
        reproducirSonido("/Sonidos/button1.mp3");
        nueva_ventana = crearVentana(widget, data, obtenerRutaRelativa("prueba2.txt"));
        ventanas_abiertas[num_ventanas_abiertas++] = nueva_ventana;
    } else if (widget == botones[2]) {
        reproducirSonido("/Sonidos/button1.mp3");
        nueva_ventana = crearVentana(widget, data, obtenerRutaRelativa("prueba3.txt"));
        ventanas_abiertas[num_ventanas_abiertas++] = nueva_ventana;
    } else if (widget == botones[3]) {
        reproducirSonido("/Sonidos/button1.mp3");
        nueva_ventana = crearVentana(widget, data, obtenerRutaRelativa("prueba4.txt"));
        ventanas_abiertas[num_ventanas_abiertas++] = nueva_ventana;
    } else if (widget == botones[4]) {
        reproducirSonido("/Sonidos/button1.mp3");
        nueva_ventana = crearVentana(widget, data, obtenerRutaRelativa("prueba5.txt"));
        ventanas_abiertas[num_ventanas_abiertas++] = nueva_ventana;
    } else if (widget == botones[5]) {
        reproducirSonido("/Sonidos/button1.mp3");
        nueva_ventana = crearVentana(widget, data, obtenerRutaRelativa("prueba6.txt"));
        ventanas_abiertas[num_ventanas_abiertas++] = nueva_ventana;
    } else if (widget == botones[6]) {
        reproducirSonido("/Sonidos/button1.mp3");
        nueva_ventana = crearVentana(widget, data, obtenerRutaRelativa("prueba7.txt"));
        ventanas_abiertas[num_ventanas_abiertas++] = nueva_ventana;
    }


}



void crearBotones(GtkWidget *grid) {
    int n = 7;

    char labels[n][20]; // Arreglo de etiquetas para los botones

    // Llenar las etiquetas de los botones
    snprintf(labels[0], sizeof(labels[0]), "Mapa Bits");
    snprintf(labels[1], sizeof(labels[1]), "Particiones Fijas");
    snprintf(labels[2], sizeof(labels[2]), "Primer Ajuste");
    snprintf(labels[3], sizeof(labels[3]), "Peor Ajuste");
    snprintf(labels[4], sizeof(labels[4]), "Mejor Ajuste");
    snprintf(labels[5], sizeof(labels[5]), "Ajuste Rapido");
    snprintf(labels[6], sizeof(labels[6]), "Socios");

    for (int i = 0; i < n; i++) {
         botones[i] = gtk_button_new_with_label(labels[i]);

        g_signal_connect(botones[i], "clicked", G_CALLBACK(on_button_clicked), NULL);
        establecerEstiloBoton(botones[i]);
        gtk_widget_set_halign(botones[i], GTK_ALIGN_START);
        gtk_widget_set_margin_top(botones[i], 20);
        gtk_widget_set_margin_start(botones[i], 20);
        gtk_grid_attach(GTK_GRID(grid), botones[i], 0, i + 3, 1, 1);
        g_signal_connect(botones[i], "enter-notify-event", G_CALLBACK(on_button_enter), NULL);
        g_signal_connect(botones[i], "leave-notify-event", G_CALLBACK(on_button_leave), NULL);
    }

}

void* mostrarVentana() {
    // Inicializar GTK
    g_setenv("GTK_DEBUG", "interactive", 1);
    gtk_init(NULL, NULL);


    g_object_set(gtk_settings_get_default(), "gtk-shell-shows-menubar", FALSE, NULL);
    // Crear la ventana principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);//con esto se crea la ventana

    gtk_window_set_title(GTK_WINDOW(window), "Rendimiento Memoria");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Centrar la ventana en la pantalla
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Crear un grid para organizar los widgets
    GtkWidget *grid = gtk_grid_new();//esto es como un contenedor que organiza la matriz bidimensional
    gtk_container_add(GTK_CONTAINER(window), grid);//se le asigna el widget

    const char* css = "window {"
                      "  background-image: url('";
    const char* ruta = obtenerRutaRelativa("prueba2.jpeg");
    const char* cssSuffix = "');"
                            "  background-repeat: no-repeat;"
                            "  background-size: cover;"
                            "}";

    char* cssConRuta = malloc(strlen(css) + strlen(ruta) + strlen(cssSuffix) + 1);
    strcpy(cssConRuta, css);
    strcat(cssConRuta, ruta);
    strcat(cssConRuta, cssSuffix);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, cssConRuta, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    // Crear los labels con información
    GtkWidget *label1 = gtk_label_new("Bienvenido al registro del Administrador de Memoria");
    gtk_widget_set_margin_top(label1, 20);
    gtk_widget_set_margin_bottom(label1, 20);
    establecerEstiloLabel(label1);


    // Permitir que los labels se expandan horizontalmente
    gtk_widget_set_hexpand(label1, TRUE);

    // Centrar los labels verticalmente dentro del grid
    gtk_widget_set_valign(label1, GTK_ALIGN_CENTER);

    // Agregar los labels al grid
    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);

    crearBotones(grid);

    // Mostrar la ventana principal
    gtk_widget_show_all(window);

    // Iniciar el bucle principal de GTK
//    return window;
    gtk_main();


}



#endif //PROYECTO_2_SO_VISTARENDIMIENTO_H
