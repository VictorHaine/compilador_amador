#include <stdarg.h>
#include <stdio.h>
#include <escritor.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

/**
 *  Função para traduzir o tipo enum cor para os bits configuradores de cor no
 *  ambiente Windows.
 *
 *  @param p_cor Cor
 *
 *  @return Bits equivalentes ao enum cor p_cor
 */
int traduzir_cor_windows(enum cor p_cor)
{
    #ifdef _WIN32
    switch(p_cor)
    {
        case COR_VERMELHO:
            return FOREGROUND_RED;
            break;
            
        case COR_AZUL:
            return FOREGROUND_BLUE;
            break;
            
        case COR_VERDE:
            return FOREGROUND_GREEN;
            break;
            
        default:
            return FOREGROUND_RED ^ FOREGROUND_GREEN ^ FOREGROUND_BLUE;
    }
#else
    return 0;
#endif
}

/**
 *  Função para traduzir o tipo enum cor para os characteres de controle
 *  de cor no ambiente *Unix.
 *
 *  @param p_cor cor
 *
 *  @return Caracteres de configuração equivalentes ao enum cor p_cor
 */
char *traduzir_cor_unix(enum cor p_cor)
{
    switch(p_cor)
    {
        case COR_VERMELHO:
            return "\e[31m";
            break;
            
        case COR_VERDE:
            return "\e[32m";
            break;
            
        case COR_AMARELO:
            return "\e[33m";
            break;
        
        case COR_AZUL:
            return "\e[34m";
            break;
        
        case COR_MAGENTA:
            return "\e[35m";
            break;
        
        case COR_CIANO:
            return "\e[36m";
            break;
            
        case COR_BRANCO:
        case COR_PADRAO:
        default:
            return "\e[0m";
            break;
    }
}

/**
 *  Procedimento universal para impressão de strings no STDOUT.
 *
 *  @param p_cor  Cor
 *  @param format Formto
 *  @param ...    Argumentos Variáveis
 */
void imprimir(enum cor p_cor, const char *format, ... )
{
    char buffer[4096];
    va_list args;
    va_start (args, format);
    vsprintf (buffer,format, args);
    va_end (args);
    
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        WORD saved_attributes;

        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        saved_attributes = consoleInfo.wAttributes;
    
        SetConsoleTextAttribute(hConsole, traduzir_cor_windows(p_cor));
        printf(buffer);
    
        SetConsoleTextAttribute(hConsole, saved_attributes);
    #else
        printf("%s%s%s", traduzir_cor_unix(p_cor), buffer, traduzir_cor_unix(COR_PADRAO));
    #endif
}

/**
 *  Função para escrever a string enviada em um certo arquivo.
 *
 *  @param p_conteudo Conteudo a ser escrito
 *  @param p_caminho  Caminho do arquivo a ser escrito
 *
 *  @return 1 = sucesso
 */
int inserir_arquivo(char *p_conteudo, char *p_caminho)
{
    if (p_caminho == NULL)
        return 0;
    
    FILE *f = fopen(p_caminho, "a");
    
    if (!f)
        return 0;
    
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s, sizeof(s), "%c", tm);
    
    fprintf(f, "%s - Compilador LFC - 4ECR\n\n", s);
    fprintf(f, "%s", p_conteudo);
    
    fclose(f);
    
    return 1;
}