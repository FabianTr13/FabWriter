#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/QSql>
#include <QSqlQuery>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
using namespace std;

//---------------------------------------------ARBOL--------------------------------------------------
class Nodo{
    public:
    QString indice;
    QString dato;

Nodo(QString inx = "", QString dat = ""){
    indice = inx;
    dato = dat;
}
};

class NodoLista{
    public:
    Nodo claves [5];
    NodoLista *hijos [5];
    NodoLista *shojas;
    int ingresados;
NodoLista(){
    this->ingresados = 0;
}
};

class Arbol{
public:
    NodoLista *raiz, *rHijo, *P, *raizHojas;
    int k, MAX, MIN, nivel;
    Nodo cajaT;
    bool esta, ea, existe ;
    QString sugerencias[4];

Arbol(){
    k = nivel = 0;
    esta    = ea, existe = false;
    MAX     = 4;
    MIN     = 2;
    raizHojas = raiz = P = rHijo = NULL;
}

bool arbolVacio(NodoLista *nodo){
    return ((nodo == NULL) || (nodo->ingresados == 0));
}

void refrescarRaizHojas(){
    nivel = altura ();
    NodoLista *aux = raiz;
    while (!arbolVacio(aux->hijos[0]))
        aux = aux->hijos[0];

    raizHojas = aux;
}

void insertar(QString valor){
    Nodo temp(valor,valor);
    insertarF(temp);
}

void insertarF (Nodo clave){
    ea = false;
    empujar(clave, raiz);
    if(ea){
        NodoLista *nuevo = new NodoLista();
        nuevo->ingresados = 1;
        nuevo->claves[1] = cajaT;
        nuevo->hijos [0] = raiz;
        nuevo->hijos [1] = rHijo;
        raiz = nuevo;
    }
    refrescarRaizHojas();
}

void empujar (Nodo clave, NodoLista *nodo){
    if(nodo == NULL){
        ea = true;
        cajaT = clave;
        rHijo = NULL;
    }
    else{
        esta = buscarNodo (clave, nodo);
        if(esta)
           qDebug()<<"\nCodigo Repetido: "<< clave.indice;
        else{
            empujar(clave, nodo->hijos[k]);
            buscarNodo(clave, nodo);

            if(ea){
                if(nodo->ingresados < MAX){
                    //qDebug()<< "Voy a meterhoja!" <<endl;
                    ea = false;
                    P = nodo;
                    meterHoja();
                }
                else{
                    //qDebug()<< "Voy a dividir!" << endl;
                    ea = true;
                    P = nodo;
                    dividir();
                }
            }
        }
    }
}

void meterHoja(){
    for(int i = P->ingresados; i >= k+1; i--){
            P->claves[i + 1] = P->claves[i];
            P->hijos [i + 1] = P->hijos [i];
    }
    P->claves[k+1] = cajaT;
    P->hijos [k+1] = rHijo;
    P->ingresados++;
}

void dividir(){
    NodoLista *temp = new NodoLista();
    int pos;

    if (k <= MIN)
        pos = MIN;
    else
        pos = MIN+1;

    for(int i = pos+1; i <= MAX; i++){
        temp->claves[i-pos] = P->claves[i];
        temp->hijos [i-pos] = P->hijos[i];
    }

    temp->ingresados = MAX-pos;
    P->ingresados = pos;

    if (k <= MIN)
        meterHoja();
    else{
        k = k-pos;
        NodoLista *aux = P;
        P = temp;
        meterHoja();
        P = aux;
    }
    cajaT = P->claves[P->ingresados];
    if(arbolVacio(P->hijos[0]))
        duplicar(temp);

    temp->hijos[0] = P->hijos[P->ingresados];
    temp->shojas = P->shojas;
    P->shojas = temp;
    P->ingresados--;
    rHijo = temp;
}

void duplicar (NodoLista *temp){
    for(int i = temp->ingresados; i >= 1; i--){
        temp->claves[i + 1] = temp->claves[i];
        temp->hijos [i + 1] = temp->hijos [i];
    }
    temp->claves[1] = cajaT;
    temp->ingresados++;
}

bool buscarNodo(Nodo clave, NodoLista *nodo){
        if(nodo == NULL)
            return false;
        else{
            if(clave.indice < nodo->claves[1].indice){
                    k = 0;
                    return esta = false;
            }
            else{
                k = nodo->ingresados;
                while((k > 1) && (clave.indice <nodo->claves[k].indice))
                    k--;

                esta = (clave.indice == nodo->claves[k].indice);
            }
            return esta;
        }
}

bool buscar(QString valor){
    Nodo clave(valor,valor);
    empujarPorExistencia(clave,raiz,valor);
    bool res = existe;
    existe = false;
    ea = false;
    esta = false;
    return res;
}

void empujarPorExistencia(Nodo clave, NodoLista *nodo,QString palabra){
    if(nodo == NULL)
        ea = true;
    else{
        esta = buscarNodo (clave, nodo);
        if(esta)
            existe = false;
        else{
            empujarPorExistencia(clave, nodo->hijos[k],palabra);
            buscarNodo(clave, nodo);
            if(ea)
                existe = false;
            else
                existe = true;
        }
    }
}

int altura(){
    int res = 0;
    NodoLista *aux = raiz;
    if (arbolVacio(aux))
        qDebug()<<"\nEl Arbol B+ esta Vacio!";
    else{
        while (!arbolVacio(aux)){
            res++;
            aux = aux->hijos[0];
        }
        return res;
    }
}

void BuscarSugerencias(QString valor){
    Nodo clave(valor,valor);
    empujarPorSugerencias(clave,raiz);
}

void empujarPorSugerencias(Nodo clave, NodoLista *nodo){
    if(nodo == NULL)
        ea = true;
    else{
        esta = buscarNodo (clave, nodo);
        if(esta)
            qDebug()<<"\nCodigo Repetido: "<< clave.indice << endl;
        else{
            empujarPorSugerencias(clave, nodo->hijos[k]);
            buscarNodo(clave, nodo);
            if(ea){
                if(nodo->ingresados < MAX){
                    ea = false;
                    P = nodo;
                    for(int i = 0; i<4;i++)
                    sugerencias[i]=P->claves[i+1].dato;
                }
                else{
                    ea = true;
                    P = nodo;
                }
            }
        }
    }
}
};
//---------------------------------------VARIABLES GLOBALES-----------------------------------------
bool menu = false;
Arbol diccionario;
QStringList listPalabras;
QStringList listaErrores;
QStringList listaColorear;
QStringList listaOMitidos;
QString nombre;
//--------------------------------------Funciones De DataBase---------------------------------------
//Opcional si gustas cargar las palabras desde una base de datos
void cargarDiccionario(){
    QString cadena = "SELECT `palabra` FROM `Libro_Nacho`.`palabra`;";
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    QSqlQuery query;
    if (db.open()){
        if(query.exec(cadena)){
            while(query.next())
                diccionario.insertar(query.value(0).toString());
        }else
            QMessageBox::critical(0,"Error","Ocurrio un error en la extraccion de paabras");
        db.close();
    }else
        QMessageBox::critical(0,"Error","Error de coneccion");
}

