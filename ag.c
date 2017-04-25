
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
#define UM_PONTO 1
#define DOIS_PONTOS 2
#define QUATRO_PONTOS 4
//freeMem para os ponteiros globais do ag.c
#define VARGLOBAIS 111


int manha[10] = {0,1,2,3,4,5,6,7,8,9};
int tarde[10] = {10,11,12,13,14,15,16,17,18,19};
int noite[10] = {20,21,22,23,24,25,26,27,28,29};
int somadisc = 62;
int primeiro  =1;
int aux;
int par,impar;
int contador = 0;
//Variaveis comuns
prof_aux *pf    = NULL;
semestre *sm    = NULL;
disc_aux *dsa   = NULL;
semestre *auxsm = NULL;

/**
 * @function avaliacao
 *
 * @param indvo *ppl - recebe um individuo como parâmetro.
 *
 * A função é responsável por avaliar (fitness) o quão bom é o individuo em relação
 * aos horários da grade escolar. A análise ocorre através de quatro situações:
 * 1 : horários de mesma disciplina e mesmo professor que estão muito distribuídos recebem
 *		 1 ponto de penalidade.
 * 2 : períodos consecutivos de mesma disciplina são penalizados com
 *		 quatro pontos ao número de ocorrências.
 *	3 : Contabiliza os horários não preferidos pelos professores, a penalidade
 *		 para este caso é de um ponto.
 *	4 : Verifica a ocorrência de um mesmo professor dos quais estão selecionados
 *		 para horários de manhã e noite no mesmo dia. Para este caso, a penalidade
 *		 sobre as ocorrências é de dois pontos.
 *		 
 * A partir dessas situações, os valores são somados para fazer a avaliação
 *	posterior.
 * A função devolve OK, se a soma foi feita corretamente.	   	 
 * 
 **/ 

int  avaliacao(indvo *ppl){

	int i,j,tam,k;
	int aparece = 0,flag = 0;
	int soma = 0;
	int *hora = (int *)malloc(sizeof(int)*5);
	int *hora2 = (int *)malloc(sizeof(int)*5);
	char nome[20];
	//Contabiliza a distribuicao dos horarios	
	for(i = 0 ; i < qtddisc;i++){
		aparece = 0;
		tam  = 0;
		flag = 0;
		
		for(j = 0; j < ppl->qtd;j++){
			if(ppl->genes_indv[j].disc == i && ppl->genes_indv[j].periodo == 2){
				for(k = 0; k < tam;k++){
					if(hora[k] == ppl->genes_indv[j].dia_sem){flag = 1;break;}
				}
				
				if(!flag){hora[tam++] = ppl->genes_indv[j].dia_sem; }		
				
			}
		
		
		
		}
		
		soma += tam;
	}
	ppl->choques = soma;	
	soma = 0;
	
	//Penaliza horarios consecutivos
	for(i = 0 ; i < qtddisc;i++){
		tam  = 0;flag = 0;aparece = 0;
		strcpy(nome,dsa[i].nome);
		for(j = 0; j < ppl->qtd;j++){
			if(ppl->genes_indv[j].disc == i && ppl->genes_indv[j].periodo == 2 &&
			   strcmp(nome,ppl->genes_indv[j].prof)==0){
				if(!flag){				
					hora[tam++] = ppl->genes_indv[j].dia_sem;flag = 1;
					//strcpy(nome,ppl->genes_indv[j].prof);
				}
				else{
					if(hora[0]+1 == ppl->genes_indv[j].dia_sem)
						hora[tam++]= ppl->genes_indv[j].dia_sem;
					else if(hora[0] - 1 == ppl->genes_indv[j].dia_sem)
						hora[tam++]= ppl->genes_indv[j].dia_sem;			
								
				}
			}
			
			
		}
		if(tam > 1)
			soma+=tam*QUATRO_PONTOS;
	}
	ppl->qtdpr = soma;
	
	//Verifica os horarios nao preferidos
	
	soma = 0;
	for(i = 0; i < qtddisc;i++){
		aparece = 0;
		tam  = 0;
		flag = 0;
		
		strcpy(nome,dsa[i].nome);
		for(j = 0; j < ppl->qtd;j++){
			if(ppl->genes_indv[j].disc == i && strcmp(nome,ppl->genes_indv[j].prof)==0){
				for(k = 0; k < ppl->genes_indv[j].numpref;k++)
					if(ppl->genes_indv[j].notpref[k] == ppl->genes_indv[j].dia_sem)
						tam += 1;			
			}
			
		
		}
		soma += tam;
		
	
	
	}
	ppl->qtdpr += soma;
	
	soma = 0;
	//Verifica horario de manha e noite no mesmo dia
	//(00,01)->(20,21), (02,03)->(22,23)
	//(04,05)->(24,25), (06,07)->(26,27)
	//(08,09)->(28,29)
	for(i = 0 ; i < qtdprof; i++){
		tam = 0;
		aparece = 0;
		strcpy(nome,pf[i].nome);
		flag = 0;
		for(j = 0; j < ppl->qtd;j++){
			if(strcmp(nome,ppl->genes_indv[j].prof)==0){
				hora[tam] = ppl->genes_indv[j].dia_sem;
				hora2[tam++] = 1;
			}
			
		
		}
		int l;
		for(k = 0 ; k < tam; k++){
			int count = hora[k];
			hora2[k] = 0;
			//if(hora2[k] == 0)break;
			//if(hora2[k] == 0)continue;
			for(l = 0; l < tam;l++){
				if(hora2[l] == 0)continue;
				if(k!=l){
					if(hora[l] == 20 || hora[l] == 21)
						if(count == 0 || count == 1){aparece++; hora2[l] = 0;}
					if(hora[l] == 28 || hora[l] == 29)
						if(count == 8 || count == 9){aparece++; hora2[l] = 0;}
					if(hora[l] == 22 || hora[l] == 23)
						if(count == 2 || count == 3){aparece++; hora2[l] = 0;}
					if(hora[l] == 26 || hora[l] == 27)
						if(count == 6 || count == 7){aparece++; hora2[l] = 0;}
					if(hora[l] == 24 || hora[l]==25 ){
						if(count == 4 || count==5){aparece++; hora2[l] = 0;}
					}
				}
					
			}	
		}
		
		soma += (aparece * DOIS_PONTOS);
	}
	ppl->qtdpr += soma;
	return OK;

}

/**
 * @function testaRestricao
 *
 * @param indvo ppl - Recebe individuo como parâmetro.
 * @param disc_aux t - struct auxiliar onde estão as disciplinas.
 * @param int horario - Horário para o teste.
 *
 * A função verifica se a disciplina a ser adiciona respeita a 
 * restrição. Caso a verificação dê negativo, a função devolve 0;
 * senão devolve 1.
 * Os pares de horários não permitidos são : (21,02) , (23,04), (25,06), (27,08).
 * 
 * 
 **/ 
 
