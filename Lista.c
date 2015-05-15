#include "Lista.h"

void L_Crear(TListaSimple *pLs, int TamanioDato)
{
	pLs->Corriente = NULL;
	pLs->Primero = NULL;
	pLs->TamanioDato = TamanioDato;
}

void L_Vaciar(TListaSimple *pLs)
{
	TNodoListaSimple *pNodo, *Siguiente;
	for(pNodo = pLs->Primero;(pNodo);pNodo=Siguiente)
	{
		Siguiente = pNodo->Siguiente;
		free(pNodo->Elem);
		free(pNodo);
	}
	pLs->Primero=pLs->Corriente=NULL;
}

int L_Vacia(TListaSimple Ls)
{
	return (Ls.Primero == NULL);
}

void L_Elem_Cte(TListaSimple Ls, void *pE)
{
	memcpy(pE, Ls.Corriente->Elem, Ls.TamanioDato);
}

int L_Mover_Cte(TListaSimple *pLs, TMovimiento_Ls M)
{
	switch (M)
	{
		case L_Primero:
			pLs->Corriente=pLs->Primero;
		break;
		case L_Siguiente:
			if (pLs->Corriente->Siguiente==NULL)
				return FALSE;
			else
				pLs->Corriente=pLs->Corriente->Siguiente;
		break;
		case L_Anterior:return FALSE;
	}
	return TRUE;
}

void L_Borrar_Cte(TListaSimple *pLs)
{
	TNodoListaSimple *pNodo=pLs->Corriente;
	if (pLs->Corriente==pLs->Primero)
	{
		pLs->Primero = pLs->Corriente->Siguiente;
		pLs->Corriente = pLs->Primero;
	}
	else
	{
		if(pLs->Corriente->Siguiente){
			/* En este caso en que el corriente no es el ultimo, puedo evitarme
			 * recorrer toda la lista buscando el anterior */
			pNodo=pLs->Corriente->Siguiente;
			memcpy(pLs->Corriente->Elem, pNodo->Elem, pLs->TamanioDato);
			pLs->Corriente->Siguiente = pNodo->Siguiente;
		}else {
			TNodoListaSimple *pAux=pLs->Primero;
			while (pAux->Siguiente!=pLs->Corriente)
				pAux = pAux->Siguiente;
			pAux->Siguiente=pLs->Corriente->Siguiente;
			pLs->Corriente = pAux; /*Si es el �ltimo queda en el Anterior al borrado */
		}
	}
	free(pNodo->Elem);
	free(pNodo);
}

int L_Insertar_Cte(TListaSimple *pLs, TMovimiento_Ls M, void* pE)
{
	TNodoListaSimple *pNodo = (TNodoListaSimple*) malloc(sizeof(TNodoListaSimple));
	if (!pNodo)
		return FALSE; /* No hay memoria disponible */
	pNodo->Elem = malloc (pLs->TamanioDato);
	if(!pNodo->Elem)
	{
		free(pNodo);
		return FALSE;
	}
	memcpy(pNodo->Elem, pE, pLs->TamanioDato);
	if ((pLs->Primero == NULL) || (M==L_Primero) || ((M==L_Anterior) && (pLs->Primero==pLs->Corriente)))
	{
		/*Si está vacía o hay que insertar en el Primero o
		hay que insertar en el Anterior y el actual es el Primero */
		pNodo->Siguiente = pLs->Primero;
		pLs->Primero = pLs->Corriente = pNodo;
	}
	else
	{
		/* Siempre inserto como siguiente, el nodo nuevo, porque es más fácil */
		pNodo->Siguiente = pLs->Corriente->Siguiente;
		pLs->Corriente->Siguiente = pNodo;
		if (M == L_Anterior)
		{
			/* Pero cuando el movimiento es Anterior, entonces swapeo los
			 * elementos */
			void* tmp = pNodo->Elem;
			pNodo->Elem = pLs->Corriente->Elem;
			pLs->Corriente->Elem = tmp;
		}
	}
	pLs->Corriente=pNodo;
	return TRUE;
}

void L_Modificar_Cte(TListaSimple Ls, void *pE)
{
	memcpy(Ls.Corriente->Elem, pE, Ls.TamanioDato);
}
