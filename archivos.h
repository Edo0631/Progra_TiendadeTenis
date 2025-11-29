#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <filesystem>
#include <map>
#include <vector>
#include <cstdio>
#include <ctime>
using namespace std;

struct DatosTenis{
    char marca[30];
    char modelo[30]; 
    int id;
    float precio;
};

class Archivo{
    private: 
        fstream archivotxt;
        fstream archivobin;
        string nom;

    public: 
        Archivo(string nom){
            this->nom = nom;
        }

        string fecha() {
            time_t ahora = time(nullptr);
            tm* t = localtime(&ahora);

            char buffer[20];
            strftime(buffer, sizeof(buffer), "%d/%m/%Y", t);

            return string(buffer);
        }

        string hora() {
            time_t ahora = time(nullptr);
            tm* t = localtime(&ahora);

            char buffer[20];
            strftime(buffer, sizeof(buffer), "%H:%M:%S", t);

            return string(buffer);
        }

        void convertirMayusculas(char *cadena) {
            for (int i = 0; i < strlen(cadena); i++) {
                cadena[i] = toupper(cadena[i]);
            }
        }

        void crearArchivotxt(){
            archivotxt.open(nom,ios::out | ios::app);
            if(!archivotxt){
                std::cout<<"\n No se pudo crear el archivo \n";
            }
            archivotxt.close();
        }

        void crearArchivobin(){
            archivobin.open(nom, ios::binary| ios::out);
            if(!archivobin){
                std::cout<<"\n No se pudo crear el archivo binario \n";
            }
            archivobin.close();
        }

        void mostrarArchivotxt(string nom){
            if (!filesystem::exists(nom)){
                    cout << "\n Aun no se ha creado el archivo ";
            }else {
                string linea;
                archivotxt.open(nom,ios::in);              
                int numero = 1;
                cout << "\n";
                while (getline(archivotxt, linea)) {  
                    cout << linea << "\n";
                }
                archivotxt.close(); 
            }
        }

        void mostrarArchivobin(DatosTenis datos){
            archivobin.open(nom, ios::binary|ios::in);
            if (!archivobin) {  
                cerr << " No se pudo abrir el archivo " << endl; 
            }
            archivobin.read((char *)&datos, sizeof(DatosTenis)); 
            while(!archivobin.eof()){
                cout << datos.marca  << " " << datos.modelo << " " << datos.id << " " << datos.precio << endl;
                archivobin.read((char *)&datos, sizeof(DatosTenis));
            }

            archivobin.close(); 
        }

        void agregarArchivotxt(DatosTenis datos){
            archivotxt.open(nom,ios::out | ios::app);
            if(!archivotxt){
                std::cout<<"\n No se pudo abrir el archivo \n";
            }

            cout << "\nIngresa la marca del tenis : ";
            cin.ignore();
            cin.getline(datos.marca, 30);
            convertirMayusculas(datos.marca);
            cout << "Ingresa el modelo del tenis : ";
            cin.getline(datos.modelo, 30);
            convertirMayusculas(datos.modelo);
            cout << "Ingresa el id : ";
            cin >> datos.id;
            cout << "Ingresa el precio del tenis : ";
            cin >> datos.precio;
            
            archivotxt << datos.id << " " << datos.marca << " " << datos.modelo << " " << datos.precio << endl;
            cout << "\nDatos agregados correctamente \n";
            archivotxt.close();
        }

        void agregarArchivobin(DatosTenis datos){
            archivobin.open(nom, ios::binary| ios::out | ios::app);
            if(!archivobin){
                std::cout<<"\n No se pudo abrir el archivo binario \n";
            }

            cout << "\nIngresa la marca del tenis : ";
            cin.ignore();
            cin.getline(datos.marca, 30);
            convertirMayusculas(datos.marca);
            cout << "Ingresa el modelo del tenis : ";
            cin.getline(datos.modelo, 30);
            convertirMayusculas(datos.modelo);
            cout << "Ingresa el id del modelo : ";
            cin >> datos.id;
            cout << "Ingresa el precio del tenis : ";
            cin >> datos.precio;

            archivobin.write((char *)&datos, sizeof(DatosTenis));
            cout << "\nDatos agregados correctamente \n";
            archivobin.close();
        }

        void eliminarArchivo(){
            if (filesystem::remove(nom)) {
                cout << "\nArchivo eliminado correctamente \n";
            } else {
                cout << "\nNo se pudo eliminar el archivo o no existe \n";
            }
        }

        void catalogoTenis(DatosTenis datos) {
            ifstream archivo(nom, ios::binary);
            if (!archivo) {
                cout << "Error al abrir el archivo\n";
                return; 
            }
            map<string, map<string, map <int, vector<float>>>> tenis;

            while (archivo.read((char *)&datos, sizeof(DatosTenis))) {
                tenis[datos.marca][datos.modelo][datos.id].push_back(datos.precio);
            }

            cout << "\n CATALOGO DE TENIS \n";

            for (auto &marca : tenis){
                cout << "\n" << marca.first << "\n";
                for (auto &modelo : marca.second){
                    cout << "\t " << modelo.first;
                    for (auto &id : modelo.second){
                        cout << " | id: " << id.first << " | ";
                        for (int i = 0; i < id.second.size(); i++){
                            cout << "$" << id.second[i];
                        }
                    }
                    cout << endl;
                }
                cout << endl;
            }

            archivo.close();
        }

