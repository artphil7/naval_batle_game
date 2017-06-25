/* SOURCE */

#include "nav_bat.h"

int matriz[10][10];
int matriz_cpu[10][10];
char mascara[10][10];
char mascara_cpu[10][10];

int limite_barcos[]={1,1,1,1,1};
int x=-1, y=-1;


/* Variaveis globais de apoio */
char *limpa_tela;
int pos_x,pos_y,nivel;

void sistema()
{
        // código para windows
        #if defined(WINDOWS)
        limpa_tela = "cls";
        // código para linux
        #elif defined(LINUX)
        limpa_tela = "clear";
        // código para OS X.
        #elif defined(OSX)
        limpa_tela = "clear";
        #endif
}

char **init_board(char a)
{
        int i;
        char **matriz = (char**)malloc(tam_mesa[nivel] * sizeof(char*));
        loop(i, tam_mesa[nivel])
        {
                matriz[i] = (char*)malloc(tam_mesa[nivel] * sizeof(char));
                memset(matriz[i], a, tam_mesa[nivel]);
        }

        return matriz;
}

void print_board(char **matriz)
{
        int i, j;

        system(limpa_tela);
        printf("    ");
        loop(i,tam_mesa[nivel]) printf("%c  ", ('M'+i));
        printf("\n\n");

        loop(i,tam_mesa[nivel])
        {
                printf("%c   ", ('A'+i));
                loop(j,tam_mesa[nivel])
                {
                        printf("%c  ", matriz[i][j]);
                }
                printf("\n");
        }
        printf("\n");
}

void print_game(char **m_a, char **m_b)
{
        int i, j;

        system(limpa_tela);
        printf("    ");
        loop(i,tam_mesa[nivel]) printf("%c  ", ('M'+i));
        printf("    ");
        loop(i,tam_mesa[nivel]) printf("%c  ", ('M'+i));
        printf("\n\n");

        loop(i,tam_mesa[nivel])
        {
                printf("%c   ", ('A'+i));
                loop(j,tam_mesa[nivel])
                {
                        printf("%c  ", m_a[i][j]);
                }
                printf("    ");
                loop(j,tam_mesa[nivel])
                {
                        printf("%c  ", m_a[i][j]);
                }
                printf("   %c", ('A'+i));
                printf("\n");
        }
        printf("\n");
}

int read_char (char i, char f)
{
        char c;

        scanf(" %c", &c);
        c = toupper(c);

        if (c < i) return -1;
        if (c > f) return -1;

        return (int) c - i;
}

int put_nav(char **matriz, int barco, int y, int x, int dir)
{
        int i;
        if (dir == 0)
        {
                if (x + barco >= tam_mesa[nivel]) return 1;

                for (i = 0; i < barco; i++)
                        if(matriz[y][x+i] != AGUA) return 2;

                for (i = 0; i < barco; i++)
                        matriz[y][x+i] = NAVE;
        }
        else if (dir == 1)
        {
                if (y + barco >= tam_mesa[nivel]) return 1;

                for (i = 0; i < barco; i++)
                        if(matriz[y+i][x] != AGUA) return 2;

                for (i = 0; i < barco; i++)
                        matriz[y+i][x] = NAVE;
        }
        else return 3;

        return 0;
}

void fill_auto(char **matriz)
{
        srand (time (NULL));

        int i;
        int barco = QTD_BARCOS-1;
        int total = 0;
        int limite[QTD_BARCOS];

        loop (i, QTD_BARCOS)
        {
                limite[i] = max_barcos[nivel][i];
                total += max_barcos[nivel][i];
        }
        while (total > 0)
        {
                printf("%s %d t= %d\n", nome_barcos[barco], limite[barco], total);
                if (limite[barco] == 0)
                        barco--;
                else if ((put_nav(matriz, barco+2, rand()%tam_mesa[nivel], \
                          rand()%tam_mesa[nivel], (rand()%2))) == 0)
                {
                        limite[barco]--;
                        total--;
                }
        }
}

