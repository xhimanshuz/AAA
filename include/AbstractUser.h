#ifndef ABSTRACTUSER_H
#define ABSTRACTUSER_H

#include <QString>

class AbstractUser
{
    virtual void fillData() = 0;
    virtual void addNew() = 0;
    virtual QStringList currentData(const QString &user) = 0;
};

#endif // ABSTRACTUSER_H
