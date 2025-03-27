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
static bool always_false(fsm_t *f) {
    (void)f;
    return false;
}

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

//void setUp(void)
//{
//}

//void tearDown(void)
//{
//}

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
 * @brief Comprueba que la función de inicialización devuelve false si el puntero a la maquina de estado es NULL 
 *
 */
//void test_fsm_init_falseWhenNullFsm(void)
//{   
   

    //TEST_IGNORE();
//}

/**
 * @brief Función de inicializacion devuelve false si la tabla de transiciones es nula
 * 
 */
//void test_fsm_init_falseWhenNullTransitions(void)
//{
    //TEST_IGNORE();
//}

/**
 * @brief La máquina de estados devuelve NULL
 *        y no llama a fsm_malloc si el estado de origen
 *        de la primera transición es -1 (fin de la tabla)
 */
void test_fsm_nullWhenFirstOrigStateIsMinusOne (void) {
  fsm_trans_t tt[] = {{-1, is_true, 1, do_nothing}};
  fsm_t *f = (fsm_t*)1;
  f = fsm_new(tt);
 
//TEST_ASSERT_EQUAL (XXX);
  TEST_FAIL_MESSAGE("Implement the test");
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si el estado de destino de la primera transición es -1 (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstDstStateIsMinusOne (void) {
  
  TEST_IGNORE();
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si la función de comprobación de la primera transición es NULL (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstCheckFunctionIsNull (void) {
  
  TEST_IGNORE();
}

/**
 * @brief Devuelve puntero no NULL y llama a fsm_malloc (usa Callback a cb_mallock y fsm_malloc_ExpectXXX) al crear la maquina de estados con una transición válida
 *        con función de actualización (salida) NULL o no NULL.
 *        Hay que liberar la memoria al final llamando a free
 * 
 */
TEST_CASE(NULL)
TEST_CASE(do_nothing)
void test_fsm_new_nonNullWhenOneValidTransitionCondition(fsm_output_func_t out)
{
        // 定义一张有效的转换表。假设状态 0 为初始状态，状态 1 为目标状态，EVENT_START 为有效事件。
    fsm_trans_t tt[] = {
        { 0, is_true, 1, out },  // 一条有效的转换记录
        { -1, NULL, -1, NULL }          // 结束标志
    };

    fsm_malloc_Stub(cb_malloc);
    fsm_free_Stub(cb_free);  // 添加这一行，设置 fsm_free 的回调
    
    fsm_t *f = fsm_new(tt);


    // 验证返回的状态机指针不为 NULL
    TEST_ASSERT_NOT_NULL(f);

    // 释放状态机占用的内存
    fsm_destroy(f);
}


/**
 * @brief Estado inicial corresponde al estado de entrada de la primera transición de la lista al crear una maquiina de estados y es valido.
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
 * @brief Comprueba que el puntero pasado a fsm_fire es pasado a la función de guarda cuando se comprueba una transición
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
 * @brief Comprueba que el fsm_fire funciona y tiene el estado correcto cuando la transición devuelve true (cambia) y cuando devuelve false (mantiene)
 * 
 */
TEST_CASE(false, 0)
TEST_CASE(true, 1)
void test_fsm_fire_checkFunctionIsCalledAndResultIsImportantForTransition(bool returnValue, int expectedState)
{
    
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_t f;
    int res;
    fsm_init(&f, tt);

  
    is_true_ExpectAnyArgsAndReturn(returnValue);
    
    
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
 * @brief La creación de una máquina de estados devuelve NULL si la reserva de memoria falla.
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

    TEST_ASSERT_NULL(fsm);  // 如果 malloc 返回 NULL，fsm_new 应该返回 NULL


    //TEST_IGNORE();
}

/**
 * @brief Llamar a fsm_destroy provoca una llamada a fsm_free
 *        Usa el Mock llamando a fsm_free_Expect sin Stub ni Callback
 *
 */
void test_fsm_destroy_callsFsmFree(void)
{
  

    fsm_t *fsm = 0x1111;
    fsm_free_Expect(fsm);

    fsm_destroy(fsm);

    //TEST_IGNORE();
}

/**
 * @brief Comprueba que solo se llame a la función de guarda que toca según el estado actual
 * 
 */
void test_fsm_fire_callsFirstIsTrueFromState0AndThenIsTrue2FromState1(void)
{
    fsm_trans_t tt[] = {
        {0, is_true, 1, NULL},
        {1, is_true2, 0, NULL},   //Descomentar cuando se haya declarado una nueva función para mock is_true2
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
 * @brief Comprueba que se pueden crear dos instancias de máquinas de estados simultánteas y son punteros distintos.
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

    //TEST_IGNORE();
}

