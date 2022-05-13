#include <iostream>
#include <fstream>
#include <vector>
#include <GL/freeglut.h>
#include <string>
using namespace std;

unsigned int porsche;
vector<vector<float>> vertices;
vector<vector<int>> faces;
vector<vector<int >> facesNM;
vector<vector<float>> normals;
vector<vector<float>> vts;
float horizontalRot;
float verticalRot;
float selfRot;
float sclf = 0.04;

float Ex = 0;
float Ey = -40.00;
float Ez = -105;
bool light0 = TRUE;
bool light1 = TRUE;
bool light2 = TRUE;
float diffuseLight[] =
{ 0.8f, 0.8f, 0.8f, 1.0f };
float specularLight[] =
{ 1.0f, 1.0f, 1.0f, 1.0f };
float LightPosition[] =
{ 0.0f, 20.0f, 2.0f, 1.0f };
float local_view[] =
{ 0.0 };

GLfloat ambient[4] = { 0.2,0.2,0.2,1.0 };


void loadObj(string fname)
{
    int read;
    float x, y, z;
    ifstream file(fname);
    if (!file.is_open()) {
        cout << "arquivo nao encontrado";
        exit(1);
    }
    else {
        string info;
        while (file >> info)
        {
            if (info == "v")
            {
                vector<float> vertice;
                float x, y, z;
                file >> x >> y >> z;
                // cout << "v: " << x << " " << y << " " << z << endl;
                vertice.push_back(x);
                vertice.push_back(y);
                vertice.push_back(z);
                vertices.push_back(vertice);
            }

            if (info == "vn")
            {
                vector<float> vn;
                float x, y, z;
                file >> x >> y >> z;
                // cout << "vn: " << x << " " << y << " " << z << endl;
                vn.push_back(x);
                vn.push_back(y);
                vn.push_back(z);
                normals.push_back(vn);
            }
            if (info == "f")
            {
                vector<int> face;
                vector<int> faceNM;
                string x, y, z;
                file >> x >> y >> z;

                int fp = stoi(x.substr(0, x.find("/"))) - 1;
                int fs = stoi(y.substr(0, y.find("/"))) - 1;
                int ft = stoi(z.substr(0, z.find("/"))) - 1;

                int Nfp = stoi(x.substr(x.find("//") + 2, x.size())) - 1;
                int Nfs = stoi(y.substr(y.find("//") + 2, y.size())) - 1;
                int Nft = stoi(z.substr(z.find("//") + 2, z.size())) - 1;

                // cout << "face (f): " << fp << " " << fs << " " << ft << endl;
                // cout << "faceNM (f): " << Nfp << " " << Nfs << " " << Nft << endl;

                face.push_back(fp);
                face.push_back(fs);
                face.push_back(ft);

                faceNM.push_back(Nfp);
                faceNM.push_back(Nfs);
                faceNM.push_back(Nft);

                faces.push_back(face);
                facesNM.push_back(faceNM);
            }
            if (info == "vt") {
                vector<float> vt;
                float x, y;
                file >> x >> y;
                // cout << "vt: " << x << " " << y << endl;
                vt.push_back(x);
                vt.push_back(y);
                vts.push_back(vt);
            }
        }
    }

    porsche = glGenLists(1);
    glPointSize(2.0);

    glNewList(porsche, GL_COMPILE);
    {
        glPushMatrix();
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < faces.size(); i++)
        {
            vector<int> face = faces[i];
            vector<int> faceNM = facesNM[i]; 

            for (size_t l = 0; l < face.size(); l++)
            {
                glVertex3f(vertices[face[l]][0], vertices[face[l]][1], vertices[face[l]][2]);
                glNormal3f(normals[faceNM[l]][0], normals[faceNM[l]][1], normals[faceNM[l]][2]);

            }
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
    file.close();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}
void drawPorsche(int i)
{
    glPushMatrix();
    glTranslatef(Ex, Ey, Ez);
    glColor3f(1.0, 0.23, 0.27);
    glScalef(sclf, sclf, sclf);
    glRotatef(horizontalRot, 0, 1, 0);
    glRotatef(verticalRot, 1, 0, 0);
    glRotatef(selfRot, 0, 0, 1);
    glCallList(porsche);
    glPopMatrix();
}

void rotateOBJ(char lado) {

    if (lado == 'R') { // Direita
        horizontalRot = horizontalRot + 1.6;
        if (horizontalRot > 360) horizontalRot = horizontalRot - 360;
    }
    if (lado == 'L') { // Esquerda
        horizontalRot = horizontalRot - 1.6;
        if (horizontalRot < 0) horizontalRot = horizontalRot + 360;
    }
    if (lado == 'U') { // Cima
        verticalRot = verticalRot - 1.6;
        if (verticalRot < 360) verticalRot = verticalRot + 360;
    }
    if (lado == 'D') { // Baixo
        verticalRot = verticalRot + 1.6;
        if (verticalRot > 360) verticalRot = verticalRot - 360;
    }
    if (lado == 'F') {  // Ao redor de si mesmo para a direita
        selfRot = selfRot - 1.6;
        if (selfRot < 360) selfRot = selfRot + 360;
    }
    if (lado == 'B') {  // Ao redor de si mesmo para a esquerda
        selfRot = selfRot + 1.6;
        if (selfRot > 360) selfRot = selfRot - 360;
    }

}

void scaleOBJ(char i) {

    if (i == '+') { // Aumenta
        sclf += 0.05;

    }
    if (i == '-') { // Diminui
        sclf -= 0.05;
    }
}

void traslateOBJ(char i) {
    if (i == 'w') {  // Cima
        Ey += 2;
    }
    if (i == 'a') {  // Esquerda
        Ex -= 2;
    }
    if (i == 's') { // Baixo
        Ey -= 2;
    }
    if (i == 'd') { // Direita
        Ex += 2;
    }
    if (i == 'z') { // Trás
        Ez -= 2;
    }
    if (i == 'x') { // Frente
        Ez += 2;
    }
}

void manageLight(char i) {
    if (i == '0') {
        if (light0 == TRUE) {
            glDisable(GL_LIGHT0);
            light0 = FALSE;
        }
        else {
            glEnable(GL_LIGHT0);
            light0 = TRUE;
        }

    }
    if (i == '1') {
        if (light1 == TRUE) {
            glDisable(GL_LIGHT1);
            light1 = FALSE;
        }
        else {
            glEnable(GL_LIGHT1);
            light1 = TRUE;
        }
    }
    if (i == '2') {
        if (light2 == TRUE) {
            glDisable(GL_LIGHT2);
            light2 = FALSE;
        }
        else {
            glEnable(GL_LIGHT2);
            light2 = TRUE;
        }
    }
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawPorsche(1);
    glutSwapBuffers();

    glEnable(GL_NORMALIZE);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}


void keyboard(unsigned char key, int x, int y) {
    std::cout << key;
    switch (key) {
    case 27:
        exit(0);
        break;
    case '+': // Aumenta o objeto
        scaleOBJ('+');
        break;
    case '-': // Diminui o objeto
        scaleOBJ('-');
        break;
    case 'w':  // Anda pra cima
        traslateOBJ('w');
        break;
    case 'a': // Anda pra esquerda
        traslateOBJ('a');
        break;
    case 's': // Anda pra baixo
        traslateOBJ('s');
        break;
    case 'd': // Anda pra direita
        traslateOBJ('d');
        break;
    case 'z': // Anda pra trás
        traslateOBJ('z');
        break;
    case 'x': // Anda pra frente
        traslateOBJ('x');
        break;
    case '0': // Liga ou desliga a luz 0
        manageLight('0');
        break;
    case '1':  // Liga ou desliga a luz 1
        manageLight('1');
        break;
    case '2': // Liga ou desliga a luz 2
        manageLight('2');
        break;
    }
}


void keyboard_special(int key, int x, int y) {
    std::cout << key;
    switch (key) {

    case GLUT_KEY_CTRL_L:  // Rotaciona ao redor de si mesmo para a esquerda 
        rotateOBJ('B');
        break;

    case GLUT_KEY_SHIFT_L: // Rotaciona ao redor de si mesmo para a direita 
        rotateOBJ('F');
        break;
    case GLUT_KEY_DOWN:  // Rotaciona verticalmente para baixo
        rotateOBJ('D');
        break;

    case GLUT_KEY_UP:  // Rotaciona verticalmente para cima
        rotateOBJ('U');
        break;

    case GLUT_KEY_RIGHT: // Rotaciona horizontalmente para a direita
        rotateOBJ('R');
        break;

    case GLUT_KEY_LEFT: // Rotaciona horizontalmente para a esquerda
        rotateOBJ('L');
        break;

    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Coloca a cor de background para preto e opaco

    glEnable(GL_LIGHTING); // Habilita iluminação

    glClearDepth(1.0f); // Buffer de profundidade mais distante possível
    glEnable(GL_DEPTH_TEST); // Culling de profundidade
    glDepthFunc(GL_LEQUAL); // Tipo de teste de profundidade

    // Define as informações da luz (cada tipo de luz)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);

    glLightfv(GL_LIGHT2, GL_POSITION, LightPosition);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

    // Habilita cada luz
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    // Material Info
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    float ambient[] = { 0.05375f, 0.05f, 0.06625f, 0.82f };
    float diffuse[] = { 0.54f, 0.89f, 0.63f, 0.95f };
    float specular[] = { 0.316228f, 0.316228f, 0.316228f, 0.95f };
    float position[] = { 200.0f, 300.0f, 100.0f, 0.0f };
    GLint especMaterial = 12.8f;
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMateriali(GL_FRONT, GL_SHININESS, especMaterial);
}

void Initialize() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Carregar OBJ");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_special);

    glutTimerFunc(10, timer, 0);
    loadObj("elepham.obj");
    init();
    glutMainLoop();
    return 0;
}