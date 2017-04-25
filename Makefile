#   Universidade Federal da Fronteira Sul
# 
#                TRABALHO I
#   
#   Disciplina: Inteligência Artificial
#   Professor: José Carlos Bins Filho
#    
#   Aplicação do algoritmo genético para geração de horários. 
# 	
#
#   Alunos : Edson Lemes da Silva && Lucas Cezar Parnoff
#
#
#  --- MAKEFILE DE COMPILAÇÃO E EXECUÇAO ---

all: prog

prog: ag.c ag.h readFile.c
	gcc ag.c ag.h readFile.c -o  main && ./main curso.dat