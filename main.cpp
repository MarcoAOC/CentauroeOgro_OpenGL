#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "CarregarArquivo.cpp"
#include <cmath>
using std::string;

bool full = false;
void PosicionaObservador(void);
void EspecificaParametrosVisualizacao(void);

GLfloat angle, fAspect, rotX, rotY,translx,transly,translz;
GLdouble obsX, obsY, obsZ;
CarregarArquivo Ogro;
CarregarArquivo Centauro;

float rodar = 0.0;
float volante = 0.0;
GLubyte tex[1024][1024][3];
GLubyte tex2[256][256][3];

float posiciona_carro = 90.0;

float trans_X = 0.0;
float trans_Z = 0.0;
float dire_x = 0.0;
float dire_z = 0.0;

int velocidade = 0.0;
// Desativa repetição da textura
char repete = 0;

bool andar = false;
// Desativa interpolação linear da textura
GLint filtro = GL_NEAREST;

// Define valor final da coordenada de textura
float final = 1.0;

// Armazena identificação da textura
GLuint textura_id,textura_id2,textura_id3;
GLuint texturaskybox[6];

//1 = chao, 2 = centauro, 3 = ogro

void DefineIluminacao (void)
{
    GLfloat luzAmbiente[4]= {0.4,0.4,0.4,1.0};
    GLfloat luzDifusa[4]= {0.7,0.7,0.7,1.0}; // "cor"
    GLfloat luzEspecular[4]= {1.0, 1.0, 1.0, 1.0}; // "brilho"
    GLfloat posicaoLuz[4]= {0.0, 150.0, 0.0, 1.0};
// Capacidade de brilho do material
    GLfloat especularidade[4]= {1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;
// Define a refletância do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
// Define a concentração do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
// Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
// Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
    glEnable(GL_LIGHT0);
}
// Função callback chamada para fazer o desenho
void Desenha_SkyBox(){
    int i;

    float t=1.0;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

//	glEnable(GL_TEXTURE_2D);

	GLint* viewport = new GLint[4];
	GLdouble* model = new GLdouble[16];
	GLdouble* proj = new GLdouble[16];

	GLdouble posx, posy, posz;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	gluUnProject( (viewport[2]-viewport[0])/2, (viewport[3]-viewport[1])/2, 0.0, model, proj, viewport, &posx, &posy, &posz);

	glPushMatrix();
	glTranslatef(posx, posy, posz);
    glColor3f(1.0, 1.0, 1.0);
   glBindTexture(GL_TEXTURE_2D,texturaskybox[0]);
   glBegin(GL_QUADS);			// X Négatif
		glTexCoord2f(0.0, 1.0); glVertex3f(t,-t,t);
		glTexCoord2f(1.0, 1.0); glVertex3f(t,-t,-t);
		glTexCoord2f(1.0, 0.0); glVertex3f(t,t,-t);
		glTexCoord2f(0.0, 0.0); glVertex3f(t,t,t);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,texturaskybox[1]);

	glBegin(GL_QUADS);			// X Positif
		glTexCoord2f(0.0, 1.0); glVertex3f(-t,-t,-t);
		glTexCoord2f(1.0, 1.0); glVertex3f(-t,-t,t);
		glTexCoord2f(1.0, 0.0); glVertex3f(-t,t,t);
		glTexCoord2f(0.0, 0.0); glVertex3f(-t,t,-t);
	glEnd();


	glBindTexture(GL_TEXTURE_2D,texturaskybox[2]);

	glBegin(GL_QUADS);			// Y Négatif
		glTexCoord2f(0.0, 1.0); glVertex3f(-t,-t,t);
		glTexCoord2f(1.0, 1.0); glVertex3f(t,-t,t);
		glTexCoord2f(1.0, 0.0); glVertex3f(t,t,t);
		glTexCoord2f(0.0, 0.0); glVertex3f(-t,t,t);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,texturaskybox[3]);

	glBegin(GL_QUADS);			// Y Positif
		glTexCoord2f(0.0, 1.0); glVertex3f(t,-t,-t);
		glTexCoord2f(1.0, 1.0); glVertex3f(-t,-t,-t);
		glTexCoord2f(1.0, 0.0); glVertex3f(-t,t,-t);
		glTexCoord2f(0.0, 0.0); glVertex3f(t,t,-t);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,texturaskybox[4]);

	glBegin(GL_QUADS);			// Z Négatif
		glTexCoord2f(0.0, 0.0); glVertex3f(t,t,-t);
		glTexCoord2f(1.0, 0.0); glVertex3f(-t,t,-t);
		glTexCoord2f(1.0, 1.0); glVertex3f(-t,t,t);
		glTexCoord2f(0.0, 1.0); glVertex3f(t,t,t);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,texturaskybox[5]);

	glBegin(GL_QUADS);			// Z Positif
		glTexCoord2d(0.0, 1.0);	glVertex3f(t, -t, -t);
		glTexCoord2d(1.0, 1.0);	glVertex3f(t, -t, t);
		glTexCoord2d(1.0, 0.0); glVertex3f(-t, -t, t);
		glTexCoord2d(0.0, 0.0);	glVertex3f(-t, -t, -t);
	glEnd();


	glPopMatrix();

	//glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);


	delete[] proj;
	delete[] model;
	delete[] viewport;



}
void Desenha_Centauro(){
    glBindTexture(GL_TEXTURE_2D, textura_id2);

    glTranslated(trans_X,0.0,trans_Z);
    glRotated(posiciona_carro,0,1,0);
    glRotated(180,0,1,0);
    for ( int j = 0; j < (Centauro.faces).size(); ++j )
    {

        glBegin ( GL_POLYGON );

        for (int i = 0; i < (Centauro.faces[j]).size() ; ++i )
        {

            GLfloat vert[3] = {(Centauro.vertices1[Centauro.faces[j][i][0]][0]),(Centauro.vertices1[Centauro.faces[j][i][0]][1]),(Centauro.vertices1[Centauro.faces[j][i][0]][2])};
            GLfloat nor[3] = {Centauro.normais[Centauro.faces[j][i][2]][0],Centauro.normais[Centauro.faces[j][i][2]][1],Centauro.normais[Centauro.faces[j][i][2]][2]};
            GLfloat tex[2] = {Centauro.texturas[Centauro.faces[j][i][1]][0],Centauro.texturas[Centauro.faces[j][i][1]][1]};
            glTexCoord2fv(tex);
            glNormal3fv (nor);
            glVertex3fv (vert);


        }

        glEnd( );
    }
}
void Desenha_Chao(){
    glBindTexture(GL_TEXTURE_2D, textura_id);

    glBegin(GL_POLYGON);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-200.0,0.0,-200.0);
    glTexCoord2f(1.0,1.0);
    glVertex3f(200.0,0.0,-200.0);
    glTexCoord2f(1.0,0.0);
    glVertex3f(200.0,0.0,200.0);
    glTexCoord2f(0.0,0.0);
    glVertex3f(-200.0,0.0,200.0);
    glEnd();
}
void Desenha_Ogro(){
    glBindTexture(GL_TEXTURE_2D, textura_id3);

    glTranslated(10,0.0,0);
    glRotated(180,0,1,0);
    for ( int j = 0; j < (Ogro.faces).size(); ++j )
    {

        glBegin ( GL_POLYGON );

        for (int i = 0; i < (Ogro.faces[j]).size() ; ++i )
        {

            GLfloat vert[3] = {(Ogro.vertices1[Ogro.faces[j][i][0]][0]),(Ogro.vertices1[Ogro.faces[j][i][0]][1]),(Ogro.vertices1[Ogro.faces[j][i][0]][2])};
            GLfloat nor[3] = {Ogro.normais[Ogro.faces[j][i][2]][0],Ogro.normais[Ogro.faces[j][i][2]][1],Ogro.normais[Ogro.faces[j][i][2]][2]};
            GLfloat tex[2] = {Ogro.texturas[Ogro.faces[j][i][1]][0],Ogro.texturas[Ogro.faces[j][i][1]][1]};
            glTexCoord2fv(tex);
            glNormal3fv (nor);
            glVertex3fv (vert);


        }

        glEnd( );
    }
}

