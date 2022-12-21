#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QKeyEvent>

class MyGui : public QMainWindow {
    
    Q_OBJECT
    QLineEdit *bets_form;
    QTextEdit *edit;
    const int MIN = 1;
    const int MAX = 60;
    
public:
    MyGui(QWidget *parent = nullptr);
    
private slots:
    void gerar();
    void novo_func();
    void abrir_func();
    void salvar_func();
    void sobre_func();
protected:
    void keyPressEvent(QKeyEvent *event);
};