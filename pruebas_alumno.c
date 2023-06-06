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
	hash_destruir(hash_2);
}

void pruebas_insercion()
{
	const char *clave = "K";
	int elemento = 10;
	void *anterior;

	pa2m_afirmar(hash_insertar(NULL, clave, &elemento, &anterior) == NULL,
		     "Insertar en un hash nulo devuelve NULL");

	size_t capacidad = 8;
	hash_t *hash = hash_crear(capacidad);
	pa2m_afirmar(
		hash_insertar(hash, NULL, &elemento, &anterior) == NULL,
		"Insertar en un hash valido con clave nula es igual a NULL");

	pa2m_afirmar(
		hash_insertar(hash, clave, NULL, &anterior) == hash,
		"Se puede insertar un elemento NULL en un hash valido con clave valida");

	pa2m_afirmar(
		hash_insertar(hash, clave, &elemento, &anterior) == hash,
		"Se puede insertar en un hash valido con clave valida y un elemento no nulo");
	pa2m_afirmar(hash_cantidad(hash) == 1,
		     "Al insertar aumenta la cantidad de elementos en el hash");

	hash_destruir(hash);

	size_t capacidad_2 = 20;
	hash_t *hash_2 = hash_crear(capacidad_2);

	int e1 = 1;
	int e2 = 2;
	int e3 = 3;
	int e4 = 4;
	int e5 = 5;

	hash_insertar(hash_2, "A", &e1, &anterior);
	hash_insertar(hash_2, "B", &e2, &anterior);
	hash_insertar(hash_2, "C", &e3, &anterior);
	hash_insertar(hash_2, "D", &e4, &anterior);
	hash_insertar(hash_2, "E", &e5, &anterior);

	pa2m_afirmar(
		hash_cantidad(hash_2) == 5,
		"Pude insertar varios elementos y tengo la cantidad correcta");

	hash_destruir(hash_2);

	size_t capacidad_3 = 10;
	hash_t *hash_3 = hash_crear(capacidad_3);

	int e6 = 6;
	int e7 = 7;
	int e8 = 8;
	int e9 = 9;
	int e10 = 10;

	hash_insertar(hash_3, "A", &e1, &anterior);
	hash_insertar(hash_3, "B", &e2, &anterior);
	hash_insertar(hash_3, "C", &e3, &anterior);
	hash_insertar(hash_3, "D", &e4, &anterior);
	hash_insertar(hash_3, "E", &e5, &anterior);
	hash_insertar(hash_3, "F", &e6, &anterior);
	hash_insertar(hash_3, "G", &e7, &anterior);
	hash_insertar(hash_3, "H", &e8, &anterior);
	hash_insertar(hash_3, "I", &e9, &anterior);
	hash_insertar(hash_3, "J", &e10, &anterior);

	pa2m_afirmar(hash_insertar(hash_3, "X", &e1, NULL) == hash_3,
		     "Inserto elementos suficientes para causar un rehash");

	pa2m_afirmar(
		hash_cantidad(hash_3) == 11,
		"Luego de insertar varios elementos y hacer rehash tengo la cantidad correcta");

	pa2m_afirmar(hash_insertar(hash_3, "C", &e10, &anterior) == hash_3,
		     "Actualizo un elemento");
	pa2m_afirmar(*(int *)anterior == e3,
		     "En anterior queda el elemento reemplazado");

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

	void *anterior;
	int valores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const char *claves[10] = { "A", "B", "C", "D", "E",
				   "F", "G", "H", "I", "J" };
	for (int i = 0; i < 10; i++)
		hash_insertar(hash, claves[i], &valores[i], &anterior);

	pa2m_afirmar(hash_quitar(hash, "k") == NULL,
		     "Quitar un elemento que no existe devuelve NULL");
	pa2m_afirmar(hash_quitar(hash, "E") == &valores[4],
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

	int elemento = 10;

	void *anterior;
	hash_insertar(hash, "k", &elemento, &anterior);

	pa2m_afirmar(hash_obtener(hash, NULL) == NULL,
		     "Obtener con una clave NULL devuelve NULL");
	pa2m_afirmar(
		hash_obtener(hash, "l") == NULL,
		"Obtener un elemento que no existe en el hash devuelve NULL");
	pa2m_afirmar(
		hash_obtener(hash, "k") == &elemento,
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

	void *anterior;
	int valores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const char *claves[10] = { "A", "B", "C", "D", "E",
				   "F", "G", "H", "I", "J" };
	for (int i = 0; i < 10; i++)
		hash_insertar(hash, claves[i], &valores[i], &anterior);

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
	return true;
}

void pruebas_iterador_interno()
{
	int aux = 5;
	pa2m_afirmar(hash_con_cada_clave(NULL, funcion_para_iterar_completo,
					 &aux) == 0,
		     "Iterar con un hash nulo devuelve 0");

	size_t capacidad = 20;
	hash_t *hash = hash_crear(capacidad);

	pa2m_afirmar(hash_con_cada_clave(hash, NULL, &aux) == 0,
		     "Iterar un hash valido con funcion nula devuelve 0");

	void *anterior;
	int e1 = 5;
	int e2 = 26;
	hash_insertar(hash, "B", &e1, &anterior);
	hash_insertar(hash, "K", &e2, &anterior);

	pa2m_afirmar(hash_con_cada_clave(hash, funcion_para_iterar_completo,
					 NULL) == 2,
		     "Se puede iterar el hash con un auxiliar nulo");

	hash_quitar(hash, "B");
	hash_quitar(hash, "K");

	void *anterior_2;
	int valores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const char *claves[10] = { "A", "B", "C", "D", "E",
				   "F", "G", "H", "I", "J" };
	for (int i = 0; i < 10; i++)
		hash_insertar(hash, claves[i], &valores[i], &anterior_2);

	pa2m_afirmar(hash_con_cada_clave(hash, funcion_para_iterar_completo,
					 &aux) == 10,
		     "Iterar el hash completo devuelve la cantidad correcta");
	pa2m_afirmar(
		hash_con_cada_clave(hash, funcion_para_no_iterar_completo,
				    &aux) == 4,
		"Puedo iterar el hash hasta cierta parte y devuelve la cantidad correcta");
	hash_destruir(hash);
}
int main()
{
	pa2m_nuevo_grupo("\nPruebas de Creacion");
	pruebas_creacion();
	pa2m_mostrar_reporte();

	pa2m_nuevo_grupo("\nPruebas de Insercion");
	pruebas_insercion();
	pa2m_mostrar_reporte();

	pa2m_nuevo_grupo("\nPruebas de Eliminacion");
	pruebas_eliminacion();
	pa2m_mostrar_reporte();

	pa2m_nuevo_grupo("\nPruebas de Obtencion");
	pruebas_obtencion();
	pa2m_mostrar_reporte();

	pa2m_nuevo_grupo("\nPruebas de Contener");
	pruebas_contener();
	pa2m_mostrar_reporte();

	pa2m_nuevo_grupo("\nPruebas de Iterador interno");
	pruebas_iterador_interno();
	pa2m_mostrar_reporte();

	return pa2m_mostrar_reporte();
}