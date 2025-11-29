#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "menu.h"
#include "archivos.h"
using namespace std;

class Tienda{
    protected:
        const string contraAdmin = "11";
        bool accesoAdmin, accesoCliente;
        Archivo inventario{"inventario"};
        Archivo ticket{"tickets.txt"};
        Archivo corteCaja{"corteCaja.txt"};

    public:
        static vector<DatosTenis> comprasDelDia;
        Tienda(){
            accesoAdmin = false;
            accesoCliente = false;
        }

        bool entradaAdmin(string contra){
            if (contra == contraAdmin){
                accesoAdmin = true;
                return true;
            }else{
                return false;
            }
        }

        bool getaccesoAdmin() { return accesoAdmin; }
        void setaccesoAdmin(bool acceso) { accesoAdmin = acceso; }
        bool getaccesoCliente() { return accesoCliente; }
        void setaccesoCliente(bool acceso) { accesoCliente = acceso; }

};

class Tenis{
    protected:
        DatosTenis datos;

    public:
        Tenis(){
            strcpy(datos.marca, "");
            strcpy(datos.modelo, "");
            datos.id = 0;
            datos.precio = 0.0;
        }

        void setDatosTenis(DatosTenis datos) { datos = datos; }
        DatosTenis getDatosTenis() { return datos; }

};

class Administrador : public Tienda, public Tenis{
    public:
        void agregarTenis(){
            inventario.agregarArchivobin(datos);
        }

        void mostrarInventario(){
            inventario.mostrarArchivobin(datos);
        }

        void corteDeCaja(){
            corteCaja.corteCaja(Tienda::comprasDelDia);
            if (Tienda::comprasDelDia.empty()){
                cout << "\n\n AUN NO HAY NINGUNA VENTA \n\n ";
            }else{
                corteCaja.mostrarArchivotxt("corteCaja.txt");
            }
        }

};
 
class Cliente : public Tienda, public Tenis{
    public:
        void catalogo(){
            inventario.catalogoTenis(datos);
        }

        void buscarTenis(){
            char marca[30];
            cout << "\n Que Marca de Tenis deseas buscar : ";
            cin >> marca;
            inventario.convertirMayusculas(marca);
            inventario.buscarTenis(marca, datos);
        }

        void comprarTenis(){
            int compra, r;
            cout << "\n NECESITAS EL ID DEL TENIS PARA COMPRAR"; 
            do {
                cout << "\n(1) Ver catalogo para ver id \n(2) Comprar tenis \n Que deseas hacer : ";
                cin >> r;
                if (r == 1){
                    catalogo();
                }
            }while (r != 2);
            cout << "\n Ingresa el id del tenis que quieres comprar : ";
            cin >> compra;
            inventario.comprar(datos, compra, Tienda::comprasDelDia);
            
        }

};

vector<DatosTenis> Tienda::comprasDelDia;

int main(){ 
    Tienda emaedo;
    Administrador admin;
    Cliente cliente;
    int sesion, opAdmin, opCliente;

    do{
        //Menu principal
        sesion = menu("MENU",{"Iniciar sesion como Administrador", "Iniciar sesion como Cliente"});
        switch (sesion){
            case 1 : 
            {
                string contra;
                cout << "\nContrasenia : ";
                cin >> contra;                  
                if (emaedo.entradaAdmin(contra)){
                    cout << "\nBienvenido tienes acceso como Administrador \n";
                }else{ 
                    cout << "\nContrasenia incorrecta intenta de nuevo \n";
                }
                break;
            }
            case 2 :
                cout << "\nBienvenido tienes acceso como Cliente \n";
                emaedo.setaccesoAdmin(false);
                emaedo.setaccesoCliente(true);
                break;
            case 0 :
                cout << "\n\n SALIENDO DE LA TIENDA  \n\n";
                emaedo.setaccesoAdmin(false);
                emaedo.setaccesoCliente(false);
                break;
            default : 
                cout << "Opcion invalida..................";
                emaedo.setaccesoAdmin(false);
                emaedo.setaccesoCliente(false);
                break;
        }

        //Acceso a las acciones de admin
        if (emaedo.getaccesoAdmin()){
            do{
                opAdmin = menu("ADMINISTRADOR",{"Ingresar Tenis a Almacen", "Reporte de inventario", "Corte de Caja"});

                switch (opAdmin){
                    case 1: 
                        admin.agregarTenis();
                        break;
                    case 2:
                        cout << "\n_____________Reporte de inventario_______________\n";
                        admin.mostrarInventario();
                        break;
                    case 3:
                        admin.corteDeCaja();
                        break;
                    case 0 :
                        cout << "\nRegresando a menu principal......................... \n";
                        emaedo.setaccesoAdmin(false);
                        emaedo.setaccesoCliente(false);
                        break;
                    default:
                        cout << "\n Opcion invalida porfavor ingresa una opcion dentro del menu\n";
                        break;
                }
            }while (opAdmin != 0);

        }
        if (emaedo.getaccesoCliente()){
        //Acceso a las acciones de cliente
            do{
                opCliente = menu(" CLIENTE ",{"Ver catalogo de Tenis", "Realizar compra", "Busqueda de Tenis"});

                switch (opCliente){
                    case 1:
                        cliente.catalogo();
                        break;
                    case 2:
                        cliente.comprarTenis();
                        break;
                    case 3:
                        cliente.buscarTenis();
                        break;
                    case 0 :
                        cout << "\nRegresando a menu principal......................... \n";
                        emaedo.setaccesoAdmin(false);
                        emaedo.setaccesoCliente(false);
                        break;
                    default:
                        cout << "\n Opcion invalida porfavor ingresa una de las del menu \n";
                        break;
                }
            }while (opCliente != 0);
        }

    }while (sesion != 0);

    cout << endl;
    return 0;
}