#include <iostream>

using namespace std;

struct Arquivo {
	int import;//inportancia
	int tam; //tamanho
	int index;//posição inicial do arquivo
};
struct Pacote {
	int import;//soma das importancias de cada arquivo dentro do pacote
	int tam;//soma dos tamanhos dos arquivos dentro do pacote
	int qtdArquivosNoPacote;//length(arquivosNoPacote) <- armazena o tamanho real usado do vetor arquivosNoPacote
	Arquivo arquivosNoPacote[100];//armazena os arquivos que serão enviados nesse pacote
};

int main() {
	
	Arquivo arquivos[1000];//vetor de arquivos, podem haver no máximo 1000 arquivos
	Pacote pacotes[1000];
	int qtdArquivos;//quantidade entre 0 e 1000 
	int qtdPacotes = 0;//quantidade de pacotes necessários para enviar os
						//arquivos. Inicia em 0 e vai crescendo de acordo com a necessidade
	int qtdEmpacotados = 0;//quantidade de arquivos que já pertencem à um pacote (diretamente atrelado com jaEmpacotados[]
	int jaEmpacotados[1000];//armazena os indices dos arquivos que já foram empacotados
	
	
	cout << "Quantos arquivos?" << endl;
	cin >> qtdArquivos;
	
	
	for(int i = 0; i < qtdArquivos; i++) {
		cin >> arquivos[i].import >> arquivos[i].tam;
		arquivos[i].index = i;
		jaEmpacotados[i] = -1;
	}
	
	//este é um bloco de sort, organiza os arquivos com maior importancia no começo do vetor
	//serve para facilitar na escolha de arquivos com maior grau de importância no processo de empacotamento
	for (int i = 0; i < qtdArquivos; i++) {
        for (int j = i + 1; j < qtdArquivos; j++) {
            if (arquivos[j].import > arquivos[i].import) {
            	Arquivo aux;
                aux = arquivos[i];
                arquivos[i] = arquivos[j];
                arquivos[j] = aux;
            }
        }
    }
    
    
    //inicializa apenas o primeiro pacote. Demais pacotes são iniciado de maneira dinamica dentro do loop de checagem
    pacotes[0].tam = 0;
    pacotes[0].import = 0;
    pacotes[0].qtdArquivosNoPacote = 0;
    
    
    //EMPACOTAMENTO
    //o bloco do while irá continuar enquanto houver arquivos sem pacote
    do {
    	
    	//este bloco for irá varrer todos os arquivos do vetor
    	//verificando se eles já estão ou não em pacote
    	//e caso não estejam && ao adicionar ao pacote atual, não exceda o tamanho limite de 1460,
    	//ira adicioná-lo ao pacote
    	for(int i = 0; i < qtdArquivos; i++) {
    		
    		//este bloco faz um teste e verifica se o arquivo em específico já foi colocado em um pacote, 
    		//se sim, não irá utilizá-lo novamente 
    		bool existe = false;
    		for(int j = 0; j < qtdArquivos; j++) {
    			if(jaEmpacotados[j] == arquivos[i].index) {
    				existe = true;
				}
			}
			
			//verifica se o arquivo cabe dentro do pacote atual e 
			//se ele ainda não foi utilizado em outro pacote ou neste mesmo
			if(arquivos[i].tam + pacotes[qtdPacotes].tam <= 1460 && !existe) {
				pacotes[qtdPacotes].import += arquivos[i].import;//soma a importancia do novo arquivo ao grupo do pacote
				pacotes[qtdPacotes].tam += arquivos[i].tam;//soma o tamanho do arquivo no grupo do pacote
				pacotes[qtdPacotes].arquivosNoPacote[pacotes[qtdPacotes].qtdArquivosNoPacote] = arquivos[i];//add o arquivo no vetor de arquivos do pacote[i]
				jaEmpacotados[qtdEmpacotados] = arquivos[i].index;//grava que este arquivo já pertence à um pacote (grava apenas o index)
				qtdEmpacotados++;//como foi adicionado um novo arquivo, a quantidade de arquivos já pertencentes à um grupo aumenta
				pacotes[qtdPacotes].qtdArquivosNoPacote++;//aumenta o total de arquivos pertencente ao grupo (tamanho real utilizado do vetor arquivosNoPacote[]
			}
		}
		
		//CASO HAJA ARQUIVOS QUE NÃO PERTECEM À NENHUM PACOTE
		//quando o bloco for acima encerra, temos que nenhum dos arquivos que faltam ser empacotados não satisfazem a regra do tamanho total
		//sendo assim iniciamos um novo pacote para os arquivos restantes
		
		qtdPacotes++;//expande o tamanho necessário do vetor pacotes (tamanho de uso)
		
		//inicialização padrão do pacote subsequente
		pacotes[qtdPacotes].tam = 0;
		pacotes[qtdPacotes].import = 0;
		pacotes[qtdPacotes].qtdArquivosNoPacote = 0;
	}while(qtdEmpacotados < qtdArquivos);//caso ainda existam arquivos sem pacote, o novo pacote criado será usado para repetir a rotina do bloco for
										//caso não existam arquivos sem pacote, dará sequência no algoritmo
    
    
    
    //sort em cada pacote, ordenando os arquivos por index (0, 1, 2, 3, ... n)
    for(int o = 0; o < qtdPacotes; o++) {
    	for (int i = 0; i < pacotes[o].qtdArquivosNoPacote; i++) {
	        for (int j = i + 1; j < pacotes[o].qtdArquivosNoPacote; j++) {
	            if (pacotes[o].arquivosNoPacote[j].index < pacotes[o].arquivosNoPacote[i].index) {
	            	Arquivo aux;
	                aux = pacotes[o].arquivosNoPacote[i];
	                pacotes[o].arquivosNoPacote[i] = pacotes[o].arquivosNoPacote[j];
	                pacotes[o].arquivosNoPacote[j] = aux;
	            }
	        }
	    }
	}
	
	
	//DEFINE QUAIS PACOTES DEVEM SER ENVIADO PRIMEIRO
    //sort dos pacotes discriminando os que tem maior importância
    for (int i = 0; i < qtdArquivos; i++) {
        for (int j = i + 1; j < qtdArquivos; j++) {
            if (pacotes[j].import > pacotes[i].import) {
             	Pacote aux;
                aux = pacotes[i];
                pacotes[i] = pacotes[j];
                pacotes[j] = aux;
            }
        }
    }
    
    //as ordenações acima existem para facilitação de apresentação dos resultados. 
    //caso não usadas, o output seria feito de forma desorganizada e fora do padrão pedido
    
    //exposição dos resultados:
    //O for externo varre o vetor de pacotes 
    //O for interno varre os arquivos j dentro do pacote i
    for(int i = 0; i < qtdPacotes; i++) {
    	cout << "Pacote "<< i + 1 << ":" << endl;
    	for(int j = 0; j < pacotes[i].qtdArquivosNoPacote; j++) {
    		cout << pacotes[i].arquivosNoPacote[j].index << " ";
		}
		cout << endl;
	}
	
	return 0;
}
