#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>//arredondar pra cima

typedef struct rgb{
    int r;
    int g;
    int b;
}rgb;//estrutura do RGB

void aloca(rgb**p,int tam);//alocar rgb
void converte(rgb*ponteiro,int tam);//funcao da formula do preto e branco
void preto_branco();//funcao chamada na main de preto e branco
void redimencionar();//funcao chamada na main de redimencionar
void backup(int*w,int*h,rgb**ponteiro);//funcao grava o arquivo de imagem no ponteiro
int menu();//funcao que mostra o menu
void continuar();//funcao para dar breck na mostragem do programa
void copiar(rgb* p,rgb* aux,int w,int h);//copia o aux para o ponteiro
void gravar(rgb*p,int*w,int*h);// grava no arquivo
void sepia();//funcao chamada na main para virar sepia
void altera(rgb*p);
void girar();//funcao chamada na main girar
void cortar();//funcao chamada na main para corta
void criar_imagem();//funcao chamada na main fazer formas na imagem
void quadrado();//funcao para fazer quadrado
void circulo();//funcao para fazer circulo
void triangulo();//funcao para fazer triangulo
void imagem(char* nome,int* w,int* h);//funcao para verificar se a imagem ja existe
void compara(int* p);//funcao para nao deixar os valores de rgb passarem de 255 ou menor que 0

int main(){
    int op;
    do{
		op=menu();
		if(op==1)//opcao 1
			preto_branco();
		else if(op==2)//opcao 2
			redimencionar();
        else if(op==3)//opcao 3
            sepia();
        else if(op==4)//opcao 4
            girar();
        else if(op==5)//opcao 5
            cortar();
        else if(op==6)//opcao 6
            criar_imagem();
    }while(op!=7);//sair
    return 0;
}

