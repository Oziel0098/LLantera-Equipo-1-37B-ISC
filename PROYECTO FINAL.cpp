#include <graphics.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <ctype.h>

#define CSTR(x) ((char*)(x))

// Paleta de Colores
#define COLOR_FONDO         DARKGRAY
#define COLOR_TEXTO_TITULO  LIGHTCYAN
#define COLOR_TEXTO_NORMAL  WHITE
#define COLOR_TEXTO_ENC     LIGHTBLUE
#define COLOR_BOTON_FONDO   BLUE
#define COLOR_BOTON_BORDE   WHITE
#define COLOR_ADVERTENCIA   LIGHTRED
#define COLOR_EXITO        LIGHTGREEN

#define MAX_LINEA_LONG 200
#define MAX_LINEAS_PAGINA 15

void limpiarPantalla() {
    setfillstyle(SOLID_FILL, COLOR_FONDO);
    bar(0, 0, getmaxx(), getmaxy());
}

void mostrarTitulo(const char* titulo) {
    setcolor(COLOR_TEXTO_TITULO);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(50, 30, (char*)titulo);
    line(50, 70, getmaxx() - 50, 70);
}

void dibujarBoton(int x, int y, int ancho, int alto, const char* texto) {
    setfillstyle(SOLID_FILL, COLOR_BOTON_FONDO);
    bar(x, y, x + ancho, y + alto);
    setcolor(COLOR_BOTON_BORDE);
    rectangle(x, y, x + ancho, y + alto);
    setcolor(COLOR_BOTON_BORDE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    int texto_ancho = textwidth((char*)texto);
    int texto_alto = textheight((char*)texto);
    outtextxy(x + (ancho - texto_ancho) / 2, y + (alto - texto_alto) / 2, (char*)texto);
}

void mostrarListaArchivo(const char* titulo, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        limpiarPantalla();
        mostrarTitulo(titulo);
        setcolor(COLOR_ADVERTENCIA);
        outtextxy(50, 150, (char*)"ERROR: No se pudo abrir el archivo");
        outtextxy(50, 200, (char*)"Presione ESC para regresar...");
        while (true) {
            if (kbhit() && getch() == 27) break;
            delay(50);
        }
        return;
    }

    char lineas[100][MAX_LINEA_LONG];
    int total_lineas = 0;
    while (fgets(lineas[total_lineas], MAX_LINEA_LONG, archivo) && total_lineas < 100) {
        total_lineas++;
    }
    fclose(archivo);

    int pagina_actual = 0;
    int total_paginas = (total_lineas + MAX_LINEAS_PAGINA - 1) / MAX_LINEAS_PAGINA;
    bool debe_redibujar = true;

    while (true) {
        if (debe_redibujar) {
            limpiarPantalla();
            mostrarTitulo(titulo);

            int y = 130;
            int max_lineas_en_pagina = MAX_LINEAS_PAGINA;
            if (pagina_actual == total_paginas - 1) {
                max_lineas_en_pagina = total_lineas - pagina_actual * MAX_LINEAS_PAGINA;
            }

            setcolor(COLOR_TEXTO_NORMAL);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

            for (int i = 0; i < max_lineas_en_pagina; i++) {
                outtextxy(50, y + i * 20, lineas[pagina_actual * MAX_LINEAS_PAGINA + i]);
            }

            char indicador[50];
            sprintf(indicador, "Pagina %d de %d", pagina_actual + 1, total_paginas);
            outtextxy(getmaxx() - 150, getmaxy() - 50, indicador);

            outtextxy(50, getmaxy() - 70, (char*)"Flechas para navegar, ESC para salir.");
            debe_redibujar = false;
        }

        if (kbhit()) {
            int tecla = getch();
            if (tecla == 27) break;
            else if (tecla == 75 && pagina_actual > 0) {
                pagina_actual--;
                debe_redibujar = true;
            } else if (tecla == 77 && pagina_actual < total_paginas - 1) {
                pagina_actual++;
                debe_redibujar = true;
            }
        }
        delay(20);
    }
}

