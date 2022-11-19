cmake_minimum_required(VERSION 3.23)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c
        sons.c sons.h
        Caixa.c Caixa.h
        Carrinho.c Carrinho.h
        contasAreceber.c contasAreceber.h
        ControleCategorias.c ControleCategorias.h
        ControleClientes.c ControleClientes.h
        ControleDeDadosLocadora.c ControleDeDadosLocadora.h
        ControleDeNumeros.c ControleDeNumeros.h
        ControleFilmes.c ControleFilmes.h
        controleFuncionarios.c controleFuncionarios.h
        Devolucoes.c Devolucoes.h
        fornecedores.c fornecedores.h
        FuncUtilitarias.c FuncUtilitarias.h
        ManipulacoesDeTexto.c ManipulacoesDeTexto.h
        menus.c menus.h
        contasAPagar.h contasApagar.c
        EntradaDeFilmes.c EntradaDeFilmes.h)
