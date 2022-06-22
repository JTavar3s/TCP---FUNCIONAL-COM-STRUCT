#include <iostream>

using namespace std;

struct Arquivo {
	int import;//inportancia
	int tam; //tamanho
	int index;//posi��o inicial do arquivo
};
struct Pacote {
	int import;//soma das importancias de cada arquivo dentro do pacote
	int tam;//soma dos tamanhos dos arquivos dentro do pacote
	int qtdArquivosNoPacote;//length(arquivosNoPacote) <- armazena o tamanho real usado do vetor arquivosNoPacote
	Arquivo arquivosNoPacote[100];//armazena os arquivos que ser�o enviados nesse pacote
};

int main() {
	
	Arquivo arquivos[1000];//vetor de arquivos, podem haver no m�ximo 1000 arquivos
	Pacote pacotes[1000];
	int qtdArquivos;//quantidade entre 0 e 1000 
	int qtdPacotes = 0;//quantidade de pacotes necess�rios para enviar os
						//arquivos. Inicia em 0 e vai crescendo de acordo com a necessidade
	int qtdEmpacotados = 0;//quantidade de arquivos que j� pertencem � um pacote (diretamente atrelado com jaEmpacotados[]
	int jaEmpacotados[1000];//armazena os indices dos arquivos que j� foram empacotados
	
	
	cout << "Quantos arquivos?" << endl;
	cin >> qtdArquivos;
	
	
	for(int i = 0; i < qtdArquivos; i++) {
		cin >> arquivos[i].import >> arquivos[i].tam;
		arquivos[i].index = i;
		jaEmpacotados[i] = -1;
	}
	
	//este � um bloco de sort, organiza os arquivos com maior importancia no come�o do vetor
	//serve para facilitar na escolha de arquivos com maior grau de import�ncia no processo de empacotamento
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
    
    
    //inicializa apenas o primeiro pacote. Demais pacotes s�o iniciado de maneira dinamica dentro do loop de checagem
    pacotes[0].tam = 0;
    pacotes[0].import = 0;
    pacotes[0].qtdArquivosNoPacote = 0;
    
    
    //EMPACOTAMENTO
    //o bloco do while ir� continuar enquanto houver arquivos sem pacote
    do {
    	
    	//este bloco for ir� varrer todos os arquivos do vetor
    	//verificando se eles j� est�o ou n�o em pacote
    	//e caso n�o estejam && ao adicionar ao pacote atual, n�o exceda o tamanho limite de 1460,
    	//ira adicion�-lo ao pacote
    	for(int i = 0; i < qtdArquivos; i++) {
    		
    		//este bloco faz um teste e verifica se o arquivo em espec�fico j� foi colocado em um pacote, 
    		//se sim, n�o ir� utiliz�-lo novamente 
    		bool existe = false;
    		for(int j = 0; j < qtdArquivos; j++) {
    			if(jaEmpacotados[j] == arquivos[i].index) {
    				existe = true;
				}
			}
			
			//verifica se o arquivo cabe dentro do pacote atual e 
			//se ele ainda n�o foi utilizado em outro pacote ou neste mesmo
			if(arquivos[i].tam + pacotes[qtdPacotes].tam <= 1460 && !existe) {
				pacotes[qtdPacotes].import += arquivos[i].import;//soma a importancia do novo arquivo ao grupo do pacote
				pacotes[qtdPacotes].tam += arquivos[i].tam;//soma o tamanho do arquivo no grupo do pacote
				pacotes[qtdPacotes].arquivosNoPacote[pacotes[qtdPacotes].qtdArquivosNoPacote] = arquivos[i];//add o arquivo no vetor de arquivos do pacote[i]
				jaEmpacotados[qtdEmpacotados] = arquivos[i].index;//grava que este arquivo j� pertence � um pacote (grava apenas o index)
				qtdEmpacotados++;//como foi adicionado um novo arquivo, a quantidade de arquivos j� pertencentes � um grupo aumenta
				pacotes[qtdPacotes].qtdArquivosNoPacote++;//aumenta o total de arquivos pertencente ao grupo (tamanho real utilizado do vetor arquivosNoPacote[]
			}
		}
		
		//CASO HAJA ARQUIVOS QUE N�O PERTECEM � NENHUM PACOTE
		//quando o bloco for acima encerra, temos que nenhum dos arquivos que faltam ser empacotados n�o satisfazem a regra do tamanho total
		//sendo assim iniciamos um novo pacote para os arquivos restantes
		
		qtdPacotes++;//expande o tamanho necess�rio do vetor pacotes (tamanho de uso)
		
		//inicializa��o padr�o do pacote subsequente
		pacotes[qtdPacotes].tam = 0;
		pacotes[qtdPacotes].import = 0;
		pacotes[qtdPacotes].qtdArquivosNoPacote = 0;
	}while(qtdEmpacotados < qtdArquivos);//caso ainda existam arquivos sem pacote, o novo pacote criado ser� usado para repetir a rotina do bloco for
										//caso n�o existam arquivos sem pacote, dar� sequ�ncia no algoritmo
    
    
    
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
    //sort dos pacotes discriminando os que tem maior import�ncia
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
    
    //as ordena��es acima existem para facilita��o de apresenta��o dos resultados. 
    //caso n�o usadas, o output seria feito de forma desorganizada e fora do padr�o pedido
    
    //exposi��o dos resultados:
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
