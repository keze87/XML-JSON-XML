#ifndef __LISTASIMPLE_h__
	#define __LISTASIMPLE_h__

	#include <stdlib.h>
	#include <memory.h>
	
	#if !defined(NULL)
		#define NULL 0
	#endif
	
	#if !defined(FALSE)
		#define FALSE 0
	#endif
	
	#if !defined(TRUE)
		#define TRUE 1
	#endif
	
	/**
	 * Movimientos que va a manejar la estructura. Son de conocimiento público,
	 * pero sólo deberían usarse para el manejo puntual de esta estructura.
	 */
	typedef enum
	{
		L_Primero,
		L_Siguiente,
		L_Anterior
	} TMovimiento_Ls;
	
	/**
	 * Estructura auxiliar de la lista simple. Es privada y no debe usarse bajo
	 * ningún concepto en la aplicación.
	 */
	typedef struct TNodoListaSimple
	{
		void* Elem;
		struct TNodoListaSimple *Siguiente;
	} TNodoListaSimple;
	
	/**
	 * Estructura cabecera, este es el tipo que se deberá instanciar, aunque
	 * nunca acceder a sus campos.
	 */
	typedef struct
	{
		TNodoListaSimple *Primero, *Corriente;
		int TamanioDato;
	} TListaSimple;
	
	/**
	 * L_CREAR
	 * Pre: Ls no fue creada.
	 * Post: Ls creada y vacia
	 */
	void L_Crear(TListaSimple *pLs, int TamanioDato);
	
	/**
	 * L_VACIAR
	 * Pre: Ls creada.
	 * Post: Ls vacia.
	 */
	void L_Vaciar(TListaSimple *pLs);
	
	/**
	 * L_VACIA
	 * Pre: Ls creada.
	 * Post: Si Ls tiene elementos devuelve FALSE sino TRUE.
	 */
	int L_Vacia(TListaSimple Ls);
	
	/**
	 * L_ELEM_CTE
	 * Pre: Ls creada y no vacia.
	 * Post: Se devuelve en E el elemento Corriente de la lista.
	 */
	void L_Elem_Cte(TListaSimple Ls, void* pE);
	
	/**
	 * L_MOVER_CTE
	 * Pre: Ls creada y no vacia.
	 * Post: Si Ls esta vacia, devuelve FALSE. Sino:
	 * Si M = L_Primero, el nuevo elemento Corriente es el Primero. Devuelve TRUE
	 * Si M = L_Siguiente, el nuevo elemento Corriente es el Siguiente al
	 * Anterior. Si estaba en el ultimo elemento, devuelve FALSE, sino TRUE.
	 * Si M = L_Anterior, devuelve FALSE.
	 */
	int L_Mover_Cte(TListaSimple *pLs, TMovimiento_Ls M);
	
	/**
	 * L_BORRAR_CTE
	 * Pre: Ls creada y no vacia.
	 * Post: Se elimino el elemento Corriente, El nuevo elemento es el Siguiente o
	 * el Anterior si el Corriente era el ultimo elemento.
	 */
	void L_Borrar_Cte(TListaSimple *pLs);
	
	/**
	 * L_INSERTAR_CTE
	 * Pre: Ls creada.
	 * Post: E se agrego a la lista y es el actual elemento Corriente.
	 * Si M=L_Primero: se inserto como Primero de la lista.
	 * Si M=L_Siguiente: se inserto despues del elemento Corriente.
	 * Si M=L_Anterior: se inserto antes del elemento Corriente.
	 * Si pudo insertar el elemento devuelve TRUE, sino FALSE.
	 */
	int L_Insertar_Cte(TListaSimple *pLs, TMovimiento_Ls M, void* E);

	/* L_ELEM_CTE
	 * Pre: Ls creada y no vacía.
	 * Post: Se modifica el elemento corriente para que pase a tener los datos indicados en pE.*/
	void L_Modificar_Cte(TListaSimple Ls, void *pE);

#endif

