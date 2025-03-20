/*Resolucion matrices mediante gauss
 * ver 19/6/2024 by DPS*/
#include <stdio.h>
#include <stdlib.h>

void leerMatriz(FILE *archivo,int x, int y, float matriz[x][y]);
void calculos(int x, int y, float matriz[x][y]);
void soluciones(int x, int y, float matriz[x][y]);
void esCD(int x,int y, float matriz[x][y]);
void filasDeCeros(int x, int y, float matriz[x][y],int *filas,int *filas2);

struct solucion{
	float numerador[1000];
	float denominador[1000];
};

int main (){
	printf("Resolucion matrices mediante gauss\n\tver 19/6/2024\n");
	
	FILE *archivo;
	archivo = fopen("matr.csv","r");
	if (archivo == NULL){
		printf("Archivo no valido\n");
		
	}
	else{
		int x,y;
		printf("Inserte numero de filas de la matriz:");
		scanf("%d",&x);
		printf("Inserte numero de columnas de la matriz:");
		scanf("%d",&y);
		float matriz[x][y];
		leerMatriz(archivo,x,y,matriz);
		calculos(x,y,matriz);
	}

	return 0;
}

void leerMatriz(FILE *archivo,int x,int y, float matriz[x][y]){
	printf("Matriz inicial:\n");
	for(int i = 0; i < x ; i++){
		for(int j = 0 ; j < y ; j++){
			fscanf(archivo,"%f,",&matriz[i][j]);
			printf("%g\t",matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	fclose(archivo);
}

void calculos(int x, int y, float matriz[x][y]){
	int numero = 0;
	while(numero < x-1){
		int multiplicador = 1,valorTemporal = 0;
		for(int i = numero; i < x ; ++i){
			if (matriz[i][numero] != 0){
				multiplicador *= matriz[i][numero];
			}
		}
		for(int i = numero; i < x ; i++){
			valorTemporal = matriz[i][numero];
			for(int j = numero ; j < y ; j++){
				if(matriz[i][numero] != 0){
					matriz[i][j] *= multiplicador/valorTemporal;
				}
			}
		}
		printf("Matriz multiplicada:\n");
		for(int i = 0; i < x ; i++){
			for(int j = 0 ; j < y ; j++){
				printf("%g\t",matriz[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		for(int i = numero + 1; i < x ; i++){
			valorTemporal = matriz[i][numero];
			for(int j = numero ; j < y ; j++){
				if(valorTemporal != 0){
					matriz[i][j] -= matriz[numero][j];
				}
			}
		}
		printf("Simplificamos la columna %d:\n",numero);
		for(int i = 0; i < x ; i++){
			for(int j = 0 ; j < y ; j++){
				printf("%g\t",matriz[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		numero++;
	}
	soluciones(x,y,matriz);
}

void soluciones(int x, int y, float matriz[x][y]){
	int diagonal = x-1;
	struct solucion lista;
	int filas = x,filas2 = x;
	filasDeCeros(x,y,matriz,&filas,&filas2);
	printf("El rango de la matriz es %d\nEl rango de la matriz extendida es %d\n",filas,filas2);
	if(filas == filas2 && filas == y-1){
		while(diagonal >= 0){
			if(diagonal == x-1){
				lista.numerador[diagonal] = matriz[diagonal][y-1];
				lista.denominador[diagonal] = matriz[diagonal][diagonal];
			}
			else{
				lista.numerador[diagonal] = matriz[diagonal][y-1];
				lista.denominador[diagonal] = matriz[diagonal][diagonal];
				for(int resta = diagonal+1; resta <= x-1 ; resta++){
					lista.numerador[diagonal] += -1*matriz[diagonal][resta]*lista.numerador[resta]/lista.denominador[resta];
				}
			}
			diagonal--;
		}
		printf("Se trata de un sistema compatible determinado cuyas soluciones son:\n");
		for(int letra = 0; letra <= x-1 ; letra++){
			printf("Solucion para %c es %g/%g = %g\n",'a'+letra,lista.numerador[letra],lista.denominador[letra], (float) lista.numerador[letra]/lista.denominador[letra]);
		}
	}
	else if(filas == filas2 && filas < x){
		printf("Es un sistema compatible indeterminado y tiene infinitas soluciones\n");
	}
	else if(filas != filas2){
		printf("Es un sistema incompatible y no tiene solucion\n");
	}
}


void filasDeCeros(int x, int y, float matriz[x][y],int *filas,int *filas2){	//Filas de ceros enteras y filas de ceros menos ultimo numero, esto nos ayudara a calcular rangos para el teorema rouche-frobineus
	int todoC = 1,todoCMU = 1;	// Consideramos inicialmente que es cierto
	for(int i = 0; i < x ; i++){
		for(int j = 0 ; j < y ; j++){
			if(matriz[i][j] != 0 && j != 3){	//Si el valor no es 0 deja de ser cierto que toda la fila esta compuesa por 0
				todoC = 0;
				todoCMU = 0;
				break;
			}
			else if(matriz[i][j] != 0 && j == 3){	//Si el valor deja de ser 0 en la ultima columna deja de ser cierto que todo es 0 menos el ultimo numero
				todoC = 0;
				break;	//Este no deberia hacer falta
			}
		}
		if(todoCMU == 1 && todoC == 0){
			*filas -= 1;
		}
		else if(todoC == 1 && todoCMU == 1){
			*filas -= 1;
			*filas2 -= 1;
		}
		todoC = 1;
		todoCMU = 1;
	}
}
