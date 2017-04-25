
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
*   
*  O trabalho consiste em aplicar o algoritmo genético para
*  gerar a melhor grade de horários possível do Curso de Ciência
*  da Computação, levando em conta as disciplinas do semestre vigente.
*  A busca pelo melhor resultado refere-se na análise das preferências
*  dos professores e na menor distribuição dos horários na grade escolar.
*
*
**/

#include "ag.h"
int randnum = 0;

//int qtdprof, qtddisc,qtdsem;
/**
 * @function auxF
 *
 * @param int v - vetor para verificação
 * @param int tam - tamanho de v
 * 
 * Verifica se existe algum valor não nulo em v
 *
 **/ 
int auxF(int *v, int tam){

	int i;	
	int flag = 0;
	for(i = 0; i < tam;i++)
		if(v[i] > 0){flag = 1;break;}
		
	return flag;
	
}

/**
 * @function geraSequencia
 *
 * @param int v - vetor v que representa os horários de cada semestre
 * @param tam - tamanho de v
 * 
 * A função pega os valores de v e sorteia uma ordem aleatória,
 * porque posteriormente este vetor é lido em sequência. 
 * 
 **/ 
int *geraSequencia(int *v, int tam){

	int i,j;
	int r;
	srand(time(NULL) + randnum);
	randnum+=10;
	int *m = (int *)malloc(sizeof(int)*tam);
	int *v1 = (int *)malloc(sizeof(int)*tam);
	j = 0;

	for(i = 0 ; i < tam;i++) m[i] = 1;
	
	while(auxF(m,tam)){
		
		r= rand() %tam; 
		if(m[r] == -1){
			for(i = 0; i < tam;i++){
				if(m[i] != -1)	{	
					v1[j] = v[i];
					r = i;
				}
			}
		}
		else v1[j] = v[r];

		m[r] = -1;
		j++;	
	}
	free(m);free(v);
	return v1;
}

/**
 * @function leProfessores
 *
 * @param arq - Nome do arquivo
 *
 * Lê a primeira parte do arquivo que contém os professores, e
 * armazena na estrutura global  prof_aux; 
 * 
 **/ 
prof_aux *leProfessores(char arq[20]){
	int qtd =0;
	int ii,k,j;

	char *t = (char *)malloc(sizeof(char)*20);	
	if(!t) {printf("Alocacao professores t"); return NULL;}
	FILE *arqi;
	prof_aux *pf;
	
	arqi = fopen(arq,"r");
	if(!arqi) {printf("Erro na leitura"); free(t); return NULL;}
	fscanf (arqi, "%d", &qtdprof);
	pf = (prof_aux *)malloc(sizeof(prof_aux)*qtdprof);
	if(!t) {printf("Alocacao professores"); free(t); return NULL;}

	fseek(arqi,1,SEEK_CUR);
	for(j = 0;j <qtdprof;j++){
		pf[j].nome     = NULL;
		pf[j].horarios = NULL;
		fscanf(arqi,"%s %d", t, &ii);

		pf[j].nome = (char *)malloc(sizeof(char)*strlen(t));
		if(!pf[j].nome) {printf("Alocacao professores %d nome",j); freeMem(pf,PROF_AUX);return NULL;}
		strcpy(pf[j].nome,t);
		if(ii == 0) pf[j].horarios = NULL;
		else{
			pf[j].horarios = (int *)malloc(sizeof(int)*ii);
			if(!pf[j].horarios) {printf("Alocacao professores %d horarios",j);freeMem(pf,PROF_AUX); return NULL;}}
		pf[j].num = ii;
		
		for(k =0;k<ii;k++){
			fscanf (arqi, "%d", &qtd);
			pf[j].horarios[k] = qtd;
				
		}
		
	}
	
	posicao_arq = ftell(arqi);
	fclose(arqi);
	free(t);	
	return pf;
}

