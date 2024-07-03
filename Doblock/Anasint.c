#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Analex.h"
#include "Anasint.h"
#include "Funcaux.h"
#include "Tabidentific.h"

/*funções auxiliares para evitar repetição*/
void DeclareVariableBlock()
{
    int posicaoIdentificador;
    token = Analex(codeFile);

    if (token.categoria == SN && token.codigo != ADDRESS)
    {
        token = Analex(codeFile);
    }

    posicaoIdentificador = idVazio(1, tipo(), "ARG", false);

    while (token.categoria == SN && token.codigo == OPEN_BRACKET)
    {
        token = Analex(codeFile);
        if (token.categoria != SN || token.codigo != CLOSE_BRACKET)
        {
            erro("Sinal ']' esperado!");
        }
        if(tabIdentif.identificador[posicaoIdentificador].ehEscalar)
            tabIdentif.identificador[posicaoIdentificador].ehEscalar = false;

        tabIdentif.identificador[posicaoIdentificador].qtdDimensoes++;
        // adiciona dimensao no posicaoIdentificador
        token = Analex(codeFile);
    }
}

int idVazio(int escopo, int tipoVariavel, char categoria[3], bool ehConstante)
{
    return InsereTabelaID("", escopo, tipoVariavel, categoria, ehConstante); // retorna posicao de inserção
}

int id(int escopo, int tipoVariavel, char categoria[3], bool ehConstante)
{
    int posicaoIdentificador;

    if(token.categoria != ID)
        erro("Identificador esperado!");
    
    if ((posicaoIdentificador = BuscaTabelaIDMesmoEscopo(token.lexema, escopo)) < 0)
        posicaoIdentificador = InsereTabelaID(token.lexema, escopo, tipoVariavel, categoria, ehConstante);
    token = Analex(codeFile);

    return posicaoIdentificador;
}

int checaID()
{
    int posicaoIdentificador;

    if(token.categoria != ID)
        erro("Identificador esperado!");
    
    if ((posicaoIdentificador = BuscaTabelaID(token.lexema)) < 0)
        erro("Identificador não declarado");

    token = Analex(codeFile);

    return posicaoIdentificador;
}

void endBlock(bool ehBlockMain, int posicaoArgumento)
{
    int posicaoUltimoInserido = tabIdentif.tamTab - 1; // ultimo token

    if (!(token.categoria == PALAVRA_RESERVADA && token.codigo == ENDBLOCK))
        erro("Palavra reservada 'endblock' esperado!");

    while ((tabIdentif.identificador[posicaoUltimoInserido].escopo == 1) && (strcmp(tabIdentif.identificador[posicaoUltimoInserido].categoria, "VL") == 0))
    {
        posicaoUltimoInserido = removeUltimoIDInserido(posicaoUltimoInserido);
        tabIdentif.tamTab--;
    }

    while (strcmp(tabIdentif.identificador[posicaoArgumento].categoria, "ARG") == 0 && !ehBlockMain)
    {
        tabIdentif.identificador[posicaoArgumento].statusAtivo = false;
        posicaoArgumento --;
    }
    

    token = Analex(codeFile);
}

void decl_tipo_id_brackeys(int posicaoIdentificador)
{
    int idBuscado;
    int posicaoBracket = 0;

    if(tabIdentif.identificador[posicaoIdentificador].tipoVariavel != tipo())
        erro("Tipo não compatível com a declaração!");

    if(token.categoria != ID)
        erro("Identificador esperado!");

    strcpy(tabIdentif.identificador[posicaoIdentificador].nomeId, token.lexema);

    token = Analex(codeFile);

    while (token.categoria == SN && token.codigo == OPEN_BRACKET && posicaoBracket < tabIdentif.identificador[posicaoIdentificador].qtdDimensoes)
    {
        token = Analex(codeFile);
        if (token.categoria == CT_INT)
        {
            tabIdentif.identificador[posicaoIdentificador].dimensoes[0][posicaoBracket] = token.valor_int;
            tabIdentif.identificador[posicaoIdentificador].dimensoes[1][posicaoBracket] = 0;
            token = Analex(codeFile);
        }
        else if (token.categoria == ID)
        {
            idBuscado = BuscaTabelaID(token.lexema);

            if(idBuscado < 0)
                erro("Variável não declarada!");

            tabIdentif.identificador[posicaoIdentificador].dimensoes[0][posicaoBracket] = idBuscado;
            tabIdentif.identificador[posicaoIdentificador].dimensoes[1][posicaoBracket] = 1;
            
            token = Analex(codeFile); // Precisa ser constante inteira
        }

        if (token.categoria != SN || token.codigo != CLOSE_BRACKET)
        {
            erro("Sinal ']' esperado!");
        }
        posicaoBracket ++;
        token = Analex(codeFile);
    }

    if(token.codigo == OPEN_BRACKET)
        erro("Quantidade de dimensoes incompatíveis com a declaração.");
}

