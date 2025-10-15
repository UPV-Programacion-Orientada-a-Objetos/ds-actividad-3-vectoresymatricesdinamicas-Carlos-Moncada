#include <iostream>

float** crearMatrizPlanificacion(int productos, int semanas){
    float** matriz = new float*[productos];

    for (int i = 0; i < productos; i++)
    {
        matriz[i] = new float[semanas];
    }

    return matriz;
}

void liberarMatriz(float** matriz, int productos){

    for (int i=0; i<productos; i++){
        delete[] matriz[i];
    }
    delete[] matriz;
}

float* crearVectorDinamico(int tamano) {
    float* vector = new float[tamano];
    return vector;

}

void mostrarPlanificacion(float** matriz, int productos,int columnas){
    for (int i = 0; i < productos; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            std::cout << matriz[i][j] << std::endl;
        }
        std::cout << std::endl;
    }

}

float* calularConsumoMP(float** mostrarPlanificacion, int requierimientos[][TIPOS_MP], int semana){
    float* consumoPorMP = crearVectorDinamico(TIPOS_MP);

    for (int i = 0; i < TIPOS_MP; i++)
    {
        consumoPorMP[i]=0.0f;
    }

    for (int mp = 0; mp < TIPOS_MP; mp++)
    {
        for(int prod =0; prod < PRODUCTOS; ++prod){
            consumoPorMP[mp] += mostrarPlanificacion[prod][semana] * requierimientos[prod][mp];
        }
    }
    
    return consumoPorMP;
    

}

int main() {
    const int productos = 5;  // Filas
    int semanas = 4;    // Columnas

    float** miMatriz = crearMatrizPlanificacion(productos, semanas);

    mostrarPlanificacion(miMatriz, productos, semanas);

    liberarMatriz(miMatriz, productos);

    return 0;
}