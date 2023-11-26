#include <Windows.h>
#include "resource.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <conio.h>

using namespace std;

char eventos[12][12] = { "Uncharted", "Divergente", "Avengers", "Frozen", "Wish", "Rio", "Jumanji", "Titanic", "Rapunzel", "FNAF", "Dune", "Hugo" };
char filas[15][15] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O"};
char columnas[15][15] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15" };
char promocion[5][5] = { "No", "2x1", "10%"};
float precios[50];
int contadorPrecios = 0;
int contadorBoletos = 0;
int sumaPrecio = 0;

struct nodoUsuario {
    nodoUsuario* anteriorUsuario;
    char usuario[50] = { 0 };
    char contrasena[50] = { 0 };
    int numeroCompras = 0;
    nodoUsuario* siguienteUsuario;
};

typedef nodoUsuario* usuarioPuntero;

usuarioPuntero nuevoUsuario = nullptr;
usuarioPuntero cabezaUsuario = nullptr;
usuarioPuntero temporalUsuario = nullptr;
usuarioPuntero identificadorUsuario = nullptr;
//usuarioPuntero ActualDoctor = nullptr;
//usuarioPuntero anteriorDoctor = nullptr;

struct nodoBoleto {
    nodoBoleto* anteriorBoleto;
    char evento[15] = { 0 };
    char fila[15] = { 0 };
    char numero[15] = { 0 };
    float precio = 0;
    char Usuario[50] = { 0 };
    int numCompra = 0;
    nodoBoleto* siguienteBoleto;
};

typedef nodoBoleto* boletoPuntero;

boletoPuntero nuevoBoleto = nullptr;
boletoPuntero cabezaBoleto = nullptr;
boletoPuntero temporalBoleto = nullptr;
boletoPuntero boletoRegistrar = nullptr;

struct nodoPrecio {
    nodoPrecio* anteriorPrecio;
    float precioFinal = 0;
    int compraNum = 0;
    char USUARIO[50] = { 0 };
    nodoPrecio* siguientePrecio;
};

typedef nodoPrecio* precioPuntero;

precioPuntero nuevoPrecio = nullptr;
precioPuntero cabezaPrecio = nullptr;
precioPuntero temporalPrecio = nullptr;

HINSTANCE hInst;

LRESULT CALLBACK VentanaInicioSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaRegistrarCuenta(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaPantallaInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaEventosMes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaVentaBoletos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaPromociones(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaMisCompras(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    HWND hwnd;
    MSG mensaje;

    hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(inicioSesion), NULL, VentanaInicioSesion);
    ShowWindow(hwnd, SW_SHOWDEFAULT);

    HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));


    while (TRUE == GetMessage(&mensaje, 0, 0, 0))
    {
        TranslateMessage(&mensaje);
        DispatchMessage(&mensaje);
    }

    return mensaje.wParam;
}

LRESULT CALLBACK VentanaInicioSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg)
    {
    case WM_COMMAND: {
        if (LOWORD(wParam) == botonInicio) {
            char usuario[50] = { 0 };
            char contrasena[50] = { 0 };
            GetDlgItemTextA(hwnd, inicioUsuario, usuario, sizeof(usuario));
            GetDlgItemTextA(hwnd, inicioContrasena, contrasena, sizeof(contrasena));

            temporalUsuario = cabezaUsuario;

            if (temporalUsuario == nullptr) {
                MessageBox(hwnd, "No hay usuarios registrados", "Error", MB_OK | MB_ICONERROR);
            }

            while (temporalUsuario != nullptr) {
                if (strcmp(usuario, temporalUsuario->usuario) == 0) {
                    // Si se encuentra checar contraseña
                    if (strcmp(contrasena, temporalUsuario->contrasena) == 0) {
                        EndDialog(hwnd, 0);
                        identificadorUsuario = temporalUsuario;
                        DialogBox(hInst, MAKEINTRESOURCE(pantallaInicio), hwnd, VentanaPantallaInicio);
                        break;
                    }
                    else {
                        // Si no es la misma contraseña
                        MessageBox(hwnd, "Contraseña incorrecta", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                }
                temporalUsuario = temporalUsuario->siguienteUsuario;
                if (temporalUsuario == nullptr) {
                    MessageBox(hwnd, "No se encontró el usuario", "Error", MB_OK | MB_ICONERROR);
                    break;
                }
            }

        }
        else if (LOWORD(wParam) == botonRegistrar) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(registroUsuario), hwnd, VentanaRegistrarCuenta);

        }
    }break;

    case WM_CLOSE: {
        DestroyWindow(hwnd);
        PostQuitMessage(0);
    }break;
    }
    return 0;
}

