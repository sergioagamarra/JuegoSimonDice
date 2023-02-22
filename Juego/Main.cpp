#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <windows.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <string>
#include <time.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <utility>
#include <algorithm>


using namespace std;

struct Nodo
{
    int color;
    Nodo* siguiente;
};

void inicializarAllegro();
void destroyAllegro();
void insertarLista(Nodo*&, int);
void mostrarLista(Nodo*);
void eliminarLista(Nodo*&);
int jugar();
int menu();
int dificultad();
int records();
int cargarRecord();
int split(string, vector<string>&, char);
void lectura(string, char);
void ordenarPuntajeYEscribir(string, vector<pair<int, string>>, char);
void asignarLetra(int&, int&, int&, int, string&);
int esRecord(int);


Nodo* lista = NULL;
Nodo* aux1 = NULL;
int tamanio_lista = 0;

int ancho = 800;
int alto = 600;
float nivel = 1.0;
int puntos = 0;

vector<pair<int, string>> puntajes;

ALLEGRO_BITMAP* menu_null;
ALLEGRO_BITMAP* menu_jugar;
ALLEGRO_BITMAP* menu_dificultad;
ALLEGRO_BITMAP* menu_puntajes;
ALLEGRO_BITMAP* menu_salir;

ALLEGRO_DISPLAY* ventana;
ALLEGRO_FONT* fuente;
ALLEGRO_FONT* fuente_record;
ALLEGRO_FONT* fuente_nivel;
ALLEGRO_TIMER* segundoTimer;
ALLEGRO_TIMER* fps;
ALLEGRO_EVENT_QUEUE* event_queue;

ALLEGRO_BITMAP* juego;
ALLEGRO_BITMAP* verde;
ALLEGRO_BITMAP* rojo;
ALLEGRO_BITMAP* amarillo;
ALLEGRO_BITMAP* azul;

ALLEGRO_SAMPLE* s_verde;
ALLEGRO_SAMPLE* s_rojo;
ALLEGRO_SAMPLE* s_amarillo;
ALLEGRO_SAMPLE* s_azul;
ALLEGRO_SAMPLE* s_menu;