void editarArchivoPrecios() {
    FILE* archivo = fopen("precios.txt", "r+");
    if (!archivo) {
        limpiarPantalla();
        mostrarTitulo("Gestionar Precios");
        setcolor(COLOR_ADVERTENCIA);
        outtextxy(50, 150, (char*)"ERROR: No se pudo abrir precios.txt");
        outtextxy(50, 200, (char*)"Presione ESC para regresar...");
        while (true) {
            if (kbhit() && getch() == 27) break;
            delay(50);
        }
        return;
    }

    char lineas[100][MAX_LINEA_LONG];
    int total_lineas = 0;
    while (fgets(lineas[total_lineas], MAX_LINEA_LONG, archivo) && total_lineas < 100) {
        total_lineas++;
    }

    int pagina_actual = 0;
    int total_paginas = (total_lineas + MAX_LINEAS_PAGINA - 1) / MAX_LINEAS_PAGINA;
    bool debe_redibujar = true;

    while (true) {
        if (debe_redibujar) {
            limpiarPantalla();
            mostrarTitulo("Gestionar Precios");

            int y = 130;
            int max_lineas_en_pagina = MAX_LINEAS_PAGINA;
            if (pagina_actual == total_paginas - 1) {
                max_lineas_en_pagina = total_lineas - pagina_actual * MAX_LINEAS_PAGINA;
            }

            setcolor(COLOR_TEXTO_NORMAL);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

            for (int i = 0; i < max_lineas_en_pagina; i++) {
                outtextxy(50, y + i * 20, lineas[pagina_actual * MAX_LINEAS_PAGINA + i]);
            }

            char indicador[50];
            sprintf(indicador, "Pagina %d de %d", pagina_actual + 1, total_paginas);
            outtextxy(getmaxx() - 150, getmaxy() - 50, indicador);

            outtextxy(50, getmaxy() - 70, (char*)"Flechas para navegar, ESC para salir, 1-9 para editar.");
            debe_redibujar = false;
        }

        if (kbhit()) {
            int tecla = getch();
            if (tecla == 27) break;
            else if (tecla == 75 && pagina_actual > 0) {
                pagina_actual--;
                debe_redibujar = true;
            } else if (tecla == 77 && pagina_actual < total_paginas - 1) {
                pagina_actual++;
                debe_redibujar = true;
            } else if (tecla >= '1' && tecla <= '9') {
                int num_linea = (tecla - '1') + pagina_actual * MAX_LINEAS_PAGINA;
                if (num_linea < total_lineas) {
                    limpiarPantalla();
                    mostrarTitulo("Editar Precio");

                    char buffer[200];
                    strcpy(buffer, lineas[num_linea]);

                    setcolor(COLOR_TEXTO_NORMAL);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    outtextxy(50, 150, (char*)"Edite la linea y presione ENTER:");
                    outtextxy(50, 200, buffer);

                    int idx = strlen(buffer);
                    if (idx > 0 && (buffer[idx - 1] == '\n' || buffer[idx - 1] == '\r')) {
                        buffer[--idx] = '\0';
                    }

                    while (true) {
                        if (kbhit()) {
                            int c = getch();
                            if (c == 13) break; // Enter
                            else if (c == 8 && idx > 0) {
                                idx--;
                                buffer[idx] = '\0';
                                setfillstyle(SOLID_FILL, COLOR_FONDO);
                                bar(50, 200, 600, 220);
                                outtextxy(50, 200, buffer);
                            } else if (c >= 32 && c <= 126 && idx < 199) {
                                buffer[idx++] = (char)c;
                                buffer[idx] = '\0';
                                outtextxy(50, 200, buffer);
                            }
                        }
                        delay(20);
                    }

                    buffer[idx] = '\n';
                    buffer[idx + 1] = '\0';

                    strcpy(lineas[num_linea], buffer);

                    freopen("precios.txt", "w", archivo);
                    for (int i = 0; i < total_lineas; i++) {
                        fputs(lineas[i], archivo);
                    }
                    fflush(archivo);
                    debe_redibujar = true;
                }
            }
        }
        delay(20);
    }
    fclose(archivo);
}