int testaRestricao(indvo ppl, disc_aux t, int horario){

	
	int i;
	for(i = 0; i<ppl.qtd;i++){
			if(ppl.genes_indv[i].dia_sem == 21 ){
				if(strcmp(ppl.genes_indv[i].prof,t.nome)==0 && horario==2)			
					return 0;
				
				
			}
			else if(ppl.genes_indv[i].dia_sem == 23){
				if(strcmp(ppl.genes_indv[i].prof,t.nome)==0 && horario==4)			
					return 0;
				
			
			
			}
			else if(ppl.genes_indv[i].dia_sem == 25) {
				if(strcmp(ppl.genes_indv[i].prof,t.nome)==0 && horario==6){			
					return 0;}
				
			}
			else if(ppl.genes_indv[i].dia_sem == 27){
				if(strcmp(ppl.genes_indv[i].prof,t.nome)==0 && horario==8)			
					return 0;
				
			
			
			}
			else if(ppl.genes_indv[i].dia_sem == 2){
				if(strcmp(ppl.genes_indv[i].prof,t.nome)==0 && horario==21)			
					return 0;
				
			
			
			}
			else if(ppl.genes_indv[i].dia_sem == 4){
				if(strcmp(ppl.genes_indv[i].prof,t.nome)==0 && horario==23)			
					return 0;
				
				//puts("putds");
			
			}
			else if(ppl.genes_indv[i].dia_sem == 6){
				if(strcmp(ppl.genes_indv[i].prof,t.nome)==0 && horario==25){	
					return 0;}
				
			
			
			}
			else if(ppl.genes_indv[i].dia_sem == 8){
				if(strcmp(ppl.genes_indv[i].prof,t.nome)==0 && horario==27)			
					return 0;
				
			
			
			}
 	
	
	}
	return 1;


}

/**
 * @function copiaVetor
 *
 * @param int *a - Variável que recebe a cópia
 * @param int *b - variável do qual sera copiado.
 * @param int tam - Tamanho de b
 *
 * Esta é uma função auxiliar, da qual realiza a 
 *	copia de um vetor para outro.
 * 
 **/
int *copiaVetor(int *a, int *b, int tam){

	int i;
	
	a = (int *)malloc(sizeof(int)*tam);
	if(!a)return NULL;
	for(i = 0; i < tam;i++){
		a[i] = b[i];	
	
	
	}
	return a;


}

/**
 * @function achaSala
 *
 * @param char *s1 - String contendo o semestre a ser pesquisado.
 * 
 * A função retorna um valor inteiro que representa a posição de 
 * cada semestre no vetor de semestres. 
 * 
 **/
int achaSala(char *s1){

	int i;
	for(i = 0; i < qtdsem;i++){
		if(strcmp(s1,"V1")==0)return 0;
		if(strcmp(s1,"M3")==0)return 1;
		if(strcmp(s1,"M5")==0)return 2;
		if(strcmp(s1,"M7")==0)return 3;
		if(strcmp(s1,"N2")==0)return 4;
		if(strcmp(s1,"N4")==0)return 5;
		if(strcmp(s1,"N6")==0)return 6;
		if(strcmp(s1,"N8")==0)return 7;
		if(strcmp(s1,"N10")==0)return 8;
		
		//if(strcmp(s1,"N5")==0)return 8;	
	
	}
	return -1;
}

/**
 * @function achaProf
 *
 * @param prof_aux *pf - vetor onde foi lido os professores do arquivo curso.dat
 * @param char *nome - String contento  o nome a ser pesquisado.
 *
 * A função devolve o índice no vetor onde esta o professor lido.
 * 
 **/
int achaProf(prof_aux *pf, char *nome){

	int i;
	for(i = 0; i < qtdprof;i++){
		if(strcmp(pf[i].nome,nome)==0)return i;	
	}
	return -1;


}

/**
 * @function copiaEst
 *
 * @param semestre *ss - recebe vetor de semestres a ser copiado.
 *
 * A função devolve uma cópia do vetor semestre *ss 
 * 
 **/

semestre *copiaEst(semestre *ss){
	if(!ss) return NULL;
	semestre *aux = NULL;
	int i,j;
	aux = (semestre *)malloc(sizeof(semestre)*qtdsem);
	if(!aux) return NULL;
	for(i = 0 ; i < qtdsem;i++){
		strcpy(aux[i].se,ss[i].se);
		aux[i].num = ss[i].num;
		aux[i].sala = ss[i].sala;
		aux[i].horarios = NULL;
		aux[i].horarios = (int *)malloc(sizeof(int)*aux[i].num);
		if(!aux[i].horarios){
			freeMem(aux,SEMESTRE); return NULL;}
		for( j = 0; j < aux[i].num;j++)
			aux[i].horarios[j] = ss[i].horarios[j];
	}	
	return aux;



}

/**
 * @function geraSala
 *
 * @param semestre *aux - Recebe um vetor do tipo semestre
 * @param int n - posição no vetor semestre *aux
 *
 * A função gera um valor em sequência no vetor aux, este valor 
 * posteriormente será usado para geração dos horários iniciais.
 * 
 **/

int geraSala(semestre *aux, int n){

	int i;
	
	for(i = 0; i < aux[n].num;i++)
		if(aux[n].horarios[i] >= 0) return i;	
	
	return -1;		
	
}

/**
 * @function testaParada
 *
 * @param int *v - recebe um vetor de inteiros
 * 
 * A função testa a parada do laço da função onde são gerados os horários.
 * A condição de parada se dá quando todos os horários já foram colocados 
 * na grade gerada. 
 * 
 **/
int testaParada(int *v){

	int i;
	int flag  =0;
	for(i = 0; i < qtddisc;i++)
		if(v[i] > 0){flag = 1;break;}
		
	return flag;



}

/**
 * @function quicksort2
 *
 * @param genes *vetor - Recebe um vetor do tipo genes 
 * @param int inicio - posição inicial no vetor
 * @param fim - última posição do vetor
 *
 *	Função Quicksort para a ordenação dos genes de um indivíduos,
 * no vetor a ordenado ocorre em cima do período selecionado.
 *  
 * //codigo quicksort em: 
 * //https://www.vivaolinux.com.br/script/Ordenacao-QuickSort
 * 
 **/
void quicksort2(genes *vetor, int inicio, int fim){
   
   int pivo, i, j, meio,k;
   genes *v1 = (genes *)malloc(sizeof(genes));
   i = inicio;
   j = fim;
   
   meio = (int) ((i + j) / 2);
   pivo = vetor[meio].dia_sem;
   
   do{
      while (vetor[i].dia_sem < pivo) i = i + 1;
      while (vetor[j].dia_sem > pivo) j = j - 1;
      
      if(i <= j){
      	v1->dia_sem = vetor[i].dia_sem;
      	v1->prof = (char *)malloc(sizeof(char)*20);
      	strcpy(v1->prof,vetor[i].prof);
      	v1->notpref = (int *)malloc(sizeof(int)*vetor[i].numpref);
      	for(k= 0; k <vetor[i].numpref;k++)
      		v1->notpref[k] = vetor[i].notpref[k];
      	v1->numpref = vetor[i].numpref;
      	v1->disc = vetor[i].disc;
      	v1->sala = vetor[i].sala;
      	v1->sala_id = vetor[i]. sala_id;
      	strcpy(v1->sem,vetor[i].sem);
         
         vetor[i].dia_sem = vetor[j].dia_sem;
         strcpy(vetor[i].prof,vetor[j].prof);
         for(k = 0; k < vetor[j].numpref;k++)
         	vetor[i].notpref[k] = vetor[j].notpref[k];
         vetor[i].numpref = vetor[j].numpref;
         vetor[i].disc = vetor[j].disc;
         vetor[i].sala  =vetor[j].sala;
         vetor[i].sala_id = vetor[j].sala_id;
         strcpy(vetor[i].sem,vetor[j].sem);
         
         vetor[j].dia_sem = v1->dia_sem;
         strcpy(vetor[j].prof,v1->prof);
         for(k = 0; k < v1->numpref;k++)
         	vetor[j].notpref[k] = v1->notpref[k];
         vetor[j].numpref = v1->numpref;
         vetor[j].disc = v1->disc;
         vetor[j].sala  =v1->sala;
         vetor[j].sala_id = v1->sala_id;
         strcpy(vetor[j].sem,v1->sem);
         
         
         i = i + 1;
         j = j - 1;
      }
   }while(j > i);
   
   if(inicio < j) quicksort2(vetor, inicio, j);
   if(i < fim) quicksort2(vetor, i, fim);   

}


