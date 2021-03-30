#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include "physics.h"
#include "global.h"
#include "PIG.h"

#define DT pow(10, -2)
#define FPS 60


int main (void)
{

    SistemaEQ equacoes;
    void * thrres;
    double t;
    criarSistemaEQ(&equacoes);
    equacoes.iniciarLancamento(equacoes, 45, 10, equacoes.velocidadeInicial);
    printf("Tempo no ar: %f\n", t);
    equacoes.calcularTempoNoAr(equacoes, &t);
    printf("%.2f\n", t);

    float i;
    for(i=0.0;equacoes.tempoAtual<t;i+=DT)
    { 
        printf("%f: %.2f\t%.2f\t%.2f\t%.2f\n",i, equacoes.posicao[X], equacoes.posicao[Y], equacoes.tempoAtual, equacoes.velocidade[Y]);
        equacoes.calcularAceleracao(equacoes, equacoes.aceleracao);
        equacoes.calcularVelocidade(equacoes, equacoes.velocidade);
        equacoes.calcularPosicao(equacoes, equacoes.posicao);
        equacoes.incrementarTempo(&equacoes, DT);
        //usleep(50000);
    }
    
    liberarSistemaEQ(&equacoes);
}