void editarArchivoEmpleados() {
    FILE* archivo = fopen("empleados.txt", "r+");
    if (!archivo) {
        limpiarPantalla();
        mostrarTitulo("Control de Usuarios");
        setcolor(COLOR_ADVERTENCIA);
        outtextxy(50, 150, (char*)"ERROR: No se pudo abrir empleados.txt");
        outtextxy(50, 200, (char*)"Presione ESC para regresar...");
        while(true) {
            if(kbhit() && getch() == 27) break;
            delay(50);
        }
        return;
    }

    char empleados[100][MAX_LINEA_LONG];
    int total_empleados = 0;
    while(fgets(empleados[total_empleados], MAX_LINEA_LONG, archivo) && total_empleados < 100) {
        total_empleados++;
    }
    fclose(archivo);

    bool debe_redibujar = true;

    while(true) {
        if (debe_redibujar) {
            limpiarPantalla();
            mostrarTitulo("Control de Usuarios");

            setcolor(COLOR_TEXTO_NORMAL);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

            int y = 130;
            for(int i = 0; i < total_empleados && i < 15; i++) {
                char lineaMostrar[MAX_LINEA_LONG];
                snprintf(lineaMostrar, sizeof(lineaMostrar), "%2d. %s", i + 1, empleados[i]);
                outtextxy(50, y + i*20, lineaMostrar);
            }

            outtextxy(50, getmaxy() - 90, (char*)"A - Alta nuevo empleado");
            outtextxy(50, getmaxy() - 70, (char*)"B - Baja empleado");
            outtextxy(50, getmaxy() - 50, (char*)"ESC - Salir");
            debe_redibujar = false;
        }

        if(kbhit()) {
            int tecla = getch();
            if(tecla == 27) break; // ESC salir
            else if(tecla == 'A' || tecla == 'a') {
                limpiarPantalla();
                mostrarTitulo("Alta de Empleado");
                outtextxy(50, 150, (char*)"Ingrese nombre y estado (ALTA/BAJA), ej: Juan Perez, ALTA");
                outtextxy(50, 180, (char*)"Max 100 caracteres. Enter para guardar.");

                char buffer[150] = {0};
                int idx = 0;
                while(true) {
                    if(kbhit()) {
                        int c = getch();
                        if(c == 13) break;
                        else if(c == 8 && idx > 0) {
                            idx--;
                            buffer[idx] = '\0';
                            setfillstyle(SOLID_FILL, COLOR_FONDO);
                            bar(50, 180, 700, 200);
                            outtextxy(50, 180, buffer);
                        } else if(c >= 32 && c <= 126 && idx < 149) {
                            buffer[idx++] = (char)c;
                            buffer[idx] = '\0';
                            outtextxy(50, 180, buffer);
                        }
                    }
                    delay(50);
                }

                if (idx > 0) {
                    if (total_empleados < 100) {
                        strcat(buffer, "\n");
                        strcpy(empleados[total_empleados++], buffer);

                        FILE* f = fopen("empleados.txt", "w");
                        if (f) {
                            for (int i = 0; i < total_empleados; i++) {
                                fputs(empleados[i], f);
                            }
                            fclose(f);

                            setcolor(COLOR_EXITO);
                            outtextxy(50, 220, (char*)"Empleado agregado correctamente.");
                            Sleep(1500);
                        }
                    } else {
                        setcolor(COLOR_ADVERTENCIA);
                        outtextxy(50, 220, (char*)"No se pueden agregar mas empleados.");
                        Sleep(1500);
                    }
                }
            } else if(tecla == 'B' || tecla == 'b') {
                limpiarPantalla();
                mostrarTitulo("Baja de Empleado");
                outtextxy(50, 150, (char*)"Ingrese el numero de empleado a dar de baja:");

                char buffer[10] = {0};
                int idx = 0;
                while(true) {
                    if(kbhit()) {
                        int c = getch();
                        if(c == 13) break;
                        else if(c == 8 && idx > 0) {
                            idx--;
                            buffer[idx] = '\0';
                            setfillstyle(SOLID_FILL, COLOR_FONDO);
                            bar(50, 180, 200, 200);
                            outtextxy(50, 180, buffer);
                        } else if(c >= '0' && c <= '9' && idx < 9) {
                            buffer[idx++] = (char)c;
                            buffer[idx] = '\0';
                            outtextxy(50, 180, buffer);
                        }
                    }
                    delay(50);
                }

                int num = atoi(buffer);
                if(num > 0 && num <= total_empleados) {
                    char* p = strstr(empleados[num - 1], "ALTA");
                    if (p) {
                        memcpy(p, "BAJA", 4);
                    } else {
                        p = strstr(empleados[num - 1], "BAJA");
                        if(p) {
                            memcpy(p, "ALTA", 4);
                        }
                    }

                    FILE* f = fopen("empleados.txt", "w");
                    if(f) {
                        for(int i=0; i<total_empleados; i++) {
                            fputs(empleados[i], f);
                        }
                        fclose(f);
                        setcolor(COLOR_EXITO);
                        outtextxy(50, 220, (char*)"Estado de empleado cambiado.");
                        Sleep(1500);
                    }
                } else {
                    setcolor(COLOR_ADVERTENCIA);
                    outtextxy(50, 220, (char*)"Numero invalido.");
                    Sleep(1500);
                }
            }
            debe_redibujar = true;
        }
        delay(50);
    }
}

