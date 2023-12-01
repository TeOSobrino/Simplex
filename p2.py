# tem q pegar o compressed MPS jogar no compilado do emeps.c com o nome 
# como argumento (ex: ./main nome_do_compressedMPS.mps > nome_saida.mps) 
# pega o saida .mps e joga no smps.load_mps(/caminho/pro/nome_saida.mps)

from pysmps import smps_loader as smps
from pprint import pprint
import pandas as pd
import numpy as np

def print_arr(size:int, arr):
    for i in range(0, size):
        print("%i " %arr[i], end="")
    print()

def main():

    #devolve uma tupla https://pypi.org/project/pysmps/ da p achar aqui  
    problema = smps.load_mps("/home/teo/Documents/4s/trablin/orig/t4.mps")
    # problema = smps.load_mps("C:\\Users\\famde\\OneDrive\\Documentos\\Programacao\\t1.mps")
   
    tipos_restricao = problema[5] # tipo das restrições (eq, leq, geq, lt, gt)
    c = np.array(problema[6]) # função de custo (c)
    
    A = np.array(problema[7]) # matriz de custo (A)
    r_n = problema[8] # chave de acesso para as restrições ao lado direito
    r = problema[9] # valor das restrições (b)
    b = np.array(r[r_n[0]])
    v_n = problema[10] # chave para as restrições sobre as variáveis
    v_r = problema[11] # chave para os vetores de acesso das variáveis
    if(len(v_n) > 0):
        v_acc = v_r[v_n[0]] # dict com as restrições todas
        v_lo = np.array(v_acc['LO']) #restrições de <= para as variávies
        v_hi = np.array(v_acc['UP']) #restrições de >= para as variávies
    else:
        v_lo = []
        v_hi = []
    
    n_var = A.shape[1]
    n_res = A.shape[0]
    
    # Trocando as restriçoes das variaveis para >= 0
    for index,(restricao_lo,restricao_hi) in enumerate(zip(v_lo,v_hi)):
        # se for menor que um finito, adicionar restricao 
        if restricao_lo != np.inf and restricao_lo > 0:
            # adiciona linha na matriz A
            n_res += 1
            linha = np.zeros(n_var)
            linha[index] = 1
            A = np.insert(A, A.shape[0], linha, axis=0)
            # adiciona um valor no final do vetor b
            b = np.insert(b, b.shape[0], restricao_lo)
            tipos_restricao.append('G')
        
        if restricao_hi != np.inf and restricao_hi > 0:
            # adiciona linha na matriz A
            n_res += 1
            linha = np.zeros(n_var)
            linha[index] = 1
            A = np.insert(A, A.shape[0], linha, axis=0)
            # adiciona um valor no final do vetor b
            b = np.insert(b, b.shape[0], restricao_hi)
            tipos_restricao.append('L')
            

    # Mudando as restricoes para "="
    for indicie_restricao, restricao in enumerate(tipos_restricao):
        if restricao == 'E':
            continue
        
        # adicionar variável de folga
        n_var += 1
        # adicionando mais uma coluna para a matriz A
        coluna = np.zeros(n_res)
        coluna[indicie_restricao] = 1
        A = np.insert(A, A.shape[1], coluna, axis=1)
        # adicionando variavel de folga no custo
        c = np.insert(c, c.shape[0], 0)
        # multiplicando a linha de A por -1 se for maior ou igual
        if restricao == 'G':
            A[indicie_restricao] *= -1

    # pprint(b)
    print(f"{n_res} {n_var}")
    
    print_arr(c.size, c)
    # Convertendo a matriz NumPy para DataFrame
    df = pd.DataFrame(A)

    # Salvando o DataFrame em um arquivo Excel
    #df.to_excel("matriz_excel.xlsx", index=False)
    for i in range(0, A.shape[0]):
        for j in range(0, A.shape[1]):
            print("%i " %A[i][j], end="")
        print("%i" %b[i], end="")
        print("")

if __name__ == "__main__": 
   main() 
   