void fill_man(char **matriz)
{
        int i;
        int linha, coluna, direcao, barco;
        int total = 0;
        int limite[QTD_BARCOS];

        loop (i, QTD_BARCOS)
        {
                limite[i] = max_barcos[nivel][i];
                total += max_barcos[nivel][i];
        }

        while (total > 0)
        {
                print_board(matriz);

                printf("Digite o numero barco:\n");
                loop(i, QTD_BARCOS)
                {
                        if (limite[i] > 0)
                        printf("%d - %12s ( %d )\n", i+1, nome_barcos[i], limite[i]);
                }

                while ((barco=read_char('1','0'+QTD_BARCOS)) < 0 || \
                        limite[barco] == 0)
                {
                        if (limite[barco] == 0)
                                printf("Todos os %s ja foram usados.\nTente novamente:\n", \
                                        nome_barcos[barco]);
                        else    printf("Entrada invalida.\nTente novamente:\n");
                }

                printf("Digite a linha do %s:\n", nome_barcos[barco]);
                while ((linha=read_char('A','A'+tam_mesa[nivel])) < 0)
                {
                        printf("Entrada invalida.\nTente novamente:\n");
                }

                printf("Digite a coluna do %s:\n", nome_barcos[barco]);
                while ((coluna=read_char('M','M'+tam_mesa[nivel])) < 0)
                {
                        printf("Entrada invalida.\nTente novamente:\n");
                }

                printf("Digite a orientacao do %s:\n 1 - Horizontal.\n 2 - Vertical.\n", \
                        nome_barcos[barco]);
                while ((direcao=read_char('1','2')) < 0)
                {
                        printf("Entrada invalida.\nTente novamente:\n");
                }

                if ((i=put_nav(matriz, barco+2, linha, coluna, direcao)) == 0)
                {
                        limite[barco]--;
                        total--;
                }
                else if (i == 1)
                        printf("O %s ultrapassa os limites do mapa\nTente novamente:\n", \
                                nome_barcos[barco]);
                else if (i == 2)
                        printf("O %s sobrepoe aliado\nTente novamente:\n", \
                                nome_barcos[barco]);
        }

}


void game()
{
        int   i;
        char  **matriz_a;
        char  **matriz_b;
        char  **mascara_a;
        char  **mascara_b;

        do {
                printf("Digite o nivel do jogo:\n");
        } while ((nivel=read_char('1','3')) < 0);

        matriz_a  = init_board(AGUA);
        matriz_b  = init_board(AGUA);
        mascara_a = init_board(VAZIO);
        mascara_b = init_board(VAZIO);

        fill_auto(matriz_a);
        fill_auto(matriz_b);

        print_game(mascara_a, mascara_b);

        loop(i, tam_mesa[nivel])
        {
                free(matriz_a[i]);
                free(matriz_b[i]);
                free(mascara_a[i]);
                free(mascara_b[i]);
        }
        free(matriz_a);
        free(matriz_b);
        free(mascara_a);
        free(mascara_b);
}









void imprime_matriz()
{
        system(limpa_tela);
        char c = 'A';  //Para as linhas começarem de A
        int i, j;
        //primeira linha
        printf("\t1  2  3  4  5  6  7  8  9  10\n\n");

        for (i=0; i<10; i++)
        {
                printf("%c:\t", c++);
                for (j=0; j<10; j++)
                {
                        printf("%d  ", matriz[i][j]);
                }
                printf("\n");
        }

        printf("------------------------------------\n");

}

void imprime_cpu()
{
        char c = 'A';  //Para as linhas começarem de A
        int i, j;
        //primeira linha
        printf("\t1  2  3  4  5  6  7  8  9  10\n\n");

        for (i=0; i<10; i++)
        {
                printf("%c:\t", c++);
                for (j=0; j<10; j++)
                {
                        printf("%d  ", matriz_cpu[i][j]);
                }
                printf("\n");
        }

        printf("------------------------------------\n");

}

void imprime_mascara()
{
        char c = 'A';  //Para as linhas começarem de A
        int i, j;
        //primeira linha
        printf("\t1  2  3  4  5  6  7  8  9  10\n\n");

        for (i=0; i<10; i++)
        {
                printf("%c:\t", c++);
                for (j=0; j<10; j++)
                {
                        printf("%c  ", mascara_cpu[i][j]);
                }
                printf("\n");
        }

        printf("------------------------------------\n");

}

void imprime_jogo()
{
        system("clear");
        char c = 'A';  //Para as linhas começarem de A
        int i, j;
        //primeira linha
        printf("\t1  2  3  4  5  6  7  8  9  10\t\t1  2  3  4  5  6  7  8  9  10\n\n");

        for (i=0; i<10; i++)
        {
                printf("%c:\t", c);
                for (j=0; j<10; j++)
                {
                        printf("%c  ", mascara[i][j]);
                }
                printf("\t%c:\t", c++);
                for (j=0; j<10; j++)
                {
                        printf("%c  ", mascara_cpu[i][j]);
                }
                printf("\n");
        }

        printf("------------------------------------\t------------------------------------\n");

}


int isere_barco (int barco, int y, int x, int dir, int matrizInsere[10][10])
{
        int i;
        if (dir == 1)
        {
                if (x + barco >= 10) return 1;

                for (i = 0; i < barco; i++)
                        if(matrizInsere[y][x+i] > 0) return 2;

                for (i = 0; i < barco; i++)
                        matrizInsere[y][x+i] = 1;
        }
        else if (dir == 2)
        {
                if (y + barco >= 10) return 1;

                for (i = 0; i < barco; i++)
                        if(matrizInsere[y+i][x] > 0) return 2;

                for (i = 0; i < barco; i++)
                        matrizInsere[y+i][x] = 1;
        }

        return 0;
}

