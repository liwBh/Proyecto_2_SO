//
// Created by liwbh on 19/05/23.
//

#ifndef PROYECTO_2_SO_VISTARENDIMIENTO_H
#define PROYECTO_2_SO_VISTARENDIMIENTO_H
#include <gtk/gtk.h>
#include <stdio.h>
#include "../Archivos/Archivo.h"

//variables interfaz
GtkWidget *button;
GtkWidget *button2;
GtkWidget *button3;
GtkWidget *button4;
GtkWidget *button5;
GtkWidget *button6;

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

    printf("\nEntrooooo!  %s\n", newFilePath);

    //armando la ruta absoluta
    char *rutaAbsoluta = malloc(strlen(newFilePath) + strlen("/Archivos/") + strlen(ruta) + 1);
    strcpy(rutaAbsoluta, newFilePath);
    strcat(rutaAbsoluta, "/Archivos/");
    strcat(rutaAbsoluta, ruta);

    printf("\nEntrooooo!  %s\n", rutaAbsoluta);

    return rutaAbsoluta;
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

void crearVentana(GtkWidget *widget, gpointer data, char *ruta) {
    // Crear la ventana principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Archivo de texto");
    gtk_window_maximize(GTK_WINDOW(window));  // Maximizar la ventana
    gtk_window_set_decorated(GTK_WINDOW(window), TRUE);  // Mostrar los botones de la barra de título

    // Crear un overlay para contener los widgets
    GtkWidget *overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(window), overlay);

    // Crear el widget de texto
    GtkWidget *textview = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(overlay), textview);

    // Obtener el buffer de texto
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

    // Leer el archivo de texto y mostrarlo en el widget
    leerArchivo1(buffer, ruta);
    //leerArchivoVista(buffer, ruta);

//    // Mostrar la ventana y sus contenidos
    gtk_widget_show_all(window);
}
// Función para manejar el evento de ingreso al botón
gboolean on_button_enter(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    GdkWindow *window = gtk_widget_get_window(widget);
    GdkCursor *hand_cursor = gdk_cursor_new(GDK_HAND1);
    gdk_window_set_cursor(window, hand_cursor);
    g_object_unref(hand_cursor);
    return TRUE;
}