void Desenha(void){
    // Limpa a janela de visualização com a cor
    // de fundo definida previamente
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Troca cor corrente para azul
    glColor3f(0.0f, 1.0f, 0.0f);
    PosicionaObservador();
//    displaySkybox("resources/skybox/lado4.jpg","resources/skybox/lado3.jpg","resources/skybox/lado2.jpg","resources/skybox/lado1.jpg","resources/skybox/chao.jpg","resources/skybox/ceu.jpg");
    glPushMatrix();


    glEnable(GL_TEXTURE_2D);
    Desenha_SkyBox();
    Desenha_Chao();

    glPushMatrix();

    Desenha_Centauro();
    glPopMatrix();

    glPushMatrix();

    Desenha_Ogro();
    glPopMatrix();


    glPopMatrix();


    // Execução dos comandos de desenho
    glutSwapBuffers();
}

// Inicialização
void iniciatextura_SkyBox(){
    int isky;
    for(isky=0;isky<6;isky++){
        try
        {
            ifstream arq("resources/skybox/sky0.bmp" ,ios::binary);
            if(isky==0)
                ifstream arq("resources/skybox/sky0.bmp" ,ios::binary);
            if(isky==1)
                ifstream arq("resources/skybox/sky1.bmp" ,ios::binary);
            if(isky==2)
                ifstream arq("resources/skybox/sky2.bmp" ,ios::binary);
            if(isky==3)
                ifstream arq("resources/skybox/sky3.bmp" ,ios::binary);
            if(isky==4)
                ifstream arq("resources/skybox/sky4.bmp" ,ios::binary);
            if(isky==5)
                ifstream arq("resources/skybox/sky5.bmp" ,ios::binary);
            char c;
            if(!arq)
                cout << "Erro abriu";
            int i = 0;
            for(int i = 0; i < 122 ; i++)
                c = arq.get();
            for(int i = 0; i < 1024 ; i++)
                for(int j = 0; j < 1024 ; j++)
                {
                    c = arq.get();
                    tex[i][j][2] = c;
                    c =  arq.get();
                    tex[i][j][1] = c ;
                    c =  arq.get();
                    tex[i][j][0] = c;
                }

            arq.close();
            arq.clear();
        }
        catch(...)
        {
            cout << "Erro ao ler imagem" << endl;
        }
        // lena
        glGenTextures(1,&texturaskybox[isky]);

        // Associa a textura aos comandos seguintes
        glBindTexture(GL_TEXTURE_2D, texturaskybox[isky]);

        // Envia a textura para uso pela OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB ,GL_UNSIGNED_BYTE, tex);

        // Define os filtros de magnificação e minificação
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        // Seleciona o modo de aplicação da textura
        //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }

}
void iniciatextura_Chao(){
     try
    {

        ifstream arq("resources/grass.bmp" ,ios::binary);
        char c;
        if(!arq)
            cout << "Erro abriu";
        int i = 0;
        for(int i = 0; i < 122 ; i++)
            c = arq.get();
        for(int i = 0; i < 1024 ; i++)
            for(int j = 0; j < 1024 ; j++)
            {
                c = arq.get();
                tex[i][j][2] = c;
                c =  arq.get();
                tex[i][j][1] = c ;
                c =  arq.get();
                tex[i][j][0] = c;
            }

        arq.close();
        arq.clear();
    }
    catch(...)
    {
        cout << "Erro ao ler imagem" << endl;
    }
    // lena
    glGenTextures(1,&textura_id);

    // Associa a textura aos comandos seguintes
    glBindTexture(GL_TEXTURE_2D, textura_id);

    // Envia a textura para uso pela OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB ,GL_UNSIGNED_BYTE, tex);

    // Define os filtros de magnificação e minificação
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Seleciona o modo de aplicação da textura
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


}
void iniciatextura_Ogro(){
    try
    {

        ifstream arq("textura.bmp" ,ios::binary);
        char c;
        if(!arq)
            cout << "Erro abriu";
        int i = 0;
        for(int i = 0; i < 122 ; i++)
            c = arq.get();
        for(int i = 0; i < 256 ; i++)
            for(int j = 0; j < 256 ; j++)
            {
                c = arq.get();
                tex2[i][j][2] = c;
                c =  arq.get();
                tex2[i][j][1] = c ;
                c =  arq.get();
                tex2[i][j][0] = c;
            }

        arq.close();
        arq.clear();
    }
    catch(...)
    {
        cout << "Erro ao ler imagem" << endl;
    }
    // lena
    glGenTextures(1,&textura_id3);

    // Associa a textura aos comandos seguintes
    glBindTexture(GL_TEXTURE_2D, textura_id3);

    // Envia a textura para uso pela OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB,GL_UNSIGNED_BYTE, tex2);

    // Define os filtros de magnificação e minificação
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Seleciona o modo de aplicação da textura
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


}
void iniciatextura_Centauro(){
    try
    {

        ifstream arq("textura.bmp" ,ios::binary);
        char c;
        if(!arq)
            cout << "Erro abriu";
        int i = 0;
        for(int i = 0; i < 122 ; i++)
            c = arq.get();
        for(int i = 0; i < 256 ; i++)
            for(int j = 0; j < 256 ; j++)
            {
                c = arq.get();
                tex2[i][j][2] = c;
                c =  arq.get();
                tex2[i][j][1] = c ;
                c =  arq.get();
                tex2[i][j][0] = c;
            }

        arq.close();
        arq.clear();
    }
    catch(...)
    {
        cout << "Erro ao ler imagem" << endl;
    }
    // lena
    glGenTextures(1,&textura_id2);

    // Associa a textura aos comandos seguintes
    glBindTexture(GL_TEXTURE_2D, textura_id2);

    // Envia a textura para uso pela OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB,GL_UNSIGNED_BYTE, tex2);

    // Define os filtros de magnificação e minificação
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Seleciona o modo de aplicação da textura
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


}
void Inicializa(void)
{

    Centauro.Carregar("resources/cent.obj");//carrega o arquivo.
    Ogro.Carregar("resources/OgreOBJ.obj");
// Define a cor de fundo da janela de visualização como branca
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
// Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
//Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
// Habilita a luz de número 0
    //glEnable(GL_LIGHT0);
// Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);


    glEnable(GL_NORMALIZE);// normaliza as normais
// Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);

// Inicializa a variável que especifica o ângulo da projeção
// perspectiva
    angle=50;
// Inicializa as variáveis usadas para alterar a posição do
// observador virtual
    rotX = 0;
    rotY = 0;
    translx = 0;
    transly = 0;
    translz=0;
    iniciatextura_SkyBox();
    iniciatextura_Chao();
    iniciatextura_Centauro();
    iniciatextura_Ogro();


    DefineIluminacao();
    EspecificaParametrosVisualizacao();
}


// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void)
{
    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();
    gluLookAt(0,100,90,0,0,0,0,1,0);
    // Especifica posição do observador e do alvo
    glRotatef(rotX,1,0,0);
    glRotatef(rotY,0,1,0);
    glTranslatef(translx,transly,translz);
}


// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
    // Especifica sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de projeção
    glLoadIdentity();

    // Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
    gluPerspective(angle,fAspect,0.5,500);

    PosicionaObservador();
}


// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Para previnir uma divisão por zero
    if ( h == 0 ) h = 1;

    // Especifica as dimensões da viewport
    glViewport(0, 0, w, h);

    // Calcula a correção de aspecto
    fAspect = (GLfloat)w/(GLfloat)h;

    EspecificaParametrosVisualizacao();
}


// Callback para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN)
        {
            // Zoom-in
            if (angle >= 10)
                angle -= 5;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN)
        {
            // Zoom-out
            if (angle <= 130)
                angle += 5;
        }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}


// Função callback chamada para gerenciar eventos de teclas especiais (F1,PgDn,...)
void TeclasEspeciais (int tecla,int x, int y)
{
    switch (tecla)
    {
    case GLUT_KEY_LEFT:
        rotY--;
        break;
    case GLUT_KEY_RIGHT:
        rotY++;
        break;
    case GLUT_KEY_UP:
        rotX++;
        break;
    case GLUT_KEY_DOWN:
        rotX--;
        break;
    case GLUT_KEY_HOME:
        obsZ++;
        break;
    case GLUT_KEY_END:
        obsZ--;
        break;

    }
    PosicionaObservador();
    glutPostRedisplay();
}
void timer(int value)
{

    if(velocidade > 0)
    {
        dire_x = cos((posiciona_carro*3.14159)/180)*0.2*velocidade;
        dire_z = -sin((posiciona_carro*3.14159)/180)*0.2*velocidade;
        trans_X = trans_X + dire_x;
        trans_Z = trans_Z +dire_z;
        if(trans_X >= 200)
            trans_X = 200;
        if(trans_X <= -200)
            trans_X = -200;
        if(trans_Z >= 200)
            trans_Z = 200;
        if(trans_Z <= -200)
            trans_Z = -200;

        glutPostRedisplay();
    }
    if(velocidade < 0)
    {

        dire_x = cos((posiciona_carro*3.14159)/180)*0.2*velocidade;
        dire_z = -sin((posiciona_carro*3.14159)/180)*0.2*velocidade;
        trans_X = trans_X + dire_x;
        trans_Z = trans_Z +dire_z;
        if(trans_X >= 200)
            trans_X = 200;
        if(trans_X <= -200)
            trans_X = -200;
        if(trans_Z >= 200)
            trans_Z = 200;
        if(trans_Z <= -200)
            trans_Z = -200;

        glutPostRedisplay();
    }
     glutTimerFunc(10,timer,0);

}
void teclado(unsigned char c,int x, int y)
{
    if(c == 'j'){
        translx += 1.5;
    }
    if(c == 'l'){
        translx -=1.5;
    }
    if(c == 'n'){
        transly += 1.5;
    }
    if(c == 'm'){
        transly -= 1.5;
    }
    if(c == 'i'){
        translz += 1.5;
    }
    if(c == 'k'){
        translz -= 1.5;
    }
    if(c == 'a')
    {
        //posiciona_carro = posiciona_carro + volante*0.05;
        posiciona_carro = posiciona_carro + 5.0;
    }
    else if(c == 'd')
    {
        posiciona_carro = posiciona_carro - 5.0;;


    }
    else if(c == 'w')
    {

        velocidade = 1.0;


    }
      else if(c == 's')
    {
       velocidade = 0.0;


    }
     else if(c == 27)
    {
        if(!full){
            glutPositionWindow(0,0);
            glutFullScreen();
        }
        else{
            //glutReshapeWindow(300,300);
            glutPositionWindow(10,10);
            glutReshapeWindow(300,300);
        }
        full = (!full);

    }
    glutPostRedisplay();
}

// Programa Principal
int main()
{
    int argc = 0;
    char *argv[] = { (char *)"gl", 0 };

    glutInit(&argc,argv);

    // Define do modo de operacao da GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Especifica a posição inicial da janela GLUT
    glutInitWindowPosition(5,5);

    // Especifica o tamanho inicial em pixels da janela GLUT
    glutInitWindowSize(800,800);

    // Cria a janela passando como argumento o titulo da mesma
    glutCreateWindow("Desenho de um objeto com iluminação");
    //glutIdleFunc(Idles);
    // Registra a funcao callback de redesenho da janela de visualizacao
    glutDisplayFunc(Desenha);
    // Registra a funcao callback para tratamento das teclas especiais
    glutSpecialFunc(TeclasEspeciais);
    // Registra a funcao callback para tratamento do redimensionamento da janela
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(teclado);
    // Registra a funcao callback para tratamento do mouse
    glutMouseFunc(GerenciaMouse);
    glutTimerFunc(10,timer,0);
    Inicializa();
    glutMainLoop();
}
