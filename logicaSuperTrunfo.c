#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* =========================
   SUPER TRUNFO — NÍVEL MESTRE
   Comparação com DOIS atributos
   Regras:
   - Para cada atributo escolhido:
       * maior valor vence, EXCETO Densidade (menor vence)
   - Depois, somar os valores dos dois atributos de cada carta:
       * maior soma vence a rodada; se igual, "Empate!"
   - Menus dinâmicos via switch (o 2º menu esconde o atributo já escolhido)
   - Tratamento de entradas inválidas (default no switch + laços)
   - Uso de operador ternário na decisão
   ========================= */

typedef struct {
    char nome[32];
    double populacao_mi;     // em milhões
    double area_km2;         // km²
    double pib_usd_tri;      // em trilhões de USD
    double pib_per_capita;   // USD
    double densidade;        // hab/km²
    int pontos_turisticos;   // contagem simbólica
} Carta;

enum Atributo {
    ATR_POP = 1,
    ATR_AREA,
    ATR_PIB,
    ATR_PIB_PC,
    ATR_TURISMO,
    ATR_DENSIDADE
};

const char* nomeAtributo(int atr) {
    switch (atr) {
        case ATR_POP:       return "População (mi)";
        case ATR_AREA:      return "Área (km²)";
        case ATR_PIB:       return "PIB (US$ tri)";
        case ATR_PIB_PC:    return "PIB per capita (US$)";
        case ATR_TURISMO:   return "Pontos turísticos";
        case ATR_DENSIDADE: return "Densidade (hab/km²)";
        default:            return "Atributo inválido";
    }
}

double valorDoAtributo(const Carta* c, int atr) {
    switch (atr) {
        case ATR_POP:       return c->populacao_mi;
        case ATR_AREA:      return c->area_km2;
        case ATR_PIB:       return c->pib_usd_tri;
        case ATR_PIB_PC:    return c->pib_per_capita;
        case ATR_TURISMO:   return (double)c->pontos_turisticos;
        case ATR_DENSIDADE: return c->densidade;
        default:            return 0.0;
    }
}

/* Retorna:
   1  -> carta A venceu no atributo
   -1 -> carta B venceu no atributo
   0  -> empate no atributo
*/
int vencedorPorAtributo(double va, double vb, int atr) {
    int menorVence = (atr == ATR_DENSIDADE);
    if (!menorVence) {
        return (va > vb) ? 1 : (va < vb) ? -1 : 0; // operador ternário encadeado
    } else {
        return (va < vb) ? 1 : (va > vb) ? -1 : 0;
    }
}

void imprimirCarta(const Carta* c, int idx) {
    printf(" [%d] %-12s | Pop: %.1f mi | Area: %.0f km² | PIB: %.2f tri | PC: %.0f | Dens: %.1f | Tur: %d\n",
           idx, c->nome, c->populacao_mi, c->area_km2, c->pib_usd_tri, c->pib_per_capita, c->densidade, c->pontos_turisticos);
}

int lerOpcaoInt(const char* prompt, int min, int max) {
    int x;
    int ok = 0;
    do {
        printf("%s", prompt);
        if (scanf("%d", &x) != 1) {
            // limpar buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("Entrada inválida. Digite um número.\n");
            continue;
        }
        if (x < min || x > max) {
            printf("Opção fora do intervalo [%d..%d]. Tente novamente.\n", min, max);
        } else {
            ok = 1;
        }
    } while (!ok);
    return x;
}

int menuPrimeiroAtributo() {
    int opcao;
    while (1) {
        printf("\n=== Escolha o PRIMEIRO atributo ===\n");
        printf(" 1) %s\n", nomeAtributo(ATR_POP));
        printf(" 2) %s\n", nomeAtributo(ATR_AREA));
        printf(" 3) %s\n", nomeAtributo(ATR_PIB));
        printf(" 4) %s\n", nomeAtributo(ATR_PIB_PC));
        printf(" 5) %s\n", nomeAtributo(ATR_TURISMO));
        printf(" 6) %s  (MENOR vence)\n", nomeAtributo(ATR_DENSIDADE));
        opcao = lerOpcaoInt("Sua escolha: ", 1, 6);

        switch (opcao) {
            case ATR_POP:
            case ATR_AREA:
            case ATR_PIB:
            case ATR_PIB_PC:
            case ATR_TURISMO:
            case ATR_DENSIDADE:
                return opcao;
            default:
                // Não deve chegar aqui por causa do range, mas fica o default exigido
                printf("Atributo inválido. Tente novamente.\n");
        }
    }
}

