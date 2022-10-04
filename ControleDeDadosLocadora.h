
typedef struct locadora{
    char *nomeFantasia;
    char *razaoSocial;
    char *inscricaoEstadual;
    char *cnpj;
    char *endereco;
    int telefone;
    char *email;
    char *nomeResponsavel;
    int telefoneResp;
    
}locadora;





void VerificaExisteArquivoLocadora();

/*funcao responsavel por coletar as informacoes do usuario sobre a locadora, salva os dados em memoria*/
void cadastraLocadora(locadora *ptr);
/*abre o arquivo passado por 2 parametro no modo de escrita, caso o arquivo nao seja encontrado, a funcao criara um arquivo, o ponteiro para uma
 funcao passado no 3 parametro sera a funcao que ela executara logo apos de abrir o arquivo, ou seja, caso passe a funcao de coletar os dados 
 * de cadastro, ela coletara, caso passe uma funcao de editar um campo, ela o fara, no final da execucao da funcao, ela salva os dados alterados ou cadastrados
 no hd, e tambem continua com os dados que o ponteiro aponta em memoria */
 
void leDadosLocadora(locadora *ptr, char *nomeArq);
/*funcao responsavel por editar um dado pedido pelo usuario na locadora, edita somente em memoria, para salvar e necessario
 chamar a funcao operacoComLocadora, passando esta funcao como argumento*/

void leDadosLocadoraBin(locadora *ptr, char *nomeArq);

void editaDadosLocadora(int modo);
void reescreveDadosLocadora(locadora *ptr, FILE *f );

void reescreveDadosLocadoraBin(locadora *ptr, FILE *f);

void limpaCamposLocadoraMemoria(locadora *ptr);

