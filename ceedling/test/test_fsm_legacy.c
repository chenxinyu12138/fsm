#include "unity.h"

#define TEST_CASE(...)

#include "fsm.h"
#include "mock_test_fsm.h"

#include <stdlib.h>

/**
 * @file test_fsm_legacy.c
 * @author 
 * @author 
 * @brief Tests for existing fsm module
 * @version 0.1
 * @date 2024-04-09
 * 
 */


/**
 * @brief Stub or Callback for fsm_malloc that calls real malloc. 
 * 
 * @param[in] s Amount of bytes to allocate
 * @param[in] n Amount of calls to this function
 * 
 * @return pointer to allocated memory if success; NULL if fails
 * 
 */
static void* cb_malloc(size_t s, int n) {
    return malloc(s);
}

/**
 * @brief Stub or Callback for fsm_free that calls real free. 
 * 
 * @param[in] p Pointer to allocated memory to free
 * @param[in] n Amount of calls to this function
 * 
 */
static void cb_free(void* p, int n) {
    return free(p);
}

void setUp(void)
{
}

void tearDown(void)
{
}

/**
 * @brief Comprueba que la funcion de fsm_new devuelve NULL 
 *        y no llama a fsm_malloc si la tabla de transiciones es NULL 
 */
void test_fsm_new_nullWhenNullTransition(void)
{
    fsm_t *f = (fsm_t*)1;

    f = fsm_new(NULL);

    TEST_ASSERT_EQUAL (NULL, f);
}


/**
 * @brief La maquina de estados devuelve NULL
 *        y no llama a fsm_malloc si el estado de origen
 *        de la primera transicion es -1 (fin de la tabla)
 */
void test_fsm_nullWhenFirstOrigStateIsMinusOne(void) {

  fsm_trans_t tt[] = {{-1, is_true, 1, do_nothing}};
  fsm_t *f = (fsm_t*)1;
  f = fsm_new(tt);
  TEST_ASSERT_NULL(f);

}

/**
 * @brief La maquina de estados devuelve NULL y no llama a fsm_malloc si el estado de destino de la primera transicion es -1 (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstDstStateIsMinusOne(void){
    fsm_trans_t tt[] = {{0, is_true, -1, do_nothing}};
    fsm_t *f = (fsm_t*)1;
    f = fsm_new(tt);
    

    TEST_ASSERT_NULL(f);
  
 
}

/**
 * @brief La maquina de estados devuelve NULL y no llama a fsm_malloc si la funcion de comprobacion de la primera transicion es NULL (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstCheckFunctionIsNull(void){
    fsm_trans_t tt[] = {{0, NULL, 1, do_nothing}
    };
    fsm_t *f = (fsm_t*)1;
    f = fsm_new(tt);
    
    TEST_ASSERT_NULL(f);
  
 

}

/**
 * @brief Devuelve puntero no NULL y llama a fsm_malloc (usa Callback a cb_mallock y fsm_malloc_ExpectXXX) al crear la maquina de estados con una transicion valida
 *        con funcion de actualizacion (salida) NULL o no NULL.
 *        Hay que liberar la memoria al final llamando a free
 * 
 */
TEST_CASE(NULL)
TEST_CASE(do_nothing)
void test_fsm_new_nonNullWhenOneValidTransitionCondition(fsm_output_func_t out){
        
    fsm_trans_t tt[] = {
        { 0, is_true, 1, out },  
        { -1, NULL, -1, NULL }          
    };

    fsm_malloc_Stub(cb_malloc);
    
    fsm_free_Stub(cb_free);  
    
    fsm_t *f = fsm_new(tt);
    
    TEST_ASSERT_NOT_NULL(f);

    fsm_destroy(f);
}


/**
 * @brief Estado inicial corresponde al estado de entrada de la primera transicion de la lista al crear una maquiina de estados y es valido.
 *        Usa Stub para fsm_malloc y luego libera la memoria con free
 */
void test_fsm_new_fsmGetStateReturnsOrigStateOfFirstTransitionAfterInit(void)
{
    fsm_trans_t tt[] = {
        { 0, is_true, 1, do_nothing },  
        { -1, NULL, -1, NULL }          
    };
  
  
  fsm_malloc_Stub(cb_malloc);
  fsm_free_Stub(cb_free); 
  
  fsm_t *fsm = fsm_new(tt);

  
  TEST_ASSERT_NOT_NULL(fsm);

  
  TEST_ASSERT_EQUAL_INT(tt[0].orig_state, fsm_get_state(fsm));

  
  fsm_destroy(fsm);


    //TEST_IGNORE();
}

/**
 * @brief La maquina de estado no transiciona si la funcion devuelve 0
 * 
 */
