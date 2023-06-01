#include <string.h>
#include <stdlib.h>

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

int djb2_hash(const char *str)
{
	int hash = 5381;
	char c;
	while ((c = (char)*str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

char *strdup(const char *str)
{
	size_t length = strlen(str);
	char *copy = (char *)malloc(length + 1);
	if (copy != NULL) {
		strcpy(copy, str);
	}
	return copy;
}

struct nodo *nodo_crear(const char *clave, void *elemento)
{
	char *copia_clave = strdup(clave);
	struct nodo *nodo = calloc(1, sizeof(struct nodo));
	if (!nodo)
		return NULL;
	nodo->clave = copia_clave;
	nodo->elemento = elemento;
	return nodo;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;

	char *copia_clave = strdup(clave);

	int posicion = djb2_hash(copia_clave) % hash->capacidad;

	struct nodo *nodo = nodo_crear(clave, elemento);
	if (hash->vector[posicion] == NULL) {
		hash->vector[posicion] = nodo;
		return hash;
	}

	struct nodo *actual = hash->vector[posicion];
	while (actual) {
		if (strcmp(clave, actual->clave) == 0) {
			free(nodo);
			actual->elemento = elemento;
			return hash;
		}
		actual = actual->siguiente;
	}

	nodo->siguiente = hash->vector[posicion];
	hash->vector[posicion] = nodo;

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
	if (!hash)
		return NULL;

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

void hash_destruir_vector(hash_t *hash)
{
	if (!hash->vector || hash->cantidad == 0) {
		free(hash->vector);
		return;
	}
	struct nodo *actual = hash->vector[0];
	while (actual) {
		struct nodo *sig = actual->siguiente;
		free(actual);
		actual = sig;
	}
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_vector(hash);
	free(hash);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

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
