#ifndef EZDATABASE_H
#define EZDATABASE_H

#include <QWidget>

namespace Ui {
class ezDatabase;
}

class QSqlError;

class ezDatabase : public QWidget
{
    Q_OBJECT
    
public:
    explicit ezDatabase(QWidget *parent = 0);
    ~ezDatabase();
    
    void handleSqlError(const QSqlError &sqlError);

private slots:
    void on_bExecute_clicked();

private:
    Ui::ezDatabase *ui;
};

#endif // EZDATABASE_H