void test_fsm_fire_isTrueReturnsFalseMeansDoNothingIsNotCalledAndStateKeepsTheSame(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, do_nothing},
        {-1, NULL, -1, NULL}
    };

    fsm_t f;
    int res;
    fsm_init(&f, tt);

    res = fsm_get_state(&f);
    TEST_ASSERT_EQUAL_INT(0, res);

    is_true_ExpectAndReturn(&f, false);
    fsm_fire(&f);

    int  after_res = fsm_get_state(&f);

    TEST_ASSERT_EQUAL_INT(res, after_res);
   //TEST_IGNORE();
}

/**
 * @brief Comprueba que el puntero pasado a fsm_fire es pasado a la funcion de guarda cuando se comprueba una transicion
 * 
 */
void test_fsm_fire_checkFunctionCalledWithFsmPointerFromFsmFire(void)
{

    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };

    fsm_t f;
    int res;
    fsm_init(&f, tt);
   
    is_true_ExpectAndReturn(&f, true);
    fsm_fire(&f);

    TEST_ASSERT_EQUAL_INT(1, fsm_get_state(&f));
    //TEST_IGNORE();
}

/** 
 * @brief Comprueba que el fsm_fire funciona y tiene el estado correcto cuando la transicion devuelve true (cambia) y cuando devuelve false (mantiene)
 * 
 */
TEST_CASE(false, 0)
TEST_CASE(true, 1)
void test_fsm_fire_checkFunctionIsCalledAndResultIsImportantForTransition(bool returnValue, int expectedState){
    
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_t f;
    int res;
    fsm_init(&f, tt);

  
    is_true_IgnoreAndReturn(returnValue);
    
    
    fsm_fire(&f);
 
    if (expectedState == 0)
    {
        res = 0;
        /* code */
    }
    else if(expectedState == 1)
    {
        res = 1;
        /* code */
    }
    
    TEST_ASSERT_EQUAL_INT(res, fsm_get_state(&f));
}


/**
 * @brief La creacion de una maquina de estados devuelve NULL si la reserva de memoria falla.
 *        Usa el Mock llamando a fsm_malloc_ExpectXXX sin Stub ni Callback.
 *
 */
void test_fsm_new_nullWhenFsmMallocReturnsNull(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true, 0, NULL},
        {-1, NULL, -1, NULL}
    };
    // Esperamos que fsm_malloc sea llamado y devuelva NULL
    fsm_malloc_ExpectAnyArgsAndReturn(NULL);

    fsm_t *fsm = fsm_new(tt);

    TEST_ASSERT_NULL(fsm);  


    //TEST_IGNORE();
}

/**
 * @brief Llamar a fsm_destroy provoca una llamada a fsm_free
 *        Usa el Mock llamando a fsm_free_Expect sin Stub ni Callback
 *
 */
void test_fsm_destroy_callsFsmFree(void)
{
  

    fsm_t *fsm = (fsm_t*)1;;
    fsm_free_Expect(fsm);

    fsm_destroy(fsm);

    //TEST_IGNORE();
}

/**
 * @brief Comprueba que solo se llame a la funcion de guarda que toca segun el estado actual
 * 
 */
void test_fsm_fire_callsFirstIsTrueFromState0AndThenIsTrue2FromState1(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true2, 0, NULL},   //Descomentar cuando se haya declarado una nueva funcion para mock is_true2
        {-1, NULL, -1, NULL}
    };

    fsm_t f;
    int res;
    fsm_init(&f, tt);
    res = fsm_get_state(&f);
    
    TEST_ASSERT_EQUAL_INT(0,res);

    is_true_ExpectAndReturn(&f,true);
    fsm_fire(&f);

    res = fsm_get_state(&f);
    TEST_ASSERT_EQUAL_INT(1,res);

    is_true2_ExpectAndReturn(&f,true);
    fsm_fire(&f);

    res = fsm_get_state(&f);
    TEST_ASSERT_EQUAL_INT(0,res);


    //TEST_IGNORE();
}

/**
 * @brief Comprueba que se pueden crear dos instancias de maquinas de estados simultanteas y son punteros distintos.
 *        Usa Stub para fsm_malloc y luego libera la memoria con free
 *
 */
void test_fsm_new_calledTwiceWithSameValidDataCreatesDifferentInstancePointer(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_malloc_Stub(cb_malloc);

    fsm_t *f1 = fsm_new(tt);
    fsm_t *f2 = fsm_new(tt);

    TEST_ASSERT_NOT_NULL(f1);
    TEST_ASSERT_NOT_NULL(f2);
    TEST_ASSERT_NOT_EQUAL(f1, f2);

    free(f1);
    free(f2);
   
}



/*
Este test verifica que la funcion fsm_set_state realmente actualiza el estado de la maquina.
Se inicializa un estado y luego se cambia a otro, comprobando que el cambio se ha producido correctamente.
*/


void test_set_state_should_set_fsm_state(void) {
    fsm_t fsm;
    fsm.current_state = 0;  
    fsm_set_state(&fsm, 5); 
    TEST_ASSERT_EQUAL(5, fsm.current_state);
}

