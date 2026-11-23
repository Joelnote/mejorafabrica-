#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTOS 10
#define MAX_MATERIALES 15


void calcularTotales(float *cantidades, float *tiempos, int n, float *totalCant, float *totalTiempo) {
    *totalCant = 0.0;
    *totalTiempo = 0.0;

    for (int i = 0; i < n; i++) {
        *totalCant += *(cantidades + i);
        *totalTiempo += *(tiempos + i);
    }
}

int main() {
    char productos[MAX_PRODUCTOS][50];
    char materiales[MAX_PRODUCTOS][MAX_MATERIALES][40];
    float cantidades[MAX_PRODUCTOS][MAX_MATERIALES];
    float stockDisponible[MAX_PRODUCTOS][MAX_MATERIALES];
    float tiempos[MAX_PRODUCTOS][MAX_MATERIALES];
    int numMateriales[MAX_PRODUCTOS];
    int totalProductos = 0;

    int opcion;

    do {
        printf("\n=== MENU DE PRODUCCION ===\n");
        printf("1. Agregar producto, materiales y tiempos\n");
        printf("2. Ver todos los productos y su informacion\n");
        printf("3. Borrar producto\n");
        printf("4. Ver demanda y verificar si cumple\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();

        if (opcion == 1) {
            if (totalProductos >= MAX_PRODUCTOS) {
                printf("\n[!] Ya alcanzaste el maximo de productos.\n");
            } else {
                int idx = totalProductos;
                float *ptrCant = cantidades[idx];
                float *ptrStock = stockDisponible[idx];
                float *ptrTiempo = tiempos[idx];

                printf("\nNombre del producto #%d: ", idx + 1);
                fgets(productos[idx], 50, stdin);
                productos[idx][strcspn(productos[idx], "\n")] = 0;

                printf("Cuantos ingredientes o materiales necesita '%s'? ", productos[idx]);
                scanf("%d", &numMateriales[idx]);
                getchar();

                if (numMateriales[idx] < 1 || numMateriales[idx] > MAX_MATERIALES) {
                    printf("[!] Cantidad no valida.\n");
                    numMateriales[idx] = 0;
                } else {
                    for (int i = 0; i < numMateriales[idx]; i++) {
                        printf("\nMaterial/ingrediente #%d: ", i + 1);
                        fgets(materiales[idx][i], 40, stdin);
                        materiales[idx][i][strcspn(materiales[idx][i], "\n")] = 0;

                        printf("Cantidad necesaria por 1 '%s': ", productos[idx]);
                        scanf("%f", ptrCant + i);
                        getchar();

                        printf("Cantidad disponible de '%s': ", materiales[idx][i]);
                        scanf("%f", ptrStock + i);
                        getchar();

                        do {
                            printf("Tiempo de fabricacion para '%s' (horas, 0 si ya viene comprado): ", materiales[idx][i]);
                            scanf("%f", ptrTiempo + i);
                            getchar();
                            if (*(ptrTiempo + i) < 0) {
                                printf("[!] No puede ser negativo.\n");
                            }
                        } while (*(ptrTiempo + i) < 0);
                    }
                    totalProductos++;
                    printf("\nProducto '%s' guardado (Total: %d).\n", productos[idx], totalProductos);
                }
            }
        }
        else if (opcion == 2) {
            if (totalProductos == 0) {
                printf("\n[!] No hay productos registrados.\n");
            } else {
                // Busqueda simple con ciclo for para mostrar uno por uno
                for (int p = 0; p < totalProductos; p++) {
                    float totalCantProd = 0.0;
                    float totalTiempoProd = 0.0;
                    float *ptrCant = cantidades[p];
                    float *ptrStock = stockDisponible[p];
                    float *ptrTiempo = tiempos[p];

                    calcularTotales(ptrCant, ptrTiempo, numMateriales[p], &totalCantProd, &totalTiempoProd);

                    printf("\nProducto #%d: %s\n", p + 1, productos[p]);
                    for (int i = 0; i < numMateriales[p]; i++) {
                        printf("  - %s | Necesario/u: %.2f | Disponible: %.2f | Tiempo: %.2f h\n",
                               materiales[p][i], *(ptrCant + i), *(ptrStock + i), *(ptrTiempo + i));
                    }
                    printf("  -> Total ingredientes por unidad: %.2f\n", totalCantProd);
                    printf("  -> Tiempo total por unidad: %.2f horas\n", totalTiempoProd);
                }
            }
        }
        else if (opcion == 3) {
            if (totalProductos == 0) {
                printf("\n[!] No hay productos para borrar.\n");
            } else {
                int borrar, encontrado = -1;
                printf("\n--- BORRAR PRODUCTO ---\n");
                for (int p = 0; p < totalProductos; p++) {
                    printf("%d. %s\n", p + 1, productos[p]);
                }
                printf("Numero del producto a borrar: ");
                scanf("%d", &borrar);
                getchar();

                // Buscamos la posicion con un ciclo for (sin funciones de busqueda)
                for (int p = 0; p < totalProductos; p++) {
                    if (p + 1 == borrar) {
                        encontrado = p;
                        break;
                    }
                }

                if (encontrado == -1) {
                    printf("[!] Producto no encontrado.\n");
                } else {
                    for (int p = encontrado; p < totalProductos - 1; p++) {
                        strcpy(productos[p], productos[p + 1]);
                        numMateriales[p] = numMateriales[p + 1];
                        for (int i = 0; i < numMateriales[p]; i++) {
                            strcpy(materiales[p][i], materiales[p + 1][i]);
                            cantidades[p][i] = cantidades[p + 1][i];
                            stockDisponible[p][i] = stockDisponible[p + 1][i];
                            tiempos[p][i] = tiempos[p + 1][i];
                        }
                    }
                    totalProductos--;
                    printf("Producto borrado.\n");
                }
            }
        }
        else if (opcion == 4) {
            if (totalProductos == 0) {
                printf("\n[!] Primero agrega un producto.\n");
            } else {
                int sel, demanda, cumple = 1, idx = -1;
                printf("\n--- VERIFICAR DEMANDA ---\n");
                for (int p = 0; p < totalProductos; p++) {
                    printf("%d. %s\n", p + 1, productos[p]);
                }
                printf("Elige el numero del producto: ");
                scanf("%d", &sel);
                getchar();

                // Busqueda propia con for
                for (int p = 0; p < totalProductos; p++) {
                    if (p + 1 == sel) {
                        idx = p;
                        break;
                    }
                }

                if (idx == -1) {
                    printf("[!] Seleccion no valida.\n");
                } else {
                    float *ptrCant = cantidades[idx];
                    float *ptrStock = stockDisponible[idx];
                    float *ptrTiempo = tiempos[idx];

                    printf("Cuantas unidades de '%s' quieres fabricar? ", productos[idx]);
                    scanf("%d", &demanda);
                    getchar();

                    float tiempoRequeridoTotal = 0.0;

                    printf("\nPara %d unidades de '%s':\n", demanda, productos[idx]);
                    for (int i = 0; i < numMateriales[idx]; i++) {
                        float necesario = (*(ptrCant + i)) * demanda;
                        float tiempoIng = (*(ptrTiempo + i)) * demanda;
                        tiempoRequeridoTotal += tiempoIng;

                        printf("  - %s: necesita %.2f | disponible %.2f",
                               materiales[idx][i], necesario, *(ptrStock + i));

                        if (*(ptrStock + i) < necesario) {
                            printf(" -> [INSUFICIENTE: faltan %.2f]\n", necesario - *(ptrStock + i));
                            cumple = 0;
                        } else {
                            printf(" -> [OK]\n");
                        }
                    }

                    printf("Tiempo total estimado: %.2f horas\n", tiempoRequeridoTotal);

                    if (cumple) {
                        printf("\n>>> SI CUMPLE con los ingredientes.\n");
                    } else {
                        printf("\n>>> NO CUMPLE: ingredientes insuficientes.\n");
                    }
                }
            }
        }
        else if (opcion == 0) {
            printf("\nSaliendo...\n");
        }
        else {
            printf("\n[!] Opcion no valida.\n");
        }

    } while (opcion != 0);

    return 0;
}
