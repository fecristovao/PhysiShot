/// @file physics.c

/*
 * Autor: Fernando Andrade Cristovão
 * 
 * Esta é uma library que foi implementada para compor o jogo da
 * disciplina de ...
 * Ela poderá ser disponibilizada depois e para alterar o seu funcionamento
 * somente é necessário alterar as funções de callbacks para uma função que
 * faça o movimento de sua escolha, apenas seguindo o mesmo formato do protótipo
 * definido na estrutura no arquivo "physics.h"
 * 
 * A lógica seguida foi a seguinte:
 *  As funções callbacks pertecem a um sistema, porém pode-se aplicar em outro sistema
 *  o retorno dela é um vetor, ao invés de alterar o próprio sistema
 *  
 *  As funções de callbacks ja existem padrão para o funcionamento do jogo,
 *  elas são facilmente identificada por terem um sufixo "_"
 * 
 *  Os códigos de erros podem ser adicionado e consultados na biblioteca "global.h"
 * 
 *  Para ativar saídas de debug, na hora do build ativar a flag _DEBUG (Olhar a biblioteca "debug.h")
 *      -D_DEBUG na linha de build
 *  
 *  Em toda função nova adicionar TRACE_ENTER() no inicio e TRACE_EXIT(retorno) no final
 *  para uma depuração mais fácil em caso de erros
 *  
*/

#include "physics.h"

/**
 * Calcula o modulo de um vetor
 * Formula: raiz(x² + y² + ...²)
 * @param[in] inputVetor Vetor de entrada
 * @param[in] ptrResult Ponteiro para uma variável double que vai armazenar o resultado
 * @param[out] errorCode
 */
err_t ModuloVetor( const Vetor inputVetor, double *ptrResult )
{
    TRACE_ENTER();
    err_t ret = SUCCESS;
    int i = 0;
    double result = 0.0;

    if(inputVetor)
    {
        for(i=0;i<VECTOR_DIMENSION;i++)
            result += pow(inputVetor[i], 2);
        result = sqrt(result);
        *ptrResult = result;
    }
    else
    {
        ret = INVALID_PTR;
    }

    TRACE_EXIT(ret);
    return ret;   
}

/**
 * Função auxiliar que aloca um vetor vetor na memória
 * @param[in] ptrVector Ponteiro para o vetor (ponteiro de ponteiro) double
 * @param[out] errorCode
 */
static err_t criarVetor( Vetor *ptrVector )
{
    TRACE_ENTER();
    err_t ret = SUCCESS;
    Vetor novo = (Vetor)calloc(VECTOR_DIMENSION + 1, sizeof(double));
    if(novo)
    {
        *ptrVector = novo;
    }
    else
    {
        ret = ALLOC_ERR;
    }   
    
    TRACE_EXIT(ret);
    return ret;
}

/**
 * Função auxiliar que libera um vetor alocado
 * @param[in] ptrVector Ponteiro para o vetor (ponteiro de ponteiro) double
 * @param[out] errorCode
 */
static void liberarVetor( Vetor *ptrVetor )
{
    TRACE_ENTER();
    free(ptrVetor);
    TRACE_EXIT(0);
}

/**
 * Função que cria uma estrutura com os vetores necessários
 * E preenche as callbacks para as funções com funções padrões
 * Que podem ser modificadas depois
 * @param[in] ptrSistemaEQ Ponteiro para a estrutura de sistemas
 * @param[out] errorCode
 */