LRESULT CALLBACK VentanaRegistrarCuenta(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    char usuario[50] = { 50 };
    char contrasena[50] = { 50 };

    switch (msg)
    {
    case WM_COMMAND: {
        if (LOWORD(wParam) == botonCrearCuenta) {
            GetDlgItemTextA(hwnd, crearUsuario, usuario, sizeof(usuario));
            GetDlgItemTextA(hwnd, crearContrasena, contrasena, sizeof(contrasena));

            if (strlen(usuario) == 0 || strlen(contrasena) == 0) {
                MessageBox(hwnd, "Registre todos los campos.", "Error", MB_OK | MB_ICONERROR);
                return 0;
            }

            temporalUsuario = cabezaUsuario;
            while (temporalUsuario != nullptr) {
                if (strcmp(temporalUsuario->usuario, usuario) == 0) {
                    MessageBox(hwnd, "El usuario ya existe. Ingrese otro nombre de usuario.", "Error", MB_OK | MB_ICONERROR);
                    return 0;
                }
                temporalUsuario = temporalUsuario->siguienteUsuario;
            }

            nuevoUsuario = new nodoUsuario;

            strcpy_s(nuevoUsuario->usuario, sizeof(nuevoUsuario->usuario), usuario);
            strcpy_s(nuevoUsuario->contrasena, sizeof(nuevoUsuario->contrasena), contrasena);

            if (cabezaUsuario == nullptr) {
                nuevoUsuario->siguienteUsuario = cabezaUsuario;
                cabezaUsuario = nuevoUsuario;
            }
            else {
                usuarioPuntero actual = cabezaUsuario;
                usuarioPuntero anterior = nullptr;

                while (actual != nullptr) {
                    anterior = actual;
                    actual = actual->siguienteUsuario;
                }

                nuevoUsuario->siguienteUsuario = actual;
                anterior->siguienteUsuario = nuevoUsuario;
            }

            MessageBox(hwnd, "Se a registrado con exito.", "Éxito", MB_OK | MB_ICONINFORMATION);

            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(inicioSesion), hwnd, VentanaInicioSesion);
        }
    }break;

    case WM_CLOSE: {
        DestroyWindow(hwnd);
        PostQuitMessage(0);
    }break;
    }
    return 0;
}

LRESULT CALLBACK VentanaPantallaInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {
    case WM_COMMAND: {
        if (LOWORD(wParam) == ID_PANTALLADEINICIO) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(pantallaInicio), hwnd, VentanaPantallaInicio);
        }
        else if (LOWORD(wParam) == ID_EVENTOSDELMES) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(eventosMes), hwnd, VentanaEventosMes);
        }
        else if (LOWORD(wParam) == ID_VENTADEBOLETOS) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(ventaBoletos), hwnd, VentanaVentaBoletos);
        }
        else if (LOWORD(wParam) == ID_MISCOMPRAS) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(misCompras), hwnd, VentanaMisCompras);
        }
        else if (LOWORD(wParam) == ID_SALIR) {
            DestroyWindow(hwnd);
            PostQuitMessage(0);
        }
    }break;

    case WM_CLOSE: {
        DestroyWindow(hwnd);
        PostQuitMessage(0);
    }break;
    }
    return 0;
}

LRESULT CALLBACK VentanaEventosMes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg)
    {
    case WM_INITDIALOG: {
        for (int i = 0; i < 12; i++) {
            SendMessage(GetDlgItem(hwnd, listaEventoMes), LB_INSERTSTRING, i, (LPARAM)eventos[i]);
        }
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == ID_PANTALLADEINICIO) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(pantallaInicio), hwnd, VentanaPantallaInicio);
        }
        else if (LOWORD(wParam) == ID_EVENTOSDELMES) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(eventosMes), hwnd, VentanaEventosMes);
        }
        else if (LOWORD(wParam) == ID_VENTADEBOLETOS) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(ventaBoletos), hwnd, VentanaVentaBoletos);
        }
        else if (LOWORD(wParam) == ID_MISCOMPRAS) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(misCompras), hwnd, VentanaMisCompras);
        }
        else if (LOWORD(wParam) == ID_SALIR) {
            DestroyWindow(hwnd);
            PostQuitMessage(0);
        }
    }break;

    case WM_CLOSE: {
        DestroyWindow(hwnd);
        PostQuitMessage(0);
    }break;
    }
    return 0;
}

