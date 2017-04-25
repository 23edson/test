	Universidade Federal da Fronteira Sul.
 
	TRABALHO I
   
	Disciplina: Inteligência Artificial;
	Professor: José Carlos Bins Filho;
    
	Aplicação do algoritmo genético para geração de horários. 
 	

	Alunos : Edson Lemes da Silva && Lucas Cezar Parnoff.

**Descrição**

O algoritmo genético primeiro gera a população definida pela constante TAM_POPULACAO, todos
são gerados de forma aleatória, de modo que respeite a restrição de que um professor
não pode estar agregado ao último período do dia e o primeiro do dia seguinte.

Ao gerar um indivíduo, também é adicionado a ele um custo, onde este é calculado
por uma função de fitness, chamado neste programa de "avaliacao"; os critérios definidos
são: 
	A distribuição dos horários na grade, são contabilizados o quão distribuído esta
os horários de um mesmo professor, pois quando maior este valor maior será a chance
de haver choques e aumentar a avaliação dos outros horários. Este valor é somado para
ocorrência no vetor de genes.
	Horários consecutivos, isto é, um mesmo professor designado para um turno inteiro.
Quando isso ocorre, é calculado a soma de quantidade de vezes, porém como é algo não desejado,
essa soma é multiplicada por 4 pontos.
	A terceira situação é a verificação dos horários não preferidos, é feito a soma
dessas ocorrências de acordo com a preferência de cada professor. Neste caso não há
penalidades extras.
	E a última é a situação onde há um professor designado para disciplinas no turno
matutino e noturno no mesmo dia (não há ocorrência de turno vespertino). Deste modo,
é realizado a soma e por último penalizado com o dobro do valor, pois também é algo
não desejado.   

A execução ocorre em torno do número de gerações, definido pela constante GERACOES. Deste modo,
internamente para cada geração é utilizado o método do torneio para seleção dos
indivíduos para cruzamento/mutação.
As probabilidades definidas pelas constantes PROBCRUZAMENTO e PROBMUTACAO são 0.7 e 0.3
respectivamente; o caso a ser destacado é que uma mutação ocorre apenas se há um cruzamento,
isto é, haverá mutação apenas no novo indivíduo criado, e não nos existentes. 
No final da execução, para cada geração o algoritmo seleciona o indivíduo com maior
fitness para eliminação, evitando assim o crescimento da população.
Representação: Cada indivíduo é uma grade completa, e cada gene é um horário específico.
Os indivíduos e seus genes são representados por structs.
Uma população é representada por uma struct do tipo plcao, internamente
ela possui um vetor de structs do tipo indvo, cada posição deste vetor representa
um indivíduo. Deste modo, a struct indvo possui internamente um vetor de structs
do tipo genes, então cada posição deste vetor representa um gene de um indivíduo. E 
por fim, a struct genes possui internamente variaveis de controles, tais como:
int dia_sem (período selecionado para aquele horário), char prof (nome do professor),
int notpref ( horários não preferidos), int numpref (tamanho de notpref), int disc (
número inteiro que representa a disciplina selecionada), int sala(sala selecionada),
int sala_id(posicao no vetor de salas), int periodo(quantidade de períodos desta disciplina),
char sem( semestre da disciplina).

Cruzamento: O cruzamento ocorre pela escolha aleatória de dois indivíduos entre a população,
então, para evitar as duplicações de horários e parâmetros incorretos, ficou definido
que o cruzamento ocorre com horários de mesmo semestre, isto é, apenas disciplinas
que estão em um mesmo semestre é cruzado. A escolha do semestre para cruzamento é feito de 
forma aleatória.
Assim, para a criação do novo indivíduo é selecionado algum dos dois e é realizado uma
cópia de seus genes, assim, aquele que não foi selecionado é obtido algum gene interno
que seja do mesmo semestre selecionado, e é sobrescrito em algum gene do novo indivíduo.
Como haverá duplicação deste gene, o antigo é realocado para onde há duplicação, ficando
assim todos os genes respeitando os horários e a restrição principal.

Mutação: O cruzamento ocorre também pela escolha aleatória de semestre e disciplina. Assim,
para facilitar a implementação, foi definido que a mutação ocorre em horários livres,
isto é, apenas em semestres onde há horários livres para fazer a troca. Como é utilizado
apenas horários livres, não há o problema de duplicações.

Resultado final: O resultado final é retornado a partir do melhor indivíduo, isto é, aquele
que possui a menor penalidade acumulada durante a execução do algoritmo. Para facilitar
a visualização, o algoritmo imprime os horários calculados em forma de tabela, sendo
que cada semestre é impresso de forma separada para facilitar a implementação.

**Compilação**

Para compilar é necessário digitar na linha de comando "make".

Compilado e executado no LINUX.

O arquivo Makefile se encarrega de iniciar a execução do arquivo compilado.
Alternativamente, para remover o executável gerado basta digitar no terminal: make clean.

**Execução**

Assume-se que o arquivo "curso.dat" esteja no mesmo diretório que o arquivo compilado.
O arquivo curso.dat é passado como parâmetro na execução do problema, lido pelo argv[1].

Após a execução do algoritmo é imprimido na tela o indivíduo com menor penalidade,
deste modo cada semestre é imprimido de forma separada, cada um com sua tabela.

Para facilitar a visualização, a tabela pode ser exportado pra um arquivo de texto
na hora da execução, usando por exemplo: make >> text.txt