err_t criarSistemaEQ( SistemaEQ *ptrSistemaEQ )
{
    TRACE_ENTER();
    err_t ret = SUCCESS;

    if(ptrSistemaEQ)
    {
        ret += criarVetor(&ptrSistemaEQ->posicao);
        ret += criarVetor(&ptrSistemaEQ->vento);
        ret += criarVetor(&ptrSistemaEQ->aceleracao);
        ret += criarVetor(&ptrSistemaEQ->velocidade);
        ret += criarVetor(&ptrSistemaEQ->posicaoInicial);
        ret += criarVetor(&ptrSistemaEQ->aceleracaoInicial);
        ret += criarVetor(&ptrSistemaEQ->velocidadeInicial);
        DEBUG("CRIOU\n%s", "");
        if(ret == SUCCESS)
        {
            srand(time(NULL));
            DEBUG("%s\n", "");
            ptrSistemaEQ->aceleracaoInicial[X] = 0.0;
            DEBUG("%s\n", "");
            
            ptrSistemaEQ->aceleracaoInicial[Y] = -GRAVITY;
            DEBUG("%s\n", "");
            ptrSistemaEQ->tempoAtual = 0.0;
            DEBUG("%s\n", "");
            ptrSistemaEQ->calcularPosicao = &calcularPosicao_;
            DEBUG("%s\n", "");
            ptrSistemaEQ->calcularVelocidade = (void*)&calcularVelocidade_;
            DEBUG("%s\n", "");
            ptrSistemaEQ->sortearVento = (void*)&sortearVento_;
            DEBUG("%s\n", "");
            ptrSistemaEQ->incrementarTempo = (void*)&incrementarTempo_;
            DEBUG("%s\n", "");
            ptrSistemaEQ->calcularAceleracao = (void*)&calcularAceleracao_;
            DEBUG("%s\n", "");
            ptrSistemaEQ->iniciarLancamento = (void*)&iniciarLancamento_;
            ptrSistemaEQ->calcularTempoNoAr = (void*)&calcularTempoNoAr_;
            DEBUG("ATRIBUIU\n%s", "");

        }
        else
        {
            ret = ALLOC_ERR;
        }
    }
    else
    {
        ret = ALLOC_ERR;
    }
    TRACE_EXIT(ret);
    return ret;
}

/**
 * Função libera toda a estrutura para evitar vazamento de memória
 * @param[in] ptrSistemaEQ Ponteiro para a estrutura de sistemas
 * @param[out] errorCode
 */
err_t liberarSistemaEQ( SistemaEQ *ptrSistemaEQ )
{
    TRACE_ENTER();
    err_t ret = SUCCESS;
    if(ptrSistemaEQ)
    {
        free(ptrSistemaEQ->posicao);
        free(ptrSistemaEQ->vento);
        free(ptrSistemaEQ->aceleracao);
        free(ptrSistemaEQ->velocidade);
        free(ptrSistemaEQ->posicaoInicial);
        free(ptrSistemaEQ->aceleracaoInicial);
        free(ptrSistemaEQ->velocidadeInicial);
    }
    else
    {
        ret = INVALID_PTR;
    }

    TRACE_EXIT(ret);
    return ret;
}

/**
 * Função auxiliar que sorteia um vetor que representará
 * o vento de forma aleatória, atentar que recebe um ponteiro
 * logo a estrutura que é passada no parametro será modificada
 * @param[in] ptrSistemaEQ Ponteiro para a estrutura de sistemas
 * @param[out] errorCode
 */
static err_t sortearVento_( SistemaEQ *ptrSistemaEQ )
{
    TRACE_ENTER();
    err_t ret = SUCCESS;
    double range = (MAX_WIND-MIN_WIND);
    int i = 0;
    Vetor vento;

    if(ptrSistemaEQ && ptrSistemaEQ->vento)
    {
        vento = ptrSistemaEQ->vento;
        for(i=0;i<VECTOR_DIMENSION;i++)
        {
            vento[i] = MIN_WIND + (rand()/(RAND_MAX/range));
            usleep(1);
        }
    }
    else
    {
        ret = INVALID_PTR;
    }

    TRACE_EXIT(ret);
    return ret;
}

/**
 * Função auxiliar que incremente um tempo DT na estrutuda d sistema
 * atentar que recebe um ponteiro
 * logo a estrutura que é passada no parametro será modificada
 * @param[in] ptrSistemaEQ Ponteiro para a estrutura de sistemas
 * @param[in] dt
 * @param[out] errorCode
 */
static err_t incrementarTempo_( SistemaEQ *ptrSistemaEQ, const Tempo dt )
{
    TRACE_ENTER();

    err_t ret = SUCCESS;
    
    if(ptrSistemaEQ)
    {
        ptrSistemaEQ->tempoAtual += dt;
    }
    else
    {
        ret = INVALID_PTR;
    }

    TRACE_EXIT(ret);
    return ret;
}

/**
 * Função auxiliar padrão que calcula a velocidade do sistema
 * @param[in] ptrSistemaEQ Ponteiro para a estrutura de sistemas
 * @param[in] vetorResultado
 * @param[out] errorCode
 */
static err_t calcularVelocidade_( const SistemaEQ ptrSistemaEQ, Vetor result )
{
    TRACE_ENTER();
    err_t ret = SUCCESS;
    
    if(&ptrSistemaEQ && ptrSistemaEQ.velocidade 
        && ptrSistemaEQ.velocidadeInicial 
        && ptrSistemaEQ.aceleracao)
    {
        Vetor velocidadeInicial = ptrSistemaEQ.velocidadeInicial;
        Vetor aceleracao = ptrSistemaEQ.aceleracao;
        Tempo tempoAtual = ptrSistemaEQ.tempoAtual;
           
        result[X] = velocidadeInicial[X] + aceleracao[X]*tempoAtual;
        result[Y] = velocidadeInicial[Y] + aceleracao[Y]*tempoAtual;
      //  printf("VElo %f\nAceleracao %f\n", result[Y], aceleracao[Y]*tempoAtual);
    }
    else
    {
        ret = INVALID_PTR;
    }

    TRACE_EXIT(ret);
    return ret;
}

