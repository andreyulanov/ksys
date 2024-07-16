#include <QDebug>
#include "ksettings.h"

KSettings::KSettings(QObject *parent)
    : QObject{parent},
    q_settings("ksys", "knav")
{
    qDebug() << "Load settings from" << q_settings.fileName();
    connect(this, &KSettings::passwordChanged,
            this, &KSettings::savedAccountPresenceChanged);
    connect(this, &KSettings::jidChanged,
            this, &KSettings::savedAccountPresenceChanged);
}

QString KSettings::jid()
{
    return q_settings.value(login_path).toString();
}

QString KSettings::password()
{
    return q_settings.value(password_path).toString();
}

bool KSettings::thereIsASavedAccount()
{
    return q_settings.contains(login_path) && q_settings.contains(password_path);
}

bool KSettings::autologin()
{
    return q_settings.value(autologin_path).toBool();
}

void KSettings::setJid(QString new_jid)
{
    if (new_jid != jid())
    {
        saveJid(new_jid);
        jidChanged();
    }
}
void KSettings::setPassword(QString new_password)
{
    if (new_password != password())
    {
        savePassword(new_password);
        passwordChanged();
    }
}
void KSettings::setAutologin(bool new_autologin)
{
    if (new_autologin != autologin())
    {
        saveAutologin(new_autologin);
        autologinChanged();
    }
}

void KSettings::saveJid(QString jid)
{
    qDebug() << "saving jid:" << jid;
    q_settings.setValue(login_path, jid);
}

void KSettings::savePassword(QString password)
{
    qDebug() << "saving password:" << password;
    q_settings.setValue(password_path, password);
}

void KSettings::saveAutologin(bool autologin)
{
    qDebug() << "saving autologin:" << autologin;
    q_settings.setValue(autologin_path, autologin);
}

void KSettings::saveAccount(QString jid, QString password)
{
    saveJid(jid);
    savePassword(password);
    q_settings.sync();
}