/**
 * @function imprimeaux
 *
 * @param indvo *ppl - Vetor do tipo indivíduo
 * @param char semestre - Recebe o semestre a ser testado.
 * 
 * Esta é uma função auxiliar para a função de impressão, ela
 * selecionada no vetor ppl todas as posições referentes ao
 * semestre. 
 * 
 **/ 

int *imprimeaux(indvo *ppl, char semestre[4]){
	int i;
	aux = 0;
	int a = ppl->qtd;
	int *v = (int * )malloc(sizeof(int)*10);
	
	par = 0;impar =0;
	for(i = 0 ; i < a; i++){
		if(strcmp(ppl->genes_indv[i].sem,semestre)==0){
			v[aux++] = i;	
	
		
			if(ppl->genes_indv[i].dia_sem%2==0){
				if(ppl->genes_indv[i].dia_sem > par)
					par = ppl->genes_indv[i].dia_sem;
			}
			else{
				if(ppl->genes_indv[i].dia_sem > impar)
					impar = ppl->genes_indv[i].dia_sem;		
			}
	
		}
	}
	return v;
}


/**
 * @function imprimeaux2
 *
 * @param indvo *ppl - Vetor do tipo indivíduo
 * @param int *v - Vetor calculado na função imprimeaux
 * @param char turn - carácter referente ao turno matutino ou noturno.
 *
 * A função permite formar um vetor útil para imprimir os horários em forma
 * de tabela. Sendo assim, os horários onde não estão nos genes gerados,
 * é colocado uma flag de -1 para não imprimir posteriormente.  
 * 
 **/ 
int *imprimeaux2(indvo *ppl, int * v, char turn){
	int j = 0;
	int i,k;
	int temp;
	int *v2 = (int * )malloc(sizeof(int)*20);
	int *v1 = (int * )malloc(sizeof(int)*10);
	//int *v3 = (int * )malloc(sizeof(int)*20);
	for(i = 0; i < aux;i++){
		
		v1[i] = ppl->genes_indv[v[i]].dia_sem;	
	}
	//Caso for um semestre matutino
	if(turn == 'M'){
		temp = 0;
	
		//Copia valores pares menores que 10
		for(i = 0; i < aux; i++){
			if((v1[i]%2 == 0 && v1[i] < 10) && v1[i]==temp){
				v2[j] = v1[i];
				j++;
				temp+=2;
			}
			else if((v1[i]%2 == 0 && v1[i] < 10) && v1[i]!=temp){
				v2[j] = -1;
				j++;i--;temp+=2;			
			
			}
			
		}
		if(v1[i-1] < 8){
			if(j == 0){
				for(i = 0; i < 5;i++)
				v2[j++] = -1;
					
			}
			if(j == 1){
				for(i = 0; i < 4;i++)
				v2[j++] = -1;
			}
			
			if(j == 2){
				for(i = 0; i < 3;i++)
				v2[j++] = -1;
			}
			if(j == 3){
				for(i = 0; i < 2;i++)
				v2[j++] = -1;
			}
			if(j == 4){
				v2[j++] = -1;
			}	
			
		}
		temp = 1;
		//Copia valores impares menores que 10
		for(i = 0 ; i < aux; i++){
			if((v1[i]%2 != 0 && v1[i] < 10)&& v1[i]==temp){
				v2[j] = v1[i];
				j++;temp+=2;
			
			}
			else if((v1[i]%2!=0 && v1[i] < 10) && v1[i]!=temp){
				v2[j] = -1;			
				j++;i--;temp+=2;
			} 	
		}
		if(v1[i-1] < 9){
			if(j == 6){
				for(i = 0; i < 4;i++)
				v2[j++] = -1;
					
			}
			if(j == 7){
				for(i = 0; i < 3;i++)
				v2[j++] = -1;
			}
			
			if(j == 8){
				for(i = 0; i < 2;i++)
				v2[j++] = -1;
			}
			if(j == 9){
				
				v2[j++] = -1;
			}
					
		
		
		}
		
		temp = 10;
		//Copia valores pares maiores que 9
		for(i = 0; i < aux; i++){
			if((v1[i]%2 == 0 && v1[i] >= 10) && v1[i]==temp){
				v2[j] = v1[i];
				j++;temp+=2;
			
			}
			else if((v1[i]%2==0 && v1[i] >= 10) && v1[i]!=temp){
				v2[j] = -1;
				j++;i--;temp+=2;			
			
			}	
			
		}
		if(v1[i-1] < 18){
			if(j == 11){
				for(i = 0; i < 3;i++)
				v2[j++] = -1;
					
			}
			if(j == 12){
				for(i = 0; i < 2;i++)
				v2[j++] = -1;
			}
			
			if(j == 13){
				for(i = 0; i < 1;i++)
				v2[j++] = -1;
			}
			if(j == 14){
				v2[j++] = -1;
			}
					
			
		
		
		}
		temp = 11;
			
		//Copia valores impares maiores que 9
		for(i = 0 ; i < aux; i++){
			if((v1[i]%2 != 0 && v1[i] >= 10) && v1[i]==temp){
				v2[j] = v1[i];
				j++;temp+=2;
			
			}
			else if((v1[i]%2!=0 && v1[i] >=10) && v1[i]!=temp){
				v2[j] = -1;
				j++;i--;temp+=2;
			
			} 	
		}
		if(j < 20){
			for(; j < 20;j++)	
				v2[j] = -1;
		}
		
		
	}
	//caso o semestre for noturno
	if(turn=='N'){
		
		j = 0;
		temp = 20;
		for(i = 0; i < aux; i++){
			if((v1[i]%2 == 0 && v1[i]==temp)){
				v2[j] = v1[i];
				j++;
				temp+=2;
			}
			else if((v1[i]%2 == 0 && v1[i]!=temp)){
				v2[j] = -1;
				j++;i--;temp+=2;			
			
			}
		}
		if(v1[i-1] < 28){
			if(j == 0){
				for(i = 0; i < 5;i++)
				v2[j++] = -1;
					
			}
			if(j == 1){
				for(i = 0; i < 4;i++)
				v2[j++] = -1;
			}
			
			if(j == 2){
				for(i = 0; i < 3;i++)
				v2[j++] = -1;
			}
			if(j == 3){
				for(i = 0; i < 2;i++)
				v2[j++] = -1;
			}
			if(j == 4){
				v2[j++] = -1;
			}		
		
		
		}
		temp = 21;
		for(i = 0; i < aux; i++){
			if((v1[i]%2 != 0 && v1[i]==temp)){
				v2[j] = v1[i];
				j++;
				temp+=2;
			}
			else if((v1[i]%2 != 0 && v1[i]!=temp)){
				v2[j] = -1;
				j++;i--;temp+=2;			
			
			}
		}
		if(j < 10)
			for(;j < 10;j++)
				v2[j] = -1;
		
	}
	if(turn=='M')temp = 20;
	else temp = 10;
	
	for(k = 0; k < temp; k++){
		for(i = 0; i < aux;i++){
			if(v2[k]==v1[i] && v2[k]!=-1){
				v2[k] = v[i];		
				break;
			}
		}				
	}
	
	return v2;
		

}

/*int pertenceA(int *v,int test){
	int i,j;
	for(i = 0; i < aux;i++){
		if(v[i] == test)return 1;
	}
	return 0;


}*/


/**
 * @function imprime
 *
 * @param indvo *ppl - vetor de indivíduo
 *
 * A função é responsável por imprimir os horários dos semestres
 * em forma de tabela. Para cada semestre, é imprimido os horários
 * de forma separada.
 *  
 **/ 
 
