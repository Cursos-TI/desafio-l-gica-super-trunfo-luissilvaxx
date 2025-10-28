/* super_trunfo_aventureiro.c
 * Nível Aventureiro — Menu (switch) + comparação aninhada
 *
 * Atributos: Nome (string, informativo), População (int), Área (float km²),
 * PIB (float, bilhões US$), Pontos turísticos (int), Densidade (float, hab/km²).
 * Regra geral: MAIOR vence. Exceção: Densidade -> MENOR vence.
 * Empate: if-else aninhado aplicando critérios secundários.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char   nome[64];
    int    populacao;     // habitantes
    float  area_km2;      // km²
    float  pib_bi;        // bilhões US$
    int    pontos;        // nº de pontos turísticos
    float  densidade;     // hab/km² = populacao / area_km2
} Carta;

/* ------------ utilidades de E/S seguras ------------ */
static void trim(char *s){ if(!s) return; size_t n=strlen(s); while(n&&isspace((unsigned char)s[n-1])) s[--n]='\0'; }
static void ler_str(const char* p,char*buf,size_t n){
    for(;;){ fputs(p,stdout); if(!fgets(buf,(int)n,stdin)) exit(0); trim(buf); if(*buf) return; puts("Entrada vazia. Tente novamente."); }
}
static int ler_int(const char* p){
    char t[64]; for(;;){ ler_str(p,t,sizeof t); char*e; long v=strtol(t,&e,10);
        if(*t&&*e=='\0') return (int)v; puts("Inteiro inválido. Tente novamente."); }
}
static float ler_float(const char* p){
    char t[64]; for(;;){ ler_str(p,t,sizeof t); char*e; float v=strtof(t,&e);
        if(*t&&*e=='\0') return v; puts("Número inválido (ex.: 8515767.12)."); }
}
static void pause_enter(void){ fputs("\nPressione ENTER para continuar...",stdout); int c; while((c=getchar())!='\n'&&c!=EOF){} }

/* ------------ cadastro / exibição ------------ */
static void calc_dens(Carta* c){ c->densidade = (c->area_km2>0)? (c->populacao / c->area_km2) : 0.0f; }

static void cadastrar(Carta* c,int idx){
    printf("\n=== Cadastro da Carta %d ===\n", idx);
    ler_str("Nome do país: ", c->nome, sizeof c->nome);
    c->populacao = ler_int("População (habitantes): ");
    c->area_km2  = ler_float("Área (km²): ");
    c->pib_bi    = ler_float("PIB (bilhões US$): ");
    c->pontos    = ler_int("Pontos turísticos: ");
    calc_dens(c);
    printf("Densidade calculada: %.2f hab/km²\n", c->densidade);
}

static void mostrar(const Carta* a, const Carta* b){
    puts("\n------ CARTA 1 ------");
    printf("País: %s\nPopulação: %d\nÁrea: %.2f km²\nPIB: %.2f bi US$\nPontos: %d\nDensidade: %.2f hab/km²\n",
           a->nome,a->populacao,a->area_km2,a->pib_bi,a->pontos,a->densidade);
    puts("------ CARTA 2 ------");
    printf("País: %s\nPopulação: %d\nÁrea: %.2f km²\nPIB: %.2f bi US$\nPontos: %d\nDensidade: %.2f hab/km²\n\n",
           b->nome,b->populacao,b->area_km2,b->pib_bi,b->pontos,b->densidade);
}

/* ------------ desempates (if-else aninhados) ------------ */
static const char* emp_nome(const char* n1,const char* n2){
    int cmp = strcmp(n1,n2);
    if(cmp<0) return n1; else if(cmp>0) return n2; else return NULL;
}

/* Cadeia de desempate padrão (quando o atributo principal NÃO é densidade):
   1) PIB (maior) -> 2) Área (maior) -> 3) Pontos (maior) -> 4) Densidade (menor) -> 5) Nome (A–Z) */
