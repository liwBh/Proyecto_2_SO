cmake_minimum_required(VERSION 3.24.2)
project(Proyecto_2_SO C)

set(CMAKE_C_STANDARD 11)

# Incluye los paquetes de GTK y GDK Pixbuf
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GDK_PIXBUF REQUIRED gdk-pixbuf-2.0)

# Agrega las banderas de compilación y los directorios de inclusión necesarios
add_compile_options(${GTK3_CFLAGS_OTHER})
include_directories(${GTK3_INCLUDE_DIRS} ${GDK_PIXBUF_INCLUDE_DIRS})

# Agrega el ejecutable y enlaza las bibliotecas
add_executable(Proyecto_2_SO main.c Listas/Lista.h Listas/Nodo.h Listas/ListaDirecionesM.h Listas/NodoDirecionesM.h MatrizMemoria/Matriz.h Logica/Logica.h Archivos/Archivo.h)
target_link_libraries(Proyecto_2_SO ${GTK3_LIBRARIES} ${GDK_PIXBUF_LIBRARIES})

