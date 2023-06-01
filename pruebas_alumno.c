#include "pa2m.h"
#include "src/hash.h"

void pruebas_creacion()
{
	size_t capacidad_1 = 4;
	hash_t *hash_1 = hash_crear(capacidad_1);
	pa2m_afirmar(hash_1 != NULL,
		     "Se puede rear un hash con capacidad mayor a 3");
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

	hash_destruir(hash);

	size_t capacidad = 20;
	hash_t *hash_2 = hash_crear(capacidad);

	int valores[5] = { 1, 2, 3, 4, 5 };
	const char *claves[5] = { "A", "B", "C", "D", "E" };
	for (int i = 0; i < 6; i++) {
		pa2m_afirmar(hash_insertar(hash_2, claves[i], valores[i],
					   anterior) == hash,
			     "Se pueden insertar varios elementos");
	};

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
int main()
{
	pa2m_nuevo_grupo("\nPruebas de Creacion");
	pruebas_creacion();

	pa2m_nuevo_grupo("\nPruebas de Insercion");
	pruebas_insercion();

	pa2m_nuevo_grupo("\nPruebas de Eliminacion");
	pruebas_eliminacion();

	return pa2m_mostrar_reporte();
}