static const char* desempate_padrao(const Carta* c1,const Carta* c2){
    if(c1->pib_bi   > c2->pib_bi)   return c1->nome; else if(c1->pib_bi   < c2->pib_bi)   return c2->nome;
    if(c1->area_km2 > c2->area_km2) return c1->nome; else if(c1->area_km2 < c2->area_km2) return c2->nome;
    if(c1->pontos   > c2->pontos)   return c1->nome; else if(c1->pontos   < c2->pontos)   return c2->nome;
    if(c1->densidade< c2->densidade)return c1->nome; else if(c1->densidade> c2->densidade)return c2->nome;
    return emp_nome(c1->nome,c2->nome);
}

/* Cadeia de desempate quando o principal É densidade:
   1) Pontos (maior) -> 2) PIB (maior) -> 3) Área (maior) -> 4) Nome (A–Z) */
static const char* desempate_dens(const Carta* c1,const Carta* c2){
    if(c1->pontos   > c2->pontos)   return c1->nome; else if(c1->pontos   < c2->pontos)   return c2->nome;
    if(c1->pib_bi   > c2->pib_bi)   return c1->nome; else if(c1->pib_bi   < c2->pib_bi)   return c2->nome;
    if(c1->area_km2 > c2->area_km2) return c1->nome; else if(c1->area_km2 < c2->area_km2) return c2->nome;
    return emp_nome(c1->nome,c2->nome);
}

/* ------------ comparação por atributo (switch + if-else aninhados) ------------ */
static void comparar(const Carta* c1,const Carta* c2,int op){
    switch(op){
        case 1: // Nome — apenas exibe
            printf("\n[Atributo informativo]\n%s vs %s\n\n", c1->nome, c2->nome);
            break;

        case 2: { // População — maior vence
            if (c1->populacao > c2->populacao) {
                printf("\nAtributo: População\n%s: %d  |  %s: %d\nVencedora: %s\n\n",
                       c1->nome,c1->populacao,c2->nome,c2->populacao,c1->nome);
            } else if (c1->populacao < c2->populacao) {
                printf("\nAtributo: População\n%s: %d  |  %s: %d\nVencedora: %s\n\n",
                       c1->nome,c1->populacao,c2->nome,c2->populacao,c2->nome);
            } else { // empate -> aninhados
                const char* v = desempate_padrao(c1,c2);
                printf("\nAtributo: População (empate)\n%s: %d  |  %s: %d\n",
                       c1->nome,c1->populacao,c2->nome,c2->populacao);
                printf("%s\n\n", v? (char[]){'V','e','n','c','e','d','o','r','a',':',' ',0}, v : "Empate!");
                if (v) printf("Vencedora: %s\n\n", v);
            }
        } break;

        case 3: { // Área — maior vence
            if (c1->area_km2 > c2->area_km2) {
                printf("\nAtributo: Área\n%s: %.2f km²  |  %s: %.2f km²\nVencedora: %s\n\n",
                       c1->nome,c1->area_km2,c2->nome,c2->area_km2,c1->nome);
            } else if (c1->area_km2 < c2->area_km2) {
                printf("\nAtributo: Área\n%s: %.2f km²  |  %s: %.2f km²\nVencedora: %s\n\n",
                       c1->nome,c1->area_km2,c2->nome,c2->area_km2,c2->nome);
            } else {
                const char* v = desempate_padrao(c1,c2);
                printf("\nAtributo: Área (empate)\n%s: %.2f km²  |  %s: %.2f km²\n",
                       c1->nome,c1->area_km2,c2->nome,c2->area_km2);
                if (v) printf("Vencedora: %s\n\n", v); else puts("Empate!\n");
            }
        } break;

        case 4: { // PIB — maior vence
            if (c1->pib_bi > c2->pib_bi) {
                printf("\nAtributo: PIB\n%s: %.2f bi US$  |  %s: %.2f bi US$\nVencedora: %s\n\n",
                       c1->nome,c1->pib_bi,c2->nome,c2->pib_bi,c1->nome);
            } else if (c1->pib_bi < c2->pib_bi) {
                printf("\nAtributo: PIB\n%s: %.2f bi US$  |  %s: %.2f bi US$\nVencedora: %s\n\n",
                       c1->nome,c1->pib_bi,c2->nome,c2->pib_bi,c2->nome);
            } else {
                const char* v = desempate_padrao(c1,c2);
                printf("\nAtributo: PIB (empate)\n%s: %.2f bi US$  |  %s: %.2f bi US$\n",
                       c1->nome,c1->pib_bi,c2->nome,c2->pib_bi);
                if (v) printf("Vencedora: %s\n\n", v); else puts("Empate!\n");
            }
        } break;

        case 5: { // Pontos turísticos — maior vence
            if (c1->pontos > c2->pontos) {
                printf("\nAtributo: Pontos turísticos\n%s: %d  |  %s: %d\nVencedora: %s\n\n",
                       c1->nome,c1->pontos,c2->nome,c2->pontos,c1->nome);
            } else if (c1->pontos < c2->pontos) {
                printf("\nAtributo: Pontos turísticos\n%s: %d  |  %s: %d\nVencedora: %s\n\n",
                       c1->nome,c1->pontos,c2->nome,c2->pontos,c2->nome);
            } else {
                const char* v = desempate_padrao(c1,c2);
                printf("\nAtributo: Pontos turísticos (empate)\n%s: %d  |  %s: %d\n",
                       c1->nome,c1->pontos,c2->nome,c2->pontos);
                if (v) printf("Vencedora: %s\n\n", v); else puts("Empate!\n");
            }
        } break;

        case 6: { // Densidade — MENOR vence
            if (c1->densidade < c2->densidade) {
                printf("\nAtributo: Densidade (MENOR vence)\n%s: %.2f  |  %s: %.2f\nVencedora: %s\n\n",
                       c1->nome,c1->densidade,c2->nome,c2->densidade,c1->nome);
            } else if (c1->densidade > c2->densidade) {
                printf("\nAtributo: Densidade (MENOR vence)\n%s: %.2f  |  %s: %.2f\nVencedora: %s\n\n",
                       c1->nome,c1->densidade,c2->nome,c2->densidade,c2->nome);
            } else {
                const char* v = desempate_dens(c1,c2);
                printf("\nAtributo: Densidade (empate)\n%s: %.2f  |  %s: %.2f\n",
                       c1->nome,c1->densidade,c2->nome,c2->densidade);
                if (v) printf("Vencedora: %s\n\n", v); else puts("Empate!\n");
            }
        } break;

        default:
            puts("\nOpção inválida! Use um número entre 0 e 6.\n");
            break;
    }
}