void imprime(indvo *ppl){
	if(!ppl){ printf("individuo NULL:void imprime"); return;}

	int i,j,k;
	int *v = NULL,*v2 = NULL ;
	int ispar = 2;
	
	for(i  = 0; i < qtdsem;i++){
		j = 0;ispar = 2;k=0;
		printf("\nSemestre : %s / Sala: %d\n", sm[i].se,sm[i].sala);
		printf("---------------------------------------------------------------------------|\n");
	   printf("|-----|  Segunda     |  Terça     |  Quarta     |  Quinta     |  Sexta     |\n");
	   printf("___________________________________________________________________________|\n");
		//Seleciona os horarios do semestre v1		
		if(strcmp(sm[i].se,"V1")==0){
			
	  		printf("13:30-|");
	  		v= imprimeaux(ppl,"V1");
			while(1){
				if(ispar%2==0 && ppl->genes_indv[v[j]].dia_sem%2 == 0){
					printf("%s(%s)|", dsa[ppl->genes_indv[v[j]].disc].cod,ppl->genes_indv[v[j]].prof);
					
					if(ppl->genes_indv[v[j]].dia_sem==18){
						ispar = 1;j=0;
						printf("\n");printf("15:20-|");					
					}				
				
				
				}
				else if(ispar%2!=0 && ppl->genes_indv[v[j]].dia_sem%2!=0){
					printf("%s(%s)|", dsa[ppl->genes_indv[v[j]].disc].cod,ppl->genes_indv[v[j]].prof);
					
					if(ppl->genes_indv[v[j]].dia_sem==19){
						ispar = 2;free(v);
						printf("\n");					
						break;
					}				
				
				
				}
				if(j == aux)j=0;
				else j++;			
				
			
			}
		}
		//Seleciona os horarios do matutino
		else if(sm[i].se[0]=='M'){
	//else if(strcmp(sm[i].se,"M3")==0)	{
			v= imprimeaux(ppl,sm[i].se);
			v2 = imprimeaux2(ppl,v,sm[i].se[0]);
		
			printf("07:30-|");
			for(k = 0; k < 20; k ++){
				if(v2[k]!= -1)
					printf("%d %s(%s)|", ppl->genes_indv[v2[k]].dia_sem,dsa[ppl->genes_indv[v2[k]].disc].cod,ppl->genes_indv[v2[k]].prof);
				else printf(" --------- |");	
				if(k >= 4 && k<5){
					printf("\n10:10-|");
			
				}
					
				else if(k >=9 && k < 10)
					printf("\n13:30-|"); 
		
				else if(k >=14 && k < 15)
					printf("\n15:20-|");
			}
			printf("\n");
		}
		//seleciona os horarios do noturno
		else if(sm[i].se[0]=='N'){
		
			v= imprimeaux(ppl,sm[i].se);
		
			v2 = imprimeaux2(ppl,v,sm[i].se[0]);
		
			printf("19:10-|");
			for(k = 0; k < 10;k++){
				if(v2[k]!=-1)		
					printf("%d %s(%s)|", ppl->genes_indv[v2[k]].dia_sem,dsa[ppl->genes_indv[v2[k]].disc].cod,ppl->genes_indv[v2[k]].prof);
				else printf(" -------- |");
				if(k>=4 && k < 5)
					printf("\n21:00-|");
		
		
			}
	
	
			printf("\n");
		}		
		
		
		
	}

}


/**
 * @function criaIndv
 *
 * @param indvo *ppl - Recebe vetor de indivíduo
 *
 * A função cria um vetor do tipo indivíduo e copia
 * os dados do vetor ppl. 
 * 
 **/ 
indvo *criaIndv(indvo *ppl){

	indvo *nv      = NULL;
	int i,j;
	nv             = (indvo *)malloc(sizeof(indvo));
	if(!nv)   
		return NULL;// verificacao
	nv->genes_indv = NULL;
	nv->qtd        = ppl->qtd;
	nv->qtdpr      = ppl->qtdpr;
	nv->choques    = ppl->choques;
	nv->genes_indv = (genes *)malloc(sizeof(genes)*150);
	if(!nv->genes_indv){ 
		freeMem(nv,INDVO); return NULL;}
	for(i = 0; i < ppl->qtd;i++){
		nv->genes_indv[i].dia_sem = ppl->genes_indv[i].dia_sem;
		nv->genes_indv[i].prof    = NULL;
		nv->genes_indv[i].prof    = (char *)malloc(sizeof(char)*20);
		if(!nv->genes_indv[i].prof){
			freeMem(nv,INDVO); return NULL;}
		strcpy(nv->genes_indv[i].prof,ppl->genes_indv[i].prof);
		nv->genes_indv[i].notpref = NULL;
		nv->genes_indv[i].notpref = (int *)malloc(sizeof(int)*ppl->genes_indv[i].numpref);
		if(!nv->genes_indv[i].notpref){
			freeMem(nv,INDVO); return NULL;}
		
		for(j = 0; j < ppl->genes_indv[i].numpref;j++){
			nv->genes_indv[i].notpref[j] = ppl->genes_indv[i].notpref[j]; 		
		
		}
		nv->genes_indv[i].numpref = ppl->genes_indv[i].numpref;
		nv->genes_indv[i].disc    = ppl->genes_indv[i].disc;
		nv->genes_indv[i].sala    = ppl->genes_indv[i].sala;
		nv->genes_indv[i].sala_id = ppl->genes_indv[i].sala_id;
		nv->genes_indv[i].periodo = ppl->genes_indv[i].periodo;
		strcpy(nv->genes_indv[i].sem, ppl->genes_indv[i].sem);
	}
	return nv;
}


/**
 * @function cruzamento
 *
 * @param indvo *ppl1- Recebe um vetor do tipo indivíduo
 * @param indvo *ppl2 -Recebe outro vetor do tipo indivíduo
 *
 * A função faz o cruzamento de dois indivíduos. Essa operação 
 * ocorre da seguinte forma:
 * Primeiramente é selecionado algum dos dois indivíduos passados por
 * parâmetro Assim, é selecionado algum (aleatório) dos genes do indivíduo que foi
 * selecionado, então é escolhido um gene no outro indivíduo, este gene é
 * colocado no horário do primeiro gene anteriormente selecionado. Para
 * evitar duplicações de horários, o primeiro gene substituído é realocado
 * em outra posição no vetor de genes.
 * Para simplificar a implementação, o cruzamento ocorre com genes do
 * mesmo semestre.
 * 
 **/ 
