#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QObject>

#include "../global/ezcrud_global.h"

template <typename ForwardIterator>
void deleteAll(ForwardIterator begin, ForwardIterator end)
{
    while (begin != end) {
        QObject *obj = qobject_cast<QObject *>(reinterpret_cast<QObject *>(*begin));
        if(obj)
            obj->deleteLater();
        else
            delete *begin;
        ++begin;
    }
}

template <typename Container>
inline void deleteAll(const Container &c)
{
    deleteAll(c.begin(), c.end());
}

#endif // ALGORITHMS_H