void dibujarInterfazVendedor();
void dibujarInterfazAlmacenista();
void dibujarInterfazGerente();
void dibujarInterfazContador();

void dibujarAutenticacion(const char* rol) {
    char password[30] = {0};
    const char* clave = "PEPEPECAS";  // Única contraseña válida

    bool debe_redibujar = true;

    while (true) {
        if (debe_redibujar) {
            limpiarPantalla();
            mostrarTitulo("Autenticacion");

            setcolor(COLOR_TEXTO_NORMAL);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            outtextxy(50, 150, (char*)"Ingrese la contraseña para el rol:");
            outtextxy(50, 180, (char*)rol);
            outtextxy(50, 210, (char*)"Contraseña: ");

            char mascara[30] = {0};
            for (int i = 0; i < (int)strlen(password); i++) mascara[i] = '*';
            mascara[strlen(password)] = '\0';
            outtextxy(150, 210, mascara);
            debe_redibujar = false;
        }

        if (kbhit()) {
            int c = getch();
            if (c == 13) {
                if (strcmp(password, clave) == 0) {
                    if (strcmp(rol, "Vendedor") == 0) {
                        dibujarInterfazVendedor();
                    } else if (strcmp(rol, "Almacenista") == 0) {
                        dibujarInterfazAlmacenista();
                    } else if (strcmp(rol, "Gerente") == 0) {
                        dibujarInterfazGerente();
                    } else if (strcmp(rol, "Contador") == 0) {
                        dibujarInterfazContador();
                    }
                    break;
                } else {
                    limpiarPantalla();
                    mostrarTitulo("Autenticacion");
                    setcolor(COLOR_ADVERTENCIA);
                    outtextxy(50, 150, (char*)"Contraseña incorrecta.");
                    outtextxy(50, 180, (char*)"Presione una tecla para intentar de nuevo...");
                    getch();
                    memset(password, 0, sizeof(password));
                    debe_redibujar = true;
                }
            } else if (c == 8 && strlen(password) > 0) {
                password[strlen(password) - 1] = '\0';
                debe_redibujar = true;
            } else if (c >= 32 && c <= 126 && strlen(password) < 29) {
                int len = strlen(password);
                password[len] = (char)c;
                password[len + 1] = '\0';
                debe_redibujar = true;
            } else if (c == 27) { // ESC cancelar
                break;
            }
        }
        delay(20);
    }
}