/* Funções seguindo a gramática */
void prog()
{
    token = Analex(codeFile);
    decl_list_var(0);
    while(token.categoria == PALAVRA_RESERVADA && token.codigo == BLOCK && tokenLookAhead.categoria == ID)
    {
        decl_block_prot();
    }
    block_main();
    block_def();
} 

/*Escopo sendo 0 global e 1 local*/
void decl_list_var(int escopo)
{
    while(token.categoria == PALAVRA_RESERVADA && (token.codigo == CONST || EH_TIPO))
    {
        bool ehConstante = false;
        
        if (token.categoria == PALAVRA_RESERVADA && token.codigo == CONST) // const
        {
            token = Analex(codeFile);
            ehConstante = true;
        }

        decl_var(escopo, tipo(), ehConstante);

        while(token.categoria == SN && token.codigo == COMMA)
        {
            token = Analex(codeFile);
            decl_var(escopo, tipo(), ehConstante);
        }
    }
}

void decl_block_prot()
{
        if (token.categoria == PALAVRA_RESERVADA)
        {
            if (token.codigo != BLOCK)
                erro("Palavra reservada 'block' esperado!");
        }
        else
            erro("Palavra reservada esperada!");

        token = Analex(codeFile);

        id(0, BLOCK, "BLK", false);

        if(token.categoria == PALAVRA_RESERVADA &&  token.codigo == WITH)
        {
            DeclareVariableBlock();
            while(token.categoria == SN &&  token.codigo == COMMA)
            {
                DeclareVariableBlock();
            }
        }
}

void block_main()
{
    if (token.categoria != PALAVRA_RESERVADA && token.codigo != BLOCK)
    {
		erro("Palavra reservada 'block' esperado!");
	}

    token = Analex(codeFile);

    if (token.categoria != PALAVRA_RESERVADA && token.codigo != MAIN)
    {
		erro("Palavra reservada 'main' esperado!");
	}

    token = Analex(codeFile);

    decl_list_var(1);
    cmd();

    endBlock(true, 0);
}

void decl_var(int escopo, int tipo, bool ehConstante)
{
    int idBuscado;
    int posicaoIdentificador;
    int posicaoBracket = 0;

    if(escopo == 0)
        posicaoIdentificador = id(escopo, tipo, "VG", ehConstante);
    else if (escopo == 1)
        posicaoIdentificador = id(escopo, tipo, "VL", ehConstante);

    while(token.categoria == SN && token.codigo == OPEN_BRACKET)
    {
        token = Analex(codeFile);
        
        tabIdentif.identificador[posicaoIdentificador].qtdDimensoes++;
        tabIdentif.identificador[posicaoIdentificador].ehEscalar = false;
        if(token.categoria == CT_INT)
        {
            tabIdentif.identificador[posicaoIdentificador].dimensoes[0][posicaoBracket] = token.valor_int;
            tabIdentif.identificador[posicaoIdentificador].dimensoes[1][posicaoBracket] = 0;
            token = Analex(codeFile);
        }
        else if (token.categoria == ID)
        {
            idBuscado = BuscaTabelaID(token.lexema);

            if(idBuscado < 0)
                erro("Variável não declarada!");

            tabIdentif.identificador[posicaoIdentificador].dimensoes[0][posicaoBracket] = idBuscado;
            tabIdentif.identificador[posicaoIdentificador].dimensoes[1][posicaoBracket] = 1;
            
            token = Analex(codeFile); // Precisa ser constante inteira
        }
        else
        {
            erro("Constante inteira ou identificador esperado!");
        }

        if (token.categoria != SN && token.codigo != CLOSE_BRACKET)
        {
            erro("Palavra reservada ']' esperado!");
        }

        posicaoBracket++;

        token = Analex(codeFile);
    }

    if (token.categoria == SN && token.codigo == ATTRIB)
    {
        token = Analex(codeFile);
        if (INTCON || REALCON || CHARCON || STRINGCON){}
        else if (token.categoria == SN && token.codigo == OPEN_KEY)
        {
            token = Analex(codeFile);
            if (!(INTCON || REALCON || CHARCON || STRINGCON)) //VALOR CONSTANTE
            {
                erro("Constante esperada!");
            }

            token = Analex(codeFile);
            
            while(token.categoria == SN && token.codigo == COMMA)
            {
                token = Analex(codeFile);
                if (!(INTCON || REALCON || CHARCON || STRINGCON)) //VALOR CONSTANTE
                {
                    erro("Constante esperada!");
                }
                token = Analex(codeFile);
            }

            if (token.categoria != PALAVRA_RESERVADA && token.codigo != CLOSE_KEY)
            {
                erro("Palavra reservada '}' esperado!");
            }
        }
        token = Analex(codeFile);
    }
}