void querysInsert(QString palabra){
    QString cadena = "INSERT INTO `Libro_Nacho`.`palabra` (`palabra`) VALUES ('"+palabra+"');";
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    QSqlQuery query;
    if (db.open()){
       if(query.exec(cadena));
       db.close();
    }
}
//----------------------------------------------constructor del mainwindows--------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->txtPag->setFocus();
    cargarDiccionario();
    startTimer(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//---------------------------------------------FUNCIONES DEL EDITOR----------------------------------
QString textFinal(){
    for(int i = 0;i<listPalabras.size();i++){
        for(int j = 0;j<listaErrores.size();j++){
            if(listPalabras.at(i)==listaErrores.at(j))
                listPalabras.replace(i,listaColorear.at(j));
        }
    }
    QString text;
     for(int i = 0;i<listPalabras.size();i++){
        if(i==0)
            text=listPalabras.at(i);
        else
            text = text +" "+listPalabras.at(i);
     }
   return text;
}

void colorear(){
    //limpieza
    listaErrores.removeDuplicates();
    for(int i = 0; i<listaErrores.size(); i++){
        if(listaErrores.at(i) == "")
            listaErrores.removeAt(i);
    }
    for(int i = 0; i<listaOMitidos.size(); i++){
        for(int j = 0; j<listaErrores.size(); j++){
            if(listaErrores.at(j) == listaOMitidos.at(i))
                listaErrores.removeAt(j);
        }
    }
    //coloracion
    for(int i = 0; i<listaErrores.size(); i++){
        QString color = "<font color = \"red\"> "+listaErrores.at(i) +"</font>";
        listaColorear<<color;
    }
}

void cargaErrores(){
    for(int i = 0;i<listPalabras.size();i++){
        if(!diccionario.buscar(listPalabras.at(i).toLower()))
            listaErrores<<listPalabras.at(i);
    }
    colorear();
}

void keylogger(QString text){
    listPalabras.clear();
    listaErrores.clear();
    listaColorear.clear();
    QRegExp rx("(\\ |\\,|\\.|\\:|\\t|\\;|\\?|\\=)");
    listPalabras = text.split(rx);
    cargaErrores();
}
//-------------------------------------------Manejo de archivos---------------------------------------
void MainWindow::abrir(){
    if(nombre!="")
        guardar();

    QString file = QFileDialog::getOpenFileName(this,"Abrir archivos");
    nombre = file;
    if(!file.isEmpty()){
        QFile archivo(file);
        if(archivo.open(QIODevice::ReadOnly|QFile::Text)){
            QTextStream lectura(&archivo);
            QString texto = lectura.readAll();
            archivo.close();
            ui->txtPag->clear();
            ui->txtPag->appendPlainText(texto);
            ui->txtPag->setGeometry(10,80,761,451);
            menu = false;
          }
     }
}

void MainWindow::guardar(){
    if(nombre!=""){
        QFile archivo(nombre);
        if(archivo.open(QFile::WriteOnly|QFile::Text)){
            QTextStream lectura(&archivo);
            lectura<<ui->txtPag->toPlainText();
            archivo.flush();
            archivo.close();
        }
    }else
        guardarComo();
}

void MainWindow::guardarComo(){
    QString newName = QFileDialog::getSaveFileName(this,"Guardar como: ");
    if(!newName.isEmpty()){
        nombre = newName;
        guardar();
    }
}

void MainWindow::reload(){
    ui->cbPalabras->clear();
    for(int i = 0; i<listaErrores.size(); i++)
       ui->cbPalabras->addItem(listaErrores.at(i));
}
//-------------------------------------------------------EVENTOS--------------------------------------
void MainWindow::on_btnMenu_clicked(){
    if(menu == false){
        ui->txtPag->setGeometry(180,80,590,451);
        menu = true;
    }else{
        ui->txtPag->setGeometry(10,80,761,451);
        menu = false;
    }
}

void MainWindow::on_txtPag_textChanged(){
    keylogger(ui->txtPag->toPlainText());
    ui->lbCantErrores->setText(QString::number(listaErrores.size()));
    ui->lbCPalabras->setText(QString::number(listPalabras.size()));
}


void MainWindow::on_btnOmitir_clicked(){
    listaOMitidos<<ui->cbPalabras->currentText();
    ui->txtPag->setGeometry(10,80,761,451);
    menu = false;
}

void MainWindow::on_btnAgregar_clicked(){
    if(!ui->cbPalabras->currentText().isEmpty()){
        diccionario.insertar(ui->cbPalabras->currentText());
        querysInsert(ui->cbPalabras->currentText());
        ui->txtPag->setGeometry(10,80,761,451);
        menu = false;
    }
}

void MainWindow::on_cbPalabras_currentIndexChanged(int index){
    ui->lwSug->clear();
    diccionario.BuscarSugerencias(ui->cbPalabras->currentText());
       for(int i = 0; i<4; i++)
           ui->lwSug->addItem(diccionario.sugerencias[i]);
}

void MainWindow::timerEvent(QTimerEvent *event){
    QString text = textFinal();
    ui->txtPag->clear();
    ui->txtPag->appendHtml(text);
}

void MainWindow::on_lwSug_doubleClicked(const QModelIndex &index){
    QString correccion = ui->lwSug->currentItem()->text();
    QString error = ui->cbPalabras->currentText();
    QString text;
    for(int i = 0;i<listPalabras.size();i++){
            if(listPalabras.at(i)==error){
                listPalabras.replace(i,correccion);
            }
     }
     for(int i = 0;i<listPalabras.size();i++){
        if( i==0)
            text=listPalabras.at(i);
        else
            text = text +" "+listPalabras.at(i);
     }
    ui->txtPag->clear();
    ui->txtPag->appendPlainText(text);
    ui->txtPag->setGeometry(10,80,761,451);
    menu = false;
    reload();
}

void MainWindow::nuevo(){
    if(nombre!="")
        guardar();
    ui->txtPag->clear();
    listaOMitidos.clear();
    nombre = "";
    ui->txtPag->setGeometry(10,80,761,451);
    menu = false;
}

void MainWindow::on_sbTam_valueChanged(int arg1)
{
    QFont fuente = ui->txtPag->font();
    fuente.setPointSize(ui->sbTam->value());
    ui->txtPag->setFont(fuente);
}

void MainWindow::on_cbFont_currentIndexChanged(int index)
{
    ui->txtPag->setFont(ui->cbFont->currentFont());
}
