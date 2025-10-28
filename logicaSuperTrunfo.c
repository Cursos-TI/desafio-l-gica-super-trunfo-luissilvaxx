/*
  Super Trunfo — Nível Novato
  Comparação de 2 cartas com 1 atributo escolhido DIRETAMENTE NO CÓDIGO.

  Requisitos atendidos:
  - Duas cartas pré-definidas (estado, código, cidade, populacao, area, pib, pontosTuristicos)
  - Cálculo de Densidade Populacional (populacao / area)
  - Cálculo de PIB per capita ( (pib_bilhoes * 1e9) / populacao )
  - Escolha de UM atributo feita no código (sem entrada do usuário)
  - Comparação com if / if-else:
      * Regra geral: MAIOR vence
      * Exceção: Densidade Populacional -> MENOR vence
  - Saída clara informando atributo, valores e vencedora

  Observação:
  - Os valores de PIB utilizados aqui são ilustrativos (em bilhões de R$).
*/

#include <stdio.h>

typedef struct {
    char estado[3];
    char codigo[6];
    char cidade[40];
    int  populacao;     // habitantes
    float area;         // km²
    float pib_bilhoes;  // bilhões de R$
    int  pontosTuristicos;
    // calculados:
    float densidade;    // hab/km²
    float pibPerCapita; // R$ por habitante
} Carta;

/* --------- Cálculos derivados --------- */
void calcular_derivados(Carta *c) {
    if (c->area > 0.0f) {
        c->densidade = (float)c->populacao / c->area;
    } else {
        c->densidade = 0.0f; // fallback simples para evitar divisão por zero
    }

    if (c->populacao > 0) {
        double pib_total = (double)c->pib_bilhoes * 1e9; // bilhões -> unidades
        c->pibPerCapita = (float)(pib_total / (double)c->populacao);
    } else {
        c->pibPerCapita = 0.0f;
    }
}

/* --------- Impressão resumida --------- */
void mostrar_carta(const Carta *c) {
    printf("Carta %s - %s (%s)\n", c->codigo, c->cidade, c->estado);
    printf("  Populacao: %d hab\n", c->populacao);
    printf("  Area: %.2f km²\n", c->area);
    printf("  PIB: R$ %.2f bilhões\n", c->pib_bilhoes);
    printf("  Pontos Turisticos: %d\n", c->pontosTuristicos);
    printf("  Densidade: %.2f hab/km²\n", c->densidade);
    printf("  PIB per capita: R$ %.2f\n", c->pibPerCapita);
}

/* --------- Escolha do atributo (no código) ---------
   Altere o valor de SELECTED_ATTR para escolher o atributo da rodada:
   1 = População
   2 = Área
   3 = PIB
   4 = Densidade Populacional (MENOR vence)
   5 = PIB per capita
*/
#define ATR_POPULACAO 1
#define ATR_AREA 2
#define ATR_PIB 3
#define ATR_DENSIDADE 4
#define ATR_PIB_PER_CAPITA 5

#define SELECTED_ATTR ATR_POPULACAO   /* <<< MUDE AQUI o atributo da comparação */

/* --------- Leitura do valor/rotulo do atributo --------- */
float valor_atributo(const Carta *c, int atributo) {
    if (atributo == ATR_POPULACAO) {
        return (float)c->populacao;
    } else if (atributo == ATR_AREA) {
        return c->area;
    } else if (atributo == ATR_PIB) {
        return c->pib_bilhoes; // em bilhões
    } else if (atributo == ATR_DENSIDADE) {
        return c->densidade;
    } else if (atributo == ATR_PIB_PER_CAPITA) {
        return c->pibPerCapita;
    } else {
        return 0.0f;
    }
}

const char* nome_atributo(int atributo) {
    if (atributo == ATR_POPULACAO) {
        return "Populacao";
    } else if (atributo == ATR_AREA) {
        return "Area";
    } else if (atributo == ATR_PIB) {
        return "PIB";
    } else if (atributo == ATR_DENSIDADE) {
        return "Densidade Populacional";
    } else if (atributo == ATR_PIB_PER_CAPITA) {
        return "PIB per capita";
    } else {
        return "Atributo desconhecido";
    }
}

