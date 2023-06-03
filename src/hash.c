#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MINIMA 3

struct nodo {
	char *clave;
	void *elemento;
	struct nodo *siguiente;
};

struct hash {
	int capacidad;
	int cantidad;
	struct nodo **vector;
};

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash)
		return NULL;
	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;
	hash->capacidad = (int)capacidad;
	hash->vector = calloc(1, sizeof(struct nodo *) * capacidad);
	if (!hash->vector)
		return NULL;
	return hash;
}

char *strdup(const char *clave_original)
{
	size_t tamanio = strlen(clave_original) + 1;
	char *copia_clave = malloc(tamanio);
	if (!copia_clave)
		return NULL;
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

struct nodo *nodo_crear(char *clave, void *elemento)
{
	struct nodo *nodo = malloc(sizeof(struct nodo));
	if (!nodo)
		return NULL;
	nodo->clave = clave;
	nodo->elemento = elemento;
	nodo->siguiente = NULL;
	return nodo;
}

void reinsertar_par(struct nodo **vector_nuevo, struct nodo *par, int capacidad)
{
	if (vector_nuevo == NULL || par == NULL)
		return;

	int resultado = abs((int)funcion_hash(par->clave));
	int posicion = resultado % capacidad;

	if (vector_nuevo[posicion] == NULL) {
		vector_nuevo[posicion] = par;
		return;
	}

	struct nodo *actual = vector_nuevo[posicion];
	while (actual) {
		if (strcmp(par->clave, actual->clave) == 0) {
			actual->elemento = par->elemento;
			return;
		}
		actual = actual->siguiente;
	}

	par->siguiente = vector_nuevo[posicion];
	vector_nuevo[posicion] = par;

	return;
}

// implementar un insertar propio que reciba el par
hash_t *rehash(hash_t *hash)
{
	int capacidad_vieja = hash->capacidad;
	struct nodo **vector_viejo = hash->vector;
	hash->vector =
		calloc(1, sizeof(struct nodo) *
				  (long unsigned int)hash->capacidad * 2);
	if (!hash->vector) {
		free(vector_viejo);
		return NULL;
	}
	hash->capacidad *= 2;

	for (int posicion = 0; posicion < capacidad_vieja; posicion++) {
		struct nodo *actual = vector_viejo[posicion];
		while (actual) {
			reinsertar_par(hash->vector, actual, hash->capacidad);
			actual = actual->siguiente;
		}
	}
	free(vector_viejo);
	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave || hash->vector == NULL)
		return NULL;

	float factor_de_carga = (float)hash->cantidad / (float)hash->capacidad;
	if (factor_de_carga > FACTOR_CARGA_MAXIMO) {
		hash = rehash(hash);
	}

	//fijarse la copia esta despues, puede dar error
	char *copia_clave = strdup(clave);

	int resultado = abs((int)funcion_hash(clave));
	int posicion = resultado % hash->capacidad;

	if (hash->vector[posicion] == NULL) {
		struct nodo *nodo = nodo_crear(copia_clave, elemento);
		hash->vector[posicion] = nodo;
		hash->cantidad++;
		return hash;
	}

	struct nodo *actual = hash->vector[posicion];
	while (actual) {
		if (strcmp(clave, actual->clave) == 0) {
			//*anterior = actual->elemento; // si, pero hay un error
			actual->elemento = elemento;
			free(copia_clave);
			return hash;
		}
		actual = actual->siguiente;
	}

	struct nodo *nodo = nodo_crear(copia_clave, elemento);
	nodo->siguiente = hash->vector[posicion];
	hash->vector[posicion] = nodo;
	hash->cantidad++;

	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash)
		return NULL;

	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	char *copia_clave = strdup(clave);

	int resultado = abs((int)funcion_hash(copia_clave));
	int posicion = resultado % hash->capacidad;

	if (hash->vector[posicion] == NULL) {
		free(copia_clave);
		return NULL;
	}

	struct nodo *actual = hash->vector[posicion];
	while (actual) {
		if (strcmp(copia_clave, actual->clave) == 0) {
			free(copia_clave);
			return actual->elemento;
		}
		actual = actual->siguiente;
	}
	free(copia_clave);
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash)
		return NULL;

	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return (size_t)hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	if (!hash)
		return;
	for (size_t i = 0; i < hash->capacidad; i++) {
		struct nodo *actual = hash->vector[i];
		while (actual != NULL) {
			struct nodo *siguiente = actual->siguiente;
			free(actual->clave);
			free(actual);
			actual = siguiente;
		}
	}
	free(hash->vector);
	free(hash);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash || !destructor)
		return;

	if (hash->vector != NULL) {
		for (size_t i = 0; i < hash->cantidad; i++) {
			struct nodo *actual = hash->vector[i];
			while (actual) {
				struct nodo *siguiente = actual->siguiente;
				free(actual->clave);
				destructor(actual->elemento);
				free(actual);
				actual = siguiente;
			}
		}
	}
	free(hash->vector);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash || !f)
		return n;

	return n;
}
