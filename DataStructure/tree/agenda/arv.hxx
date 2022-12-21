#include <cstdio>
#include <cstring>
#include <cstdlib>
/*!
 *
 * @begin @section terms_of_use Terms of Use
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * @end @section terms_of_use Terms of Use
 * 
 * @begin @section author Author
 * 
 * @file     arv.hxx
 * @version  0.3
 * @brief    Árvore binária.
 * @utility  Agenda telefonica.
 * @Original_Consult https://www.vivaolinux.com.br/script/Agenda-feita-em-C-usando-arvore-binaria
 * @Autor: Marcos Augusto
 * @Email_Autor: marcosccp04@gmail.com
 * @Modified: Jean Zonta
 * @Copyright (C) 2017 Marcos Augusto, Jean Zonta.
 * 
 * @end @section author Author
 *
 */
struct Arvore
{
 char Nome[100];
 int telefone;
 Arvore *esq;
 Arvore *dir;
};

void inserir(Arvore**,char*);
void ordem(Arvore*);
void busca(Arvore*,char*);
void alterar( Arvore**, char*);
void excluir(Arvore**,char*);
Arvore** menor_dir(Arvore*);
Arvore** maior_esq(Arvore*);
void getNames(char*);
void getPhones(int*);
void maiuscula(char*);
void menu(int*);