void dibujarInterfazVendedor() {
    bool debe_redibujar = true;

    while(true) {
        if(debe_redibujar) {
            limpiarPantalla();
            mostrarTitulo("Interfaz Vendedor");
            dibujarBoton(100, 150, 400, 40, "1. Lista Completa");
            dibujarBoton(100, 200, 400, 40, "2. Por Tipo de Vehiculo");
            dibujarBoton(100, 250, 400, 40, "3. Por Tipo de Uso");
            dibujarBoton(100, 300, 400, 40, "4. Llantas Agotadas");
            dibujarBoton(100, 350, 400, 40, "0. Salir");
            outtextxy(50, getmaxy() - 70, (char*)"Seleccione una opcion:");
            debe_redibujar = false;
        }

        if(kbhit()) {
            int tecla = getch();
            if(tecla == '0') break;
            switch(tecla) {
                case '1': mostrarListaArchivo("Lista Completa", "inventario.txt"); break;
                case '2': mostrarListaArchivo("Por Tipo de Vehiculo", "llantas_por_tipo.txt"); break;
                case '3': mostrarListaArchivo("Por Tipo de Uso", "llantas_por_uso.txt"); break;
                case '4': mostrarListaArchivo("Llantas Agotadas", "llantas_agotadas.txt"); break;
            }
            debe_redibujar = true;
        }
        delay(20);
    }
}
void dibujarInterfazAlmacenista() {
    bool debe_redibujar = true;

    while(true) {
        if(debe_redibujar) {
            limpiarPantalla();
            mostrarTitulo("Interfaz Almacenista");
            dibujarBoton(100, 150, 400, 40, "1. Ver Inventario");
            dibujarBoton(100, 200, 400, 40, "2. Agregar Producto");
            dibujarBoton(100, 250, 400, 40, "3. Eliminar Producto");
            dibujarBoton(100, 300, 400, 40, "0. Salir");
            outtextxy(50, getmaxy() - 70, (char*)"Seleccione una opcion:");
            debe_redibujar = false;
        }

        if(kbhit()) {
            int tecla = getch();
            if(tecla == '0') break;

            switch(tecla) {
                case '1': {
                    // Mostrar inventario, por ejemplo leer archivo "inventario.txt"
                    mostrarListaArchivo("Inventario", "inventario.txt");
                    debe_redibujar = true;
                    break;
                }
                case '2': {
                    // Agregar producto - Ejemplo básico con input en pantalla
                    limpiarPantalla();
                    mostrarTitulo("Agregar Producto");
                    outtextxy(50, 150, (char*)"Ingrese nombre del producto:");
                    char buffer[150] = {0};
                    int idx = 0;
                    while(true) {
                        if(kbhit()) {
                            int c = getch();
                            if(c == 13) break;
                            else if(c == 8 && idx > 0) {
                                idx--;
                                buffer[idx] = '\0';
                                setfillstyle(SOLID_FILL, COLOR_FONDO);
                                bar(50, 180, 700, 200);
                                outtextxy(50, 180, buffer);
                            } else if(c >= 32 && c <= 126 && idx < 149) {
                                buffer[idx++] = (char)c;
                                buffer[idx] = '\0';
                                outtextxy(50, 180, buffer);
                            }
                        }
                        delay(50);
                    }
                    if(idx > 0) {
                        FILE* f = fopen("inventario.txt", "a");
                        if(f) {
                            strcat(buffer, "\n");
                            fputs(buffer, f);
                            fclose(f);
                            setcolor(COLOR_EXITO);
                            outtextxy(50, 220, (char*)"Producto agregado correctamente.");
                            Sleep(1500);
                        } else {
                            setcolor(COLOR_ADVERTENCIA);
                            outtextxy(50, 220, (char*)"No se pudo abrir inventario.txt");
                            Sleep(1500);
                        }
                    }
                    debe_redibujar = true;
                    break;
                }
                case '3': {
                    // Eliminar producto: mostrar lista y seleccionar para eliminar
                    FILE* archivo = fopen("inventario.txt", "r");
                    if(!archivo) {
                        limpiarPantalla();
                        mostrarTitulo("Eliminar Producto");
                        setcolor(COLOR_ADVERTENCIA);
                        outtextxy(50, 150, (char*)"No se pudo abrir inventario.txt");
                        outtextxy(50, 200, (char*)"Presione ESC para regresar...");
                        while(true) {
                            if(kbhit() && getch() == 27) break;
                            delay(50);
                        }
                        debe_redibujar = true;
                        break;
                    }
                    char lineas[100][MAX_LINEA_LONG];
                    int total_lineas = 0;
                    while(fgets(lineas[total_lineas], MAX_LINEA_LONG, archivo) && total_lineas < 100) total_lineas++;
                    fclose(archivo);

                    limpiarPantalla();
                    mostrarTitulo("Eliminar Producto");
                    setcolor(COLOR_TEXTO_NORMAL);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

                    int y = 130;
                    for(int i=0; i<total_lineas && i<15; i++) {
                        char lineaMostrar[MAX_LINEA_LONG];
                        snprintf(lineaMostrar, sizeof(lineaMostrar), "%2d. %s", i+1, lineas[i]);
                        outtextxy(50, y + i*20, lineaMostrar);
                    }
                    outtextxy(50, getmaxy()-70, (char*)"Ingrese numero para eliminar o ESC para salir:");

                    char buffer[10] = {0};
                    int idx = 0;
                    while(true) {
                        if(kbhit()) {
                            int c = getch();
                            if(c == 27) break;
                            else if(c == 13) break;
                            else if(c == 8 && idx > 0) {
                                idx--;
                                buffer[idx] = '\0';
                                setfillstyle(SOLID_FILL, COLOR_FONDO);
                                bar(50, getmaxy()-50, 150, getmaxy()-30);
                                outtextxy(50, getmaxy()-50, buffer);
                            } else if(c >= '0' && c <= '9' && idx < 9) {
                                buffer[idx++] = (char)c;
                                buffer[idx] = '\0';
                                setfillstyle(SOLID_FILL, COLOR_FONDO);
                                bar(50, getmaxy()-50, 150, getmaxy()-30);
                                outtextxy(50, getmaxy()-50, buffer);
                            }
                        }
                        delay(50);
                    }

                    if(strlen(buffer) > 0) {
                        int num = atoi(buffer);
                        if(num > 0 && num <= total_lineas) {
                            // Eliminar línea seleccionada
                            for(int i = num - 1; i < total_lineas - 1; i++) {
                                strcpy(lineas[i], lineas[i+1]);
                            }
                            total_lineas--;

                            FILE* f = fopen("inventario.txt", "w");
                            if(f) {
                                for(int i = 0; i < total_lineas; i++) {
                                    fputs(lineas[i], f);
                                }
                                fclose(f);
                                setcolor(COLOR_EXITO);
                                limpiarPantalla();
                                mostrarTitulo("Eliminar Producto");
                                outtextxy(50, 150, (char*)"Producto eliminado correctamente.");
                                Sleep(1500);
                            }
                        } else {
                            setcolor(COLOR_ADVERTENCIA);
                            outtextxy(50, getmaxy()-100, (char*)"Numero invalido.");
                            Sleep(1500);
                        }
                    }

                    debe_redibujar = true;
                    break;
                }
            }
        }
        delay(20);
    }
}
void dibujarInterfazGerente() {
    bool debe_redibujar = true;

    while(true) {
        if(debe_redibujar) {
            limpiarPantalla();
            mostrarTitulo("Interfaz Gerente");
            dibujarBoton(100, 150, 400, 40, "1. Ganacias");
            dibujarBoton(100, 200, 400, 40, "2. Gestionar Precios");
            dibujarBoton(100, 250, 400, 40, "3. Control de Usuarios");
            dibujarBoton(100, 300, 400, 40, "0. Salir");
            outtextxy(50, getmaxy() - 70, (char*)"Seleccione una opcion:");
            debe_redibujar = false;
        }

        if(kbhit()) {
            int tecla = getch();
            if(tecla == '0') break;
            switch(tecla) {
                case '1': {
                    // Panel General: Resumen ventas, ganancias, pedidos pendientes
                    mostrarListaArchivo("Ganancias", "ganancias.txt");
                    debe_redibujar = true;
                    break;
                }
                case '2': {
                    // Gestionar Precios
                    editarArchivoPrecios();
                    debe_redibujar = true;
                    break;
                }
                case '3': {
                    // Control Usuarios
                    editarArchivoEmpleados();
                    debe_redibujar = true;
                    break;
                }
            }
        }
        delay(20);
    }
}
void dibujarInterfazContador() {
    bool debe_redibujar = true;

    while(true) {
        if(debe_redibujar) {
            limpiarPantalla();
            mostrarTitulo("Interfaz Contador");
            dibujarBoton(100, 150, 400, 40, "1. Ingresos");
            dibujarBoton(100, 200, 400, 40, "2. Egresos");
            dibujarBoton(100, 250, 400, 40, "3. Ganancias");
            dibujarBoton(100, 300, 400, 40, "0. Salir");
            outtextxy(50, getmaxy() - 70, (char*)"Seleccione una opcion:");
            debe_redibujar = false;
        }

        if(kbhit()) {
            int tecla = getch();
            if(tecla == '0') break;

            switch(tecla) {
                case '1': mostrarListaArchivo("Ingresos", "ingresos.txt"); debe_redibujar = true; break;
                case '2': mostrarListaArchivo("Egresos", "egresos.txt"); debe_redibujar = true; break;
                case '3': mostrarListaArchivo("Ganancias", "ganancias.txt"); debe_redibujar = true; break;
            }
        }
        delay(20);
    }
}
int main() {
    initwindow(1024, 768, "Sistema Llantera - Menu Principal");
    bool debe_redibujar = true;
    while(true) {
        if(debe_redibujar) {
            limpiarPantalla();
            mostrarTitulo("Sistema Llantera - Menu Principal");
            dibujarBoton(100, 150, 300, 50, "1. Vendedor");
            dibujarBoton(100, 230, 300, 50, "2. Almacenista");
            dibujarBoton(100, 310, 300, 50, "3. Gerente");
            dibujarBoton(100, 390, 300, 50, "4. Contador");
            dibujarBoton(100, 470, 300, 50, "0. Salir");
            setcolor(COLOR_TEXTO_NORMAL);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            outtextxy(100, 550, (char*)"Seleccione una opcion y presione ENTER");
            debe_redibujar = false;
        }

        if(kbhit()) {
            int tecla = getch();
            if(tecla == '0') break;
            else if(tecla == '1') {
                dibujarAutenticacion("Vendedor");
                debe_redibujar = true;
            }
            else if(tecla == '2') {
                dibujarAutenticacion("Almacenista");
                debe_redibujar = true;
            }
            else if(tecla == '3') {
                dibujarAutenticacion("Gerente");
                debe_redibujar = true;
            }
            else if(tecla == '4') {
                dibujarAutenticacion("Contador");
                debe_redibujar = true;
            }
        }
        delay(20);
    }
    closegraph();
    return 0;
}

