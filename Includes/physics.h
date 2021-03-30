#ifndef _PHYSICS_INCLUDED_
#define _PHYSICS_INCLUDED_

/// @file physics.h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "global.h"
#include "debug.h"

#define VECTOR_DIMENSION  2                 ///< Constante que define as dimensões dos vetores 
#define X                 0
#define Y                 1
#define GRAVITY           9.80665           ///< Constante que define o valor da gravidade
#define MAX_WIND          20                ///< Constante que define o range maximo que o vento podera ter
#define MIN_WIND          0                 ///< Constante que define o range minimo que o vento podera ter
#define PI                3.14159265        ///< Constante matemática PI

typedef double* Vetor;
typedef double  Tempo;
typedef double  Angulo;


struct SistemaEQ_ {
    Vetor vento;
    Vetor aceleracao;
    Vetor velocidade;
    Vetor posicao;
    Vetor aceleracaoInicial;
    Vetor velocidadeInicial;
    Vetor posicaoInicial;
    Tempo tempoAtual;
    err_t (*calcularAceleracao)(struct SistemaEQ_, Vetor);                  ///> Callback para calcular aceleracao
    err_t (*calcularPosicao)(struct SistemaEQ_, Vetor);                     ///> Callback para calcular posicao
    err_t (*calcularVelocidade)(struct SistemaEQ_, Vetor);                  ///> Callback para calcular velocidade
    err_t (*iniciarLancamento)(struct SistemaEQ_, Angulo, double, Vetor);   ///> Callback para calcular velocidade inicail
    err_t (*sortearVento)(struct SistemaEQ_*);                              ///> Callback para calcular sortear vento
    err_t (*incrementarTempo)(struct SistemaEQ_*, Tempo);                   ///> Callback para incrementar um dt
    err_t (*calcularTempoNoAr)(struct SistemaEQ_, double*);                 ///> Callback para calcular tempo de um objeto no ar
};

typedef struct SistemaEQ_ SistemaEQ;

err_t criarSistemaEQ( SistemaEQ *ptrSistemaEQ );
err_t liberarSistemaEQ( SistemaEQ *ptrSistemaEQ );
static err_t criarVetor( Vetor *ptrVector );
static void liberarVetor( Vetor *ptrVetor );
static err_t sortearVento_( SistemaEQ *ptrSistemaEQ );  
static err_t calcularVelocidade_( const SistemaEQ ptrSistemaEQ, Vetor result );
static err_t calcularAceleracao_( const SistemaEQ ptrSistemaEQ, Vetor result );
static err_t calcularPosicao_( const SistemaEQ ptrSistemaEQ, Vetor result );
static err_t incrementarTempo_( SistemaEQ *ptrSistemaEQ, const Tempo dt );
static err_t iniciarLancamento_( const SistemaEQ ptrSistemaEQ, const Angulo tetha, const double velocidadeSaida, Vetor result);
static err_t calcularTempoNoAr_( const SistemaEQ ptrSistemaEQ, double * ptrTempo);
err_t ModuloVetor( const Vetor inputVetor, double *ptrResult );


#endif