/**
 * @brief Verifica que la funcion de salida (out) se llama correctamente durante una transicion valida.
 *
 * Este test configura una tabla de transiciones donde:
 *  - La condicion de entrada (is_true) siempre devuelve verdadero.
 *  - La funcion de salida (do_nothing) debe ser llamada cuando ocurre la transicion.
 *
 * Se utiliza mocking para:
 *  - Ignorar la condicion con is_true_IgnoreAndReturn(true)
 *  - Verificar que se llama la funcion do_nothing con cualquier argumento
 * Finalmente se comprueba que el estado de la FSM ha cambiado correctamente.
 */

void test_fire_should_call_out_function(void){

    fsm_trans_t tt[] = {
        {0, is_true, 1, do_nothing},
        {-1, NULL, -1, NULL}
    };

    fsm_t fsm;
    fsm_init(&fsm,tt);
 
    is_true_IgnoreAndReturn(true);
    do_nothing_ExpectAnyArgs();
    fsm_fire(&fsm);

    TEST_ASSERT_EQUAL(1, fsm_get_state(&fsm));


}

/**
 * @brief Verifica que fsm_init no provoca errores al recibir una tabla de transiciones NULL.
 *
 * Este test inicializa manualmente un objeto fsm y luego llama a fsm_init pasando un puntero NULL
 * como tabla de transiciones. El objetivo es comprobar que no se produce ningun fallo o comportamiento inesperado.
 *
 * Se verifica que fsm.p_tt sigue siendo NULL despues de la inicializacion.
 */

void test_fsm_init_should_not_crash_with_null_transition_table(void) {
    fsm_t fsm;

    fsm.p_tt = NULL;
  
    fsm_init(&fsm, NULL);

  
    TEST_ASSERT_NULL(fsm.p_tt);

}

/**
 * @brief Comprueba que la funcion fsm_init devuelve 0 si la tabla de transiciones excede el limite permitido.
 * 
 * Una devolucion de 0 indica error en la inicializacion por exceso de transiciones.
 * En caso contrario, devuelve el numero de transiciones validas en la tabla.
 */

void test_fsm_init_ReturnsZeroWhenExceedingMaxTransitions(void){

    int n = rand() % (FSM_MAX_TRANSITIONS + 100);
    fsm_trans_t tt[n + 2];

    for (int i = 0; i < n + 1; i++) {
        tt[i].orig_state = i;
        tt[i].in = is_true;
        tt[i].dest_state = i + 1;
        tt[i].out = do_nothing;
    }

    tt[n + 1].orig_state = -1;
    tt[n + 1].in = NULL;
    tt[n + 1].dest_state = -1;
    tt[n + 1].out = NULL;

    fsm_t f;
    int result = fsm_init(&f, tt);

    if(n > FSM_MAX_TRANSITIONS){
        TEST_ASSERT_EQUAL_INT(0, result);
    }else
    {
        TEST_ASSERT_EQUAL_INT(n, result);

    }
    

}
/**
 * @brief  Comprueba que la funcion fsm_new devuelve NULL si la tabla de transiciones excede el limite de transiciones. 
 * 
 */

void test_fsm_new_ReturnsNullWhenTransitionsExceedLimit(void) {
   
    int n = 129;  
    fsm_trans_t tt[n + 2];

    for (int i = 0; i < n + 1; i++) {
        tt[i].orig_state = i;
        tt[i].in = is_true;
        tt[i].dest_state = i + 1;
        tt[i].out = do_nothing;
    }

    tt[n + 1].orig_state = -1;
    tt[n + 1].in = NULL;
    tt[n + 1].dest_state = -1;
    tt[n + 1].out = NULL;

    fsm_t *result = fsm_new(tt);
    TEST_ASSERT_NULL(result);

       
     
}

/**
 * @brief  Comprueba que la funcion fsm_new devuelve NULL si la tabla de transiciones no termina con el marcador {-1, NULL, -1, NULL}.             
 * 
 */

void test_fsm_new_returnsNullWhenTransitionTableMissingEndMarker(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, do_nothing},
        {1, is_true, 2, do_nothing},
        //  {-1, NULL, -1, NULL}
    };

    fsm_t *fsm = fsm_new(tt);

    TEST_ASSERT_NULL(fsm);
}


/**
 * @brief Comprueba que la funcion de guarda NULL actua como verdadero y provoca la transicion.
 *
 * Este test configura una tabla de transiciones donde:
 *  - La condicion de entrada es NULL (actua como verdadero).
 *  - La funcion de salida es NULL.
 *
 * Se verifica que el estado de la FSM cambia correctamente al estado destino.
 */     
void test_fsm_fire_GuardNullActsAsTrueAndTriggersTransition(void)
{
    fsm_trans_t tt[] = {
        {0, NULL ,1, NULL}, 
        {-1, NULL, -1, NULL}
    };

    fsm_t fsm;
    fsm_init(&fsm, tt);
    fsm_set_state(&fsm, 0); 

    fsm_fire(&fsm);

    TEST_ASSERT_EQUAL_INT(1, fsm_get_state(&fsm));
    
}