indvo *cruzamento(indvo *ppl1, indvo *ppl2){
	
	int escolha;
	int i,j,ki,k,out = 0;
	indvo *novo;
	genes *temp = (genes *)malloc(sizeof(genes)*150);
	if(!temp)return NULL;
	
	for(i=0;i<150;i++){
		temp[i].prof    = NULL;
		temp[i].notpref = NULL;
	}
	srand(time(NULL)+contador);
	contador+=10;
	int esc = rand()%2;
	if(!esc){
		novo = criaIndv(ppl1);
		if(!novo) freeMem(temp,GENES); return NULL;
	}
	else{ 
		novo = criaIndv(ppl2);
		if(!novo) freeMem(temp,GENES); return NULL;
	}
	
	int dv = rand()%novo->qtd;
	
	if(strcmp(novo->genes_indv[dv].sem,"V1")==0){
		
		//outro = 10+ rand()%10;
		if(!esc){
			for(i = 0; i < ppl2->qtd;i++)
				if(ppl2->genes_indv[i].sem[0]=='V') 
					escolha = i;
			
		}
		else{
			for(i = 0; i < ppl1->qtd;i++)
				if(ppl1->genes_indv[i].sem[0]=='V') 
					escolha = i;
				
		}
		//else outro = rand()%ppl1->qtd;
		
		
	}
	else if(strcmp(novo->genes_indv[dv].sem,"M3")==0){
		//outro = rand()%10;
		if(!esc){
			for(i = 0; i < ppl2->qtd;i++){
				if(strcmp(ppl2->genes_indv[i].sem,"M3")==0){
					escolha = i;break;
				}
			}
			
		}
		else{
			for(i = 0; i < ppl1->qtd;i++){
				if(strcmp(ppl1->genes_indv[i].sem,"M3")==0){ 
					escolha = i;break;
				}
			}
					
		}
		
		//else outro = rand()%ppl1->qtd;
	}
	else if(strcmp(novo->genes_indv[dv].sem,"M5")==0){
		
		//outro = rand()%10;
		if(!esc){
			for(i = 0; i < ppl2->qtd;i++)
				if(strcmp(ppl2->genes_indv[i].sem,"M5")==0)
					escolha = i;
			
		}
		else{
			for(i = 0; i < ppl1->qtd;i++)
				if(strcmp(ppl1->genes_indv[i].sem,"M5")==0)
					escolha = i;		
					
		}
		
		//else outro = rand()%ppl1->qtd;
	}
	else if(strcmp(novo->genes_indv[dv].sem,"M7")==0){
		
		//outro = rand()%10;
		if(!esc){
			for(i = 0; i < ppl2->qtd;i++)
				if(strcmp(ppl2->genes_indv[i].sem,"M7")==0)
					escolha = i;
			
		}
		else{
			for(i = 0; i < ppl1->qtd;i++)
				if(strcmp(ppl1->genes_indv[i].sem,"M7")==0)
					escolha = i;		
		}
		
		//else outro = rand()%ppl1->qtd;
	}
	else if(strcmp(novo->genes_indv[dv].sem,"N2")==0){
		
		//outro = 20 +rand()%10;
		if(!esc){
			for(i = 0; i < ppl2->qtd;i++)
				if(strcmp(ppl2->genes_indv[i].sem,"N2")==0) 
					escolha = i;
				
		}
		else{
			for(i = 0; i < ppl1->qtd;i++)
				if(strcmp(ppl1->genes_indv[i].sem,"N2")==0) 
					escolha = i;		
		}
		
		//else outro = rand()%ppl1->qtd;
	}
	else if(strcmp(novo->genes_indv[dv].sem,"N4")==0){
		
		//outro = 20  + rand()%10;
		if(!esc){
			for(i = 0; i < ppl2->qtd;i++)
				if(strcmp(ppl2->genes_indv[i].sem,"N4")==0)
					escolha = i;
				
		}
		else{
			for(i = 0; i < ppl1->qtd;i++)
				if(strcmp(ppl1->genes_indv[i].sem,"N4")==0 )
					escolha = i;		
				
		}
		
		
		//else outro = rand()%ppl1->qtd;
	}
	else if(strcmp(novo->genes_indv[dv].sem,"N6")==0){
		
		//outro = 20 + rand()%10;
		if(!esc){
			for(i = 0; i < ppl2->qtd;i++)
				if(strcmp(ppl2->genes_indv[i].sem,"N6")==0 )
					escolha = i;
				
		}
		else{
			for(i = 0; i < ppl1->qtd;i++)
				if(strcmp(ppl1->genes_indv[i].sem,"N6")==0 )
					escolha = i;		
		}
		
		//else outro = rand()%ppl1->qtd;
	}
	else if(strcmp(novo->genes_indv[dv].sem,"N8")==0){
		
		//outro = 20 + rand()%10;
		if(!esc){
			for(i = 0; i < ppl2->qtd;i++)
				if(strcmp(ppl2->genes_indv[i].sem,"N8")==0 )
					escolha = i;
				
		}
		else{
			for(i = 0; i < ppl1->qtd;i++)
				if(strcmp(ppl1->genes_indv[i].sem,"N8")==0 )
					escolha = i;		
		
		}
		
		//else outro = rand()%ppl1->qtd;
	}
	else if(strcmp(novo->genes_indv[dv].sem,"N10")==0){
		
		//outro = 20 + rand()%10;
		if(!esc){
			for(i = 0; i < ppl2->qtd;i++)
				if(strcmp(ppl2->genes_indv[i].sem,"N10")==0 )
					escolha = i;
		}
		else{
			for(i = 0; i < ppl1->qtd;i++)
				if(strcmp(ppl1->genes_indv[i].sem,"N10")==0 )
					escolha = i;		
		}
		
		//else outro = rand()%ppl1->qtd;
	}
	
	temp->dia_sem = novo->genes_indv[dv].dia_sem;
	temp->prof = (char *)malloc(sizeof(char)*20);
	if(!temp->prof){
		freeMem(temp,GENES); freeMem(novo,INDVO); return NULL;	
	
	}
	temp->notpref = (int *)malloc(sizeof(int)*novo->genes_indv[dv].numpref);
	if(!temp->notpref){freeMem(temp,GENES); freeMem(novo,INDVO); return NULL;}
	
	strcpy(temp->prof, novo->genes_indv[dv].prof);
	for(i = 0;  i < novo->genes_indv[dv].numpref;i++)
		temp->notpref[i] = novo->genes_indv[dv].notpref[i];
	temp->numpref = novo->genes_indv[dv].numpref;
	temp->disc = novo->genes_indv[dv].disc;
	temp->sala = novo->genes_indv[dv].sala;
	temp->sala_id = novo->genes_indv[dv].sala_id;
	temp->periodo = novo->genes_indv[dv].periodo;
	strcpy(temp->sem, novo->genes_indv[dv].sem);
	
	if(!esc){
		
		if(!testaRestricao(*novo, dsa[ppl2->genes_indv[escolha].disc], 
			novo->genes_indv[dv].dia_sem)){
				freeMem(temp,GENES);freeMem(novo,INDVO);return NULL;}
		
		for(ki = 0; k < novo->qtd;ki++){
			if(novo->genes_indv[k].dia_sem == ppl2->genes_indv[escolha].dia_sem &&		
				novo->genes_indv[k].disc == ppl2->genes_indv[escolha].disc &&
				strcmp(novo->genes_indv[k].prof,ppl2->genes_indv[escolha].prof)==0){
				freeMem(temp,GENES);freeMem(novo,INDVO);return NULL;}
		
		}
		
		if(!out)
			for(ki = 0; ki < novo->qtd;ki++){
				if(strcmp(novo->genes_indv[ki].prof,ppl2->genes_indv[escolha].prof)==0 &&
					novo->genes_indv[ki].disc == ppl2->genes_indv[escolha].disc){
				
					break;
				}
			}
		

		strcpy(novo->genes_indv[dv].prof,ppl2->genes_indv[escolha].prof);
		for(i = 0; i < ppl2->genes_indv[escolha].numpref;i++)
			novo->genes_indv[dv].notpref[i] = ppl2->genes_indv[escolha].notpref[i];
		novo->genes_indv[dv].numpref = ppl2->genes_indv[escolha].numpref;
		novo->genes_indv[dv].disc = ppl2->genes_indv[escolha].disc;
		novo->genes_indv[dv].sala = ppl2->genes_indv[escolha].sala;
		novo->genes_indv[dv].sala_id = ppl2->genes_indv[escolha].sala_id;
		novo->genes_indv[dv].periodo =  ppl2->genes_indv[escolha].periodo;
		strcpy(novo->genes_indv[dv].sem,ppl2->genes_indv[escolha].sem);

		if(!testaRestricao(*novo, dsa[temp->disc], ppl2->genes_indv[escolha].dia_sem)){
			freeMem(temp,GENES);freeMem(novo,INDVO);return NULL;		
		
		}
		//return NULL;
	}
	else{
	
		if(!testaRestricao(*novo, dsa[ppl1->genes_indv[escolha].disc], 
			novo->genes_indv[dv].dia_sem)){
			freeMem(temp,GENES);freeMem(novo,INDVO);return NULL;		
		}
		
		for(ki = 0; k < novo->qtd;ki++){
			if(novo->genes_indv[k].dia_sem == ppl1->genes_indv[escolha].dia_sem &&		
				novo->genes_indv[k].disc == ppl1->genes_indv[escolha].disc &&
				strcmp(novo->genes_indv[k].prof,ppl1->genes_indv[escolha].prof)==0){
				
					freeMem(temp,GENES);freeMem(novo,INDVO);return NULL;				
				
				}
			
		
		}
		
		if(!out)
			for(ki = 0; ki < novo->qtd;ki++){
				if(strcmp(novo->genes_indv[ki].prof,ppl2->genes_indv[escolha].prof)==0 &&
					novo->genes_indv[ki].disc == ppl2->genes_indv[escolha].disc){
					break;
				}
			}
		
		strcpy(novo->genes_indv[dv].prof,ppl1->genes_indv[escolha].prof);
		for(i = 0; i < ppl1->genes_indv[escolha].numpref;i++)
			novo->genes_indv[dv].notpref[i] = ppl1->genes_indv[escolha].notpref[i];
		novo->genes_indv[dv].numpref = ppl1->genes_indv[escolha].numpref;
		novo->genes_indv[dv].disc = ppl1->genes_indv[escolha].disc;
		novo->genes_indv[dv].sala = ppl1->genes_indv[escolha].sala;
		novo->genes_indv[dv].sala_id = ppl1->genes_indv[escolha].sala_id;
		novo->genes_indv[dv].periodo =  ppl1->genes_indv[escolha].periodo;
		strcpy(novo->genes_indv[dv].sem,ppl1->genes_indv[escolha].sem);
		
		if(!testaRestricao(*novo, dsa[temp->disc], ppl1->genes_indv[escolha].dia_sem)){
			freeMem(temp,GENES);freeMem(novo,INDVO);return NULL;
		}
			
		for(j = 0; j < novo->qtd;j++){
			if(novo->genes_indv[j].dia_sem == novo->genes_indv[ki].dia_sem &&		
				novo->genes_indv[j].disc == novo->genes_indv[ki].disc &&
				strcmp(novo->genes_indv[j].prof,novo->genes_indv[ki].prof)==0){
				freeMem(temp,GENES);freeMem(novo,INDVO);return NULL;
			}
		}		
		
	
	}
	
	
	strcpy(novo->genes_indv[ki].prof,temp->prof);
	if(novo->genes_indv[ki].notpref!=NULL)
		free(novo->genes_indv[ki].notpref);
	novo->genes_indv[ki].notpref = NULL;
	novo->genes_indv[ki].notpref = (int *)malloc(sizeof(int)*temp->numpref);//puts("inde7");
	if(!novo->genes_indv[ki].notpref){
		freeMem(temp,GENES);freeMem(novo,INDVO);return NULL;	
	
	}	
	
	for(j = 0;  j < temp->numpref;j++)
		novo->genes_indv[ki].notpref[j]=temp->notpref[j];
	novo->genes_indv[ki].numpref= temp->numpref;
	novo->genes_indv[ki].disc = temp->disc;
	novo->genes_indv[ki].sala = temp->sala;
	novo->genes_indv[ki].sala_id = temp->sala_id;
	novo->genes_indv[ki].periodo = temp->periodo;
	strcpy(novo->genes_indv[ki].sem,temp->sem);
	
	quicksort2(novo->genes_indv,0,novo->qtd-1);
	avaliacao(novo);
	
	
	
	return novo;
}


