# tem q pegar o compressed MPS jogar no compilado do emeps.c com o nome 
# como argumento (ex: ./main nome_do_compressedMPS.mps > nome_saida.mps) 
# pega o saida .mps e joga no smps.load_mps(/caminho/pro/nome_saida.mps)

from pysmps import smps_loader as smps

def print_arr(size:int, arr):
    for i in range(0, size):
        print(arr[i])

def main():

    #devolve uma tupla https://pypi.org/project/pysmps/ da p achar aqui  
    b = smps.load_mps("/home/teo/Documents/4s/trablin/orig/t1.mps")
   
    r_t = b[5] # tipo das restrições (eq, leq, geq, lt, gt)
    c = b[6] # função de custo (c)
    A = b[7] # matriz de custo (A)
    r_n = b[8] # chave de acesso para as restrições ao lado direito
    r = b[9] # valor das restrições (b)
    v_n = b[10] # chave para as restrições sobre as variáveis
    v_r = b[11] # chave para os vetores de acesso das variáveis
    v_acc = v_r[v_n[0]] # dict com as restrições todas
    v_lo = v_acc['LO'] #restrições de <= para as variávies
    v_hi = v_acc['UP'] #restrições de >= para as variávies

    n_var = A.shape[1]
    n_res = A.shape[0]

    print(f"{n_var} {n_res}")
    
    print(c)
    print(A)
    print(r[r_n[0]])
    print(v_hi)
    print(v_lo)

if __name__ == "__main__": 
   main() 