/* --------- Comparação (apenas if / if-else) --------- */
int main(void) {
    /* Duas cartas pré-definidas */
    Carta carta1 = {
        .estado = "SP",
        .codigo = "A01",
        .cidade = "Sao Paulo",
        .populacao = 12300000,
        .area = 1521.11f,
        .pib_bilhoes = 760.0f,  // ilustrativo
        .pontosTuristicos = 25
    };

    Carta carta2 = {
        .estado = "RJ",
        .codigo = "B02",
        .cidade = "Rio de Janeiro",
        .populacao = 6748000,
        .area = 1182.30f,
        .pib_bilhoes = 400.0f,  // ilustrativo
        .pontosTuristicos = 18
    };

    /* Calcula derivados para ambas */
    calcular_derivados(&carta1);
    calcular_derivados(&carta2);

    /* Mostra as cartas e os derivados (requisito de exibir cálculos) */
    printf("=== Cartas cadastradas ===\n");
    mostrar_carta(&carta1);
    printf("\n");
    mostrar_carta(&carta2);
    printf("\n");

    /* Define atributo escolhido diretamente no código */
    int atributo = SELECTED_ATTR;
    const char *rotulo = nome_atributo(atributo);

    /* Recupera valores para comparação */
    float v1 = valor_atributo(&carta1, atributo);
    float v2 = valor_atributo(&carta2, atributo);

    /* Exibe cabeçalho da comparação */
    printf("=== Comparacao de cartas (Atributo: %s) ===\n", rotulo);

    /* Mostra valores com unidade apropriada (didático) */
    if (atributo == ATR_POPULACAO) {
        printf("Carta 1 - %s (%s): %d hab\n", carta1.cidade, carta1.estado, carta1.populacao);
        printf("Carta 2 - %s (%s): %d hab\n", carta2.cidade, carta2.estado, carta2.populacao);
    } else if (atributo == ATR_AREA) {
        printf("Carta 1 - %s (%s): %.2f km²\n", carta1.cidade, carta1.estado, carta1.area);
        printf("Carta 2 - %s (%s): %.2f km²\n", carta2.cidade, carta2.estado, carta2.area);
    } else if (atributo == ATR_PIB) {
        printf("Carta 1 - %s (%s): R$ %.2f bilhões\n", carta1.cidade, carta1.estado, carta1.pib_bilhoes);
        printf("Carta 2 - %s (%s): R$ %.2f bilhões\n", carta2.cidade, carta2.estado, carta2.pib_bilhoes);
    } else if (atributo == ATR_DENSIDADE) {
        printf("Carta 1 - %s (%s): %.2f hab/km²\n", carta1.cidade, carta1.estado, carta1.densidade);
        printf("Carta 2 - %s (%s): %.2f hab/km²\n", carta2.cidade, carta2.estado, carta2.densidade);
    } else if (atributo == ATR_PIB_PER_CAPITA) {
        printf("Carta 1 - %s (%s): R$ %.2f\n", carta1.cidade, carta1.estado, carta1.pibPerCapita);
        printf("Carta 2 - %s (%s): R$ %.2f\n", carta2.cidade, carta2.estado, carta2.pibPerCapita);
    } else {
        printf("Atributo inválido.\n");
        return 0;
    }

    /* Decide vencedora:
       - Regra geral: MAIOR vence
       - Exceção (densidade): MENOR vence
    */
    int carta1Vence = 0;
    int carta2Vence = 0;

    if (atributo == ATR_DENSIDADE) {
        /* MENOR vence */
        if (v1 < v2) {
            carta1Vence = 1;
        } else if (v2 < v1) {
            carta2Vence = 1;
        }
    } else {
        /* MAIOR vence */
        if (v1 > v2) {
            carta1Vence = 1;
        } else if (v2 > v1) {
            carta2Vence = 1;
        }
    }

    /* Exibe resultado */
    if (carta1Vence == 1) {
        printf("Resultado: Carta 1 (%s) venceu!\n", carta1.cidade);
    } else if (carta2Vence == 1) {
        printf("Resultado: Carta 2 (%s) venceu!\n", carta2.cidade);
    } else {
        printf("Resultado: Empate!\n");
    }

    return 0;
}