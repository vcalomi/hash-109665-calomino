#include "pa2m.h"
#include "src/hash.h"
#include <string.h>

void pruebas_creacion()
{
	size_t capacidad_1 = 4;
	hash_t *hash_1 = hash_crear(capacidad_1);
	pa2m_afirmar(hash_1 != NULL,
		     "Se puede rear un hash con capacidad mayor a 3");
	pa2m_afirmar(hash_cantidad(hash_1) == 0, "El hash se inicializa vacio");
	capacidad_1 = 2;
	hash_t *hash_2 = hash_crear(capacidad_1);
	pa2m_afirmar(hash_2 != NULL,
		     "Se puede rear un hash con capacidad menor a 3");
	hash_destruir(hash_1);
	pa2m_afirmar(
		hash_1 == NULL,
		"La funcion destructora libera la memoria del hash correctamente");
	hash_destruir(hash_2);
}

void pruebas_insercion()
{
	const char *clave = "K";
	int *elemento = 10;
	int *anterior;
	pa2m_afirmar(hash_insertar(NULL, clave, elemento, anterior) == NULL,
		     "Insertar en un hash nulo devuelve NULL");

	size_t capacidad = 8;
	hash_t *hash = hash_crear(capacidad);
	pa2m_afirmar(
		hash_insertar(hash, NULL, elemento, anterior) == NULL,
		"Insertar en un hash valido con clave nula es igual a NULL");

	pa2m_afirmar(
		hash_insertar(hash, clave, NULL, anterior) == hash,
		"Se puede insertar un elemento NULL en un hash valido con clave valida");

	pa2m_afirmar(
		hash_insertar(hash, clave, elemento, anterior) == hash,
		"Se puede insertar en un hash valido con clave valida y un elemento no nulo");
	pa2m_afirmar(hash_cantidad(hash) == 1,
		     "Al insertar aumenta la cantidad de elementos en el hash");

	hash_destruir(hash);

	size_t capacidad = 20;
	hash_t *hash_2 = hash_crear(capacidad);

	int valores[5] = { 1, 2, 3, 4, 5 };
	const char *claves[5] = { "A", "B", "C", "D", "E" };
	for (int i = 0; i < 6;
	     i++) { //esto va a mostrar una prueba varias veces
		pa2m_afirmar(hash_insertar(hash_2, claves[i], valores[i],
					   anterior) == hash,
			     "Se pueden insertar varios elementos");
	};

	pa2m_afirmar(
		hash_cantidad(hash_2) == 5,
		"Luego de insertar varios elementos tengo la cantidad correcta");

	hash_destruir(hash_2);

	size_t capacidad = 10;
	hash_t *hash_3 = hash_crear(capacidad);

	int valores_2[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const char *claves_2 = { "A", "B", "C", "D", "E",
				 "F", "G", "H", "I", "J" };

	for (int i = 0; i < 11; i++) {
		pa2m_afirmar(
			hash_insertar(hash_3, claves_2[i], valores_2,
				      anterior) == hash,
			"Inserto valores suficientes para provocar un rehash");
	}

	pa2m_afirmar(
		hash_cantidad(hash_2) == 10,
		"Luego de insertar varios elementos y hacer rehash tengo la cantidad correcta");

	pa2m_afirmar(hash_insertar(hash_3, claves_2[1], valores_2[7],
				   anterior) == hash,
		     "Actualizo un elemento");

	hash_destruir(hash_3);
}

void pruebas_eliminacion()
{
	char *clave = "A";
	pa2m_afirmar(hash_quitar(NULL, clave) == NULL,
		     "Quitar un elemento de un hash nulo devuelve NULL");

	size_t capacidad = 10;
	hash_t *hash = hash_crear(capacidad);

	pa2m_afirmar(
		hash_quitar(hash, NULL) == NULL,
		"Quitar de un hash valido con una clave nula devuelve NULL");

	int *anterior;
	int valores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const char *claves = {
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"
	};
	for (int i = 0; i < 11; i++)
		hash_insertar(hash, claves[i], valores[i], anterior);

	pa2m_afirmar(hash_quitar(hash, "k") == NULL,
		     "Quitar un elemento que no existe devuelve NULL");
	pa2m_afirmar(hash_quitar(hash, "E") == valores[4],
		     "Quitar un elemento que existe devuelve el elemento");
	pa2m_afirmar(hash_quitar(hash, "E") == NULL,
		     "Quitar un elemento que ya saque devuelve NULL");
	hash_destruir(hash);
}

void pruebas_obtencion()
{
	pa2m_afirmar(hash_obtener(NULL, "K") == NULL,
		     "Buscar en un hash nulo devuelve NULL");

	size_t capacidad = 10;
	hash_t *hash = hash_crear(capacidad);

	int *anterior;
	hash_insertar(hash, "k", 10, anterior);

	pa2m_afirmar(hash_obtener(hash, NULL) == NULL,
		     "Obtener con una clave NULL devuelve NULL");
	pa2m_afirmar(
		hash_obtener(hash, "l") == NULL,
		"Obtener un elemento que no existe en el hash devuelve NULL");
	pa2m_afirmar(
		hash_obtener(hash, "k") == 10,
		"Obtener con una clave que se encuentra en el hash devuelve el elemento correcto");
	hash_quitar(hash, "k");
	pa2m_afirmar(hash_obtener(hash, "k") == NULL,
		     "Obtener con una clave que se quito devuelve NULL");
	hash_destruir(hash);
}

void pruebas_contener()
{
	pa2m_afirmar(
		hash_contiene(NULL, "k") == false,
		"Fijarse si una clave esta en un hash nulo devuelve falso");

	size_t capacidad = 10;
	hash_t *hash = hash_crear(capacidad);

	pa2m_afirmar(
		hash_contiene(hash, NULL) == false,
		"Fijarse si una clave nula esta en el hash devuelve falso");

	int *anterior;
	int valores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const char *claves = {
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"
	};
	for (int i = 0; i < 11; i++)
		hash_insertar(hash, claves[i], valores[i], anterior);

	pa2m_afirmar(
		hash_contiene(hash, "E") == true,
		"Fijarse si una clave valida que inserte esta en el hash devuelve true");
	pa2m_afirmar(
		hash_contiene(hash, "k") == false,
		"Fijarse si un hash contiene una clave valida que no esta en el hash devuelve false");
	hash_quitar(hash, "C");
	pa2m_afirmar(
		hash_contiene(hash, "C") == false,
		"Fijarse si el hash contiene una clave valida que quite devuelve falso");
	hash_destruir(hash);
}

bool funcion_para_iterar_completo(const char *clave, void *elemento, void *aux)
{
	return true;
}

bool funcion_para_no_iterar_completo(const char *clave, void *elemento,
				     void *aux)
{
	if (strcmp(clave, "F") == 0)
		return false;
}

void pruebas_iterador_interno()
{
	int *aux = 5;
	pa2m_afirmar(hash_con_cada_clave(NULL, funcion_para_iterar_completo,
					 aux) == 0,
		     "Iterar con un hash nulo devuelve 0");

	size_t capacidad = 10;
	hash_t *hash = hash_crear(capacidad);

	pa2m_afirmar(hash_con_cada_clave(hash, NULL, aux) == 0,
		     "Iterar un hash valido con funcion nula devuelve 0");

	int *anterior;
	hash_insertar(hash, "B", 5, anterior);
	hash_insertar(hash, "K", 26, anterior);

	pa2m_afirmar(hash_con_cada_clave(hash, funcion_para_iterar_completo,
					 NULL) == 2,
		     "Se puede iterar el hash con un auxiliar nulo");

	hash_quitar(hash, "B");
	hash_quitar(hash, "K");

	int *anterior;
	int valores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const char *claves = {
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"
	};
	for (int i = 0; i < 11; i++)
		hash_insertar(hash, claves[i], valores[i], anterior);

	pa2m_afirmar(hash_con_cada_clave(hash, funcion_para_iterar_completo,
					 aux) == 10,
		     "Iterar el hash completo devuelve la cantidad correcta");
	pa2m_afirmar(
		hash_con_cada_clave(hash, funcion_para_no_iterar_completo,
				    aux) == 6,
		"Puedo iterar el hash hasta cierta parte y devuelve la cantidad correcta");
	hash_destruir(hash);
}
int main()
{
	pa2m_nuevo_grupo("\nPruebas de Creacion");
	pruebas_creacion();

	pa2m_nuevo_grupo("\nPruebas de Insercion");
	pruebas_insercion();

	pa2m_nuevo_grupo("\nPruebas de Eliminacion");
	pruebas_eliminacion();

	pa2m_nuevo_grupo("\nPruebas de Obtencion");
	pruebas_obtencion();

	pa2m_nuevo_grupo("\nPruebas de Contener");
	pruebas_contener();

	pa2m_nuevo_grupo("\nPruebas de Iterador interno");
	pruebas_iterador_interno();

	return pa2m_mostrar_reporte();
}