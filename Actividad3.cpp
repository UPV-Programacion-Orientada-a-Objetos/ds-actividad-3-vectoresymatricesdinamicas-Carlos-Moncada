#include <iostream>

const int PRODUCTOS = 5;
const int TIPOS_MP = 4;

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
            std::cout << matriz[i][j] << " ";
        }
        std::cout << std::endl;
    }

}

float* calcularConsumoMP(float** planificacion, int requerimientos[][TIPOS_MP], int semana) {
    float* consumoPorMP = crearVectorDinamico(TIPOS_MP);

    for (int i = 0; i < TIPOS_MP; ++i) {
        consumoPorMP[i] = 0.0f;
    }

    for (int mp = 0; mp < TIPOS_MP; ++mp) {
        for (int prod = 0; prod < PRODUCTOS; ++prod) {
            consumoPorMP[mp] += planificacion[prod][semana] * requerimientos[prod][mp];
        }
    }
    
    return consumoPorMP;
}

float calcularCOGS(float** planificacion, int requerimientos[][TIPOS_MP], float* costosMP, int semana) {
    
    float* consumo = calcularConsumoMP(planificacion, requerimientos, semana);
    
    float costoTotal = 0.0f;
    
    for (int mp = 0; mp < TIPOS_MP; ++mp) {

        costoTotal += consumo[mp] * costosMP[mp];
    }
    delete[] consumo;
    
    return costoTotal;
}

void redimensionarMatriz(float**& matriz, int filas, int& columnas) {
    int nuevasColumnas = columnas + 1;
    std::cout << "\nRedimensionando la matriz a " << nuevasColumnas << " semanas..." << std::endl;

    // Paso 1: Crear una nueva matriz más grande.
    float** nuevaMatriz = crearMatrizPlanificacion(filas, nuevasColumnas);

    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            nuevaMatriz[i][j] = matriz[i][j];
        }
        nuevaMatriz[i][columnas] = 0.0f; 
    }

    liberarMatriz(matriz, filas);
    matriz = nuevaMatriz;
    columnas = nuevasColumnas;
}

void actualizarPlanificacion(float** matriz, int producto, int semana, float nuevaCantidad, int totalProductos, int totalSemanas) {
    

    if (producto >= 0 && producto < totalProductos && semana >= 0 && semana < totalSemanas) {
        matriz[producto][semana] = nuevaCantidad;

        std::cout << "-> Actualizacion exitosa." << std::endl;

    } else {

        std::cout << "-> Error: El producto o la semana estan fuera de los limites validos." << std::endl;
    }
}

void mostrarMenu() {
    std::cout << "\n--- Menu Principal ---" << std::endl;
    std::cout << "1. Ver Plan de Produccion" << std::endl;
    std::cout << "2. Agregar Nueva Semana" << std::endl;
    std::cout << "3. Modificar Produccion" << std::endl;
    std::cout << "4. Calcular COGS y Reporte" << std::endl;
    std::cout << "5. Salir" << std::endl;
    std::cout << "Opcion seleccionada: ";
}

float calcularProduccionTotal(float** matriz, int semana) {
    float total = 0.0f;
    for (int i = 0; i < PRODUCTOS; ++i) {
        total += matriz[i][semana];
    }
    return total;
}

float calcularValorInventarioFinal(float* inventarioInicial, float* consumo, float* costos) {
    float valorTotal = 0.0f;
    for (int i = 0; i < TIPOS_MP; ++i) {
        float restante = inventarioInicial[i] - consumo[i];
        if (restante > 0) { 
            valorTotal += restante * costos[i];
        }
    }
    return valorTotal;
}

int main() {


    float* inventarioInicialMP = crearVectorDinamico(TIPOS_MP);
    inventarioInicialMP[0] = 500; 
    inventarioInicialMP[1] = 400; 
    inventarioInicialMP[2] = 600; 
    inventarioInicialMP[3] = 300; 

    delete[] inventarioInicialMP;

    int semanas = 4;
    int opcion; 

    int requerimientos[PRODUCTOS][TIPOS_MP] = {
        {2, 1, 0, 0}, {0, 2, 1, 0}, {1, 0, 2, 1}, {0, 3, 0, 1}, {1, 1, 1, 1}
    };
    float* costosMP = crearVectorDinamico(TIPOS_MP);
    costosMP[0] = 1.5; costosMP[1] = 2.0; costosMP[2] = 3.0; costosMP[3] = 2.5;

    float** miMatriz = crearMatrizPlanificacion(PRODUCTOS, semanas);
    for(int i=0; i < PRODUCTOS; ++i) {
        for(int j=0; j < semanas; ++j) {
            miMatriz[i][j] = 0.0f;
        }
    }
    
    std::cout << "Inicializacion exitosa." << PRODUCTOS << " Productos x " << semanas << " Semanas." << std::endl;

    do {
        mostrarMenu();
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                mostrarPlanificacion(miMatriz, PRODUCTOS, semanas);
                break;

            case 2:
                redimensionarMatriz(miMatriz, PRODUCTOS, semanas);
                break;

            case 3: {
                int prod, sem;
                float cant;
                std::cout << "Ingrese Producto (0-" << PRODUCTOS - 1 << "): ";
                std::cin >> prod;
                std::cout << "Ingrese Semana (0-" << semanas - 1 << "): ";
                std::cin >> sem;
                std::cout << "Cantidad a producir: ";
                std::cin >> cant;
                actualizarPlanificacion(miMatriz, prod, sem, cant, PRODUCTOS, semanas);
                break;
            }
            case 4: { // Calcular Reporte Completo
            int sem;
            std::cout << "Ingrese Semana para el reporte (0-" << semanas - 1 << "): ";
            std::cin >> sem;

            if (sem >= 0 && sem < semanas) {
            float produccion_total = calcularProduccionTotal(miMatriz, sem);
            float* consumo = calcularConsumoMP(miMatriz, requerimientos, sem);
            float cogs = calcularCOGS(miMatriz, requerimientos, costosMP, sem);
            float valor_final = calcularValorInventarioFinal(inventarioInicialMP, consumo, costosMP);

             std::cout << "\n--- Reporte de Costos (Semana " << sem << ") ---" << std::endl;
            std::cout << "Produccion Total (Semana " << sem << "): " << produccion_total << " unidades." << std::endl;
            std::cout << "Costo Total de Produccion (COGS): $" << cogs << std::endl;
        
            std::cout << "Consumo de Materia Prima: ";
            for (int i = 0; i < TIPOS_MP; ++i) {
            if (consumo[i] > 0) { // Solo muestra las MP que se usaron
                std::cout << "MP-" << i << " (" << consumo[i] << " uni), ";
            }
               }
            std::cout << std::endl;

            std::cout << "Valor del Inventario Final (M.P. restante): $" << valor_final << std::endl;

        
             delete[] consumo;

           } else {
             std::cout << "-> Error: Semana invalida." << std::endl;
            }
            break;
        }  
            case 5:
                std::cout << "Liberando memoria..." << std::endl;
                break;

            default:
                std::cout << "-> Error: Opcion no valida." << std::endl;
                break;
        }

    } while (opcion != 5);

    liberarMatriz(miMatriz, PRODUCTOS);
    delete[] costosMP;

    std::cout << "Sistema cerrado." << std::endl;
    return 0;
}
