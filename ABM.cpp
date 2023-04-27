#include <iostream>
#include <sqlite3.h>

using namespace std;

// Definición de la estructura Usuario
struct Usuario {
    int id;
    string nombre;
    string apellido;
    string email;
};

// Función para imprimir los datos de un usuario
void imprimirUsuario(Usuario usuario) {
    cout << "ID: " << usuario.id << endl;
    cout << "Nombre: " << usuario.nombre << endl;
    cout << "Apellido: " << usuario.apellido << endl;
    cout << "Email: " << usuario.email << endl;
}

// Función para ejecutar una consulta SQL
int ejecutarConsulta(sqlite3* db, string consulta, int(*callback)(void*, int, char**, char**), void* datos = nullptr) {
    char* mensajeError;
    int resultado = sqlite3_exec(db, consulta.c_str(), callback, datos, &mensajeError);
    if (resultado != SQLITE_OK) {
        cout << "Error en la consulta: " << mensajeError << endl;
        sqlite3_free(mensajeError);
    }
    return resultado;
}

// Función para obtener el último ID insertado en la tabla Usuarios
int obtenerUltimoID(sqlite3* db) {
    int ultimoID = 0;
    string consulta = "SELECT MAX(id) FROM Usuarios";
    ejecutarConsulta(db, consulta, [](void* datos, int numColumnas, char** columnas, char** nombresColumnas) {
        if (columnas[0]) {
            *((int*)datos) = stoi(columnas[0]);
        }
        return 0;
    }, &ultimoID);
    return ultimoID;
}

// Función para insertar un nuevo usuario en la tabla Usuarios
void altaUsuario(sqlite3* db) {
    Usuario nuevoUsuario;
    cout << "Introduce el ID del usuario: ";
    cin >> nuevoUsuario.id;
    cin.ignore(); // Ignorar el salto de línea pendiente en el buffer de entrada
    cout << "Introduce el nombre del usuario: ";
    getline(cin, nuevoUsuario.nombre);
    cout << "Introduce el apellido del usuario: ";
    getline(cin, nuevoUsuario.apellido);
    cout << "Introduce el email del usuario: ";
    getline(cin, nuevoUsuario.email);

    string consulta = "INSERT INTO Usuarios (id, nombre, apellido, email) VALUES (" +
        to_string(nuevoUsuario.id) + ", '" + nuevoUsuario.nombre + "', '" +
        nuevoUsuario.apellido + "', '" + nuevoUsuario.email + "')";
    if (ejecutarConsulta(db, consulta, nullptr) == SQLITE_OK) {
        cout << "Usuario agregado correctamente" << endl;
    }
}

// Función para eliminar un usuario de la tabla Usuarios
void bajaUsuario(sqlite3* db) {
    int idUsuario;
    cout << "Introduce el ID del usuario que quieres eliminar: ";
    cin >> idUsuario;

    string consulta = "DELETE FROM Usuarios WHERE id = " + to_string(idUsuario);
    if (ejecutarConsulta(db, consulta, nullptr) == SQLITE_OK) {
        if (sqlite3_changes(db) > 0) {
            cout << "Usuario eliminado correctamente" << endl;
        } else {
            cout << "No se encontró ningún usuario con ese ID" << endl;
        }
    }
}