/**
 * @function leSemestre
 *
 * @param arq - Nome do arquivo
 *
 * Lê a segunda parte do arquivo que contém os semestres, e
 * armazena na estrutura global  semestre *semn; 
 * 
 **/ 
semestre *leSemestre(char arq[20]){
	backup = posicao_arq;
	FILE *arqi;
	semestre *semn;
	char *t = (char *)malloc(sizeof(char)*5);	
	if(!t) {printf("Alocacao semestre t"); return NULL;}
	int ii,i,j,k,l;
	arqi = fopen(arq,"r");
	if(!arqi) {printf("Erro na leitura"); free(t); return NULL;}
	fseek(arqi,posicao_arq+1,SEEK_CUR);
	fscanf (arqi, "%d", &qtdsem);
	semn = (semestre *)malloc(sizeof(semestre)*qtdsem);
	if(!semn) {printf("Alocacao semestre"); free(t); return NULL;}
	
	for(i = 0;i<qtdsem;i++){
		semn[i].horarios = NULL;
		fscanf(arqi,"%s %d %d", t, &ii,&l);
		strcpy(semn[i].se,t);
		semn[i].sala = ii;
		semn[i].horarios = (int *)malloc(sizeof(int)*l); // ao inves de "*ii" vamos usar *l pois não precisa mais do que o numero de horarios que está disponivel para utilizar.
		if(!semn[i].horarios) {printf("Alocacao semestre %d horarios",i); freeMem(semn,SEMESTRE); free(t); return NULL;}
		semn[i].num = l;
		for(j = 0;j < l;j++){
			fscanf(arqi,"%d", &k);
			semn[i].horarios[j] = k;
		}
		semn[i].horarios= geraSequencia(semn[i].horarios,semn[i].num);
		
	}
	
	posicao_arq = ftell(arqi);
	backup = posicao_arq;
	fclose(arqi);
	free(t);
	return semn;	
}

/**
 * @function leDisciplina
 *
 * @param arq - Nome do arquivo
 *
 * Lê a última parte do arquivo que contém as disciplinas, e
 * armazena na estrutura global  disc_aux *discc; 
 * 
 **/ 
disc_aux *leDisciplina(char arq[20]){

	FILE *arqi;
	disc_aux *discc;
	int i,j;
	char *t = (char *)malloc(sizeof(char)*5);	
	if(!t) { printf("Alocacao disciplina t  "); return NULL;}
	char *t2 = (char *)malloc(sizeof(char)*5);	
	if(!t2) {printf("Alocacao disciplina t2 "); free(t); return NULL;}
	char *t1 = (char *)malloc(sizeof(char)*20);
	if(!t1) {printf("Alocacao disciplina t1 "); free(t); free(t2); return NULL;}

	arqi = fopen(arq,"r");
	if(!arqi) {printf("Erro na leitura"); free(t); free(t2); free(t1); return NULL;}
	fseek(arqi,posicao_arq+1,SEEK_CUR);

	fscanf (arqi, "%d", &qtddisc);
	discc = (disc_aux *)malloc(sizeof(disc_aux)*qtddisc);
	if(!discc) {printf("Alocacao disciplina"); free(t); free(t2); free(t1); return NULL;}

	for(i = 0; i < qtddisc;i++){
		discc[i].nome = NULL;
		fscanf(arqi,"%s %d %s %s", t, &j,t2,t1);
		strcpy(discc[i].cod,t);
		discc[i].periodo = j;
		strcpy(discc[i].cod_sem,t2);
		discc[i].nome = (char *)malloc(sizeof(char)*strlen(t1));
		if(!discc[i].nome) {printf("Alocacao disciplina %d nome",i); freeMem(discc,DISC_AUX); free(t); free(t2); free(t1);  return NULL;}
		strcpy(discc[i].nome,t1);
	
	
	
	}
	free(t);free(t1);free(t2);
	fclose(arqi);
	return discc;

}
