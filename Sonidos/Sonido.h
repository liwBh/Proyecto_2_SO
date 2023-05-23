//
// Created by liwbh on 05/04/23.
//

#ifndef PROYECTO_01_SO_SONIDO_H
#define PROYECTO_01_SO_SONIDO_H

#include <stdio.h>
#include <gst/gst.h>


void reproducirSonido(char* ruta){

    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;

    /* Initialize GStreamer */
    gst_init (NULL, NULL);



    char *currentDir = g_get_current_dir();//ruta del proyecto
    char newFilePath[200];//variable para depurar la ruta del proyecto
    char rutaAbsoluta[200];//variable para la ruta absoluta
    memset(newFilePath, '\0', sizeof(newFilePath));
    memset(rutaAbsoluta, '\0', sizeof(rutaAbsoluta));
    char *subStr = "/cmake-build-debug";//cadena a eliminar
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
    strcat(rutaAbsoluta,"playbin uri=file:////");
    strcat(rutaAbsoluta,newFilePath);
    strcat(rutaAbsoluta,ruta);


    pipeline = gst_parse_launch(rutaAbsoluta, NULL);

    /* Start playing */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus = gst_element_get_bus (pipeline);
    msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* Free resources */
    if (msg != NULL)
        gst_message_unref (msg);
        gst_object_unref (bus);
        gst_element_set_state (pipeline, GST_STATE_NULL);
        gst_object_unref (pipeline);
    }



#endif //PROYECTO_01_SO_SONIDO_H
