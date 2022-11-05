#ifndef CONTROLECLIENTES_H
#define CONTROLECLIENTES_H

typedef struct cliente{
    int codigo;
    char *nomeCompleto;
    char *rua;
    char *bairro;
    int numeroDaCasa;
    char *cpf;
    int telefone;
    char *email;
    int sexo;
    int estadoCivil;
    int diaNascimento;
    int mes;
    int ano;
    int flag;
    
    
}cliente;

void apagaCliente(int modo);

int leDadosClientesBin(cliente **ptr);

int leDadosClientes(cliente **ptr);

void cadastraCliente(int modoLeitura);

void limpaDadosClienteMemoria(cliente *ptr, int quantidade);

void reescreveDadosClienteBin(cliente *ptr, int quantidade);

void reescreveDadosCliente(cliente *ptr, int quantidade);

cliente *encontraClienteCodigo(cliente *ptr, int quantidade, char *codigo, int ignorar);

cliente *encontraClienteNome(cliente *ptr, int quantidade, char *nome);

cliente *encontraClienteCpf(cliente *ptr, int quantidade, char *cpf);

cliente *buscaCliente(cliente *buscar, int quantidade, char *mensagem);

void editaCliente(int modoAbertura);

void mostraListaClientes(cliente *ptr, int quantidade);

int verificaExisteClientes(cliente *ptr, int quantidade);

#endif