void preenche_barcos()
{
        int i, ins;
        int linha, coluna;
        char clinha;                //caractere da linha
        int orientado;
        int barco;
        int maxbarcos = 0;
        int limite[4];
        char *tipo_barco[4] = {"Submarinos", "Contra_torpedeiros", "Navio_tanque", "Porta_avioes"};

        for (i=0; i<4; i++)
        {
                maxbarcos += limite_barcos[i];
                limite[i] = limite_barcos[i];
        }

        while (maxbarcos > 0)
        {
                printf("Digite o barco:\n2 - %s(%d)\n3 - %s(%d)\n4 - %s(%d)\n5 - %s(%d)\n", \
                       tipo_barco[0], limite[0], tipo_barco[1], limite[1], \
                       tipo_barco[2], limite[2], tipo_barco[3], limite[3]);
                scanf(" %d", &barco);
                while (limite[barco-2] == 0 || barco < 2 || barco > 5)
                {
                        if (limite[barco-2] == 0)
                                printf("Todos os %s ja foram usados.\nTente de novo\n", tipo_barco[barco-2]);
                        if (barco < 2 || barco > 5)
                                printf("Entrada invalida.\nTente de novo\n");
                        scanf(" %d", &barco);
                }

                printf("Digite a linha do %s:\n", tipo_barco[barco-2]);
                scanf(" %c", &clinha);
                clinha = toupper(clinha);
                while(clinha > 'J' || clinha < 'A')
                {
                        printf("Entrada invalida.\nDigite a linha do %s:\n", tipo_barco[barco-2]);
                        scanf(" %c", &clinha);
                }
                linha = (int)(clinha - 'A');

                printf("Digite a coluna do %s:\n", tipo_barco[barco-2]);
                scanf(" %d", &coluna);
                while (coluna < 1 || coluna > 10)
                {
                        printf("Entrada invalida.\nDigite a coluna do %s:\n", tipo_barco[barco-2]);
                        scanf(" %d", &coluna);
                }
                coluna -= 1;

                printf("Digite 1 para a orientacao horizontal e 2 para vertical do %s:\n", tipo_barco[barco-2]);
                scanf(" %d", &orientado);
                while (orientado < 1 || orientado > 2)
                {
                        printf("Entrada invalida.\nDigite 1 para a orientacao horizontal e 2 para vertical do %s:\n", tipo_barco[barco-2]);
                        scanf(" %d", &orientado);
                }

                ins = isere_barco(barco, linha, coluna, orientado, matriz);
                if (ins == 0)
                {
                        printf("Inserido! %d %d %d %d \n", barco, linha, coluna, orientado);
                        limite[barco-2]--;
                        maxbarcos--;
                        // system("clear");
                        imprime_matriz();
                }
                else if (ins == 1)
                        printf("O %s ultrapassa os limites do mapa\nTente de novo\n\n", tipo_barco[barco-2]);
                else if (ins == 2)
                        printf("O %s sobrepoe aliado\nTente de novo.\n\n", tipo_barco[barco-2]);

        }

}

void preenche_cpu()
{
        srand (time (NULL));

        int barco = 0;
        int ins, i;
        int limite[4];
        for (i=0; i<4; i++)
        {
                limite[i] = limite_barcos[i];
        }
        while (barco < 4)
        {
                ins = isere_barco(barco+2, rand()%10, rand()%10, ( rand()%2 )+1, matriz_cpu);
                if (ins == 0)
                {
                        limite[barco]--;
                        if (limite[barco] == 0) barco++;
                }
        }

}

int player_joga (int *barcos)
{
        int linha, coluna, feito=0;
        char clinha;

        while (!feito)
        {
                printf("Digite a linha que quer atacar\n");
                scanf(" %c", &clinha);
                clinha = toupper(clinha);
                while(clinha > 'J' || clinha < 'A')
                {
                        printf("Entrada invalida.\nDigite a linha que quer atacar\n");
                        scanf(" %c", &clinha);
                }
                linha = (int)(clinha - 'A');

                printf("Digite a coluna que quer atacar\n");
                scanf(" %d", &coluna);
                while (coluna < 1 || coluna > 10)
                {
                        printf("Entrada invalida.\nDigite a coluna que quer atacar\n");
                        scanf(" %d", &coluna);
                }
                coluna -= 1;

                if (matriz_cpu[linha][coluna] == 0)
                {
                        matriz_cpu[linha][coluna] = 2;
                        mascara_cpu[linha][coluna] = 'X';
                        printf("Voce errou!\n");
                        feito = 1;
                }
                else if (matriz_cpu[linha][coluna] == 1)
                {
                        matriz_cpu[linha][coluna] = 3;
                        mascara_cpu[linha][coluna] = '#';
                        (*barcos)--;
                        printf("Belo tiro!\n");
                        feito = 1;
                }
                else if (matriz_cpu[linha][coluna] > 1)
                {
                        printf("voce ja atirou aqui.\nTente denovo.\n");
                }
        }

        imprime_jogo();

        return 0;
}