int tipo()
{
    int aux;
    if(token.categoria == PALAVRA_RESERVADA && EH_TIPO)
    {
        aux = token.codigo;
        token = Analex(codeFile);
    }
    else
        erro("Tipo esperado esperado!");

    return aux;
}

void block_def()
{
    while(token.categoria == PALAVRA_RESERVADA && token.codigo == BLOCK)
    {
        int posicaoIdentificador;
        token = Analex(codeFile);

        posicaoIdentificador = checaID();

        if(token.categoria == PALAVRA_RESERVADA &&  token.codigo == WITH)
        {
            posicaoIdentificador++;
            token = Analex(codeFile);
            decl_tipo_id_brackeys(posicaoIdentificador);

            while(token.categoria == SN && token.codigo == COMMA)
            {
                posicaoIdentificador++;
                token = Analex(codeFile);
                decl_tipo_id_brackeys(posicaoIdentificador);
            }
        }
        decl_list_var(1);
        cmd();
        endBlock(false, posicaoIdentificador);
    }
}

void cmd()
{
    while((token.categoria == PALAVRA_RESERVADA && EH_COMANDO || token.categoria == ID && tokenLookAhead.categoria == SN && (tokenLookAhead.codigo == ATTRIB || tokenLookAhead.codigo == OPEN_BRACKET)))
    {
        cmdOnce();
    }
}

void cmdOnce()
{
    if (token.categoria == ID && tokenLookAhead.categoria == SN && (tokenLookAhead.codigo == ATTRIB || tokenLookAhead.codigo == OPEN_BRACKET))
    {
        attrib();
    }
    else if (token.codigo == IF)
    {
        token = Analex(codeFile);
        if (token.categoria == SN && token.codigo == OPEN_PARENTHESIS)
        {
            token = Analex(codeFile);
            expr();
            if (!(token.categoria == SN && token.codigo == CLOSE_PARENTHESIS))
            {
                erro("Palavra reservada ')' esperado!");
            }
            token = Analex(codeFile);
            cmd();

            while (token.categoria == PALAVRA_RESERVADA && token.codigo == ELSEIF)
            {
                token = Analex(codeFile);
                if (token.categoria == SN && token.codigo == OPEN_PARENTHESIS)
                {
                    token = Analex(codeFile);
                    expr();
                    if (token.categoria == SN && token.codigo == CLOSE_PARENTHESIS)
                    {
                        token = Analex(codeFile);
                        cmd();
                    }
                    else
                        erro("Palavra reservada ')' esperado!");
                }
                else
                    erro("Palavra reservada '(' esperado!");
            }

            if (token.categoria == PALAVRA_RESERVADA && token.codigo == ELSE)
            {
                token = Analex(codeFile);
                cmd();
            }

            if (!(token.categoria == PALAVRA_RESERVADA && token.codigo == ENDIF))
            {
                erro("Palavra reservada 'ENDIF' esperado!");
            }

            token = Analex(codeFile);
        }
        else
            erro("Palavra reservada '(' esperado!");
    }
    else if (token.codigo == WHILE)
    {
        token = Analex(codeFile);
        if (token.categoria == SN && token.codigo == OPEN_PARENTHESIS)
        {
            token = Analex(codeFile);
            expr();
            if (!(token.categoria == SN && token.codigo == CLOSE_PARENTHESIS))
            {
                erro("Palavra reservada ')' esperado!");
            }

            token = Analex(codeFile);

            cmd();

            if (!(token.categoria == PALAVRA_RESERVADA && token.codigo == ENDWHILE))
            {
                erro("Palavra reservada 'ENDWHILE' esperado!");
            }
            token = Analex(codeFile);
        }
        else
            erro("Palavra reservada '(' esperado!");
    }
    else if (token.codigo == DO)
    {
        token = Analex(codeFile);

        if (token.categoria == ID && !(tokenLookAhead.categoria == SN && tokenLookAhead.codigo == ATTRIB))
        {
            token = Analex(codeFile);
            if (token.categoria == PALAVRA_RESERVADA && token.codigo == WITH)
            {
                token = Analex(codeFile);
                checaID();
                while (token.categoria == SN && token.codigo == COMMA)
                {
                    token = Analex(codeFile);
                    checaID();
                }
            }
        }
        else
            cmdOnce();

        if(token.categoria == PALAVRA_RESERVADA && token.codigo == VARYING)
        {
            token = Analex(codeFile);
            checaID();
            if(token.categoria == PALAVRA_RESERVADA && token.codigo == FROM)
            {
                token = Analex(codeFile);
                expr();
                if (!(token.categoria == PALAVRA_RESERVADA && (token.codigo == TO || token.codigo == DOWNTO)))
                {
                    erro("Palavra reservada 'to' ou 'downto' esperado!");
                }
                token = Analex(codeFile);
                expr();
            }
        }
        else if(token.categoria == PALAVRA_RESERVADA && token.codigo == WHILE)
        {
            token = Analex(codeFile);
            if (token.categoria == SN && token.codigo == OPEN_PARENTHESIS)
            {
                token = Analex(codeFile);
                expr();
                if (!(token.categoria == SN && token.codigo == CLOSE_PARENTHESIS))
                {
                    erro("Palavra reservada ')' esperado!");
                }
            }
        }
        else if(token.categoria == PALAVRA_RESERVADA && token.codigo == FOR)
        {
            token = Analex(codeFile);
            expr();
            if(!(token.categoria == PALAVRA_RESERVADA && token.codigo == TIMES))
                erro("Palavra reservada 'times' esperado!");
        }        
    }   
    else if (token.codigo == GOBACK)
    {
        token = Analex(codeFile);
    }
    else if (token.codigo == GETINT)
    {
        token = Analex(codeFile);
        checaID();
    }
    else if (token.codigo == GETREAL)
    {
        token = Analex(codeFile);
        checaID();
    }
    else if (token.codigo == GETCHAR)
    {
        token = Analex(codeFile);
        checaID();
    }
    else if (token.codigo == PUTINT)
    {
        token = Analex(codeFile);
        checaID();
    }
    else if (token.codigo == PUTREAL)
    {
        token = Analex(codeFile);
        checaID();
    }
    else if (token.codigo == PUTCHAR)
    {
        token = Analex(codeFile);
        checaID();
    }
    else
    {
        erro("Comando não reconhecido ou ausente!");
    }
}