int menu(){
	int op;
	system("cls");
    printf("Menu:\n\n1-Preto e Branco\n2-Redimensionar\n3-Sepia\n4-Rotacionar\n5-Recortar\n6-Criar Imagem\n7-Sair\n\nEscolha sua opcao: ");
    scanf("%d",&op);
	return op;
}
void backup(int*w,int*h,rgb**ponteiro){
	FILE* fp = NULL;
    char nome[150];

    printf("digite o nome do arquivo: ");
    scanf(" %[^\n]",nome);//digita nome do arquivo imagem
    strcat(nome,".bin");//coloca a extencao na string
    if((fp=fopen(nome,"rb"))==NULL)
       printf("arquivo nao encotrado\n");
    else{
        fseek(fp,0,0);
        fread(w,sizeof(int),1,fp);
        fread(h,sizeof(int),1,fp);
        aloca(ponteiro,(*w)*(*h));
        fread(*ponteiro,sizeof(rgb),(*w)*(*h),fp);
        fclose(fp);
    }//abre grava o arquivo no ponteiro e seus tamanhos de dimencoes
}
void aloca(rgb**p,int tam){
    if((*p=(rgb*)realloc(*p,tam*sizeof(rgb)))==NULL){
        printf("erro de alocacao\n");
        exit(1);
    }
}
void converte(rgb*ponteiro,int tam){
    int i;
    double p_b;

    for(i=0;i<tam;i++){
        p_b= ((ponteiro+i)->r)*0.2989;
        p_b+= ((ponteiro+i)->g)*0.5870;
        p_b+= ((ponteiro+i)->b)*0.1140;
        (ponteiro+i)->r=(int)p_b;
        (ponteiro+i)->g=(int)p_b;
        (ponteiro+i)->b=(int)p_b;
    }//contas para transforma em preto e branco
}
void gravar(rgb*p,int*w,int*h){
    FILE* fp = NULL;
    char nome[100];
    printf("digite o nome do arquivo final: ");
    scanf(" %[^\n]",nome);//digita nome do arquivo imagem
    strcat(nome,".bin");//coloca a extencao na string

    if((fp=fopen(nome,"wb"))==NULL)
			printf("erro de abertura de arquivo\n");
		else{
			fwrite(w,sizeof(int),1,fp);//grava w
			fwrite(h,sizeof(int),1,fp);//grava h
			fwrite(p,sizeof(rgb),(*w)*(*h),fp);// grava o ponteiro inteiro
		}
		fclose(fp);
}
void preto_branco(){
    int w,h;
    rgb* ponteiro =NULL;

    system("cls");
    backup(&w,&h,&ponteiro);//carrega arquivo
    if(ponteiro!=NULL){//se o arquivo nao for encotrado nao faz nada
		printf("Largura: %d\nAltura: %d\n",w,h);
		converte(ponteiro,w*h);//altera os valores do rgb
		gravar(ponteiro,&w,&h); // grava o arquivo
		printf("\nImagem convertida com sucesso\n");
	}
	continuar();
}
void redimencionar(){
    int w,h,nw,nh,i,j,ajuda;
    double par;
    rgb* ponteiro = NULL;
    rgb* aux = NULL;

    system("cls");
    backup(&w,&h,&ponteiro);//carrega arquivo

    if(ponteiro!=NULL){//se nao encotra nao faz nada
		printf("Largura: %d\nAltura: %d\n",w,h);
		printf("\nDigite a Largura: ");
		scanf("%d",&nw);//nova largura
		printf("Digite a Altura: ");
		scanf("%d",&nh);//nova altura

		if(w<nw){//aumentar largura
            aloca(&aux,nw*h);
            par=(double)nw/w; //acha a razao entre a nova medida e antiga
            for (i = 0; i < nw; i++){
                ajuda=(int)i/par; // divide o i pela razao
                for(j=0;j<h;j++){//copia a coluna em relacao a razao
                    *(aux+(j*nw)+i)=*(ponteiro+(j*w)+ajuda);
                }
            }
            w=nw;//guarda valor da nova largura
            aloca(&ponteiro,w*h);//aloca o novo ponteiro
            copiar(ponteiro,aux,w,h);
        }

		else if(w>nw){//diminui largura
            aloca(&aux,nw*h);
            par=(double)w/nw;//acha a razao entre a nova medida e antiga
            for (i = 0; i < w; i++){
                ajuda=(int)i/par;// divide o i pela razao
                for(j=0;j<h;j++){//copia a coluna em relacao a razao
                    *(aux+(j*nw)+ajuda)=*(ponteiro+(j*w)+i);
                }
            }
            w=nw;//guarda valor da nova largura
            aloca(&ponteiro,w*h);//aloca o novo ponteiro
            copiar(ponteiro,aux,w,h);
		}

		if(h<nh){//aumenta altura
            aloca(&aux,w*nh);
            par=(double)nh/h;//acha a razao entre a nova medida e antiga
            for (i = 0; i < nh; i++){
                ajuda=(int)i/par;// divide o i pela razao
                for(j=0;j<w;j++){//copia a linha em relacao a razao
                    *(aux+(i*w)+j)=*(ponteiro+(ajuda*w)+j);
                }
            }
            h=nh;//guarda valor da nova altura
            aloca(&ponteiro,w*h);//aloca o novo ponteiro
            copiar(ponteiro,aux,w,h);
		}

		else if(h>nh){//diminui altura
            aloca(&aux,w*nh);
            par=(double)h/nh;//acha a razao entre a nova medida e antiga
            for (i = 0; i < h; i++){
                ajuda=(int)i/par;// divide o i pela razao
                for(j=0;j<w;j++){//copia a linha em relacao a razao
                    *(aux+(ajuda*w)+j)=*(ponteiro+(i*w)+j);
                }
            }
            h=nh;//guarda valor da nova altura
            aloca(&ponteiro,w*h);//aloca o novo ponteiro
            copiar(ponteiro,aux,w,h);
		}

		gravar(ponteiro,&w,&h);// grava o arquivo
		printf("\nImagem redimencionada com sucesso\n");

	}
	continuar();
}
void continuar(){
	printf("\nAperte enter para continuar: ");
	getchar();//para poder ver oque esta acontecendo
	getchar();
}
void copiar(rgb* p,rgb* aux,int w,int h){
int i,j;
    for(i=0;i<w;i++){
        for(j=0;j<h;j++){
            *(p+(j*w)+i)=*(aux+(j*w)+i);//copia os valores
        }
    }
}
void sepia(){
    FILE *fp=NULL;
    int p1_x,p1_y,p2_x,p2_y,h,w,i,j;
    long int ajuda;
    char nome[80];
    rgb aux;

    printf("Digite o nome da Imagem:");
    scanf(" %[^\n]",nome);
    strcat(nome,".bin");//pega o nome da imagem digitada

    if((fp=fopen(nome,"rb"))==NULL)
        printf("\nArquivo nao encontrado");
    else
    {
        fseek(fp,0,0);
        fread(&w,sizeof(int),1,fp);
        fread(&h,sizeof(int),1,fp);
        fclose(fp);
    }
    if(fp!=NULL)
    {
        system("cls");
        printf("Largura: %d\nAltura:%d",w,h);
        do{
            printf("\nDigite a coordenada x do canto superior esquerdo: ");
            scanf("%d",&p1_x);
            p1_x--;
          }while(p1_x<0 || p1_x>=w);//recebe coord x superior
        do{
            printf("\nDigite a coordenada y do canto superior esquerdo: ");
            scanf("%d",&p1_y);
            p1_y--;
          }while(p1_y<0 || p1_y>=h);//recebe coord y superior
          do{
            printf("\nDigite a coordenada x do canto inferior deireito: ");
            scanf("%d",&p2_x);
            p2_x--;
          }while(p2_x<0 || p2_x>=w);//recebe coord x inferior
        do{
            printf("\nDigite a coordenada y do canto inferior deireito: ");
            scanf("%d",&p2_y);
            p2_y--;
          }while(p2_y<0 || p2_y>=h);//recebe coord y inferior

        if((fp=fopen(nome,"rb+"))==NULL)
            printf("\nArquivo nao encontrado");
        else{
            for(i=p1_y;i<p2_y;i++){
                for(j=p1_x;j<p2_x;j++){
                    ajuda=sizeof(int)*2;
                    ajuda+=(i*w+j)*sizeof(rgb);
                    fseek(fp,ajuda,0);
                    fread(&aux,sizeof(rgb),1,fp);
                    altera(&aux);
                    fseek(fp,ajuda,0);
                    fwrite(&aux,sizeof(rgb),1,fp);
                }
            }//funcao que percorre apenas o quadrado nas coord recebidas
        fclose(fp);
        }
        printf("\nFiltro adicionado com sucesso\n");
        continuar();
    }
}
void altera(rgb*p){
    rgb aux;
    aux.r=(p->r*0.393+p->g*0.769+p->b*0.189);
    if(aux.r>255)
        aux.r=255;
    aux.g=(p->r*0.349+p->g*0.686+p->b*0.168);
    if(aux.g>255)
        aux.g=255;
    aux.b=(p->r*0.272+p->g*0.534+p->b*0.131);
    if(aux.b>255)
        aux.b=255;
    *p=aux;
}//funcao que altera os valores para sepia
void girar(){
    int w,h,nw,nh,i,j,ajuda,angulo;
    double par;
    rgb* ponteiro = NULL;
    rgb* aux = NULL;
    FILE* fp = NULL;
    char nome[100];

    system("cls");

    printf("digite o nome do arquivo: ");
    scanf(" %[^\n]",nome);//digita nome do arquivo imagem
    strcat(nome,".bin");//coloca a extencao na string

    if((fp=fopen(nome,"rb"))==NULL)
       printf("arquivo nao encotrado\n");
    else{
        fseek(fp,0,0);
        fread(&w,sizeof(int),1,fp);
        fread(&h,sizeof(int),1,fp);
        aloca(&ponteiro,w*h);
        fread(ponteiro,sizeof(rgb),(w*h),fp);
        fclose(fp);
    }//abre grava o arquivo no ponteiro e seus tamanhos de dimencoes

    aloca(&aux,w*h);

    if(ponteiro!=NULL){
        printf("Digite o angulo do giro em sentido horario: ");
        scanf("%d",&angulo);
        angulo/=90;

        while(angulo--){//gira quantas vezes foi dado o angulo multiplo de 90

            for(i=0;i<w;i++){
                for(j=0;j<h;j++){
                    *(aux+(i*h)+j)=*(ponteiro+((h-j-1)*w)+i);
                }
            }
            nw=h;
            h=w;
            w=nw;

        for(i=0;i<w;i++){
            for(j=0;j<h;j++){
                *(ponteiro+(j*w)+i)=*(aux+(j*w)+i);
            }
        }

    }//linha vira coluna
    if((fp=fopen(nome,"wb"))==NULL)
            printf("\nArquivo nao encontrado");
    else{
        fseek(fp,0,0);
        fwrite(&w,sizeof(int),1,fp);
        fwrite(&h,sizeof(int),1,fp);
        fwrite(ponteiro,sizeof(rgb),(w*h),fp);
        fclose(fp);
    }
    printf("\nImagem rotacionada com sucesso\n");

    }
    else{
        printf("\nArquivo nao encotrado\n");
    }
    continuar();//grava no arquivo
}
void cortar(){

    FILE *fp=NULL;
    FILE *faux=NULL;

    int p1_x,p1_y,p2_x,p2_y,h,w,i,j,k;
    int nova_h,nova_w;
    long int ajuda;
    char nome[80],nome_2[80];
    rgb aux;

    printf("Digite o nome da Imagem:");
    scanf(" %[^\n]",nome);
    strcat(nome,".bin");

    if((fp=fopen(nome,"rb"))==NULL)
        printf("\nArquivo nao encontrado");
    else
    {
        fseek(fp,0,0);
        fread(&w,sizeof(int),1,fp);
        fread(&h,sizeof(int),1,fp);
        fclose(fp);
    }

    if(fp!=NULL)
    {
        system("cls");
        printf("Largura: %d\nAltura:%d",w,h);
        do{
            printf("\nDigite a coordenada x do canto superior esquerdo: ");
            scanf("%d",&p1_x);
            p1_x--;
          }while(p1_x<0 || p1_x>=w);
        do{
            printf("\nDigite a coordenada y do canto superior esquerdo: ");
            scanf("%d",&p1_y);
            p1_y--;
          }while(p1_y<0 || p1_y>=h);
          do{
            printf("\nDigite a coordenada x do canto inferior deireito: ");
            scanf("%d",&p2_x);
            p2_x--;
          }while(p2_x<0 || p2_x>=w);
        do{
            printf("\nDigite a coordenada y do canto inferior deireito: ");
            scanf("%d",&p2_y);
            p2_y--;
          }while(p2_y<0 || p2_y>=h);

        nova_h=p2_y-p1_y;
        nova_w=p2_x-p1_x;

        printf("\nDigite o nome do arquivo de saida: ");
        scanf(" %[^\n]",nome_2);
        strcat(nome_2,".bin");

        if((fp=fopen(nome,"rb"))==NULL)
            printf("\nArquivo nao encontrado");
        else{
            if((faux=fopen(nome_2,"wb"))==NULL)
                printf("\nErro de abertura de arquivo");
            else{
                fseek(faux,0,0);
                fwrite(&nova_w,sizeof(int),1,faux);
                fwrite(&nova_h,sizeof(int),1,faux);
                for(i=p1_y,k=0;i<p2_y;i++){
                    for(j=p1_x;j<p2_x;j++,k++){
                        ajuda=sizeof(int)*2;
                        ajuda+=(i*w+j)*sizeof(rgb);
                        fseek(fp,ajuda,0);
                        fread(&aux,sizeof(rgb),1,fp);
                        fseek(faux,(sizeof(rgb)*k)+(2*sizeof(int)),0);
                        fwrite(&aux,sizeof(rgb),1,faux);

                    }
                }
            }
        fclose(faux);
        fclose(fp);
        printf("\nImagem recortada com sucesso\n");
        }
        continuar();
    }
}
void imagem(char* nome,int* w,int* h){
	FILE* fp = NULL;
	int i;
	rgb aux;

	aux.b=255;
	aux.g=255;
	aux.r=255;
    printf("Escolha o nome da imagem: ");
	scanf(" %[^\n]",nome);
	strcat(nome,".bin");

    if((fp=fopen(nome,"rb"))==NULL){
        fclose(fp);

        printf("Escolha a largura da imagem: ");
        scanf("%d",w);
        printf("Escolha a Altura da imagem: ");
        scanf("%d",h);

        fp=fopen(nome,"wb");
        fwrite(w,sizeof(int),1,fp);
        fwrite(h,sizeof(int),1,fp);
        for(i=0;i<(*w)*(*h);i++)
            fwrite(&aux,sizeof(rgb),1,fp);
        fclose(fp);
    }
    else{
        fread(w,sizeof(int),1,fp);
        printf("\nLargura da imagem de: %d",*w);
        fread(h,sizeof(int),1,fp);
        printf("\nAltura da imagem de: %d",*h);
    }
}