// Función para manejar el evento de salida del botón
gboolean on_button_leave(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    GdkWindow *window = gtk_widget_get_window(widget);
    GdkCursor *default_cursor = gdk_cursor_new(GDK_LEFT_PTR);
    gdk_window_set_cursor(window, default_cursor);
    g_object_unref(default_cursor);
    return TRUE;
}
void establecerEstiloLabel(GtkWidget *label) {
    const gchar *css_label = "label { color: white; font-size: 30px; }";
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

//Menu de opciones, captura el click y dependiendo del boton se abre una ventana con el txt
void on_button_clicked(GtkWidget *widget, gpointer data){

    //obtenerRutaRelativa("prueba.txt");

    if (widget == button) {
        crearVentana(widget, data, obtenerRutaRelativa("prueba.txt"));
    } else if (widget == button2) {
        crearVentana(widget, data, obtenerRutaRelativa("prueba2.txt"));
    } else if (widget == button3) {
        crearVentana(widget, data, obtenerRutaRelativa("prueba3.txt"));
    } else if (widget == button4) {
        crearVentana(widget, data, obtenerRutaRelativa("prueba4.txt"));
    } else if (widget == button5) {
        crearVentana(widget, data, obtenerRutaRelativa("prueba5.txt"));
    } else if (widget == button6) {
        crearVentana(widget, data, obtenerRutaRelativa("prueba6.txt"));
    }
}

void mostrarVentana() {
    // Inicializar GTK
    gtk_init(NULL, NULL);

    // Crear la ventana principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);//con esto se crea la ventana
    gtk_window_set_title(GTK_WINDOW(window), "Como Arroz!!!!!");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Centrar la ventana en la pantalla
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Crear un grid para organizar los widgets
    GtkWidget *grid = gtk_grid_new();//esto es como un contenedor que organiza la matriz bidimensional
    gtk_container_add(GTK_CONTAINER(window), grid);//se le asigna el widget

    // Establecer el estilo CSS para la imagen de fondo
    const char* css = "window {"
                      "  background-image: url('/home/elmer/Documentos/GitHub/Proyecto_2_SO/prueba2.jpeg');"
                      "  background-repeat: no-repeat;"
                      "  background-size: cover;"
                      "}";
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
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

//    // Agregar un widget "espaciador" en la fila intermedia
//    GtkWidget *spacer = gtk_label_new(NULL);
//    gtk_widget_set_hexpand(spacer, TRUE);
//    gtk_widget_set_vexpand(spacer, TRUE);
//    gtk_grid_attach(GTK_GRID(grid), spacer, 0, 1, 1, 1);

    // Crear un botón para abrir la ventana
    button = gtk_button_new_with_label("Mapa de bits");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    establecerEstiloBoton(button);

    // Centrar el botón dentro del grid
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);

    // Agregar el botón al grid en la última fila
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);

    // Establecer el margen superior para el botón
    gtk_widget_set_margin_top(button, 20);
    // Conectar los eventos enter-notify-event y leave-notify-event
    g_signal_connect(button, "enter-notify-event", G_CALLBACK(on_button_enter), NULL);
    g_signal_connect(button, "leave-notify-event", G_CALLBACK(on_button_leave), NULL);



    // Crear botones adicionales
    button2 = gtk_button_new_with_label("Particiones fijas");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button_clicked), NULL);
    establecerEstiloBoton(button2);
    gtk_widget_set_halign(button2, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(button2, 20);
    gtk_grid_attach(GTK_GRID(grid), button2, 0, 3, 1, 1);
    //esto captura cuando esta el cursor encima y cuando no o "entra y sale"
    g_signal_connect(button2, "enter-notify-event", G_CALLBACK(on_button_enter), NULL);
    g_signal_connect(button2, "leave-notify-event", G_CALLBACK(on_button_leave), NULL);

    button3 = gtk_button_new_with_label("Primer ajuste");
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button_clicked), NULL);
    establecerEstiloBoton(button3);
    gtk_widget_set_halign(button3, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(button3, 20);
    gtk_grid_attach(GTK_GRID(grid), button3, 0, 4, 1, 1);
    g_signal_connect(button3, "enter-notify-event", G_CALLBACK(on_button_enter), NULL);
    g_signal_connect(button3, "leave-notify-event", G_CALLBACK(on_button_leave), NULL);

    button4 = gtk_button_new_with_label("Peor ajuste");
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button_clicked), NULL);
    establecerEstiloBoton(button4);
    gtk_widget_set_halign(button4, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(button4, 20);
    gtk_grid_attach(GTK_GRID(grid), button4, 0, 5, 1, 1);
    g_signal_connect(button4, "enter-notify-event", G_CALLBACK(on_button_enter), NULL);
    g_signal_connect(button4, "leave-notify-event", G_CALLBACK(on_button_leave), NULL);

    button5 = gtk_button_new_with_label("Mejor ajuste");
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button_clicked), NULL);
    establecerEstiloBoton(button5);
    gtk_widget_set_halign(button5, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(button5, 20);
    gtk_grid_attach(GTK_GRID(grid), button5, 0, 6, 1, 1);
    g_signal_connect(button5, "enter-notify-event", G_CALLBACK(on_button_enter), NULL);
    g_signal_connect(button5, "leave-notify-event", G_CALLBACK(on_button_leave), NULL);

    button6 = gtk_button_new_with_label("Ajuste mas rapido");
    g_signal_connect(button6, "clicked", G_CALLBACK(on_button_clicked), NULL);
    establecerEstiloBoton(button6);
    gtk_widget_set_halign(button6, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(button6, 20);
    gtk_grid_attach(GTK_GRID(grid), button6, 0, 7, 1, 1);
    g_signal_connect(button6, "enter-notify-event", G_CALLBACK(on_button_enter), NULL);
    g_signal_connect(button6, "leave-notify-event", G_CALLBACK(on_button_leave), NULL);

    // Mostrar la ventana principal
    gtk_widget_show_all(window);

    // Iniciar el bucle principal de GTK
    gtk_main();
}



#endif //PROYECTO_2_SO_VISTARENDIMIENTO_H