void attrib()
{
    checaID();
    while (token.categoria == SN && token.codigo == OPEN_BRACKET)
    {
        token = Analex(codeFile);
        expr();
        if (token.categoria != SN || token.codigo != CLOSE_BRACKET)
        {
            erro("Sinal ']' esperado!");
        }
        token = Analex(codeFile);
    }

    if(token.categoria == SN && token.codigo == ATTRIB)
    {
        token = Analex(codeFile);
        expr();
    }
    else
        erro("Sinal '=' esperado!");

}

void expr()
{
    expr_simp();
    if(token.categoria == SN && EH_OPERADOR_RELACIONAL) // op_rel
    {
        token = Analex(codeFile);
        expr_simp();
    }
}

void expr_simp()
{
    if(token.categoria == SN && (token.codigo == ADD || token.codigo == SUBTRACT))
        token = Analex(codeFile);
    termo();
    while(token.categoria == SN && (token.codigo == ADD || token.codigo == SUBTRACT || token.codigo == OR))
    {
        token = Analex(codeFile);
        termo();
    }
}

void termo()
{
    fator();
    while(token.categoria == SN && (token.codigo == MULTIPLY || token.codigo == DIVIDE || token.codigo == AND))
    {
        token = Analex(codeFile);
        fator();
    }
}

void fator()
{
    if(token.categoria == ID)
    {
        checaID();
        while (token.categoria == SN && token.codigo == OPEN_BRACKET)
        {
            token = Analex(codeFile);
            expr();
            if (token.categoria != SN || token.codigo != CLOSE_BRACKET)
            {
                erro("Sinal ']' esperado!");
            }
            token = Analex(codeFile);
        }
    }
    else if(token.categoria == SN && token.codigo == OPEN_PARENTHESIS)
    {
        token = Analex(codeFile);
        expr();
        if (token.categoria != SN || token.codigo != CLOSE_BRACKET)
        {
            erro("Sinal ']' esperado!");
        }
        token = Analex(codeFile);
    }
    else if(token.categoria == SN && token.codigo == NOT_EQUAL)
    {
        token = Analex(codeFile);
        fator();
    }
    else if (token.categoria == CT_INT)
    {
        token = Analex(codeFile);
    }
    else if (token.categoria == CT_REAL)
    {
        token = Analex(codeFile);
    }
    else if (token.categoria == CT_CHAR)
    {
        token = Analex(codeFile);
    }
    
    

}