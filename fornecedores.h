typedef struct fornecedor fornecedor;

typedef struct fornecedor{
    
    int codigo;
    char *nomeFantasia;
    char *razaoSocial;
    char *inscricaoEstadual;
    char *CNPJ;
    char *bairro;
    char *rua;
    int numero;
    int telefone;
    char *email;
    int flag;
    
}fornecedor;

void mostraDadosFornecedores(fornecedor *ptr, int quantidade);
void limpaMemoriaStringsFornecedor(fornecedor *ptr, int quantidade);
int leDadosFornecedores(fornecedor **ptr);
int leDadosFornecedoresBin(fornecedor **ptr);
void reescreveDadosFornecedores(fornecedor *ptr, int quantidade);
void reescreveDadosFornecedoresBin(fornecedor *ptr, int quantidade);
void cadastraFornecedor(int modoAbertura);
int verificaExisteFornecedores(fornecedor *ptr, int quantidade);
fornecedor * encontraFornecedorCNPJ(fornecedor *ptr, int quantidade, char *dado);
fornecedor *encontraFornecedorNomeFantasia(fornecedor *ptr, int quantidade, char *dado);
fornecedor *buscaFornecedor(fornecedor *ptr, int quantidade);
void editaFornecedor(int modoAbertura);
void apagaFornecedor(int modoAbertura);