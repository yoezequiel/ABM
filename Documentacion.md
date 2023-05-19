# Documentación del Código en C++

El siguiente documento proporciona una explicación detallada del código en C++ que implementa un programa de administración de usuarios utilizando una base de datos SQLite. A continuación, se describe cada sección y función del código.

## Estructuras de Datos

### `Usuario`

La estructura `Usuario` representa los datos de un usuario y tiene los siguientes campos:

- `id`: Identificador del usuario.
- `nombre`: Nombre del usuario.
- `apellido`: Apellido del usuario.
- `email`: Correo electrónico del usuario.

## Funciones de Utilidad

### `imprimirUsuario(const Usuario& usuario)`

Esta función imprime en la consola los datos de un usuario. Recibe como parámetro un objeto `Usuario` y muestra los siguientes campos:

- ID del usuario.
- Nombre del usuario.
- Apellido del usuario.
- Correo electrónico del usuario.

### `ejecutarConsulta(sqlite3* db, const string& consulta, int(*callback)(void*, int, char**, char**), void* datos = nullptr)`

Esta función ejecuta una consulta SQL en la base de datos SQLite. Recibe los siguientes parámetros:

- `db`: Puntero a la base de datos SQLite.
- `consulta`: Cadena de consulta SQL a ejecutar.
- `callback`: Puntero a una función de devolución de llamada opcional que se invoca para cada fila de resultados.
- `datos` (opcional): Puntero a datos adicionales que se pasan a la función de devolución de llamada.

## Funciones Principales

### `obtenerNuevoID(sqlite3* db)`

Esta función obtiene el próximo ID disponible para un nuevo usuario en la base de datos SQLite. Toma como parámetro un puntero a la base de datos y devuelve una cadena de texto con el nuevo ID.

### `altaUsuario(sqlite3* db)`

Esta función permite agregar un nuevo usuario a la base de datos SQLite. Solicita al usuario que ingrese los datos del nuevo usuario, incluyendo nombre, apellido y correo electrónico.

### `bajaUsuario(sqlite3* db)`

Esta función permite eliminar un usuario de la base de datos SQLite. Solicita al usuario que ingrese el ID del usuario que desea eliminar.

### `modificarUsuario(sqlite3* db)`

Esta función permite modificar los datos de un usuario existente en la base de datos SQLite. Solicita al usuario que ingrese el ID del usuario que desea modificar y los nuevos datos a modificar, como nombre, apellido y correo electrónico.

### `callbackMostrarUsuarios(void* datos, int numColumnas, char** columnas, char** nombresColumnas)`

Esta función de devolución de llamada se utiliza para mostrar los usuarios en la consulta de usuarios. Recibe los siguientes parámetros:

- `datos`: Puntero a datos adicionales.
- `numColumnas`: Número de columnas en la fila de resultados.
- `columnas`: Puntero a un array de cadenas que representan los valores de las columnas.
- `nombresColumnas`: Puntero a un array de cadenas que representan los nombres de las columnas.

### `consultarUsuarios(sqlite3* db)`

Esta función consulta y muestra todos los usuarios en la base de datos SQLite.

### `inicializarBD()`

Esta función inicializa la conexión con la base de datos SQLite y crea la tabla "Usuarios" si no existe.

## Función Principal

### `main()`

La función principal del programa se encarga de ejecutar el menú de opciones y gestionar

 las diferentes acciones del programa, como agregar usuario, eliminar usuario, modificar usuario y consultar usuarios. Utiliza un bucle `do-while` para mostrar el menú y ejecutar la opción seleccionada por el usuario hasta que se seleccione la opción de salir.

## Conclusiones

En resumen, el código proporciona un programa de administración de usuarios que utiliza una base de datos SQLite para almacenar y manipular los datos de los usuarios. Proporciona opciones para agregar, eliminar, modificar y consultar usuarios. Cada función y sección del código está diseñada para cumplir una tarea específica y contribuir al funcionamiento general del programa.