void criar_imagem(){
    int op;
	system("cls");
    printf("Menu:\n1-Quadrado\n2-Circulo\n3-Triangulo\n\nEscolha sua opcao: ");
    scanf("%d",&op);

	if(op==1)//opcao 1
		quadrado();
	else if(op==2)//opcao 2
		circulo();
       else if(op==3)
        triangulo();
}
void compara(int* p){
    if(*p<0)
        *p=0;
    if(*p>255)
        *p=255;
}
void quadrado(){
    FILE* fp = NULL;
    char nome[80];
    rgb aux,bk,cmp;
    long int pos;
    int x,y,w,h,lado,i,j,espessura;
	imagem(nome,&w,&h);
    bk.b=255;
    bk.g=255;
    bk.r=255;

	printf("\nDigite a cordenada X do ponto: ");
	scanf("%d",&x);
    printf("\nDigite a cordenada Y do ponto: ");
    scanf("%d",&y);
    printf("\nDigite o lado do quadrado: ");
    scanf("%d",&lado);
    printf("\nDigite a espessura do quadrado: ");
    scanf("%d",&espessura);
    printf("\nDigite a cor R B G do quadrado: ");
    scanf("%d%d%d",&aux.r,&aux.g,&aux.b);

    compara(&aux.r);
    compara(&aux.g);
    compara(&aux.b);

    if((fp=fopen(nome,"rb+"))==NULL)
        printf("erro na abertura do arquivo");
    else{
        for(i=x;i<x+lado;i++){
            if(i<w){
                for(j=y;j<y+lado;j++){
                    if(j<h){
                        if(i<x+espessura || i>x+lado-espessura){
                                pos=sizeof(int)*2;
                                pos+=(i*w+j)*sizeof(rgb);
                                fseek(fp,pos,0);
                                fwrite(&aux,sizeof(rgb),1,fp);
                        }
                         if(j<y+espessura || j>y+lado-espessura){
                                pos=sizeof(int)*2;
                                pos+=(i*w+j)*sizeof(rgb);
                                fseek(fp,pos,0);
                                fwrite(&aux,sizeof(rgb),1,fp);
                        }
                    }
                }
            }
        }
        fclose(fp);
    }
	printf("Quadrado adicionado a imagem com sucesso\n");
	continuar();
}
void circulo(){
    FILE* fp = NULL;
    char nome[80];
    rgb aux,bk,cmp;
    long int pos;
    int x,y,w,h,raio,i,j,espessura;
	imagem(nome,&w,&h);
    bk.b=255;
    bk.g=255;
    bk.r=255;

	printf("\nDigite a cordenada X do ponto: ");
	scanf("%d",&x);
    printf("\nDigite a cordenada Y do ponto: ");
    scanf("%d",&y);
    printf("\nDigite o raio da circuferencia: ");
    scanf("%d",&raio);
    printf("\nDigite a espessura da circuferencia: ");
    scanf("%d",&espessura);
    printf("\nDigite a cor R B G da circuferencia: ");
    scanf("%d%d%d",&aux.r,&aux.g,&aux.b);

    compara(&aux.r);
    compara(&aux.g);
    compara(&aux.b);

    if((fp=fopen(nome,"rb+"))==NULL)
        printf("erro na abertura do arquivo");
    else{
        for(i=x-raio;i<x+raio;i++){
            if(i<w)
                for(j=y-raio;j<y+raio;j++){
                    if(j<h){
                        if(((i-x)*(i-x))+((j-y)*(j-y))<=raio*raio){
                            if(((i-x)*(i-x))+((j-y)*(j-y))>(raio-espessura)*(raio-espessura)){
                                pos=sizeof(int)*2;
                                pos+=(i*w+j)*sizeof(rgb);
                                fseek(fp,pos,0);
                                fwrite(&aux,sizeof(rgb),1,fp);
                            }
                        }
                    }
                }
        }
        fclose(fp);
    }
	printf("Circulo adicionado a imagem com sucesso\n");
	continuar();
}
void triangulo(){

	printf("Triangulo adicionado a imagem com sucesso\n");
	continuar();
}
