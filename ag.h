
/**
*   Universidade Federal da Fronteira Sul
*
*   TRABALHO I
*   Disciplina: Inteligência Artificial
*   Professor: José Carlos Bins Filho
*
*
* 	 Alunos : Edson Lemes da Silva & Lucas Cezar Parnoff
*   
*   Compilação pelo arquivo Makefile
*   ou gcc ag.c ag.h readFile.c -o m && ./m
*   
*  O trabalho consiste em aplicar o algoritmo genético para
*  gerar a melhor grade de horários possível do Curso de Ciência
*  da Computação, levando em conta as disciplinas do semestre vigente.
*  A busca pelo melhor resultado refere-se na análise das preferências
*  dos professores e na menor distribuição dos horários na grade escolar.
*
*
**/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include <unistd.h>

#define TAM_POPULACAO 500
#define GERACOES 100

#define PROBMUTACAO 0.3
#define PROBCRUZAMENTO 0.7

#define TORNEIO 200
//Flags para controle de erros
#define ERRONALEITURA 1
#define ERROMUTACAO 3
#define ERROCRUZAMENTO 4
#define ERROALOCACAO 5
#define ERROINDIVIDUO 6
#define OK 2

//#define QTDPROF
//#define QTDDISC//
//#define QTDSALA 

//para freeMem (liberação de memória)
#define SEMESTRE 101
#define PROF_AUX 102
#define DISC_AUX 103
#define PLCAO 104
#define INDVO 105
#define GENES 106

//Quantidades lidas no arquivo curso.dat (professor,semestre,disciplina)
int qtdprof,qtdsem,qtddisc;
int posicao_arq,backup;

//int tam_populacao = 10;


//Representa 1 gene de um indivíduo
typedef struct gene{

	int dia_sem;	//periodo
	char *prof;    //nome professor
	int *notpref;  //vetor de hor. nao preferidos
	int numpref;   //tamanho de notpref
	int disc;      //disciplina
	int sala;      //sala
	int sala_id;   //posicao no vetor da sala
	int periodo;   //quatidade de periodos
	char sem[4];   //semestre
	//outres *test;
}genes;


//Representa um indivíduo
typedef struct ind{

	genes *genes_indv;  //vetor de genes 
	int qtdpr;    //soma das penalidades
	int choques;  //soma da distribuição
	int qtd;   //tamanho de genes_indv

}indvo;

//Representa a populacao
typedef struct pop{
//	int *qtd;
	indvo *individuos; //vetor de individuos

}plcao;



//auxiliar para leitura de todos os professores
typedef struct teac{

	char *nome;
	int num;
	int *horarios;


}prof_aux;

//auxiliar para leitura de todos os semestres
typedef struct sem{
	char se[3];
	int num;
	int sala;
	int *horarios;

}semestre;

//auxiliar para leitura de todas as disciplinas
typedef struct disc{

	char cod[6];
	int periodo;
	char cod_sem[3];
	char *nome;

}disc_aux;

//protótipos comuns para as funções
prof_aux *leProfessores(char arq[20]);
semestre *leSemestre(char arq[20]);
disc_aux *leDisciplina(char arq[20]);

//int testaParada(int *v);

void freeMem(void *algo,int component);
