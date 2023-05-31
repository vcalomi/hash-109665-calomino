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
int main()
{
	pa2m_nuevo_grupo("\nPruebas de Creacion");
	pruebas_creacion();

	return pa2m_mostrar_reporte();
}