/**
 * @function mutacao
 * @param indvo *ppl - vetor do indivíduo
 * 
 * A função de mutação seleciona um gene no vetor de um indivíduo,
 * então é selecionado um horário livre para trocar, ou seja, troca
 * um horário preenchido por um livre. Sendo assim, a mutação ocorre
 * em cima da troca de horário ( para facilitar a implementação, foi 
 * utilizado a mutação apenas em semestres onde possui horários livres
 * para suas respectivas salas).
 * 
 * 
 **/ 
int mutacao(indvo *ppl){
	if(!ppl) return ERROINDIVIDUO;
	int r = rand() % ppl->qtd;
	
	int numsala;
	
	int distt,i,j,flag;
	int x = 0;
	
	//if(!sm) return ERRONALEITURA;
	auxsm = copiaEst(sm);
	if(!auxsm) return ERROALOCACAO;
	if(strcmp(ppl->genes_indv[r].sem,"V1")==0){
			numsala = achaSala(ppl->genes_indv[r].sem);
	}
	else if(ppl->genes_indv[r].sem[0]=='M'){
			numsala = achaSala(ppl->genes_indv[r].sem);	
	}
	else if(ppl->genes_indv[r].sem[0]=='N'){
			numsala = achaSala(ppl->genes_indv[r].sem);	
	}
		
		
		
	for(i = 0 ; i < auxsm[numsala].num;i++){
		for(j = 0; j < ppl->qtd;j++){
			if(strcmp(ppl->genes_indv[j].sem,auxsm[numsala].se)==0 && (ppl->genes_indv[j].dia_sem == auxsm[numsala].horarios[i]))
				auxsm[numsala].horarios[i] = -1;
		}
	}
	int tee = 0;	
	for(i = 0; i < auxsm[numsala].num;i++){
		if(auxsm[numsala].horarios[i]!=-1)
			tee = 1;		
	}
	if(tee == 0 ){
		return ERROMUTACAO;
	}
	
	if(ppl->genes_indv[r].dia_sem != auxsm[numsala].horarios[auxsm[numsala].num-1]){
		for(i = 0; i < auxsm[numsala].num;i++){
			if((auxsm[numsala].horarios[i]!= -1 && 
				auxsm[numsala].horarios[i]!= ppl->genes_indv[r].dia_sem)){
					distt = i;flag = 1;break;				
				} 
		}
		
		while(1){
			flag = 1;
			if(!testaRestricao(*ppl, dsa[ppl->genes_indv[r].disc], auxsm[numsala].horarios[distt])){
				
				for(i = distt; i < auxsm[numsala].num;i++){
					if((auxsm[numsala].horarios[i]!= -1) && 
					(auxsm[numsala].horarios[i]!= auxsm[numsala].horarios[distt])){
						distt = i;flag = 0;break;				
					} 			
			
				}					
				if(i>=auxsm[numsala].horarios[auxsm[numsala].num])
					return ERROMUTACAO;
					
			}
			for(i = 0; i < ppl->qtd;i++){
				if(ppl->genes_indv[i].dia_sem == auxsm[numsala].horarios[distt] &&
				strcmp(ppl->genes_indv[i].sem,sm[ppl->genes_indv[i].sala_id].se)==0){
					int dfi;	
							
					for(dfi = distt; dfi < auxsm[numsala].num;dfi++){
					//if((auxsm[numsala].horarios[dfi] != auxsm[numsala].horarios[distt]) &&
						if(auxsm[numsala].horarios[dfi] >= 0){distt = dfi;flag=0;break;}			
						}
					if(i>=auxsm[numsala].horarios[auxsm[numsala].num])
						return ERROMUTACAO;
					//continue;
				}
			}
			
			if(flag == 1)break;
			x++;
			if(x >= sm[numsala].num)return ERROMUTACAO;
		}
		ppl->genes_indv[r].dia_sem = auxsm[numsala].horarios[distt];
		quicksort2(ppl->genes_indv,0,ppl->qtd-1);
		avaliacao(ppl);
		return OK;
	}
	else return ERROMUTACAO;
	//distt = geraSala(auxsm,numsala);
		

}