int menuSegundoAtributo(int primeiro) {
    int opcao;
    while (1) {
        printf("\n=== Escolha o SEGUNDO atributo (diferente do primeiro) ===\n");
        for (int i = 1; i <= 6; i++) {
            if (i == primeiro) continue; // menu dinâmico: esconde o já escolhido
            if (i == ATR_DENSIDADE)
                printf(" %d) %s  (MENOR vence)\n", i, nomeAtributo(i));
            else
                printf(" %d) %s\n", i, nomeAtributo(i));
        }
        opcao = lerOpcaoInt("Sua escolha: ", 1, 6);

        switch (opcao) {
            case ATR_POP:
            case ATR_AREA:
            case ATR_PIB:
            case ATR_PIB_PC:
            case ATR_TURISMO:
            case ATR_DENSIDADE:
                if (opcao == primeiro) {
                    printf("Você não pode escolher o mesmo atributo duas vezes. Tente outro.\n");
                } else {
                    return opcao;
                }
                break;
            default:
                printf("Atributo inválido. Tente novamente.\n");
        }
    }
}

int main(void) {
    // Cartas pré-cadastradas (valores aproximados/simbólicos para fins de jogo)
    Carta baralho[] = {
        {"Brasil",     203.0, 8516000.0, 2.13, 10600.0, 24.0,  25},
        {"Argentina",   46.2, 2780000.0, 0.62, 13500.0, 16.0,  18},
        {"Chile",       19.5,  756000.0, 0.31, 16400.0, 26.0,  15},
        {"EUA",        333.0, 9834000.0, 27.0,  81000.0, 36.0, 40},
        {"China",     1410.0, 9597000.0, 18.0,  12700.0,150.0, 30},
        {"Índia",     1428.0, 3287000.0, 3.7,   2600.0, 470.0, 22}
    };
    const int N = (int)(sizeof(baralho)/sizeof(baralho[0]));

    printf("==== SUPER TRUNFO — NÍVEL MESTRE (Comparação com 2 atributos) ====\n");
    printf("Baralho disponível:\n");
    for (int i = 0; i < N; i++) {
        imprimirCarta(&baralho[i], i);
    }

    int iA = lerOpcaoInt("\nEscolha o índice da Carta A: ", 0, N-1);
    int iB = lerOpcaoInt("Escolha o índice da Carta B (diferente da A): ", 0, N-1);
    while (iB == iA) {
        printf("A Carta B deve ser diferente da A.\n");
        iB = lerOpcaoInt("Escolha outro índice para Carta B: ", 0, N-1);
    }

    const Carta *A = &baralho[iA];
    const Carta *B = &baralho[iB];

    int atr1 = menuPrimeiroAtributo();
    int atr2 = menuSegundoAtributo(atr1);

    // Valores individuais
    double A1 = valorDoAtributo(A, atr1);
    double B1 = valorDoAtributo(B, atr1);
    double A2 = valorDoAtributo(A, atr2);
    double B2 = valorDoAtributo(B, atr2);

    // Resultado por atributo (usa operador ternário)
    int r1 = vencedorPorAtributo(A1, B1, atr1);
    int r2 = vencedorPorAtributo(A2, B2, atr2);

    // Soma para decisão final
    double somaA = A1 + A2;
    double somaB = B1 + B2;

    int vencedor_final = (somaA > somaB) ? 1 : (somaA < somaB) ? -1 : 0;

    // === Relatório organizado ===
    printf("\n============================================================\n");
    printf("Cartas:  A = %-12s   vs   B = %-12s\n", A->nome, B->nome);
    printf("------------------------------------------------------------\n");
    printf("Atributo 1: %s\n", nomeAtributo(atr1));
    printf("  %-12s: %10.2f   |   %-12s: %10.2f   => %s\n",
           A->nome, A1, B->nome, B1,
           r1 ==  1 ? "A venceu" :
           r1 == -1 ? "B venceu" : "Empate");

    printf("\nAtributo 2: %s\n", nomeAtributo(atr2));
    printf("  %-12s: %10.2f   |   %-12s: %10.2f   => %s\n",
           A->nome, A2, B->nome, B2,
           r2 ==  1 ? "A venceu" :
           r2 == -1 ? "B venceu" : "Empate");

    printf("\n------------------------- SOMA -----------------------------\n");
    printf("  Soma %-12s = %10.2f\n", A->nome, somaA);
    printf("  Soma %-12s = %10.2f\n", B->nome, somaB);

    printf("\n==================== RESULTADO FINAL =======================\n");
    if (vencedor_final == 0) {
        printf("Empate!\n");
    } else {
        const char* vencedorNome = (vencedor_final == 1) ? A->nome : B->nome;
        printf("Venceu: %s\n", vencedorNome);
    }
    printf("============================================================\n");

    return 0;
}