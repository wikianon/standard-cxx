/*
 * Copyright (C) 2013-2018 Daniel Nicoletti <dantti12@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "controller_p.h"

#include "application.h"
#include "dispatcher.h"
#include "action.h"
#include "common.h"

#include <QMetaClassInfo>
#include <QRegularExpression>

static const QString _path(QLatin1String("Path"));
static const QString _args(QLatin1String("Args"));
static const QString _autoArgs(QLatin1String("AutoArgs"));
static const QString _captureArgs(QLatin1String("CaptureArgs"));
static const QString _autoCaptureArgs(QLatin1String("AutoCaptureArgs"));
static const QString _private(QLatin1String("Private"));

static const QString _backSlashD(QLatin1String("\\D"));
static const QString _name(QLatin1String("name"));
static const QString _reverse(QLatin1String("reverse"));
static const QString _attributes(QLatin1String("attributes"));

using namespace Cutelyst;

Controller::Controller(QObject *parent) : QObject(parent)
  , d_ptr(new ControllerPrivate(this))
{
}

Controller::~Controller()
{
    Q_D(Controller);
    qDeleteAll(d->actionList);
    delete d_ptr;
}

QString Controller::ns() const
{
    Q_D(const Controller);
    return d->pathPrefix;
}

Action *Controller::actionFor(const QString &name) const
{
    Q_D(const Controller);
    Action *ret = d->actions.value(name);
    if (ret) {
        return ret;
    }
    return d->dispatcher->getAction(name, d->pathPrefix);
}

ActionList Controller::actions() const
{
    Q_D(const Controller);
    return d->actionList;
}

bool Controller::operator==(const char *className)
{
    return !qstrcmp(metaObject()->className(), className);
}

bool Controller::preFork(Application *app)
{
    Q_UNUSED(app)
    return true;
}

bool Controller::postFork(Application *app)
{
    Q_UNUSED(app)
    return true;
}

ControllerPrivate::ControllerPrivate(Controller *parent) :
    q_ptr(parent)
{
}

void ControllerPrivate::init(Application *app, Dispatcher *_dispatcher)
{
    Q_Q(Controller);

    dispatcher = _dispatcher;
    application = app;

    // Application must always be our parent
    q->setParent(app);

    const QMetaObject *meta = q->metaObject();
    const QString className = QString::fromLatin1(meta->className());
    q->setObjectName(className);

    bool namespaceFound = false;
    for (int i = meta->classInfoCount() - 1; i >= 0; --i) {
        if (qstrcmp(meta->classInfo(i).name(), "Namespace") == 0) {
            pathPrefix = QString::fromLatin1(meta->classInfo(i).value());
            while (pathPrefix.startsWith(QLatin1Char('/'))) {
                pathPrefix.remove(0, 1);
            }
            namespaceFound = true;
            break;
        }
    }

    if (!namespaceFound) {
        QString controlerNS;
        bool lastWasUpper = true;

        for (int i = 0; i < className.length(); ++i) {
            const QChar c = className.at(i);
            if (c.isLower() || c.isDigit()) {
                controlerNS.append(c);
                lastWasUpper = false;
            } else if (c == QLatin1Char('_')) {
                controlerNS.append(c);
                lastWasUpper = true;
            } else {
                if (!lastWasUpper) {
                    controlerNS.append(QLatin1Char('/'));
                }
                controlerNS.append(c.toLower());
                lastWasUpper = true;
            }
        }
        pathPrefix = controlerNS;
    }

    registerActionMethods(meta, q, app);
}

void ControllerPrivate::setupFinished()
{
    Q_Q(Controller);

    const ActionList beginList = dispatcher->getActions(QStringLiteral("Begin"), pathPrefix);
    if (!beginList.isEmpty()) {
        beginAutoList.append(beginList.last());
    }

    beginAutoList.append(dispatcher->getActions(QStringLiteral("Auto"), pathPrefix));

    const ActionList endList = dispatcher->getActions(QStringLiteral("End"), pathPrefix);
    if (!endList.isEmpty()) {
        end = endList.last();
    }

    const auto actions = actionList;
    for (Action *action : actions) {
        action->dispatcherReady(dispatcher, q);
    }

    q->preFork(qobject_cast<Application *>(q->parent()));
}

bool Controller::_DISPATCH(Context *c)
{
    Q_D(Controller);

    bool ret = true;

    // Dispatch to Begin and Auto
    const auto beginAutoList = d->beginAutoList;
    for (Action *action : beginAutoList) {
        if (!action->dispatch(c)) {
            ret = false;
            break;
        }
    }

    // Dispatch to Action
    if (ret && !c->action()->dispatch(c)) {
        ret = false;
    }

    // Dispatch to End
    if (d->end && !d->end->dispatch(c)) {
        ret = false;
    }

    return ret;
}

Action *ControllerPrivate::actionClass(const QVariantHash &args)
{
    const auto attributes = args.value(_attributes).value<QMap<QString, QString> >();
    const QString actionClass = attributes.value(QStringLiteral("ActionClass"));

    QObject *object = instantiateClass(actionClass, "Cutelyst::Action");
    if (object) {
        Action *action = qobject_cast<Action*>(object);
        if (action) {
            return action;
        }
        qCWarning(CUTELYST_CONTROLLER) << "ActionClass"
                                       << actionClass
                                       << "is not an ActionClass";
        delete object;
    }

    return new Action;
}

Action *ControllerPrivate::createAction(const QVariantHash &args, const QMetaMethod &method, Controller *controller, Application *app)
{
    Action *action = actionClass(args);
    if (!action) {
        return nullptr;
    }

    QStack<Component *> roles = gatherActionRoles(args);
    for (int i = 0; i < roles.size(); ++i) {
        Component *code = roles.at(i);
        code->init(app, args);
        code->setParent(action);
    }
    action->applyRoles(roles);
    action->setMethod(method);
    action->setController(controller);
    action->setName(args.value(_name).toString());
    action->setReverse(args.value(_reverse).toString());
    action->setupAction(args, app);

    return action;
}

void ControllerPrivate::registerActionMethods(const QMetaObject *meta, Controller *controller, Application *app)
{
    // Setup actions
    for (int i = 0; i < meta->methodCount(); ++i) {
        const QMetaMethod method = meta->method(i);
        const QByteArray name = method.name();

        // We register actions that are either a Q_SLOT
        // or a Q_INVOKABLE function which has the first
        // parameter type equal to Context*
        if (method.isValid() &&
                (method.methodType() == QMetaMethod::Method || method.methodType() == QMetaMethod::Slot) &&
                (method.parameterCount() && method.parameterType(0) == qMetaTypeId<Cutelyst::Context *>())) {

            // Build up the list of attributes for the class info
            QByteArray attributeArray;
            for (int i = meta->classInfoCount() - 1; i >= 0; --i) {
                QMetaClassInfo classInfo = meta->classInfo(i);
                if (name == classInfo.name()) {
                    attributeArray.append(classInfo.value());
                }
            }
            QMap<QString, QString> attrs = parseAttributes(method, attributeArray, name);

            QString reverse;
            if (controller->ns().isEmpty()) {
                reverse = QString::fromLatin1(name);
            } else {
                reverse = controller->ns() + QLatin1Char('/') + QString::fromLatin1(name);
            }

            Action *action = createAction({
                                              {_name      , QVariant::fromValue(name)},
                                              {_reverse   , QVariant::fromValue(reverse)},
                                              {QStringLiteral("namespace") , QVariant::fromValue(controller->ns())},
                                              {_attributes, QVariant::fromValue(attrs)}
                                          },
                                          method,
                                          controller,
                                          app);

            actions.insertMulti(action->reverse(), action);
            actionList.append(action);
        }
    }
}

QMap<QString, QString> ControllerPrivate::parseAttributes(const QMetaMethod &method, const QByteArray &str, const QByteArray &name)
{
    QMap<QString, QString> ret;
    std::vector<std::pair<QString, QString> > attributes;
    // This is probably not the best parser ever
    // but it handles cases like:
    // :Args:Local('fo"')o'):ActionClass('foo')
    // into
    // (("Args",""), ("Local","'fo"')o'"), ("ActionClass","'foo'"))

    int size = str.size();
    int pos = 0;
    while (pos < size) {
        QString key;
        QString value;

        // find the start of a key
        if (str.at(pos) == ':') {
            int keyStart = ++pos;
            int keyLength = 0;
            while (pos < size) {
                if (str.at(pos) == '(') {
                    // attribute has value
                    int valueStart = ++pos;
                    while (pos < size) {
                        if (str.at(pos) == ')') {
                            // found the possible end of the value
                            int valueEnd = pos;
                            if (++pos < size && str.at(pos) == ':') {
                                // found the start of a key so this is
                                // really the end of a value
                                value = QString::fromLatin1(str.mid(valueStart, valueEnd - valueStart));
                                break;
                            } else if (pos >= size) {
                                // found the end of the string
                                // save the remainig as the value
                                value = QString::fromLatin1(str.mid(valueStart, valueEnd - valueStart));
                                break;
                            }
                            // string was not '):' or ')$'
                            continue;
                        }
                        ++pos;
                    }

                    break;
                } else if (str.at(pos) == ':') {
                    // Attribute has no value
                    break;
                }
                ++keyLength;
                ++pos;
            }

            // stopre the key
            key = QString::fromLatin1(str.mid(keyStart, keyLength));

            // remove quotes
            if (!value.isEmpty()) {
                if ((value.startsWith(QLatin1Char('\'')) && value.endsWith(QLatin1Char('\''))) ||
                        (value.startsWith(QLatin1Char('"')) && value.endsWith(QLatin1Char('"')))) {
                    value.remove(0, 1);
                    value.remove(value.size() - 1, 1);
                }
            }

            // store the key/value pair found
            attributes.push_back({ key, value });
            continue;
        }
        ++pos;
    }

    // Add the attributes to the map in the reverse order so
    // that values() return them in the right order
    auto i = attributes.crbegin();
    const auto end = attributes.crend();
    while (i != end) {
        QString key = i->first;
        QString value = i->second;
        if (key == QLatin1String("Global")) {
            key = _path;
            value = parsePathAttr(QLatin1Char('/') + QString::fromLatin1(name));
        } else if (key == QLatin1String("Local")) {
            key = _path;
            value = parsePathAttr(QString::fromLatin1(name));
        } else if (key == QLatin1String("Path")) {
            value = parsePathAttr(value);
        } else if (key == QLatin1String("Args")) {
            QString args = value;
            if (!args.isEmpty()) {
                value = args.remove(QRegularExpression(_backSlashD));
            }
        } else if (key == QLatin1String("CaptureArgs")) {
            QString captureArgs = value;
            value = captureArgs.remove(QRegularExpression(_backSlashD));
        } else if (key == QLatin1String("Chained")) {
            value = parseChainedAttr(value);
        }

        ret.insertMulti(key, value);
        ++i;
    }

    // Handle special AutoArgs and AutoCaptureArgs case
    if (!ret.contains(_args) && !ret.contains(_captureArgs) &&
            (ret.contains(_autoArgs) || ret.contains(_autoCaptureArgs))) {
        if (ret.contains(_autoArgs) && ret.contains(_autoCaptureArgs)) {
            qFatal("Action '%s' has both AutoArgs and AutoCaptureArgs, which is not allowed", name.constData());
        } else {
            QString parameterName;
            if (ret.contains(_autoArgs)) {
                ret.remove(_autoArgs);
                parameterName = _args;
            } else {
                ret.remove(_autoCaptureArgs);
                parameterName = _captureArgs;
            }

            // If the signature is not QStringList we count them
            if (!(method.parameterCount() == 2 && method.parameterType(1) == QMetaType::QStringList)) {
                int parameterCount = 0;
                for (int i = 1; i < method.parameterCount(); ++i) {
                    int typeId = method.parameterType(i);
                    if (typeId == QMetaType::QString) {
                        ++parameterCount;
                    }
                }
                ret.insert(parameterName, QString::number(parameterCount));
            }
        }

    }

    // If the method is private add a Private attribute
    if (!ret.contains(_private) && method.access() == QMetaMethod::Private) {
        ret.insert(_private, QString());
    }

    return ret;
}

QStack<Component *> ControllerPrivate::gatherActionRoles(const QVariantHash &args)
{
    QStack<Component *> roles;
    const auto attributes = args.value(_attributes).value<ParamsMultiMap>();
    auto doesIt = attributes.constFind(QStringLiteral("Does"));
    while (doesIt != attributes.constEnd() && doesIt.key() == QLatin1String("Does")) {
        QObject *object = instantiateClass(doesIt.value(), QByteArrayLiteral("Cutelyst::Component"));
        if (object) {
            roles.push(qobject_cast<Component *>(object));
        }
        ++doesIt;
    }
    return roles;
}

QString ControllerPrivate::parsePathAttr(const QString &value)
{
    QString ret = pathPrefix;
    if (value.startsWith(QLatin1Char('/'))) {
        ret = value;
    } else if (!value.isEmpty()) {
        ret = pathPrefix + QLatin1Char('/') + value;
    }
    return ret;
}

QString ControllerPrivate::parseChainedAttr(const QString &attr)
{
    QString ret = QStringLiteral("/");
    if (attr.isEmpty()) {
        return ret;
    }

    if (attr == QStringLiteral(".")) {
        ret.append(pathPrefix);
    } else if (!attr.startsWith(QLatin1Char('/'))) {
        if (!pathPrefix.isEmpty()) {
            ret.append(pathPrefix + QLatin1Char('/') + attr);
        } else {
            // special case namespace '' (root)
            ret.append(attr);
        }
    } else {
        ret = attr;
    }

    return ret;
}

QObject *ControllerPrivate::instantiateClass(const QString &name, const QByteArray &super)
{
    QString instanceName = name;
    if (!instanceName.isEmpty()) {
        instanceName.remove(QRegularExpression(QStringLiteral("\\W")));

        int id = QMetaType::type(instanceName.toLatin1().data());
        if (!id) {
            if (!instanceName.endsWith(QLatin1Char('*'))) {
                instanceName.append(QLatin1Char('*'));
            }

            id = QMetaType::type(instanceName.toLatin1().data());
            if (!id && !instanceName.startsWith(QStringLiteral("Cutelyst::"))) {
                instanceName = QLatin1String("Cutelyst::") + instanceName;
                id = QMetaType::type(instanceName.toLatin1().data());
            }
        }

        if (id) {
            const QMetaObject *metaObj = QMetaType::metaObjectForType(id);
            if (metaObj) {
                if (!superIsClassName(metaObj->superClass(), super)) {
                    qCWarning(CUTELYST_CONTROLLER)
                            << "Class name"
                            << instanceName
                            << "is not a derived class of"
                            << super;
                }

                QObject *object = metaObj->newInstance();
                if (!object) {
                    qCWarning(CUTELYST_CONTROLLER)
                            << "Could create a new instance of"
                            << instanceName
                            << "make sure it's default constructor is "
                               "marked with the Q_INVOKABLE macro";
                }

                return object;
            }
        } else {
            Component *component = application->createComponentPlugin(name);
            if (component) {
                return component;
            }

            component = application->createComponentPlugin(instanceName);
            if (component) {
                return component;
            }
        }

        if (!id) {
            qFatal("Could not create component '%s', you can register it with qRegisterMetaType<%s>(); or set a proper CUTELYST_PLUGINS_DIR",
                   qPrintable(instanceName), qPrintable(instanceName));
        }
    }
    return nullptr;
}

bool ControllerPrivate::superIsClassName(const QMetaObject *super, const QByteArray &className)
{
    if (super) {
        if (super->className() == className) {
            return true;
        }
        return superIsClassName(super->superClass(), className);
    }
    return false;
}

#include "moc_controller.cpp"
