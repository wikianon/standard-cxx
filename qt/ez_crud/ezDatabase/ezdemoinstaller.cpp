#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QDate>
#include <QIcon>

#include "ezdemoinstaller.h"
#include "ui_ezdatabase.h"

ezDatabase::ezDatabase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ezDatabase)
{
    ui->setupUi(this);
    ui->organizationName->setEnabled(false);
}

ezDatabase::~ezDatabase()
{
    delete ui;
}

void ezDatabase::handleSqlError(const QSqlError &sqlError)
{
    if(sqlError.isValid()){
        qDebug() << sqlError.text();
    }
}

void ezDatabase::on_bExecute_clicked()
{
    QString appDirPath(QApplication::applicationDirPath());
    // create System database
    if(ui->rbSystem->isChecked()){
        QString sysdbPath(appDirPath+"/sysdb.db");
        if(!QFile::exists(sysdbPath)){
            QStringList tables;
            tables << QString(QString("CREATE  TABLE IF NOT EXISTS `module` (\n") +
                              QString("  `id` INT(11) NOT NULL ,\n") +
                              QString("  `modulename` VARCHAR(200) NOT NULL ,\n") +
                              QString("  `modulemenu` VARCHAR(50) NOT NULL ,\n") +
                              QString("  PRIMARY KEY (`id`) );\n"));
            tables << QString(QString("CREATE  TABLE IF NOT EXISTS `widget` (\n") +
                              QString("  `id` INT(11) NOT NULL ,\n") +
                              QString("  `idmodule` INT(11) NOT NULL ,\n") +
                              QString("  `widgetname` VARCHAR(200) NOT NULL ,\n") +
                              QString("  `widgetmenu` VARCHAR(50) NULL ,\n") +
                              QString("  `widgettype` INT(2) NOT NULL ,\n") +
                              QString("  PRIMARY KEY (`id`) ,\n") +
                              QString("  CONSTRAINT `fk_widget_module1`\n") +
                              QString("    FOREIGN KEY (`idmodule` )\n") +
                              QString("    REFERENCES `module` (`id` )\n") +
                              QString("    ON DELETE NO ACTION\n") +
                              QString("    ON UPDATE NO ACTION);\n"));
            tables << QString(QString("CREATE  TABLE IF NOT EXISTS `revision` (\n") +
                              QString("  `id` INT(11) NOT NULL ,\n") +
                              QString("  `datetime` DATE NOT NULL ,\n") +
                              QString("  `iduser` INT(11) NOT NULL ,\n") +
                              QString("  `idorganization` INT(11) NOT NULL ,\n") +
                              QString("  PRIMARY KEY (`id`) ,\n") +
                              QString("  CONSTRAINT `fk_revision_user1`\n") +
                              QString("    FOREIGN KEY (`iduser` )\n") +
                              QString("    REFERENCES `user` (`id` )\n") +
                              QString("    ON DELETE NO ACTION\n") +
                              QString("    ON UPDATE NO ACTION,\n") +
                              QString("  CONSTRAINT `fk_revision_organization1`\n") +
                              QString("    FOREIGN KEY (`idorganization` )\n") +
                              QString("    REFERENCES `organization` (`id` )\n") +
                              QString("    ON DELETE NO ACTION\n") +
                              QString("    ON UPDATE NO ACTION);\n"));
            tables << QString(QString("CREATE  TABLE IF NOT EXISTS `module_revision` (\n") +
                              QString("  `idrevision` INT(11) NOT NULL ,\n") +
                              QString("  `idmodule` INT(11) NOT NULL ,\n") +
                              QString("  PRIMARY KEY (`idrevision`, `idmodule`) ,\n") +
                              QString("  CONSTRAINT `fk_module_revision_revision1`\n") +
                              QString("    FOREIGN KEY (`idrevision` )\n") +
                              QString("    REFERENCES `revision` (`id` )\n") +
                              QString("    ON DELETE NO ACTION\n") +
                              QString("    ON UPDATE NO ACTION,\n") +
                              QString("  CONSTRAINT `fk_module_revision_module1`\n") +
                              QString("    FOREIGN KEY (`idmodule` )\n") +
                              QString("    REFERENCES `module` (`id` )\n") +
                              QString("    ON DELETE NO ACTION\n") +
                              QString("    ON UPDATE NO ACTION);\n"));
            tables << QString(QString("CREATE  TABLE IF NOT EXISTS `widget_revision` (\n") +
                              QString("  `idrevision` INT(11) NOT NULL ,\n") +
                              QString("  `idwidget` INT(11) NOT NULL ,\n") +
                              QString("  `accessmode` INT(2) NOT NULL ,\n") +
                              QString("  PRIMARY KEY (`idrevision`, `idwidget`) ,\n") +
                              QString("  CONSTRAINT `fk_widget_revision_revision1`\n") +
                              QString("    FOREIGN KEY (`idrevision` )\n") +
                              QString("    REFERENCES `revision` (`id` )\n") +
                              QString("    ON DELETE NO ACTION\n") +
                              QString("    ON UPDATE NO ACTION,\n") +
                              QString("  CONSTRAINT `fk_widget_revision_widget1`\n") +
                              QString("    FOREIGN KEY (`idwidget` )\n") +
                              QString("    REFERENCES `widget` (`id` )\n") +
                              QString("    ON DELETE NO ACTION\n") +
                              QString("    ON UPDATE NO ACTION);\n"));
            tables << QString(QString("CREATE  TABLE IF NOT EXISTS `organization` (\n") +
                              QString("  `id` INT(11) NOT NULL ,\n") +
                              QString("  `name` VARCHAR(100) NOT NULL ,\n") +
                              QString("  `hostaddress` VARCHAR(200) NOT NULL ,\n") +
                              QString("  `hostdatabase` VARCHAR(200) NOT NULL ,\n") +
                              QString("  `hostuser` VARCHAR(200) NOT NULL ,\n") +
                              QString("  `hostpassword` VARCHAR(200) NOT NULL ,\n") +
                              QString("  PRIMARY KEY (`id`) ,\n") +
                              QString("  UNIQUE (`name`) );\n"));
            tables << QString(QString("CREATE  TABLE IF NOT EXISTS `sequences` (\n") +
                              QString("  `name` VARCHAR(200) NOT NULL ,\n") +
                              QString("  `initialvalue` BIGINT(12) NOT NULL ,\n") +
                              QString("  `nextvalue` BIGINT(12) NOT NULL ,\n") +
                              QString("  UNIQUE (`name`) ,\n") +
                              QString("  PRIMARY KEY (`name`) );\n"));
            tables << QString(QString("CREATE  TABLE IF NOT EXISTS `user` (\n") +
                              QString("  `id` INT(11) NOT NULL ,\n") +
                              QString("  `datetime` DATE NOT NULL ,\n") +
                              QString("  `loginname` VARCHAR(200) NOT NULL ,\n") +
                              QString("  `password` VARCHAR(200) NOT NULL ,\n") +
                              QString("  `username` VARCHAR(200) NOT NULL ,\n") +
                              QString("  `email` VARCHAR(200) NULL ,\n") +
                              QString("  PRIMARY KEY (`id`) ,\n") +
                              QString("  UNIQUE (`loginname`) );"));
            QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
            db.setDatabaseName("sysdb.db");
            db.open();
            handleSqlError(db.lastError());
            QSqlQuery query(db);
            foreach(QString sqlQuery, tables){
                query.exec(sqlQuery);
                handleSqlError(query.lastError());
            }

            // create root user

            query.prepare("insert into user(id, datetime, loginname, password, username, email) "
                          " values(:id, :datetime, :loginname, :password, :username, :email)");
            query.bindValue(":id", 1);
            query.bindValue(":datetime", QDate::currentDate());
            query.bindValue(":loginname", "ROOT");
            query.bindValue(":password", "rAOYEMtY0bda0EtXWi2QBA==");
            query.bindValue(":username", "Luis Valdes");
            query.bindValue(":email", "luisvaldes88@gmail.com");
            query.exec();
            handleSqlError(query.lastError());

            // insert modules
            query.exec("insert into module(id, modulename, modulemenu) values(1, 'mod_root', 'Administrador')");
            query.exec("insert into module(id, modulename, modulemenu) values(2, 'mod_generic', 'Generico')");
            handleSqlError(query.lastError());

            // insert widgets
            // Module Root
            query.exec("insert into widget(id, idmodule, widgetname, widgetmenu, widgettype) "
                       " values(1, 1, 'UserCrud', 'Usuarios', 1 )");
            query.exec("insert into widget(id, idmodule, widgetname, widgetmenu, widgettype) "
                       " values(2, 1, 'ModuleCrud', 'Modules', 1 )");
            query.exec("insert into widget(id, idmodule, widgetname, widgetmenu, widgettype) "
                       " values(3, 1, 'WidgetCrud', 'Widgets', 1 )");
            query.exec("insert into widget(id, idmodule, widgetname, widgetmenu, widgettype) "
                       " values(4, 1, 'OrganizationCrud', 'Organización', 1 )");
            // Module Generic
            query.exec("insert into widget(id, idmodule, widgetname, widgetmenu, widgettype) "
                       " values(5, 2, 'CategoriaCrud', 'Categoria', 1 )");
            query.exec("insert into widget(id, idmodule, widgetname, widgetmenu, widgettype) "
                       " values(6, 2, 'PersonaCrud', 'Persona', 1 )");
            handleSqlError(query.lastError());

            // create sequences
            query.exec("insert into sequences(name, initialvalue, nextvalue) values('seq_module', 1, 3)");
            query.exec("insert into sequences(name, initialvalue, nextvalue) values('seq_organization', 1, 1)");
            query.exec("insert into sequences(name, initialvalue, nextvalue) values('seq_revision', 1, 1)");
            query.exec("insert into sequences(name, initialvalue, nextvalue) values('seq_user', 1, 2)");
            query.exec("insert into sequences(name, initialvalue, nextvalue) values('seq_widget', 1, 7)");
            handleSqlError(query.lastError());
        }else{
            QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
            db.setDatabaseName("sysdb.db");
            db.open();
            handleSqlError(db.lastError());
            QSqlQuery query(db);
            query.prepare("select loginname, password from user ");
            query.exec();
            handleSqlError(db.lastError());
            while (query.next()) {
                QString loginName = query.value(0).toString();
                QString password = query.value(1).toString();
                qDebug() << QString("loginName = %1 | password = %2").arg(loginName, password);
            }
        }
    }else{ // create Organization database
        if(ui->organizationName->text() != ""){
            QString orgName(QString("%1.db").arg(ui->organizationName->text()));
            QString orgPath(appDirPath +"/"+orgName);
            if(!QFile::exists(orgPath)){
                QStringList tables;
                tables << QString(QString("CREATE  TABLE IF NOT EXISTS `categoria` (\n") +
                                  QString("  `id` INT(11) NOT NULL ,\n") +
                                  QString("  `nombre` VARCHAR(50) NULL ,\n") +
                                  QString("  PRIMARY KEY (`id`) );\n"));
                tables << QString(QString("CREATE  TABLE IF NOT EXISTS `persona` (\n") +
                                  QString("  `id` INT(11) NOT NULL ,\n") +
                                  QString("  `tipopersona` INT(4) NOT NULL ,\n") +
                                  QString("  `razonsocial` VARCHAR(100) NULL ,\n") +
                                  QString("  `ruc` VARCHAR(50) NULL ,\n") +
                                  QString("  `barrio` VARCHAR(100) NULL ,\n") +
                                  QString("  `calle1` VARCHAR(100) NULL ,\n") +
                                  QString("  `calle2` VARCHAR(100) NULL ,\n") +
                                  QString("  `telefono1` VARCHAR(100) NULL ,\n") +
                                  QString("  `telefono2` VARCHAR(100) NULL ,\n") +
                                  QString("  `email1` VARCHAR(100) NULL ,\n") +
                                  QString("  `email2` VARCHAR(100) NULL ,\n") +
                                  QString("  PRIMARY KEY (`id`) );\n"));
                tables << QString(QString("CREATE  TABLE IF NOT EXISTS `persona_categoria` (\n") +
                                  QString("  `idpersona` INT(11) NOT NULL ,\n") +
                                  QString("  `idcategoria` INT(11) NOT NULL ,\n") +
                                  QString("  PRIMARY KEY (`idpersona`, `idcategoria`) ,\n") +
                                  QString("  CONSTRAINT `fk_persona_has_categoria_persona1`\n") +
                                  QString("    FOREIGN KEY (`idpersona` )\n") +
                                  QString("    REFERENCES `persona` (`id` )\n") +
                                  QString("    ON DELETE NO ACTION\n") +
                                  QString("    ON UPDATE NO ACTION,\n") +
                                  QString("  CONSTRAINT `fk_persona_has_categoria_categoria1`\n") +
                                  QString("    FOREIGN KEY (`idcategoria` )\n") +
                                  QString("    REFERENCES `categoria` (`id` )\n") +
                                  QString("    ON DELETE NO ACTION\n") +
                                  QString("    ON UPDATE NO ACTION);\n"));
                tables << QString(QString("CREATE  TABLE IF NOT EXISTS `sequences` (\n") +
                                  QString("  `name` VARCHAR(200) NOT NULL ,\n") +
                                  QString("  `initialvalue` BIGINT(12) NOT NULL ,\n") +
                                  QString("  `nextvalue` BIGINT(12) NOT NULL ,\n") +
                                  QString("  PRIMARY KEY (`name`) ,\n") +
                                  QString("  UNIQUE (`name`) );"));
                QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
                db.setDatabaseName(orgName);
                db.open();
                handleSqlError(db.lastError());
                QSqlQuery query(db);
                foreach(QString sqlQuery, tables){
                    query.exec(sqlQuery);
                    handleSqlError(query.lastError());
                }
                // create sequences
                query.exec("insert into sequences(name, initialvalue, nextvalue) values('seq_categoria', 1, 1)");
                query.exec("insert into sequences(name, initialvalue, nextvalue) values('seq_persona', 1, 1)");
                handleSqlError(query.lastError());
            }
        }
    }
}
