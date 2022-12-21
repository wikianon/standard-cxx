#include <QLineEdit>

#include "../core/ezapplication.h"

#include "caseutil.h"

namespace crud {

CaseUtil::CaseUtil(QObject *parent) :
    QObject(parent),
    m_upperCaseList(),
    m_lowerCaseList()
{
}

void CaseUtil::toUpper(const QString &text)
{
    QLineEdit *le = qobject_cast<QLineEdit *>(sender());
    if (!le)
        return;
    le->setText(text.toUpper());
}

void CaseUtil::toLower(const QString &text)
{
    QLineEdit *le = qobject_cast<QLineEdit *>(sender());
    if (!le)
        return;
    le->setText(text.toLower());
}

void CaseUtil::addUpperCaseEdit(QLineEdit *lineEdit)
{
    if((m_lowerCaseList.indexOf(lineEdit) == -1) && (m_upperCaseList.indexOf(lineEdit) == -1)){
        m_upperCaseList.append(lineEdit);
        connect(lineEdit, SIGNAL(textEdited(QString)), SLOT(toUpper(QString)));
    }
}

void CaseUtil::addLowerCaseEdit(QLineEdit *lineEdit)
{
    if((m_lowerCaseList.indexOf(lineEdit) == -1) && (m_upperCaseList.indexOf(lineEdit) == -1)){
        m_lowerCaseList.append(lineEdit);
        connect(lineEdit, SIGNAL(textEdited(QString)), SLOT(toLower(QString)));
    }
}

CaseUtil *CaseUtil::instance()
{
    static CaseUtil *instance = 0;
    if(!instance){
        instance = new CaseUtil(EZApplication::instance());
    }
    return instance;
}

} // namespace crud
