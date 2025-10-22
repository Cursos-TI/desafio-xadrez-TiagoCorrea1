//===============================================================================================================
// PROJETO: WAR ESTRUTURADO (VERSÃO FINAL - JOGO DE TERMINAL)
//
// OBJETIVO:
// Criar um jogo de War funcional com uma interface de texto clara, seguindo
// um modelo de exibição específico, com mapa, missões e lógica de batalha por turnos.
//
//===============================================================================================================

// --- Bibliotecas Necessárias ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5  // O mapa terá 5 territórios fixos
#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 150

// --- Estrutura dos Territórios ---
struct Territorio {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
};

// --- Protótipos das Funções ---
void limparBufferEntrada();
void pausa();
void preencherMapaPadrao(struct Territorio* mapa);
void exibirTerritorios(const struct Territorio* mapa);
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
void atacar(struct Territorio* atacante, struct Territorio* defensor, const char* corJogador);
int verificarMissao(const char* missao, const char* corJogador, const struct Territorio* mapa);
void liberarMemoria(struct Territorio* mapa, char* missaoJogador);

//===============================================================================================================
// --- FUNÇÃO PRINCIPAL (main) ---
//===============================================================================================================
int main() {
    // --- Vetor de Missões ---
    // Missões simples para nosso jogo de 5 territórios
    const char* missoes[] = {
        "Destruir o exercito Verde.",
        "Conquistar 3 territorios no total.",
        "Eliminar o exercito Vermelho."
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // --- Variáveis do Jogo ---
    struct Territorio* mapa = NULL;
    char* missaoJogador = NULL;
    const char* corJogador = "Azul"; // O jogador será sempre o Azul
    
    int opcao = -1;
    int vencedor = 0;

    srand(time(NULL));

    // --- Alocação de Memória ---
    mapa = (struct Territorio*) malloc(NUM_TERRITORIOS * sizeof(struct Territorio));
    missaoJogador = (char*) malloc(TAM_MISSAO * sizeof(char));

    if (mapa == NULL || missaoJogador == NULL) {
        printf("ERRO CRITICO: Falha ao alocar memoria! Encerrando.\n");
        liberarMemoria(mapa, missaoJogador);
        return 1;
    }

    // --- Configuração do Jogo ---
    preencherMapaPadrao(mapa);
    atribuirMissao(missaoJogador, missoes, totalMissoes);

    // --- Loop Principal do Jogo ---
    do {
        // 1. Exibir o Mapa
        exibirTerritorios(mapa);

        // 2. Exibir a Missão
        printf("\n---SUA MISSAO (Exercito %s)---\n", corJogador);
        printf("%s\n", missaoJogador);

        // 3. Exibir o Menu
        printf("\n---MENU DE ACOES---\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missao\n");
        printf("0 - Sair\n");
        printf("Escolha sua acao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        // 4. Processar Ação
        switch (opcao) {
            case 1: { // --- FASE DE ATAQUE ---
                int idAtacante, idDefensor;
                printf("\n---FASE DE ATAQUE---\n");
                printf("Escolha o territorio atacante (1 a 5): ");
                scanf("%d", &idAtacante);
                printf("Escolha o territorio defensor (1 a 5): ");
                scanf("%d", &idDefensor);
                limparBufferEntrada();

                // Converte para índice de vetor (0 a 4)
                idAtacante--;
                idDefensor--;

                // Validações
                if (idAtacante < 0 || idAtacante >= NUM_TERRITORIOS || idDefensor < 0 || idDefensor >= NUM_TERRITORIOS) {
                    printf("!! ERRO: Numero de territorio invalido.\n");
                } else if (strcmp(mapa[idAtacante].cor, corJogador) != 0) {
                    printf("!! ERRO: Voce so pode atacar com seus territorios (%s).\n", corJogador);
                } else if (mapa[idAtacante].tropas < 2) {
                    printf("!! ERRO: E necessario ter pelo menos 2 tropas para atacar.\n");
                } else if (strcmp(mapa[idDefensor].cor, corJogador) == 0) {
                    printf("!! ERRO: Voce nao pode atacar um territorio que ja e seu.\n");
                } else {
                    // Se tudo estiver OK, ataca
                    atacar(&mapa[idAtacante], &mapa[idDefensor], corJogador);
                    // Verifica se o jogo acabou
                    if (verificarMissao(missaoJogador, corJogador, mapa)) {
                        vencedor = 1;
                    }
                }
                if(vencedor == 0) pausa(); // Pausa apenas se o jogo não acabou
                break;
            }
            case 2: // --- VERIFICAR MISSÃO ---
                if (verificarMissao(missaoJogador, corJogador, mapa)) {
                    printf("\n*** PARABENS! Voce cumpriu sua missao! ***\n");
                    vencedor = 1;
                } else {
                    printf("\nSua missao ainda nao foi cumprida. Continue lutando!\n");
                }
                pausa();
                break;
            case 0: // --- SAIR ---
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                pausa();
                break;
        }

    } while (opcao != 0 && vencedor == 0); // O jogo termina se o usuário sair (0) ou vencer (1)

    // --- Fim de Jogo ---
    if (vencedor) {
        printf("\n\n=================================================\n");
        printf("         VITORIA DO EXERCITO %s! \n", corJogador);
        printf("         Missao Cumprida: %s\n", missaoJogador);
        printf("=================================================\n");
        exibirTerritorios(mapa);
    }

    liberarMemoria(mapa, missaoJogador);
    return 0;
}

//===============================================================================================================
// --- IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES ---
//===============================================================================================================

// Limpa o buffer de entrada (consome o 'Enter' pendente)
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Pausa o jogo e espera o usuário pressionar Enter
void pausa() {
    printf("\nPressione Enter para continuar...");
    limparBufferEntrada();
}

// Preenche o mapa com os 5 territórios padrão do seu exemplo
void preencherMapaPadrao(struct Territorio* mapa) {
    strcpy(mapa[0].nome, "America");
    strcpy(mapa[0].cor, "Verde");
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Europa");
    strcpy(mapa[1].cor, "Azul");
    mapa[1].tropas = 3;

    strcpy(mapa[2].nome, "Asia");
    strcpy(mapa[2].cor, "Vermelho");
    mapa[2].tropas = 2;

    strcpy(mapa[3].nome, "Africa");
    strcpy(mapa[3].cor, "Amarelo");
    mapa[3].tropas = 4;

    strcpy(mapa[4].nome, "Oceania");
    strcpy(mapa[4].cor, "Branco");
    mapa[4].tropas = 1;
}

// Exibe o mapa formatado EXATAMENTE como no seu exemplo
void exibirTerritorios(const struct Territorio* mapa) {
    // O especificador '-%ds' alinha o texto à esquerda com um número de espaços
    // %-12s -> Alinha o nome em 12 espaços (América     )
    // %-10s -> Alinha a cor em 10 espaços  (Verde     )
    printf("\n===============MAPA DO MUNDO=====================\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%d. %-12s (Exercito: %-10s , Tropas: %d)\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
    printf("=================================================\n");
}

// Sorteia e atribui uma missão
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

// Lógica de ataque modificada para "perder 1 tropa", como no seu exemplo
void atacar(struct Territorio* atacante, struct Territorio* defensor, const char* corJogador) {
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("\n---RESULTADO DA BATALHA---\n");
    printf("Ataque (%s): %d | Defesa (%s): %d\n", atacante->nome, dadoAtaque, defensor->nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        defensor->tropas--;

        // Verifica se o território foi conquistado
        if (defensor->tropas == 0) {
            printf("CONQUISTA! O territorio %s foi dominado pelo exercito %s!\n", defensor->nome, corJogador);
            strcpy(defensor->cor, corJogador);
            defensor->tropas = 1; // O atacante move 1 tropa para ocupar
            atacante->tropas--;   // A tropa que moveu
        }
    } else {
        printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        atacante->tropas--;

        // Verifica se o atacante perdeu seu território
        if (atacante->tropas == 0) {
            printf("DERROTA CRITICA! Seu territorio %s ficou indefeso e agora e Neutro.\n", atacante->nome);
            strcpy(atacante->cor, "Neutro"); // O território fica sem dono
        }
    }
}

// Verifica se a missão do jogador foi cumprida
int verificarMissao(const char* missao, const char* corJogador, const struct Territorio* mapa) {
    // Lógica para "Destruir o exercito Verde."
    if (strcmp(missao, "Destruir o exercito Verde.") == 0) {
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0) {
                return 0; // Se encontrar QUALQUER território Verde, a missão falha.
            }
        }
        return 1; // Se o loop terminar sem achar Verdes, a missão está completa.
    }

    // Lógica para "Conquistar 3 territorios no total."
    if (strcmp(missao, "Conquistar 3 territorios no total.") == 0) {
        int contador = 0;
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contador++;
            }
        }
        return (contador >= 3); // Retorna 1 se tiver 3 ou mais, senão 0.
    }
    
    // Lógica para "Eliminar o exercito Vermelho."
    if (strcmp(missao, "Eliminar o exercito Vermelho.") == 0) {
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) {
                return 0; // Achou um Vermelho, missão incompleta
            }
        }
        return 1; // Não achou Vermelhos, missão completa
    }

    return 0; // Missão desconhecida
}

// Libera toda a memória alocada
void liberarMemoria(struct Territorio* mapa, char* missaoJogador) {
    printf("\nLiberando memoria e encerrando o jogo.\n");
    if (mapa != NULL) {
        free(mapa);
    }
    if (missaoJogador != NULL) {
        free(missaoJogador);
    }
}