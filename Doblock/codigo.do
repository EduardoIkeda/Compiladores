// Programa exemplo em Do::Block - Alternativa 1
 
// Preencher uma matriz de inteiros com 1's acima 
// da diagonal principal (inclusive) 
 
const int DIM_LIN = 10
const int DIM_COL = 10 
int i = 1
int mat[DIM_LIN][DIM_COL]
block Para_Cada_Lin with int[][], int  
block Para_Cada_Col with int[][], int, int 
 
block main 
 
int mat[DIM_LIN][DIM_COL] 
int i
 
do Para_Cada_Lin with mat, i  
varying i from 0 to DIM_LIN-1 
 
endblock 
 
block Para_Cada_Lin with int m[DIM_LIN][DIM_COL], int l 
 
int c 
 
do Para_Cada_Col with m, l, c  
varying c from 0 to DIM_COL-1 

endblock 

 
block Para_Cada_Col with int m[DIM_LIN][DIM_COL], int l, int c 

int c 

if (l >= c) m[l][c] = 1  
else m[l][c] = 0 
endif 
 

endblock 