LRESULT CALLBACK VentanaVentaBoletos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    char precioTotal[5];
    char selectorEvento[15];
    char selectorFila[15];
    char selectorNumero[15];
  

    switch (msg)
    {
    case WM_INITDIALOG: {
        for (int i = 0; i < 12; i++) {
            SendMessage(GetDlgItem(hwnd, listaEventos), LB_INSERTSTRING, i, (LPARAM)eventos[i]);
        }

        for (int i = 0; i < 15; i++) {
            SendMessage(GetDlgItem(hwnd, listaFila), LB_INSERTSTRING, i, (LPARAM)filas[i]);
            SendMessage(GetDlgItem(hwnd, listaNumero), LB_INSERTSTRING, i, (LPARAM)columnas[i]);
        }

        HWND hPrecio = GetDlgItem(hwnd, numPrecio);
        SendMessage(hPrecio, EM_SETREADONLY, TRUE, 0);

        if (contadorBoletos == 0) {
            sumaPrecio = 0;
            strcpy_s(precioTotal, "0");
            SetDlgItemText(hwnd, numPrecio, precioTotal);
        }
        return TRUE;
        
        

    }
    case WM_COMMAND: {

        if (LOWORD(wParam) == botonRegistrarBoleto) {
            int index1 = SendDlgItemMessage(hwnd, listaEventos, LB_GETCURSEL, 0, 0);
            int index2 = SendDlgItemMessage(hwnd, listaFila, LB_GETCURSEL, 0, 0);
            int index3 = SendDlgItemMessage(hwnd, listaNumero, LB_GETCURSEL, 0, 0);

            if (index1 != LB_ERR && index2 != LB_ERR && index3 != LB_ERR) {

                SendDlgItemMessage(hwnd, listaEventos, LB_GETTEXT, index1, (LPARAM)selectorEvento);
                SendDlgItemMessage(hwnd, listaFila, LB_GETTEXT, index2, (LPARAM)selectorFila);
                SendDlgItemMessage(hwnd, listaNumero, LB_GETTEXT, index3, (LPARAM)selectorNumero);

                //No repetir boleto
                temporalBoleto = cabezaBoleto;
                while (temporalBoleto != nullptr) {
                    if (strcmp(temporalBoleto->fila, selectorFila) == 0 && (temporalBoleto->numero, selectorNumero) == 0 && (temporalBoleto->evento, selectorEvento) == 0) {
                        MessageBox(hwnd, "El lugar ya esta ocupado. Seleccione otro asiento.", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }
                    temporalBoleto = temporalBoleto->siguienteBoleto;
                }

                nuevoBoleto = new nodoBoleto;
                nuevoBoleto->siguienteBoleto = nullptr;
                nuevoBoleto->anteriorBoleto = nullptr;

                strcpy_s(nuevoBoleto->evento, sizeof(nuevoBoleto->evento), selectorEvento);
                strcpy_s(nuevoBoleto->fila, sizeof(nuevoBoleto->fila), selectorFila);
                strcpy_s(nuevoBoleto->numero, sizeof(nuevoBoleto->numero), selectorNumero);
                strcpy_s(nuevoBoleto->Usuario, sizeof(nuevoBoleto->Usuario), identificadorUsuario->usuario);
                nuevoBoleto->precio = 80.0;

                if (cabezaBoleto == nullptr) {
                    nuevoBoleto->siguienteBoleto = cabezaBoleto;
                    cabezaBoleto = nuevoBoleto;
                }
                else {
                    boletoPuntero actual = cabezaBoleto;
                    boletoPuntero anterior = nullptr;

                    while (actual != nullptr) {
                        anterior = actual;
                        actual = actual->siguienteBoleto;
                    }

                    nuevoBoleto->siguienteBoleto = actual;
                    anterior->siguienteBoleto = nuevoBoleto;
                }

                if (contadorBoletos == 0) {
                    boletoRegistrar = nuevoBoleto;
                }

                MessageBox(hwnd, "Se a registrado con exito.", "Éxito", MB_OK | MB_ICONINFORMATION);

                
                sumaPrecio = sumaPrecio + nuevoBoleto->precio;

                
                _itoa_s(sumaPrecio, precioTotal, sizeof(precioTotal), 10);

                
                SetDlgItemText(hwnd, numPrecio, precioTotal);

                contadorBoletos++;

            }
            else {
                MessageBox(hwnd, "Seleccione todos los campos.", "Error", MB_OK | MB_ICONERROR);
                return 0;
            }

        }
        else if (LOWORD(wParam) == botonComprar) {
            if (contadorBoletos == 0) {
                MessageBox(hwnd, "No ha registrado ningun boleto.", "Error", MB_OK | MB_ICONERROR);
                return 0;
            }
            contadorPrecios ++;
            identificadorUsuario->numeroCompras += 1;
            temporalBoleto = boletoRegistrar;
            while (temporalBoleto != nullptr) {
                temporalBoleto->numCompra = identificadorUsuario->numeroCompras;
                temporalBoleto = temporalBoleto->siguienteBoleto;
            }
            /*contadorBoletos = 0;*/
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(promociones), hwnd, VentanaPromociones);
        }
    }break;

    case WM_CLOSE: {
        if (contadorBoletos > 0) {
            boletoPuntero temporal;
            boletoPuntero cabeza  = boletoRegistrar;
            while (cabeza != nullptr) {
                temporal = cabeza;
                cabeza = cabeza->siguienteBoleto;
                delete temporal;
            }
            boletoRegistrar = nullptr;
        }
        if (identificadorUsuario->numeroCompras == 0) {
            cabezaBoleto = nullptr;
            nuevoBoleto = nullptr;
        }
        contadorBoletos = 0;
        EndDialog(hwnd, 0);
        DialogBox(hInst, MAKEINTRESOURCE(pantallaInicio), hwnd, VentanaPantallaInicio);
    }break;
    }
    return 0;
}

