typedef struct categoria{
    int codigo;
    char *nome;
    char *descricao;
    float valorLocacao;
}categoria;

void apagaCategoria(int modo);

int leDadosCategoriaBin(categoria **ptr);

int leDadosCategoria(categoria **ptr);

void cadastraCategoria(int modoLeitura);

void limpaDadosCategoriaMemoria(categoria *ptr, int quantidade);

void reescreveDadosCategoriaBin(categoria *ptr, int quantidade);

void reescreveDadosCategoria(categoria *ptr, int quantidade);

categoria *encontraCategoriaCodigo(categoria *ptr, int quantidade, char *codigo, int ignorar);

categoria *encontraCategoriaNome(categoria *ptr, int quantidade, char *nome);

categoria *buscaCategoria(categoria *buscar, int quantidade, char *mensagem);

void editaCategoria(int modoAbertura);

int verificaExisteCategoria(categoria *ptr, int quantidade);

void mostraListaCategoria(categoria *ptr, int quantidade);