int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    inicializarAllegro();

   

    int anchoW = GetSystemMetrics(SM_CXSCREEN);
    int altoW = GetSystemMetrics(SM_CYSCREEN);

    al_set_new_display_flags(ALLEGRO_NOFRAME);
    ventana = al_create_display(ancho, alto);

    fuente = al_load_font("ARCO.otf", 40, 0);
    fuente_record = al_load_font("ARCO.otf", 60, 0);
    fuente_nivel = al_load_font("ARCO.otf", 20, 0);

    al_set_window_title(ventana, "Simon Dice");
    al_set_window_position(ventana, anchoW / 2 - ancho / 2, altoW / 2 - alto / 2);

    // Eventos timer
    segundoTimer = al_create_timer(1.0);
    fps = al_create_timer(1.0 / 60);
    event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_timer_event_source(fps));
    al_register_event_source(event_queue, al_get_timer_event_source(segundoTimer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(fps);
    al_start_timer(segundoTimer);

    srand(time(NULL));

    menu();

    destroyAllegro();

    return 0;
}

int jugar()
{
    int segundos = 3;
    int x = -1, y = -1;

    int jugada = true;
    int mostrar = false;
    int perdiste = false;

    juego = al_load_bitmap("imagenes/juego_simon.jpg");
    verde = al_load_bitmap("imagenes/verde.jpg");
    rojo = al_load_bitmap("imagenes/rojo.jpg");
    amarillo = al_load_bitmap("imagenes/amarillo.jpg");
    azul = al_load_bitmap("imagenes/azul.jpg");

    s_verde = al_load_sample("sonidos/simon1.wav");
    s_rojo = al_load_sample("sonidos/simon2.wav");
    s_amarillo = al_load_sample("sonidos/simon3.wav");
    s_azul = al_load_sample("sonidos/simon4.wav");

    int botones[] = { 0 };

    int color_jugador = -1;
    int contador;
    int continua = 0;
    int color;


    al_draw_bitmap(juego, 0, 0, 0);

    while (true)
    {
        ALLEGRO_EVENT Evento;
        al_wait_for_event(event_queue, &Evento);

        if (Evento.type == ALLEGRO_EVENT_TIMER) {
            if (Evento.timer.source == segundoTimer) {
                if (segundos > -2) {
                    segundos--;
                }
            }

        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        if (botones[0] == 0) {
            al_draw_bitmap(juego, 0, 0, 0);
        }
        else if (botones[0] == 1) {
            al_draw_bitmap(verde, 0, 0, 0);
            if (Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                al_play_sample(s_verde, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                al_rest(0.2);
                botones[0] = 0;
            }
        }
        else if (botones[0] == 2) {
            al_draw_bitmap(rojo, 0, 0, 0);
            if (Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                al_play_sample(s_rojo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                al_rest(0.2);
                botones[0] = 0;
            }
        }
        else if (botones[0] == 3) {
            al_draw_bitmap(amarillo, 0, 0, 0);
            if (Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                al_play_sample(s_amarillo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                al_rest(0.2);
                botones[0] = 0;
            }
        }
        else if (botones[0] == 4) {
            al_draw_bitmap(azul, 0, 0, 0);
            if (Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                al_play_sample(s_azul, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                al_rest(0.2);
                botones[0] = 0;
            }
        }

        if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {

            x = Evento.mouse.x;
            y = Evento.mouse.y;

            if (x >= 221 && x <= 402 && y >= 126 && y <= 307 && Evento.mouse.button & 1) {
                if (!jugada && mostrar) {
                    botones[0] = 1;
                    color_jugador = 1;
                }
            }

            if (x >= 415 && x <= 596 && y >= 126 && y <= 307 && Evento.mouse.button & 1) {
                if (!jugada && mostrar) {
                    botones[0] = 2;
                    color_jugador = 2;
                }
            }

            if (x >= 221 && x <= 402 && y >= 321 && y <= 503 && Evento.mouse.button & 1) {
                if (!jugada && mostrar) {
                    botones[0] = 3;
                    color_jugador = 3;
                }
            }

            if (x >= 415 && x <= 596 && y >= 321 && y <= 503 && Evento.mouse.button & 1) {
                if (!jugada && mostrar) {
                    botones[0] = 4;
                    color_jugador = 4;
                }
            }

            if (x >= 15 && x <= 96 && y >= 500 && y <= 583 && Evento.mouse.button & 1) {
                eliminarLista(lista);
                al_play_sample(s_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                jugada = true;
                mostrar = false;
                perdiste = false;
                puntos = 0;
                return 1;
            }

        }

        if (perdiste != false) {
            al_draw_filled_rectangle(160, 250, 650, 350, al_map_rgb(34, 186, 155));
            al_draw_textf(fuente, al_map_rgb(255, 255, 255), 300, 280, NULL, "PERDISTE!");
            al_flip_display();
            al_rest(1.5);

            if (esRecord(puntos)) {
                cargarRecord();
            }
            puntajes.clear();
            puntos = 0;
            jugada = true;
            mostrar = false;
            //perdiste = false;
            eliminarLista(lista);
            return 1;
        }

        if (segundos > 0) {
            al_draw_textf(fuente, al_map_rgb(255, 255, 255), 120, 30, NULL, "EL JUEGO COMIENZA EN: %i", segundos);
        }
        else {
            if (segundos < 1 && segundos > -2) {
                al_draw_textf(fuente, al_map_rgb(255, 255, 255), 120, 30, NULL, "MEMORIZA LA SECUENCIA!");
            }
            else {
                al_draw_textf(fuente, al_map_rgb(255, 255, 255), 260, 30, NULL, "PUNTAJE: %i", puntos);
                if (jugada && !perdiste) {
                    color = (rand() % 4) + 1;
                    insertarLista(lista, color);
                    mostrar = false;
                    mostrarLista(lista);
                    mostrar = true;
                    aux1 = lista;
                    jugada = false;
                }
                else {
                    if (aux1 != NULL) {
                        if (aux1->color == color_jugador) {
                            aux1 = aux1->siguiente;
                            color_jugador = -1;
                        }
                        else {
                            if (color_jugador != -1) {
                                eliminarLista(lista);
                                jugada = true;
                                mostrar = false;
                                perdiste = true;
                            }
                        }
                    }
                    else {
                        jugada = true;
                        mostrar = false;
                        if (botones[0] == 1) {
                            al_draw_bitmap(verde, 0, 0, 0);
                            al_play_sample(s_verde, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                        }
                        if (botones[0] == 2) {
                            al_draw_bitmap(rojo, 0, 0, 0);
                            al_play_sample(s_rojo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                        }
                        if (botones[0] == 3) {
                            al_draw_bitmap(amarillo, 0, 0, 0);
                            al_play_sample(s_amarillo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                        }
                        if (botones[0] == 4) {
                            al_draw_bitmap(azul, 0, 0, 0);
                            al_play_sample(s_azul, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                        }
                        al_draw_filled_rectangle(160, 250, 650, 350, al_map_rgb(34, 186, 155));
                        al_draw_textf(fuente, al_map_rgb(255, 255, 255), 260, 30, NULL, "PUNTAJE: %i", puntos);
                        al_draw_textf(fuente, al_map_rgb(255, 255, 255), 300, 280, NULL, "MUY BIEN!");
                        al_flip_display();
                        al_rest(0.5);
                        botones[0] = 0;
                        if (nivel == 1) {
                            puntos = puntos + 10;
                        }
                        else {
                            if (nivel == 0.5) {
                                puntos = puntos + 11;
                            }
                            else {
                                puntos = puntos + 13;
                            }
                        }

                    }
                }
            }
        }

        al_flip_display();

    }

    return 1;

}

int dificultad()
{
    int x = -1, y = -1;
    ALLEGRO_BITMAP* dif_null = al_load_bitmap("imagenes/dificultad.jpg");
    ALLEGRO_BITMAP* dif_facil = al_load_bitmap("imagenes/dificultad1.jpg");
    ALLEGRO_BITMAP* dif_normal = al_load_bitmap("imagenes/dificultad2.jpg");
    ALLEGRO_BITMAP* dif_dificil = al_load_bitmap("imagenes/dificultad3.jpg");

    int botones[] = { 0 };
    while (true)
    {
        ALLEGRO_EVENT Evento;
        al_wait_for_event(event_queue, &Evento);

        al_clear_to_color(al_map_rgb(0, 0, 0));

        if (botones[0] == 0) {
            al_draw_bitmap(dif_null, 0, 0, 0);
        }
        else if (botones[0] == 1) {
            al_draw_bitmap(dif_facil, 0, 0, 0);
        }
        else if (botones[0] == 2) {
            al_draw_bitmap(dif_normal, 0, 0, 0);
        }
        else if (botones[0] == 3) {
            al_draw_bitmap(dif_dificil, 0, 0, 0);
        }

        if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            x = Evento.mouse.x;
            y = Evento.mouse.y;

            if (x >= 254 && x <= 548 && y >= 248 && y <= 299) {
                botones[0] = 1;
                if (Evento.mouse.button & 1) {
                    al_play_sample(s_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    nivel = 1;
                    return 1;
                }
            }
            else {
                if (x >= 193 && x <= 874 && y >= 316 && y <= 366) {
                    botones[0] = 2;
                    if (Evento.mouse.button & 1) {
                        al_play_sample(s_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        nivel = 0.5;
                        return 1;
                    }
                }
                else {
                    if (x >= 193 && x <= 874 && y >= 385 && y <= 435) {
                        botones[0] = 3;
                        if (Evento.mouse.button & 1) {
                            al_play_sample(s_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            nivel = 0.3;
                            return 1;
                        }
                    }
                    else {
                        botones[0] = 0;
                    }
                }

            }
        }

        al_flip_display();

    }

    return 1;

}

int records() {
    string nombre = "puntajes/puntajes.txt";
    char delimit = '|';
    int indice = 1;
    int cargo = true;
    int x_inicio = 200;
    int y_inicio = 80;

    ALLEGRO_BITMAP* records = al_load_bitmap("imagenes/records.jpg");
    al_draw_bitmap(records, 0, 0, 0);
    al_draw_textf(fuente, al_map_rgb(34, 186, 155), 150, 20, NULL, "TOP 10 PUNTUACIONES");

    /*if (cargo) {
        lectura(nombre, delimit);
        cargo = false;
    }*/
    lectura(nombre, delimit);
    for (auto it = puntajes.begin(); it != puntajes.end(); it++) {
        al_draw_textf(fuente, al_map_rgb(255, 255, 255), x_inicio, y_inicio, NULL, "%i  %s .................... %i", indice, it->second.c_str(), it->first);
        y_inicio = y_inicio + 50;
        indice += 1;
    }
    al_flip_display();

    while (true)
    {
        int x = -1;
        int y = -1;

        ALLEGRO_EVENT Evento;
        al_wait_for_event(event_queue, &Evento);

        if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {

            x = Evento.mouse.x;
            y = Evento.mouse.y;

            if (x >= 15 && x <= 96 && y >= 500 && y <= 583 && Evento.mouse.button & 1) {
                al_play_sample(s_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                puntajes.clear();
                //cargo = true;
                return 1;
            }

        }
    }

    return 1;

}

int cargarRecord() {
    string nombre = "puntajes/puntajes.txt";
    char delimit = '|';
    int segundos = 1;
    int a = 0;
    int b = 0;
    int c = 0;
    string cadena = "";

    ALLEGRO_BITMAP* records = al_load_bitmap("imagenes/records.jpg");

    while (true)
    {
        int x = -1;
        int y = -1;



        ALLEGRO_EVENT Evento;
        al_wait_for_event(event_queue, &Evento);

        al_draw_bitmap(records, 0, 0, 0);
        al_draw_textf(fuente, al_map_rgb(255, 255, 255), 250, 30, NULL, "NUEVO RECORD");
        al_draw_filled_rectangle(170, 250, 650, 350, al_map_rgb(34, 186, 155));

        if (Evento.type == ALLEGRO_EVENT_TIMER) {
            if (Evento.timer.source == segundoTimer) {
                segundos++;
            }
        }

        if (segundos % 2 != 0 && a == 0) {
            al_draw_textf(fuente_record, al_map_rgb(255, 255, 255), 350, 270, NULL, "_");
        }
        else {
            al_draw_textf(fuente_record, al_map_rgb(255, 255, 255), 350, 270, NULL, "%c", a);
        }
        if (segundos % 2 != 0 && b == 0) {
            al_draw_textf(fuente_record, al_map_rgb(255, 255, 255), 400, 270, NULL, "_");
        }
        else {
            al_draw_textf(fuente_record, al_map_rgb(255, 255, 255), 400, 270, NULL, "%c", b);
        }
        if (segundos % 2 != 0 && c == 0) {
            al_draw_textf(fuente_record, al_map_rgb(255, 255, 255), 450, 270, NULL, "_");
        }
        else {
            al_draw_textf(fuente_record, al_map_rgb(255, 255, 255), 450, 270, NULL, "%c", c);
        }

        if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {

            x = Evento.mouse.x;
            y = Evento.mouse.y;

            if (x >= 15 && x <= 96 && y >= 500 && y <= 583 && Evento.mouse.button & 1) {
                al_play_sample(s_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                return 1;
            }

        }

        if (Evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (Evento.keyboard.keycode)
            {
                case ALLEGRO_KEY_A: asignarLetra(a, b, c, 65, cadena); break;
                case ALLEGRO_KEY_B: asignarLetra(a, b, c, 66, cadena); break;
                case ALLEGRO_KEY_C: asignarLetra(a, b, c, 67, cadena); break;
                case ALLEGRO_KEY_D: asignarLetra(a, b, c, 68, cadena); break;
                case ALLEGRO_KEY_E: asignarLetra(a, b, c, 69, cadena); break;
                case ALLEGRO_KEY_F: asignarLetra(a, b, c, 70, cadena); break;
                case ALLEGRO_KEY_G: asignarLetra(a, b, c, 71, cadena); break;
                case ALLEGRO_KEY_H: asignarLetra(a, b, c, 72, cadena); break;
                case ALLEGRO_KEY_I: asignarLetra(a, b, c, 73, cadena); break;
                case ALLEGRO_KEY_J: asignarLetra(a, b, c, 74, cadena); break;
                case ALLEGRO_KEY_K: asignarLetra(a, b, c, 75, cadena); break;
                case ALLEGRO_KEY_L: asignarLetra(a, b, c, 76, cadena); break;
                case ALLEGRO_KEY_M: asignarLetra(a, b, c, 77, cadena); break;
                case ALLEGRO_KEY_N: asignarLetra(a, b, c, 78, cadena); break;
                case ALLEGRO_KEY_O: asignarLetra(a, b, c, 79, cadena); break;
                case ALLEGRO_KEY_P: asignarLetra(a, b, c, 80, cadena); break;
                case ALLEGRO_KEY_Q: asignarLetra(a, b, c, 81, cadena); break;
                case ALLEGRO_KEY_R: asignarLetra(a, b, c, 82, cadena); break;
                case ALLEGRO_KEY_S: asignarLetra(a, b, c, 83, cadena); break;
                case ALLEGRO_KEY_T: asignarLetra(a, b, c, 84, cadena); break;
                case ALLEGRO_KEY_U: asignarLetra(a, b, c, 85, cadena); break;
                case ALLEGRO_KEY_V: asignarLetra(a, b, c, 86, cadena); break;
                case ALLEGRO_KEY_W: asignarLetra(a, b, c, 87, cadena); break;
                case ALLEGRO_KEY_X: asignarLetra(a, b, c, 88, cadena); break;
                case ALLEGRO_KEY_Y: asignarLetra(a, b, c, 89, cadena); break;
                case ALLEGRO_KEY_Z: asignarLetra(a, b, c, 90, cadena); break;
            }
        }

        al_flip_display();

        if (cadena.size() > 2) {
            puntajes.push_back({ puntos,cadena });
            ordenarPuntajeYEscribir(nombre, puntajes, delimit);
            al_draw_bitmap(records, 0, 0, 0);
            al_draw_textf(fuente, al_map_rgb(255, 255, 255), 250, 30, NULL, "NUEVO RECORD");
            al_draw_filled_rectangle(170, 250, 650, 350, al_map_rgb(34, 186, 155));
            al_draw_textf(fuente_record, al_map_rgb(255, 255, 255), 350, 270, NULL, "%c", a);
            al_draw_textf(fuente_record, al_map_rgb(255, 255, 255), 400, 270, NULL, "%c", b);
            al_draw_textf(fuente_record, al_map_rgb(255, 255, 255), 450, 270, NULL, "%c", c);
            al_flip_display();
            al_rest(1.5);
            return 1;
        }
    }

    return 1;

}

int menu() {
    int x = -1, y = -1;

    menu_null = al_load_bitmap("imagenes/MENU.jpg");
    menu_jugar = al_load_bitmap("imagenes/MENU1.jpg");
    menu_dificultad = al_load_bitmap("imagenes/MENU2.jpg");
    menu_puntajes = al_load_bitmap("imagenes/MENU3.jpg");
    menu_salir = al_load_bitmap("imagenes/MENU4.jpg");

    s_menu = al_load_sample("sonidos/menu.wav");
    al_reserve_samples(5);

    int botones[] = { 0 };

    while (true)
    {
        ALLEGRO_EVENT Evento;
        al_wait_for_event(event_queue, &Evento);

        al_clear_to_color(al_map_rgb(0, 0, 0));

        if (botones[0] == 0)
            al_draw_bitmap(menu_null, 0, 0, 0);
        else if (botones[0] == 1)
            al_draw_bitmap(menu_jugar, 0, 0, 0);
        else if (botones[0] == 2)
            al_draw_bitmap(menu_dificultad, 0, 0, 0);
        else if (botones[0] == 3)
            al_draw_bitmap(menu_puntajes, 0, 0, 0);
        else
            al_draw_bitmap(menu_salir, 0, 0, 0);

        if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            x = Evento.mouse.x;
            y = Evento.mouse.y;

            if (x >= 254 && x <= 548 && y >= 248 && y <= 299) {
                botones[0] = 1;
                if (Evento.mouse.button & 1)
                {
                    al_play_sample(s_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    jugar();
                }
            }
            else {
                if (x >= 193 && x <= 874 && y >= 316 && y <= 366) {
                    botones[0] = 2;
                    if (Evento.mouse.button & 1)
                    {
                        al_play_sample(s_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        dificultad();
                    }
                }
                else {
                    if (x >= 193 && x <= 874 && y >= 385 && y <= 435) {
                        botones[0] = 3;
                        if (Evento.mouse.button & 1) {
                            al_play_sample(s_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            records();
                        }
                    }
                    else {
                        if (x >= 193 && x <= 874 && y >= 454 && y <= 504) {
                            botones[0] = 4;
                            if (Evento.mouse.button & 1) {
                                al_play_sample(s_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                al_flip_display();
                                al_rest(0.3);
                                return 1;
                            }
                        }
                        else {
                            botones[0] = 0;
                        }
                    }

                }

            }
        }

        if (nivel == 1.0) {
            al_draw_textf(fuente_nivel, al_map_rgb(255, 255, 255), 15, 570, NULL, "NIVEL: FACIL");
        }
        else {
            if (nivel == 0.5) {
                al_draw_textf(fuente_nivel, al_map_rgb(255, 255, 255), 15, 570, NULL, "NIVEL: MEDIO");
            }
            else {
                al_draw_textf(fuente_nivel, al_map_rgb(255, 255, 255), 15, 570, NULL, "NIVEL: DIFICIL");
            }
        }

        al_flip_display();
    }
}

void insertarLista(Nodo*& lista, int n) {

    Nodo* nuevo_nodo = new Nodo();
    nuevo_nodo->color = n;
    nuevo_nodo->siguiente = NULL;
    Nodo* aux1 = new Nodo();

    if (lista == NULL) {
        lista = nuevo_nodo;
        tamanio_lista++;
    }
    else {
        aux1 = lista;
        while (aux1->siguiente != NULL)
        {
            aux1 = aux1->siguiente;
        }
        aux1->siguiente = nuevo_nodo;
        tamanio_lista++;
    }
}

void mostrarLista(Nodo* lista) {
    Nodo* actual = new Nodo();
    actual = lista;
    al_draw_bitmap(juego, 0, 0, 0);
    al_draw_textf(fuente, al_map_rgb(255, 255, 255), 260, 30, NULL, "PUNTAJE: %i", puntos);
    al_flip_display();
    al_rest(0.2);
    while (actual != NULL) {
        al_draw_bitmap(juego, 0, 0, 0);
        al_draw_textf(fuente, al_map_rgb(255, 255, 255), 260, 30, NULL, "PUNTAJE: %i", puntos);
        al_flip_display();
        al_rest(0.2);
        switch (actual->color)
        {
        case 1: {
            al_draw_bitmap(verde, 0, 0, 0);
            al_play_sample(s_verde, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;
        }
        case 2: {
            al_draw_bitmap(rojo, 0, 0, 0);
            al_play_sample(s_rojo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;
        }
        case 3: {
            al_draw_bitmap(amarillo, 0, 0, 0);
            al_play_sample(s_amarillo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;

        }
        case 4: {
            al_draw_bitmap(azul, 0, 0, 0);
            al_play_sample(s_azul, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;
        }
        }
        actual = actual->siguiente;
        al_draw_textf(fuente, al_map_rgb(255, 255, 255), 260, 30, NULL, "PUNTAJE: %i", puntos);
        al_flip_display();
        al_rest(nivel);
    }

}

void eliminarLista(Nodo*& lista)
{
    Nodo* prev = lista;

    while (lista)
    {
        lista = lista->siguiente;

        free(prev);
        prev = lista;
    }
}

int split(string texto, vector<string>& vector, char delimit) {
    string palabra = "";

    for (int i = 0; i <= texto.length(); i++) {
        if (texto[i] == delimit || texto[i] == '\0') {
            vector.push_back(palabra);
            palabra = "";
        }
        else {
            palabra += texto[i];
        }
    }

    return vector.size();
}


void lectura(string nombre, char delimit) {
    ifstream archivo(nombre, ios::in);
    if (archivo.fail()) {
        cout << "Error, al abrir el archivo" << endl;
    }
    else {
        vector<string> resultado;
        string linea;

        while (!archivo.eof())
        {
            getline(archivo, linea);
            if (split(linea, resultado, delimit) == 2) {
                puntajes.push_back({ stoi(resultado[0]), resultado[1] });
            }
            resultado.clear();
        }

        archivo.close();
    }
}

void ordenarPuntajeYEscribir(string nombre, vector<pair<int, string>> puntajes, char delimit) {
    sort(puntajes.begin(), puntajes.end(), greater<>());

    ofstream archivo(nombre, ios::out);

    int cantidad = 1;
    for (auto it = puntajes.begin(); it != puntajes.end(); it++) {
        archivo << it->first << delimit << it->second << endl;
        cantidad++;
        if (cantidad == 11) {
            break;
        }
    }
    archivo.close();

}

int esRecord(int puntos) {
    string nombre = "puntajes/puntajes.txt";
    char delimit = '|';
    lectura(nombre, delimit);
    if (puntajes.size() < 10) {
        return true;
    }
    if (puntajes[puntajes.size() - 1].first >= puntos) {
        return false;
    }
    return true;

}

void asignarLetra(int& a, int& b, int& c, int letra, string& cadena) {
    if (a == 0) {
        a = letra;
        cadena += a;
    }
    else {
        if (b == 0) {
            b = letra;
            cadena += b;
        }
        else {
            if (c == 0) {
                c = letra;
                cadena += c;
            }
        }
    }
}

void inicializarAllegro() {

    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_mouse();
    al_init_image_addon();
    al_init_acodec_addon();
    al_install_audio();
    al_install_keyboard();

}

void destroyAllegro() {

    al_destroy_bitmap(menu_jugar);
    al_destroy_bitmap(menu_dificultad);
    al_destroy_bitmap(menu_puntajes);
    al_destroy_bitmap(menu_salir);

    al_destroy_bitmap(juego);
    al_destroy_bitmap(verde);
    al_destroy_bitmap(rojo);
    al_destroy_bitmap(amarillo);
    al_destroy_bitmap(azul);

    al_destroy_sample(s_verde);
    al_destroy_sample(s_rojo);
    al_destroy_sample(s_amarillo);
    al_destroy_sample(s_azul);
    al_destroy_sample(s_menu);

    al_destroy_font(fuente);
    al_destroy_font(fuente_record);
    al_destroy_font(fuente_nivel);

    al_destroy_event_queue(event_queue);
    al_destroy_timer(fps);
    al_destroy_timer(segundoTimer);
    al_destroy_display(ventana);

}