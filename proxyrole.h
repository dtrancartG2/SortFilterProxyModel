#ifndef PROXYROLE_H
#define PROXYROLE_H

#include <QObject>
#include <QMutex>
#include <qqml.h>
#include "qqmlsortfilterproxymodel.h"

namespace qqsfpm {

class ProxyRole : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit ProxyRole(QObject *parent = nullptr);

    const QString& name() const;
    void setName(const QString& name);

    QVariant roleData(const QModelIndex& sourceIndex, const QQmlSortFilterProxyModel& proxyModel);
    virtual void proxyModelCompleted(const QQmlSortFilterProxyModel& proxyModel);

protected:
    void invalidate();

Q_SIGNALS:
    void nameAboutToBeChanged();
    void nameChanged();
    void invalidated();

private:
    virtual QVariant data(const QModelIndex& sourceIndex, const QQmlSortFilterProxyModel& proxyModel) = 0;

    QString m_name;
    QMutex m_mutex;
};

class JoinRole : public ProxyRole
{
    Q_OBJECT
    Q_PROPERTY(QStringList roleNames READ roleNames WRITE setRoleNames NOTIFY roleNamesChanged)
    Q_PROPERTY(QString separator READ separator WRITE setSeparator NOTIFY separatorChanged)

public:
    using ProxyRole::ProxyRole;

    QStringList roleNames() const;
    void setRoleNames(const QStringList& roleNames);

    QString separator() const;
    void setSeparator(const QString& separator);

Q_SIGNALS:
    void roleNamesChanged();

    void separatorChanged();

private:
    QStringList m_roleNames;
    QVariant data(const QModelIndex& sourceIndex, const QQmlSortFilterProxyModel& proxyModel) override;
    QString m_separator = " ";
};

class SwitchRoleAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
public:
    SwitchRoleAttached(QObject* parent);

    QVariant value() const;
    void setValue(QVariant value);

Q_SIGNALS:
    void valueChanged();

private:
    QVariant m_value;
};

class SwitchRole : public ProxyRole
{
    Q_OBJECT
    Q_PROPERTY(QString defaultRoleName READ defaultRoleName WRITE setDefaultRoleName NOTIFY defaultRoleNameChanged)
    Q_PROPERTY(QQmlListProperty<qqsfpm::Filter> filters READ filters)

public:
    using ProxyRole::ProxyRole;

    QString defaultRoleName() const;
    void setDefaultRoleName(QString defaultRoleName);

    QQmlListProperty<Filter> filters();
    void proxyModelCompleted(const QQmlSortFilterProxyModel& proxyModel) override;

    static SwitchRoleAttached* qmlAttachedProperties(QObject* object);

Q_SIGNALS:
    void defaultRoleNameChanged();

private:
    QVariant data(const QModelIndex& sourceIndex, const QQmlSortFilterProxyModel& proxyModel) override;

    static void append_filter(QQmlListProperty<Filter>* list, Filter* filter);
    static int count_filter(QQmlListProperty<Filter>* list);
    static Filter* at_filter(QQmlListProperty<Filter>* list, int index);
    static void clear_filters(QQmlListProperty<Filter>* list);

    QString m_defaultRoleName;
    QList<Filter*> m_filters;
};

}

QML_DECLARE_TYPEINFO(qqsfpm::SwitchRole, QML_HAS_ATTACHED_PROPERTIES)

#endif // PROXYROLE_H