LRESULT CALLBACK VentanaPromociones(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    char valorFinal[5];

    switch (msg)
    {
    case WM_INITDIALOG: {
        for (int i = 0; i < 5; i++) {
            SendMessage(GetDlgItem(hwnd, listaPromocion), LB_INSERTSTRING, i, (LPARAM)promocion[i]);
        }
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == botonSiguiente) {
            int index = SendDlgItemMessage(hwnd, listaPromocion, LB_GETCURSEL, 0, 0);

            if (index != LB_ERR) {
                if (index == 0) {
                    nuevoPrecio = new nodoPrecio;
                    nuevoPrecio->siguientePrecio = nullptr;
                    nuevoPrecio->anteriorPrecio = nullptr;

                    strcpy_s(nuevoPrecio->USUARIO, sizeof(nuevoPrecio->USUARIO), identificadorUsuario->usuario);
                    nuevoPrecio->compraNum = identificadorUsuario->numeroCompras;

                    nuevoPrecio->precioFinal = sumaPrecio + (sumaPrecio * .16);
                    float prueba = nuevoPrecio->precioFinal;
                    precios[contadorPrecios] = prueba;

                    if (cabezaPrecio == nullptr) {
                        nuevoPrecio->siguientePrecio = cabezaPrecio;
                        cabezaPrecio = nuevoPrecio;
                    }
                    else {
                        precioPuntero actual = cabezaPrecio;
                        precioPuntero anterior = nullptr;

                        while (actual != nullptr) {
                            anterior = actual;
                            actual = actual->siguientePrecio;
                        }

                        nuevoPrecio->siguientePrecio = actual;
                        anterior->siguientePrecio = nuevoPrecio;
                    }

                    contadorBoletos = 0;
                    EndDialog(hwnd, 0);
                    DialogBox(hInst, MAKEINTRESOURCE(pantallaInicio), hwnd, VentanaPantallaInicio);

                } else if (index == 1) {
                    nuevoPrecio = new nodoPrecio;
                    nuevoPrecio->siguientePrecio = nullptr;
                    nuevoPrecio->anteriorPrecio = nullptr;

                    strcpy_s(nuevoPrecio->USUARIO, sizeof(nuevoPrecio->USUARIO), identificadorUsuario->usuario);
                    nuevoPrecio->compraNum = identificadorUsuario->numeroCompras;

                    int numeroDiv = contadorBoletos / 2;
                    float precioResta = numeroDiv * 80;
                    sumaPrecio = sumaPrecio - precioResta;
                    nuevoPrecio->precioFinal = sumaPrecio + (sumaPrecio * .16);
                    float prueba = nuevoPrecio->precioFinal;
                    precios[contadorPrecios] = prueba;


                    if (cabezaPrecio == nullptr) {
                        nuevoPrecio->siguientePrecio = cabezaPrecio;
                        cabezaPrecio = nuevoPrecio;
                    }
                    else {
                        precioPuntero actual = cabezaPrecio;
                        precioPuntero anterior = nullptr;

                        while (actual != nullptr) {
                            anterior = actual;
                            actual = actual->siguientePrecio;
                        }

                        nuevoPrecio->siguientePrecio = actual;
                        anterior->siguientePrecio = nuevoPrecio;
                    }

                    contadorBoletos = 0;
                    EndDialog(hwnd, 0);
                    DialogBox(hInst, MAKEINTRESOURCE(pantallaInicio), hwnd, VentanaPantallaInicio);
                }
                else if (index == 2) {
                    nuevoPrecio = new nodoPrecio;
                    nuevoPrecio->siguientePrecio = nullptr;
                    nuevoPrecio->anteriorPrecio = nullptr;

                    strcpy_s(nuevoPrecio->USUARIO, sizeof(nuevoPrecio->USUARIO), identificadorUsuario->usuario);
                    nuevoPrecio->compraNum = identificadorUsuario->numeroCompras;

                    sumaPrecio = sumaPrecio * .9;
                    nuevoPrecio->precioFinal = sumaPrecio + (sumaPrecio * .16);
                    float prueba = nuevoPrecio->precioFinal;
                    precios[contadorPrecios] = prueba;


                    if (cabezaPrecio == nullptr) {
                        nuevoPrecio->siguientePrecio = cabezaPrecio;
                        cabezaPrecio = nuevoPrecio;
                    }
                    else {
                        precioPuntero actual = cabezaPrecio;
                        precioPuntero anterior = nullptr;

                        while (actual != nullptr) {
                            anterior = actual;
                            actual = actual->siguientePrecio;
                        }

                        nuevoPrecio->siguientePrecio = actual;
                        anterior->siguientePrecio = nuevoPrecio;
                    }

                    contadorBoletos = 0;
                    EndDialog(hwnd, 0);
                    DialogBox(hInst, MAKEINTRESOURCE(pantallaInicio), hwnd, VentanaPantallaInicio);
                }
            }
            else {
                MessageBox(hwnd, "No se ha seleccionado ninguna promocion.", "Atención", MB_OK | MB_ICONWARNING);
            }

        }
    }break;

    case WM_CLOSE: {
        DestroyWindow(hwnd);
        PostQuitMessage(0);
    }break;
    }
    return 0;
}

