#include <string.h>
#include <stdlib.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MINIMA 3

typedef struct par {
	char *clave;
	void *elemento;
	struct par *siguiente;
} par_t;

struct hash {
	int capacidad;
	int cantidad;
	par_t **pares;
};

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash)
		return NULL;
	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;
	hash->capacidad = (int)capacidad;
	hash->pares = calloc(1, sizeof(par_t *) * capacidad);
	if (!hash->pares)
		return NULL;
	return hash;
}

char *strdup(const char *clave_original)
{
	size_t tamanio = strlen(clave_original) + 1;
	char *copia_clave = calloc(1, tamanio);
	if (!copia_clave) {
		free(copia_clave);
		return NULL;
	}
	strcpy(copia_clave, clave_original);
	return copia_clave;
}

unsigned int funcion_hash(const char *str)
{
	unsigned int hash = 2166136261u;
	const unsigned int prime = 16777619u;

	while (*str != '\0') {
		hash ^= (unsigned int)(*str);
		hash *= prime;
		++str;
	}

	return hash;
}

par_t *par_crear(char *clave, void *elemento)
{
	par_t *par = malloc(sizeof(par_t));
	if (!par)
		return NULL;
	par->clave = clave;
	par->elemento = elemento;
	par->siguiente = NULL;
	return par;
}

void reinsertar_par(par_t **vector_nuevo, par_t *par, int capacidad)
{
	if (vector_nuevo == NULL || par == NULL)
		return;

	par->siguiente = NULL;

	int resultado = abs((int)funcion_hash(par->clave));
	int posicion = resultado % capacidad;

	if (vector_nuevo[posicion] == NULL) {
		vector_nuevo[posicion] = par;
		return;
	}

	par->siguiente = vector_nuevo[posicion];
	vector_nuevo[posicion] = par;

	return;
}

void guardar_en_vector(par_t **vector_viejo, par_t **vector_pares,
		       size_t *indice, int capacidad)
{
	if (vector_viejo == NULL || vector_pares == NULL || indice == NULL)
		return;

	for (int posicion = 0; posicion < capacidad; posicion++) {
		par_t *actual = vector_viejo[posicion];
		while (actual) {
			vector_pares[(*indice)++] = actual;
			actual = actual->siguiente;
		}
	}
}

void rehash(hash_t *hash)
{
	int capacidad_vieja = hash->capacidad;
	int cantidad_vieja = hash->cantidad;
	par_t **vector_viejo = hash->pares;
	hash->pares = calloc(1, sizeof(par_t) *
					(long unsigned int)hash->capacidad * 2);
	if (!hash->pares)
		return;

	hash->capacidad *= 2;
	hash->cantidad = 0;

	par_t **vector_pares =
		calloc(1, sizeof(par_t) * (long unsigned int)cantidad_vieja);
	if (!vector_pares)
		return;
	size_t indice = 0;
	guardar_en_vector(vector_viejo, vector_pares, &indice, capacidad_vieja);

	for (size_t i = 0; i < cantidad_vieja; i++) {
		reinsertar_par(hash->pares, vector_pares[i], hash->capacidad);
		hash->cantidad++;
	}

	free(vector_viejo);
	free(vector_pares);
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (hash == NULL || clave == NULL || hash->pares == NULL)
		return NULL;

	float factor_de_carga = (float)hash->cantidad / (float)hash->capacidad;
	if (factor_de_carga > FACTOR_CARGA_MAXIMO) {
		rehash(hash);
	}

	int resultado = abs((int)funcion_hash(clave));
	int posicion = resultado % hash->capacidad;

	if (hash->pares[posicion] == NULL) {
		char *copia_clave = strdup(clave);
		par_t *nodo = par_crear(copia_clave, elemento);
		hash->pares[posicion] = nodo;
		hash->cantidad++;
		if (anterior != NULL)
			*anterior = NULL;
		return hash;
	}

	par_t *actual = hash->pares[posicion];
	while (actual) {
		if (strcmp(clave, actual->clave) == 0) {
			if (anterior != NULL)
				*anterior = actual->elemento;
			actual->elemento = elemento;
			return hash;
		}
		actual = actual->siguiente;
	}
	char *copia_clave = strdup(clave);
	par_t *nodo = par_crear(copia_clave, elemento);
	nodo->siguiente = hash->pares[posicion];
	hash->pares[posicion] = nodo;
	hash->cantidad++;
	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (hash == NULL || clave == NULL)
		return NULL;

	int resultado = abs((int)funcion_hash(clave));
	int posicion = resultado % hash->capacidad;

	if (hash->pares[posicion] != NULL &&
	    strcmp(clave, hash->pares[posicion]->clave) == 0) {
		void *elemento = hash->pares[posicion]->elemento;
		par_t *a_eliminar = hash->pares[posicion];
		hash->pares[posicion] = a_eliminar->siguiente;
		hash->cantidad--;
		free(a_eliminar->clave);
		free(a_eliminar);
		return elemento;
	}

	par_t *actual = hash->pares[posicion];
	par_t *anterior = NULL;
	while (actual != NULL) {
		if (strcmp(clave, actual->clave) == 0) {
			void *elemento = actual->elemento;
			hash->cantidad--;
			anterior->siguiente = actual->siguiente;
			free(actual->clave);
			free(actual);
			return elemento;
		}
		anterior = actual;
		actual = actual->siguiente;
	}
	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (hash == NULL || clave == NULL || hash->pares == NULL)
		return NULL;

	int resultado = abs((int)funcion_hash(clave));
	int posicion = resultado % hash->capacidad;

	if (hash->pares[posicion] == NULL)
		return NULL;

	par_t *actual = hash->pares[posicion];
	while (actual) {
		if (strcmp(clave, actual->clave) == 0) {
			return actual->elemento;
		}
		actual = actual->siguiente;
	}
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (hash == NULL || clave == NULL)
		return false;

	int resultado = abs((int)funcion_hash(clave));
	int posicion = resultado % hash->capacidad;

	if (hash->pares[posicion] == NULL)
		return false;

	par_t *actual = hash->pares[posicion];
	while (actual) {
		if (strcmp(clave, actual->clave) == 0) {
			return true;
		}
		actual = actual->siguiente;
	}
	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (hash == NULL)
		return 0;
	return (size_t)hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	if (hash == NULL)
		return;
	for (size_t i = 0; i < hash->capacidad; i++) {
		par_t *actual = hash->pares[i];
		while (actual != NULL) {
			par_t *siguiente = actual->siguiente;
			free(actual->clave);
			free(actual);
			actual = siguiente;
		}
	}
	free(hash->pares);
	free(hash);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (hash == NULL)
		return;

	for (size_t i = 0; i < hash->capacidad; i++) {
		par_t *actual = hash->pares[i];
		while (actual) {
			par_t *siguiente = actual->siguiente;
			free(actual->clave);
			if (destructor != NULL)
				destructor(actual->elemento);
			free(actual);
			actual = siguiente;
		}
	}
	free(hash->pares);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash || !f)
		return n;

	for (size_t i = 0; i < hash->capacidad; i++) {
		par_t *actual = hash->pares[i];
		while (actual) {
			if (f(actual->clave, actual->elemento, aux) == true)
				n++;
			else {
				n++;
				return n;
			}
			actual = actual->siguiente;
		}
	}
	return n;
}