/* -------------------- menu -------------------- */
static void menu(void){
    puts("======= SUPER TRUNFO — NÍVEL AVENTUREIRO =======");
    puts("1) Nome do país (apenas exibe)");
    puts("2) População (maior vence)");
    puts("3) Área (maior vence)");
    puts("4) PIB (maior vence)");
    puts("5) Pontos turísticos (maior vence)");
    puts("6) Densidade demográfica (MENOR vence)");
    puts("0) Sair");
    puts("===============================================");
}

int main(void){
    setbuf(stdout, NULL);

    Carta c1 = {0}, c2 = {0};
    char resp[8];

    puts("Bem-vindo ao Super Trunfo — Nível Aventureiro!");

    /* Para facilitar, você pode usar cartas do nível anterior ou cadastrar agora */
    printf("\nDeseja usar cartas de EXEMPLO? (S/N): ");
    if(!fgets(resp,sizeof resp,stdin)) return 0;
    int usar_exemplo = (resp[0]=='S'||resp[0]=='s');

    if(usar_exemplo){
        strcpy(c1.nome,"Brasil");    c1.populacao=203000000; c1.area_km2=8515767.0f; c1.pib_bi=2280.0f; c1.pontos=25; calc_dens(&c1);
        strcpy(c2.nome,"Argentina"); c2.populacao= 45800000; c2.area_km2=2780400.0f; c2.pib_bi= 643.0f; c2.pontos=18; calc_dens(&c2);
    }else{
        cadastrar(&c1,1);
        cadastrar(&c2,2);
    }

    mostrar(&c1,&c2);

    for(;;){
        menu();
        int op = ler_int("Escolha o atributo (0–6): ");
        if(op==0){ puts("Obrigado por jogar!"); break; }
        comparar(&c1,&c2,op);
        pause_enter();
    }
    return 0;
}

