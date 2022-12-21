#ifndef CRUD_CASEUTIL_H
#define CRUD_CASEUTIL_H

#include <QObject>

#include "../global/ezcrud_global.h"

class QLineEdit;

namespace crud {

class EZCRUD_EXPORT CaseUtil : public QObject
{
    Q_OBJECT
public:
    static CaseUtil * instance();

    void addUpperCaseEdit(QLineEdit *lineEdit);
    void addLowerCaseEdit(QLineEdit *lineEdit);

protected:
    explicit CaseUtil(QObject *parent = 0);
    QList<QObject *> m_upperCaseList;
    QList<QObject *> m_lowerCaseList;
    
private slots:
    void toUpper(const QString &text);
    void toLower(const QString &text);
};

} // namespace crud

#endif // CRUD_CASEUTIL_H