/**
 * @function geraIndividuos
 *
 * @param indvo *ppl - Vetor do tipo indivíduo
 * @param char *arq - Nome do arquivo 'curso.dat' para leitura
 * 
 * A função é responsável por gerar um indivíduo válido. Nesta
 * rotina, para adicionar cada horário na grade, é verificado a
 * condição de restrição, e também não permite incluir horários
 * cuja os professores e disciplinas estejam no mesmo período. A ordem
 * de seleção das disciplinas é feita de forma aleatória, assim como os 
 * horários referentes a cada semestre em suas respectivas salas. Por último,
 * é acrescentado uma avaliação inicial para o indivíduo gerado(i.e. fitness).
 * 
 **/ 

int geraIndividuos(indvo *ppl, char *arq){


	int i,j,flag = 0,numsala;
	int discomp = 0, distt,test;
	//plcao *ppl = NULL;
	
	ppl->qtd =0;
	ppl->choques = 0;
	ppl->qtdpr = 0;
	ppl->genes_indv = (genes *)malloc(sizeof(genes)*150);
	if(!ppl->genes_indv){ freeMem(auxsm,VARGLOBAIS); return ERROALOCACAO;}
	for(i=0;i<150;i++){ppl->genes_indv[i].prof = NULL; ppl->genes_indv[i].notpref = NULL;}
	
	
	//Lê apenas uma vez, porque estas structs são usadas em outras funções depois.
	if(primeiro){
		pf = leProfessores(arq);	
		if(!pf){freeMem(ppl,INDVO); return ERROALOCACAO;} // deve ficar junto, assim não sera usado se for invalido.
		sm = leSemestre(arq);
		if(!sm){freeMem(ppl,INDVO); freeMem(pf,VARGLOBAIS); return ERROALOCACAO;}
		dsa = leDisciplina(arq);
		if(!dsa){freeMem(ppl,INDVO); freeMem(sm,VARGLOBAIS); return ERROALOCACAO;}
		primeiro = 0;
	}
	

	//sm = leSemestre(arq);
	//dsa = leDisciplina(arq);//nao tem pra que retirar essa linha sendo que essa variavel, nao e global e esta sendo usada nos strcmp abaixo.
	auxsm = copiaEst(sm);
	if(!auxsm){freeMem(ppl,INDVO); freeMem(dsa,VARGLOBAIS); return ERROALOCACAO;}

	
	//auxsm = copiaEst(sm);
		
	
	int *v = (int *)malloc(sizeof(int)*qtddisc);
	if(!v){freeMem(ppl,INDVO); freeMem(auxsm,VARGLOBAIS); return ERROALOCACAO;}
	//aqui ja começa a ficar grande as liberações
	for(i =0 ; i < qtddisc;i++){
		v[i] = dsa[i].periodo; 
	} 

	
	discomp = testaParada(v);
	srand(time(NULL) + contador);
	contador+=10;
	int x = 0;
	
	while(discomp){
		//Escolhe disciplina aleatoria
	
		//caso execução ultrapasse 70 passos, a geração é abortada,
		//pois o mesmo é inválido.
		if(x > 70){
			freeMem(ppl->genes_indv,GENES);
			return ERROINDIVIDUO;				
		
		}		
		//srand(0);
		j = rand() %qtddisc;
		
	
		if(v[j] < 1){
			for(i = 0; i < qtddisc;i++)		
				if(v[i] >0){ j = i;}
		}
		
		if(strcmp(dsa[j].cod_sem,"V1")==0){
			//temp = rand() % 10;turno = 0;
			numsala = achaSala(dsa[j].cod_sem);
		}
		else if(dsa[j].cod_sem[0]=='M'){
			//temp = 10 + rand() % 10;turno = 1;
			numsala = achaSala(dsa[j].cod_sem);	
		}
		else if(dsa[j].cod_sem[0]=='N'){
			//temp = 20 + rand() % 10;turno = 2;
			numsala = achaSala(dsa[j].cod_sem);	
		}
		
		
		distt = geraSala(auxsm,numsala);
		
		if(distt < 0)flag = 1;
	
		if(!testaRestricao(*ppl, dsa[j], auxsm[numsala].horarios[distt])){
			
				int dfi;
			
			
				for(dfi = 0; dfi < auxsm[numsala].num;dfi++)
					if((auxsm[numsala].horarios[dfi] != auxsm[numsala].horarios[distt]) &&
						auxsm[numsala].horarios[dfi] >= 0){distt = dfi;break;}			
		
				if(dfi != distt){flag= 1;} 
				
		}
		
		for(i = 0; i < ppl->qtd;i++){
			if(ppl->genes_indv[i].dia_sem == auxsm[numsala].horarios[distt] &&
				strcmp(ppl->genes_indv[i].prof,dsa[j].nome)==0){
				int dfi;				
				for(dfi = 0; dfi < auxsm[numsala].num;dfi++)
				if((auxsm[numsala].horarios[dfi] != auxsm[numsala].horarios[distt]) &&
					auxsm[numsala].horarios[dfi] >= 0){distt = dfi;break;}			
						
				if(dfi!=distt)flag = 1;	
					
					//continue;
			}		
		
			else if(ppl->genes_indv[i].dia_sem == auxsm[numsala].horarios[distt] &&
			strcmp(dsa[j].cod_sem,ppl->genes_indv[i].sem)==0){flag = 1; ;}
			
			
			/*else if(ppl->individuos[0]->genes_ind[i].dia_sem == temp &&
					 strcmp(ppl->individuos[0]->genes_ind[i].prof,dsa[j].nome)!=0){
			
			}*/
		
		}
		if(!flag){
			
			ppl->genes_indv[ppl->qtd].dia_sem = auxsm[numsala].horarios[distt];
			ppl->genes_indv[ppl->qtd].prof = (char *)malloc(sizeof(char)*20);
			if(!ppl->genes_indv[ppl->qtd].prof)return ERROALOCACAO;
				
			strcpy(ppl->genes_indv[ppl->qtd].prof,dsa[j].nome);
						
			test = achaProf(pf,dsa[j].nome);
			ppl->genes_indv[ppl->qtd].notpref = NULL;		
			ppl->genes_indv[ppl->qtd].notpref = 
					copiaVetor(ppl->genes_indv[ppl->qtd].notpref,
					pf[test].horarios, pf[test].num);		
			if(!ppl->genes_indv[ppl->qtd].notpref)
				return ERROALOCACAO;		
			ppl->genes_indv[ppl->qtd].numpref = pf[test].num;
			ppl->genes_indv[ppl->qtd].disc = j;
			ppl->genes_indv[ppl->qtd].sala = sm[numsala].sala;
			ppl->genes_indv[ppl->qtd].sala_id = numsala;	
			ppl->genes_indv[ppl->qtd].periodo = dsa[j].periodo; 			
					
			strcpy(ppl->genes_indv[ppl->qtd].sem,dsa[j].cod_sem);		
			
			
			if(v[j] == 1){
				v[j] = -1;			
			
			
			}
			else if(v[j] == 2) v[j]--;
			
			
			
			//Marca sala indisponivel			
			
			int ee = auxsm[numsala].sala;
			int b,a;
			for(b = 0; b < qtdsem;b++){
				if(auxsm[b].sala == ee ){
					for(a =0;a < auxsm[b].num;a++){
						if(auxsm[b].horarios[a]==auxsm[numsala].horarios[distt]){
							auxsm[b].horarios[a] = -1;
						}
					}
				}
			}
			auxsm[numsala].horarios[distt] = -1;
			
			ppl->qtd++;
			discomp = testaParada(v);
			
					
		}
		else flag = 0;
		
		
			
		x++;
		
	}
	
	quicksort2(ppl->genes_indv,0,ppl->qtd-1);
	
	avaliacao(ppl);
	
	return OK;
}

