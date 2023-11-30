# tem q pegar o compressed MPS jogar no compilado do emeps.c com o nome 
# como argumento (ex: ./main nome_do_compressedMPS.mps > nome_saida.mps) 
# pega o saida .mps e joga no smps.load_mps(/caminho/pro/nome_saida.mps)

from pysmps import smps_loader as smps
from pprint import pprint
import pandas as pd
import numpy as np
def print_arr(size:int, arr):
    for i in range(0, size):
        print(arr[i])

def main():

    #devolve uma tupla https://pypi.org/project/pysmps/ da p achar aqui  
    b = smps.load_mps("/home/teo/Documents/4s/trablin/orig/t1.mps")
    # b = smps.load_mps("C:\\Users\\famde\\OneDrive\\Documentos\\Programacao\\t1.mps")
   
    tipos_restricao = b[5] # tipo das restrições (eq, leq, geq, lt, gt)
    c = np.array(b[6]) # função de custo (c)
    
    A = b[7] # matriz de custo (A)
    np.array(A)
    r_n = b[8] # chave de acesso para as restrições ao lado direito
    r = b[9] # valor das restrições (b)
    v_n = b[10] # chave para as restrições sobre as variáveis
    v_r = b[11] # chave para os vetores de acesso das variáveis
    v_acc = v_r[v_n[0]] # dict com as restrições todas
    v_lo = v_acc['LO'] #restrições de <= para as variávies
    v_hi = v_acc['UP'] #restrições de >= para as variávies

    n_var = A.shape[1]
    n_res = A.shape[0]

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
    print(f"{n_var} {n_res}")
    
    print(c)
    # Convertendo a matriz NumPy para DataFrame
    df = pd.DataFrame(A)

    # Salvando o DataFrame em um arquivo Excel
    df.to_excel("matriz_excel.xlsx", index=False)
    print(r[r_n[0]])
    print(v_hi)
    print(v_lo)

if __name__ == "__main__": 
   main() 
   