    void buscarTenis(char *marca, DatosTenis datos) {
        ifstream archivo(nom, ios::binary);
        if (!archivo) {
            cout << "Error al abrir el archivo\n";
            return;
        }
        int resp;
        char modelo[30];
        bool encontradomarca = false;
        bool encontradomodelo = false;

        while (archivo.read((char *)&datos, sizeof(DatosTenis))) {
            if (strcmp(datos.marca, marca) == 0) {
                    encontradomarca = true;
                    cout << "\n ¡ MARCA DE TENIS AUN EN EXISTENCIA ! \n";
                    cout << "Marca : " << datos.marca;
                    cout << "\n\n Deseas buscar algun modelo ? (1)si (2)no : ";
                    cin >> resp;
                    if (resp == 1){
                        cout << "\n Que modelo deseas buscar : ";
                        cin >> modelo;
                        convertirMayusculas(modelo);
                        archivo.seekg(0, ios::beg);
                        while (archivo.read((char *)&datos, sizeof(DatosTenis))){
                            if (strcmp(datos.modelo, modelo) == 0){
                                cout << "\n ¡ Si existe el MODELO que buscas ! ";
                                cout << "\nid : " << datos.id;
                                cout << " | Modelo: " << datos.modelo;
                                cout << " | Precio: $" << datos.precio << endl;
                                break;
                            }
                        }
                    }
                    break;
            }
        }

        if (!encontradomarca) {
            cout << "\n No tenemos tenis de " << marca << "\n";
        }else{
            if (!encontradomodelo) {
                cout << "\n No esta disponible el modelo : " << modelo << "\n";
            }
        }

        archivo.close();
    }

    void ticket(DatosTenis datos){
        archivotxt.open("tickets.txt",ios::out);
        if(!archivotxt){
            cout<<"\n No se pudo crear el archivo \n";
        }
        
        archivotxt  << endl << "________________TICKET DE COMPRA___________________" << endl;
        archivotxt << "  Datos de la compra " << endl;
        archivotxt << "Id del producto : " << datos.id << endl;
        archivotxt << "Marca del producto : " << datos.marca << endl;
        archivotxt << "Modelo del producto : " << datos.modelo << endl;
        archivotxt << " TOTAL : $ " << datos.precio << endl << endl;
        archivotxt << " Hora de Corte de caja " << hora() << endl;
        archivotxt << " Fecha de Corte de caja " << fecha() << endl;
        archivotxt << " ¡ GRACIAS POR COMPRAR EN NUESTRA TIENDA ! "  << endl;
        archivotxt << "______________________________________________________" << endl;

        archivotxt.close();
    }

    void comprar(DatosTenis datos, int compra, vector<DatosTenis>& comprasDelDia){
        ifstream archivo(nom, ios::binary);
        if (!archivo) {
            cout << "Error al abrir el archivo\n";
            return;
        }

        ofstream archivoTemp("temp", ios::binary);
        if (!archivoTemp) {
            cout << "Error al crear archivo temporal\n";
            archivo.close();
            return;
        }

        bool encontrado = false;
        DatosTenis TenisComprado;
        while (archivo.read((char*)&datos, sizeof(DatosTenis))) {
            if (datos.id == compra) {
                TenisComprado = datos;
                encontrado = true;
                break;
            }
        }

        archivo.clear();
        archivo.seekg(0);

        if (encontrado) {
            int res;
            cout << "\n (1) Finalizar compra \n (2) Cancelar compra \n Que deseas hacer : ";
            cin >> res;

            if (res == 1) {
                while (archivo.read((char*)&datos, sizeof(DatosTenis))) {
                    if (datos.id != compra) {
                        archivoTemp.write((char*)&datos, sizeof(DatosTenis));
                    }
                }

                ticket(TenisComprado);
                mostrarArchivotxt("tickets.txt");
                comprasDelDia.push_back(TenisComprado);

                archivo.close();
                archivoTemp.close();

                remove(nom.c_str());
                rename("temp", nom.c_str());
            } else {
                cout << "\n Espero te animes la proxima vez ";
                archivo.close();
                archivoTemp.close();
                remove("temp");
            }
        } else {
            cout << "\n El tenis no esta en existencia ";
            archivo.close();
            archivoTemp.close();
            remove("temp");
        }
    }

    void corteCaja(const vector<DatosTenis>& comprasDelDia){
        archivotxt.open("corteCaja.txt",ios::out);
        if(!archivotxt){
            cout<<"\n No se pudo crear el archivo \n";
        }
        float total = 0;
        int cont = 0;

        archivotxt << endl << "_________________________ CORTE DE CAJA _________________________\n" << endl;
        archivotxt << " RESUMEN DE COMPRAS " << endl << endl;
        for (const auto& c : comprasDelDia) {
            cont++;
            archivotxt << "Compra " << cont << " | ";
            archivotxt << " id: " << c.id << " | Marca: " << c.marca << " | Modelo: " << c.modelo << " | Precio: $ " << c.precio << endl; 
            total += c.precio;
        }

        archivotxt << endl << "Tenis vendidos en total : " << cont << endl;
        archivotxt << "Total vendido en el dia : $" << total << endl<< endl;
        archivotxt << " Hora de Corte de caja " << hora() << endl;
        archivotxt << " Fecha de Corte de caja " << fecha() << endl;
        archivotxt << "_______________________________________________________________________" << endl;

        archivotxt.close();
    }

};