// Función para modificar los datos de un usuario en la tabla
void modificarUsuario(sqlite3* db) {
    int idUsuario;
    cout << "Introduce el ID del usuario que quieres modificar: ";
    cin >> idUsuario;
    cin.ignore(); // Ignorar el salto de línea pendiente en el buffer de entrada

    string consulta = "SELECT * FROM Usuarios WHERE id = " + to_string(idUsuario);
    Usuario usuarioExistente;
    ejecutarConsulta(db, consulta, [](void* datos, int numColumnas, char** columnas, char** nombresColumnas) {
        if (columnas[0]) {
            ((Usuario*)datos)->id = stoi(columnas[0]);
        }
        if (columnas[1]) {
            ((Usuario*)datos)->nombre = columnas[1];
        }
        if (columnas[2]) {
            ((Usuario*)datos)->apellido = columnas[2];
        }
        if (columnas[3]) {
            ((Usuario*)datos)->email = columnas[3];
        }
        return 0;
    }, &usuarioExistente);

    if (usuarioExistente.id == 0) {
        cout << "No se encontró ningún usuario con ese ID" << endl;
        return;
    }

    Usuario nuevoUsuario;
    nuevoUsuario.id = usuarioExistente.id;
    cout << "Introduce el nuevo nombre del usuario (actual: " << usuarioExistente.nombre << "): ";
    getline(cin, nuevoUsuario.nombre);
    cout << "Introduce el nuevo apellido del usuario (actual: " << usuarioExistente.apellido << "): ";
    getline(cin, nuevoUsuario.apellido);
    cout << "Introduce el nuevo email del usuario (actual: " << usuarioExistente.email << "): ";
    getline(cin, nuevoUsuario.email);

    if (nuevoUsuario.nombre.empty()) {
        nuevoUsuario.nombre = usuarioExistente.nombre;
    }
    if (nuevoUsuario.apellido.empty()) {
        nuevoUsuario.apellido = usuarioExistente.apellido;
    }
    if (nuevoUsuario.email.empty()) {
        nuevoUsuario.email = usuarioExistente.email;
    }

    consulta = "UPDATE Usuarios SET nombre = '" + nuevoUsuario.nombre +
        "', apellido = '" + nuevoUsuario.apellido +
        "', email = '" + nuevoUsuario.email +
        "' WHERE id = " + to_string(idUsuario);
    if (ejecutarConsulta(db, consulta, nullptr) == SQLITE_OK) {
        if (sqlite3_changes(db) > 0) {
            cout << "Usuario modificado correctamente" << endl;
        } else {
            cout << "No se encontró ningún usuario con ese ID" << endl;
        }
    }
}

// Función para mostrar todos los usuarios en la tabla Usuarios
int callbackMostrarUsuarios(void* datos, int numColumnas, char** columnas, char** nombresColumnas) {
    Usuario usuario;
    usuario.id = stoi(columnas[0]);
    usuario.nombre = columnas[1];
    usuario.apellido = columnas[2];
    usuario.email = columnas[3];
    imprimirUsuario(usuario);
    cout << endl;
    return 0;
}

void consultarUsuarios(sqlite3* db) {
    string consulta = "SELECT * FROM Usuarios";
    ejecutarConsulta(db, consulta, callbackMostrarUsuarios);
}

// Función para inicializar la base de datos
sqlite3* inicializarBD() {
    sqlite3* db;
    int resultado = sqlite3_open("usuarios.db", &db);
    if (resultado != SQLITE_OK) {
        cout << "Error al abrir la base de datos" << endl;
        return nullptr;
    }

    string crearTabla = "CREATE TABLE IF NOT EXISTS Usuarios (id INTEGER PRIMARY KEY, nombre TEXT, apellido TEXT, email TEXT);";
    if (ejecutarConsulta(db, crearTabla, nullptr) != SQLITE_OK) {
        cout << "Error al crear la tabla Usuarios" << endl;
        sqlite3_close(db);
        return nullptr;
    }

    return db;
}

int main() {
    sqlite3* db = inicializarBD();
    if (db == nullptr) {
        return 1;
    }

    int opcion;
    do {
        cout << "Menú de opciones:" << endl;
        cout << "1. Alta de usuario" << endl;
        cout << "2. Baja de usuario" << endl;
        cout << "3. Modificación de usuario" << endl;
        cout << "4. Consulta de usuarios" << endl;
        cout << "5. Salir" << endl;
        cout << "Selecciona una opción: ";
        cin >> opcion;
        cin.ignore(); // Ignorar el salto de línea pendiente en el buffer de entrada

        switch (opcion) {
            case 1:
                altaUsuario(db);
                break;
            case 2:
                bajaUsuario(db);
                break;
            case 3:
                modificarUsuario(db);
                break;
            case 4:
                consultarUsuarios(db);
                break;
            case 5:
                break;
            default:
                cout << "Opción no válida" << endl;
                break;
        }
    } while (opcion != 5);

    sqlite3_close(db);

    return 0;
}