int cpu_joga (int *barcos)
{
        int linha, coluna, feito=0;
        char clinha;

        while (!feito)
        {
                printf("Agora o inimigo vai atacar atacar\n");

                if (x>=0 && y>=0)
                {
                        if (x<9 && matriz[x+1][y] == 0)
                        {
                                linha = x+1;
                                coluna = y;
                        }
                        else if (y<9 && matriz[x][y+1] == 0)
                        {
                                linha = x;
                                coluna = y+1;
                        }
                        else if (x>0 && matriz[x-1][y] == 0)
                        {
                                linha = x-1;
                                coluna = y;
                        }
                        else if (y>0 && matriz[x][y-1] == 0)
                        {
                                linha = x;
                                coluna = y-1;
                        }
                        else
                        {
                                x = -1;
                                y = -1;
                                linha = rand()%10;
                                coluna = rand()%10;
                        }
                }
                else
                {
                        linha = rand()%10;
                        coluna = rand()%10;
                }


                if (matriz[linha][coluna] == 0)
                {
                        matriz[linha][coluna] = 2;
                        mascara[linha][coluna] = 'X';
                        printf("Ele errou!\n");
                        feito = 1;
                }
                else if (matriz[linha][coluna] == 1)
                {
                        x = linha, y = coluna;
                        matriz[linha][coluna] = 3;
                        mascara[linha][coluna] = '#';
                        printf("Ele te acertou!\n");
                        (*barcos)--;
                        feito = 1;
                }
        }

        imprime_jogo();

        return 0;
}

int jogar ()
{
        int i, barcos_player=0, barcos_cpu=0;

        for (i=0; i<4; i++)
        {
                barcos_player += limite_barcos[i] * (i+2);
                barcos_cpu += limite_barcos[i] * (i+2);
        }

        // printf("jogando");
        while (barcos_player > 0 && barcos_cpu > 0)
        {
                player_joga(&barcos_cpu);
                printf("%d X %d\n", barcos_player, barcos_cpu);
                if (barcos_cpu > 0)
                        cpu_joga(&barcos_player);
                printf("%d X %d\n - %d %d", barcos_player, barcos_cpu, x, y);
        }

        if (barcos_cpu == 0) printf("Parabens!!!!\nVoce ganhou!!!");
        else printf("####LOSER####");
        return 0;
}

int main ()
{
        sistema();
        game();
        /*
        int i, j, tam=10;

        int  **matriz_a;
        int  **matriz_b;
        char  **mascara_a;
        char  **mascara_b;
        // Identifica qual sistema esta sendo usado
        matriz_a = (int**)malloc(tam * sizeof(int*));
        matriz_b = (int**)malloc(tam * sizeof(int*));
        mascara_a = (char**)malloc(tam * sizeof(char*));
        mascara_b = (char**)malloc(tam * sizeof(char*));

        loop(i, tam)
        {
                matriz_a[i] = (int*)malloc(tam * sizeof(int));
                memset(matriz_a[i], 0, tam);
                matriz_b[i] = (int*)malloc(tam * sizeof(int));
                memset(matriz_b[i], 0, tam);
                mascara_a[i] = (char*)malloc(tam * sizeof(char));
                memset(mascara_a[i], '.', tam);
                mascara_b[i] = (char*)malloc(tam * sizeof(char));
                memset(mascara_b[i], '.', tam);

                for (j=0; j<10; j++)
                {
                        // mapa cpu
                        matriz_cpu[i][j]=0;
                        // mascara cpu
                        mascara_cpu[i][j]='.';
                        // mascara usuario
                        mascara[i][j]='.';
                        // mapa usuario
                        matriz[i][j]=0;
                }
        }
        print_board(matriz_a, 10);



        imprime_matriz();

        /*
           imprime_cpu();
           preenche_cpu();

           imprime_mascara();

           preenche_barcos();

           imprime_jogo();

           jogar();

        loop(i, tam)
        {
                free(matriz_a[i]);
                free(matriz_b[i]);
                free(mascara_a[i]);
                free(mascara_b[i]);
        }
        free(matriz_a);
        free(matriz_b);
        free(mascara_a);
        free(mascara_b);
        */

        return 0;
}