LRESULT CALLBACK VentanaMisCompras(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {
    case WM_INITDIALOG: {
        if (identificadorUsuario->numeroCompras > 0) {
            temporalBoleto = cabezaBoleto;
            while (temporalBoleto != nullptr) {
                SendMessage(GetDlgItem(hwnd, listaEventos), LB_INSERTSTRING, 0, (LPARAM)(" "));
                SendMessage(GetDlgItem(hwnd, listaEventos), LB_INSERTSTRING, 0, (LPARAM)temporalBoleto->numero);
                SendMessage(GetDlgItem(hwnd, listaEventos), LB_INSERTSTRING, 0, (LPARAM)temporalBoleto->fila);
                SendMessage(GetDlgItem(hwnd, listaEventos), LB_INSERTSTRING, 0, (LPARAM)temporalBoleto->evento);
                temporalBoleto = temporalBoleto->siguienteBoleto;
            }

            for (int i = 1; i <= identificadorUsuario->numeroCompras; i++) {
                float prueba = precios[i];
                std::string strPrueba = std::to_string(prueba);

                SendMessage(GetDlgItem(hwnd, listaEventos), LB_INSERTSTRING, 0, (LPARAM)(" "));
                SendMessage(GetDlgItem(hwnd, listaEventos), LB_INSERTSTRING, 0, (LPARAM)strPrueba.c_str());
            }
        }
        

    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == ID_PANTALLADEINICIO) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(pantallaInicio), hwnd, VentanaPantallaInicio);
        }
        else if (LOWORD(wParam) == ID_EVENTOSDELMES) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(eventosMes), hwnd, VentanaEventosMes);
        }
        else if (LOWORD(wParam) == ID_VENTADEBOLETOS) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(ventaBoletos), hwnd, VentanaVentaBoletos);
        }
        else if (LOWORD(wParam) == ID_MISCOMPRAS) {
            EndDialog(hwnd, 0);
            DialogBox(hInst, MAKEINTRESOURCE(misCompras), hwnd, VentanaMisCompras);
        }
        else if (LOWORD(wParam) == ID_SALIR) {
            DestroyWindow(hwnd);
            PostQuitMessage(0);
        }
    }break;

    case WM_CLOSE: {
        DestroyWindow(hwnd);
        PostQuitMessage(0);
    }break;
    }
    return 0;
}