/**
 * @function eliminaPior
 *
 * @param plcao *pop - struct que contem toda a população
 * @param indvo *ppl - novo individuo oriundo do cruzamento
 *
 * A função procura na população um individuo com a pior pontuação, isto é,
 * com a maior soma de penalidades, assim posteriormente este individuo é 
 *  sobrescrito com o novo vindo do cruzamento.
 * 
 **/ 
int eliminaPior(plcao *pop, indvo *ppl){
	int i,j,id=0;
	int maior = 0;
	for(i = 0; i < TAM_POPULACAO;i++){
		if(pop->individuos[i].qtdpr + pop->individuos[i].choques > maior){
			maior = pop->individuos[i].qtdpr + pop->individuos[i].choques;
			id = i;
		}
	
	}
	pop->individuos[id].qtdpr = ppl->qtdpr;
	pop->individuos[id].choques = ppl->choques;
	pop->individuos[id].qtd = ppl->qtd;
	for(i = 0; i < ppl->qtd;i++){
		pop->individuos[id].genes_indv[i].dia_sem = ppl->genes_indv[i].dia_sem;
		strcpy(pop->individuos[id].genes_indv[i].prof, ppl->genes_indv[i].prof);
		
		
		pop->individuos[id].genes_indv[i].notpref = (int *)malloc(sizeof(int)*ppl->genes_indv[i].numpref);
		for(j = 0; j < ppl->genes_indv[i].numpref; j++)
			pop->individuos[id].genes_indv[i].notpref[j] = ppl->genes_indv[i].notpref[j];
		pop->individuos[id].genes_indv[i].numpref = ppl->genes_indv[i].numpref;
		pop->individuos[id].genes_indv[i].disc = ppl->genes_indv[i].disc;
		pop->individuos[id].genes_indv[i].sala = ppl->genes_indv[i].sala;
		pop->individuos[id].genes_indv[i].sala_id = ppl->genes_indv[i].sala_id;
		pop->individuos[id].genes_indv[i].periodo = ppl->genes_indv[i].periodo;
		strcpy(pop->individuos[id].genes_indv[i].sem , ppl->genes_indv[i].sem);
			
	}
	
	return id;
}

/**
 * @function achaMelhor
 *
 * @param plcao *pop - struct que contem toda a população
 * 
 *
 * A função procura na população um individuo com a melhor pontuação, isto é,
 * com a menor soma de penalidades, assim posteriormente este individuo é 
 * impresso na tela como resultado final após a execução do número de 
 * gerações.
 * 
 **/
int achaMelhor(plcao *pop){
	int i,id = 0;
	int menor = 999;
	for(i = 0; i < TAM_POPULACAO;i++){
		if(pop->individuos[i].qtdpr + pop->individuos[i].choques < menor){
			menor = pop->individuos[i].qtdpr + pop->individuos[i].choques;
			id = i;
			
		}
		
	}
	
	return id;
}

/**
 * @function freeMem
 *
 * @param void *algo - Elemento esperado para liberação
 * @param int component - Tipo do elemento a ser liberado
 *
 * Função auxiliar para liberação de memória 
 * 
 **/ 
void freeMem(void *algo,int component){ /// Liberar memoria alocadas de cada estrutura separadamente
	int i;
	if(algo == NULL){
		printf("Nada para Limpar nesse %d.\n",component);
		return;
	}
	//printf("FREEMEM\n");
	switch(component) {
		case VARGLOBAIS :{   /// ponteiros globais "pf","sm","dsa","auxsm"
			if(pf != NULL){
				if(sm != NULL){
					if(dsa != NULL){
						if(auxsm != NULL){
							freeMem(auxsm,SEMESTRE);
						}
						freeMem(dsa,DISC_AUX);
					}
					freeMem(sm,SEMESTRE);
				}
				freeMem(pf,PROF_AUX);
			}
			else if(auxsm != NULL){
				freeMem(auxsm,SEMESTRE);
			}
			else
				printf("ponteiros globais nao possuem memoria alocada\n");
			break;
		}
		case DISC_AUX :{   /// "disc_aux"
			disc_aux *a = (disc_aux *)algo;
			for(i=0;i<qtddisc;i++){
		//		printf("i: %d disciplina\n",i);
				if(a != NULL){
					free(a[i].nome);}}
			free(a);
			a=NULL;
			break;
		}
		case SEMESTRE:{   /// "semestre"
			semestre *a = (semestre *)algo;
			for(i=0;i<qtdsem;i++){
			//	printf("i: %d semestre\n",i);
				if(a != NULL){
					if(a[i].horarios!=NULL)
						free(a[i].horarios);}}
			free(a);
			a=NULL;
			break;
		}
		case PROF_AUX:{   /// "prof_aux"
			prof_aux *a = (prof_aux *)algo;
			for(i=0;i<qtdprof;i++){
			//	printf("i: %d prof\n",i);
				if(a != NULL){
					if(a[i].horarios!=NULL)
						free(a[i].horarios);
					if(a[i].nome!=NULL)
						free(a[i].nome);}}
			free(a);
			a=NULL;
			break;
		}	
		case PLCAO:{   /// "plcao"
			plcao *a = (plcao *)algo;
			freeMem(a->individuos,INDVO);
			free(a);
			a=NULL;
			break;
		}	
		case INDVO:{   /// "indvo"
			indvo *a = (indvo *)algo;
			for(i=0;i<TAM_POPULACAO;i++){
				//printf("i: %d indvo\n",i);
				if(a != NULL){
					if(a[i].genes_indv != NULL)
						freeMem(a[i].genes_indv,GENES);}}
			free(a);
			break;
		}
		case GENES:{   /// "genes"
			genes *a = (genes *)algo;
			for(i=0;i<150;i++){
			//	printf("i: %d genes\n",i);
				if(a != NULL){
					if(a[i].prof!=NULL)
						free(a[i].prof);
					if(a[i].notpref!=NULL)
						free(a[i].notpref);}}
			free(a);
			break;
		}
		default:{ /// essa funcão não será usada para liberar variaveis normais que só necessitam de um unico "free('var')";
			printf("Componente nao identificado");
			break;
		}
	}
}

//Função principal
int main(int argc, char *argv[ ] ){
	int i,j;

	plcao *populacao = NULL;
	indvo *new       = NULL;
	populacao        = (plcao *)malloc(sizeof(plcao));
	if(!populacao)return ERROALOCACAO;
	
	populacao->individuos = NULL;
	populacao->individuos = (indvo *)malloc(sizeof(indvo)*TAM_POPULACAO);
	if(!populacao->individuos){ freeMem(populacao,PLCAO); return ERROALOCACAO; }
	
	for(i = 0 ; i < TAM_POPULACAO;){
		populacao->individuos[i].genes_indv = NULL;
		if(geraIndividuos(&populacao->individuos[i],argv[1])==OK){i++;}
		
	}
	srand(time(NULL));
	for(i = 0 ; i < GERACOES;i++){
		for(j = 0; j < TORNEIO;j++){
			double prob = ((double) rand() / ((double)RAND_MAX+1));
			if(prob < PROBCRUZAMENTO){
				int ind1 = rand() % TAM_POPULACAO;
				int ind2;
				do{
					ind2 = rand()% TAM_POPULACAO;				
				}while(ind1==ind2);
				
				new = cruzamento(&populacao->individuos[ind1],&populacao->individuos[ind2]);
							
				if(new!=NULL){		
					prob = ((double) rand() / ((double)RAND_MAX + 1));
					if(prob < PROBMUTACAO){
						mutacao(new);
				
					}
					//elimina o pior individuo
					eliminaPior(populacao,new);
					//imprime(&populacao->individuos[getId]);			
				}
		
			}
		}
	}
	//Procura individuo com melhor fitness
	int id = achaMelhor(populacao);
	imprime(&populacao->individuos[id]);
	

	return 0;
}
