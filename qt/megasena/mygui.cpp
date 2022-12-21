#include "mygui.h"
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QIcon>
#include <QAction>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidgetAction>
#include <QMessageBox>
#include <string>
#include <random>
#include <set>
#include <fstream>
#include <QFileDialog>


bool has_only_digits(const std::string & s){
  return s.find_first_not_of( "0123456789" ) == std::string::npos;
}

MyGui::MyGui(QWidget *parent):QMainWindow(parent) { 
    
    // Menu File
    QPixmap novo_pix("images/filenew.gif");
    QPixmap abrir_pix("images/fileopen.gif");
    QPixmap salvar_pix("images/filesave.gif");
    QPixmap quit_pix("images/exit.gif");
    QPixmap calc_pix("images/calculate.png");
    QAction *novo = new QAction(novo_pix, "&Novo", this);
    novo->setShortcut(tr("CTRL+N"));
    QAction *abrir = new QAction(abrir_pix, "&Abrir", this);
    abrir->setShortcut(tr("CTRL+O"));
    QAction *salvar = new QAction(salvar_pix, "&Salvar Como", this);
    salvar->setShortcut(tr("CTRL+S"));
    QAction *quit = new QAction(quit_pix, "Sai&r", this);
    quit->setShortcut(tr("CTRL+Q"));
    QAction *gerar = new QAction(calc_pix, "&Gerar Apostas", this);
    gerar->setShortcut(tr("CTRL+G"));
    QMenu *file;
    file = menuBar()->addMenu("&Arquivo");
    file->addAction(novo);
    file->addAction(abrir);
    file->addAction(salvar);
    file->addSeparator();
    file->addAction(gerar);
    file->addSeparator();
    file->addAction(quit);
    connect(quit, &QAction::triggered, qApp, QApplication::quit);
    connect(gerar, &QAction::triggered, this, &MyGui::gerar);
    connect(novo, &QAction::triggered, this, &MyGui::novo_func);
    connect(abrir, &QAction::triggered, this, &MyGui::abrir_func);
    connect(salvar, &QAction::triggered, this, &MyGui::salvar_func);
    
    // Menu Ajuda
    QAction *sobre = new QAction("&Sobre", this);
    sobre->setShortcut(tr("CTRL+H"));
    QMenu *ajuda;
    ajuda = menuBar()->addMenu("A&juda");
    ajuda->addAction(sobre);
    connect(sobre, &QAction::triggered, this, &MyGui::sobre_func);
    
    qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);
    
    //Toolbar
    QToolBar *toolbar = addToolBar("main toolbar");
    toolbar->addAction(novo);
    toolbar->addAction(abrir);
    toolbar->addAction(salvar);
    toolbar->addAction(quit);
    toolbar->addSeparator();
    toolbar->addAction(gerar);
    toolbar->addSeparator();
    QWidgetAction *widgetAction=new QWidgetAction(this);
    widgetAction->setDefaultWidget(new QLabel("Número de apostas:", this));
    toolbar->addAction(widgetAction);
    QWidgetAction *widgetAction2=new QWidgetAction(this);
    bets_form = new QLineEdit("1000", this);
    //bets_form->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    bets_form->setFixedSize(120, 22);
    widgetAction2->setDefaultWidget(bets_form);
    toolbar->addAction(widgetAction2);
    
    // Text Area
    edit = new QTextEdit(this);
    setCentralWidget(edit);
   
    statusBar()->showMessage("Pronto", 2000);
}

void MyGui::keyPressEvent(QKeyEvent *event) {
   if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {  
       this->gerar();
   }
}

void MyGui::gerar() {
    
    std::string num = bets_form->text().toStdString();
    if (!has_only_digits(num)) {
        QMessageBox::warning(this, "Atenção", "Número de apostas inválido");
        return;
    }else {
        edit->clear();
        statusBar()->showMessage("Calculando");
        std::set< std::set<int> > bets;
        std::set< std::set<int> >::size_type lh = std::stol(num);
    
        std::random_device seeder;
        std::mt19937 engine(seeder());
        std::uniform_int_distribution<int> dist(MIN, MAX);
        
        while (bets.size() < lh) {
            std::set<int> myset;
            std::set<int>::size_type jh = 6;
            while (myset.size() < jh) {
                myset.insert(dist(engine));
            }
            bets.insert(myset);
        }
        QString result = "";
        result.reserve(6 * std::stol(num));
        
        for (const std::set<int> &out: bets) {
            for (const int &num: out) {
                result.push_back(QString::number(num) + " ");
            }
            result.push_back("\n");
        }
        edit->insertPlainText(result);
        statusBar()->showMessage("Pronto", 2000);
    }
}

void MyGui::novo_func() {
    if (edit->toPlainText().count() == 0){
        edit->clear();
        statusBar()->showMessage("Pronto", 2000);
    }else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Test", "Deseja Salvar o Arquivo?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            MyGui::salvar_func();
            edit->clear();
        }else {
            edit->clear();
            statusBar()->showMessage("Pronto", 2000);
        }
    }
}

void MyGui::abrir_func() {
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Abrir"), "~", tr(" CSV Files (*.csv *.txt)"));
    if (fileName.count() > 0) {
        std::ifstream myfile;
        myfile.open(fileName.toStdString(), std::ios::in);
        if (myfile.is_open()) {
            std::string line;
            while (std::getline(myfile, line)) {
                edit->insertPlainText(QString::fromStdString(line));
                edit->insertPlainText("\n");
            }
            myfile.close();
            statusBar()->showMessage("Sucesso", 2000);
        }else {
            QMessageBox::warning(this, "Atenção", "Falha ao abrir o arquivo");
            statusBar()->showMessage("Erro ao Abrir Arquivo", 2000);
        }
    }
}

void MyGui::salvar_func() {
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("Salvar"), "apostas.csv", tr(" CSV Files (*.csv *.txt)"));
    if (fileName.count() > 0) {
        std::ofstream myfile;
        myfile.open(fileName.toStdString(), std::ios::out);
        if (myfile.is_open()) {
            myfile << edit->toPlainText().toStdString();
            myfile.close();
            statusBar()->showMessage("Salvo com sucesso", 2000);
        } else {
            QMessageBox::warning(this, "Atenção", "Falha ao abrir o arquivo");
            statusBar()->showMessage("Erro ao Salvar", 2000);
        }
    }
}

void MyGui::sobre_func() {
    QMessageBox::information(this, "Sobre", "Gerador de apostas megasena 0.1 - Fernando B. Giannasi - agosto/2017");
}