/**
 * Função auxiliar padrão que calcula a aceleração do sistema
 * @param[in] ptrSistemaEQ Ponteiro para a estrutura de sistemas
 * @param[in] vetorResultado
 * @param[out] errorCode
 */
static err_t calcularAceleracao_( const SistemaEQ ptrSistemaEQ, Vetor result )
{
    TRACE_ENTER();
    err_t ret = SUCCESS;
    
    if(&ptrSistemaEQ && ptrSistemaEQ.aceleracao 
        && ptrSistemaEQ.aceleracaoInicial)
    {
        result[X] = 0.0;
        result[Y] = -GRAVITY;
        
    }
    else
    {
        ret = INVALID_PTR;
    }

    TRACE_EXIT(ret);
    return ret;
}

/**
 * Função auxiliar padrão que calcula a posição do sistema
 * @param[in] ptrSistemaEQ Ponteiro para a estrutura de sistemas
 * @param[in] vetorResultado
 * @param[out] errorCode
 */
static err_t calcularPosicao_( const SistemaEQ ptrSistemaEQ, Vetor result )
{
    TRACE_ENTER();
    err_t ret = SUCCESS;
    
    if(&ptrSistemaEQ && ptrSistemaEQ.posicao 
        && ptrSistemaEQ.posicaoInicial
        && ptrSistemaEQ.velocidade
        && ptrSistemaEQ.aceleracao)
    {
        Tempo tempoAtual = ptrSistemaEQ.tempoAtual;
        Vetor velocidade = ptrSistemaEQ.velocidadeInicial;
        Vetor posicaoInicial = ptrSistemaEQ.posicaoInicial;
        Vetor aceleracao = ptrSistemaEQ.aceleracao;
        
        result[X] = posicaoInicial[X] + velocidade[X]*tempoAtual + 0.5*aceleracao[X]*(pow(tempoAtual, 2));
        result[Y] = posicaoInicial[Y] + velocidade[Y]*tempoAtual + 0.5*aceleracao[Y]*(pow(tempoAtual, 2));
        
    }
    else
    {
        ret = INVALID_PTR;
    }

    TRACE_EXIT(ret);
    return ret;
}

/**
 * Função auxiliar padrão que calcula a velocidade inicial do sistema
 * @param[in] ptrSistemaEQ Ponteiro para a estrutura de sistemas
 * @param[out] errorCode
 */
err_t iniciarLancamento_( const SistemaEQ ptrSistemaEQ, const Angulo tetha, const double velocidadeSaida, Vetor result)
{
    TRACE_ENTER();
    err_t ret = SUCCESS;
    
    if(&ptrSistemaEQ != NULL && ptrSistemaEQ.velocidadeInicial != NULL)  
    {
       result[X] = velocidadeSaida*cos((PI/180.0)*tetha);
       result[Y] = velocidadeSaida*sin((PI/180.0)*tetha);
    }
    else
    {
        ret = INVALID_PTR;
    }

    TRACE_EXIT(ret);
    return ret;
}

/**
 * Função auxiliar padrão que calcula o tempo do objeto no ar
 * Atentar que essa é uma função padrão e no caso de mudança nas outras
 * funções, ou mudança nas callbacks da estrutura padrão essa função deve ser
 * modificada.
 * @param[in] ptrSistemaEQ Ponteiro para a estrutura de sistemas
 * @param[in] ponteiroResultado
 * @param[out] errorCode
 */
static err_t calcularTempoNoAr_( const SistemaEQ ptrSistemaEQ, double * ptrTempo)
{
    TRACE_ENTER();
    err_t ret = SUCCESS;
    double endTime = 0.0;

    if(
        &ptrSistemaEQ &&
        ptrTempo
    )
    {
        double ay = ptrSistemaEQ.aceleracaoInicial[Y];
        double vy = ptrSistemaEQ.velocidadeInicial[Y];
        //printf("VElo inicial %f\n", vy);
        *ptrTempo = -((2*vy)/ay);
    }
    else
    {
        ret = INVALID_PTR;
    }

    TRACE_EXIT(ret);
    return ret;

}