#ifndef Funcionarios_h
#define Funcionarios_h

typedef struct Funcionarios{
    
    long int codigo;
    char *nome;
    char *cargo;
    char *rua;
    char *bairro;
    int numero;
    int telefone;
    char *email;
    int flag;
}Funcionarios;

void cadastraFuncionario(int modoAbertura);

void reescreveDadosFuncionariosBin(Funcionarios *ptr, int quantidadeFuncionarios, char *modoAbertura, char *nomeArqTemporario, char *nomeArqOr);

int leDadosFuncionariosBin(Funcionarios **ptr);

void reescreveDadosFuncionarios(Funcionarios *ptr, int quantidadeFuncionarios, char *modoAbertura, char *nomeArqTemporario, char *nomeArqOr);

int leDadosFuncionarios(Funcionarios **ptr);

void apagaDadosStructFuncionarios(Funcionarios *ptr, int quantidadeFuncionarios);

void apagaFuncionario(int modoAbertura);

void editaFuncionario(int modoAbertura);

int verificaExisteFuncionarios(Funcionarios *ptr, int quantidade);
Funcionarios *buscaFuncionarios(Funcionarios *ptr, int quantidade, Funcionarios *posicao);

Funcionarios *encontraFuncionarioNome(Funcionarios *ptr, char *nome, int quantidade, Funcionarios *posicao);

void mostraFuncionarios(int modoAbertura);

Funcionarios *encontraFuncionarioCodigo(Funcionarios *ptr, long int codigo, int quantidade, Funcionarios *posicao);
#endif