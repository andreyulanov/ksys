#ifndef KSETTINGS_H
#define KSETTINGS_H

#include <QSettings>
#include <QObject>

/// The class manages configuration and settings.
/// The main purpose of the class is to provide abstration over settings storage.
/// A new instance of the class is cheep to create and remove, so use it like this:
/// \code{.cpp}
/// class MyClass
/// {
/// public:
/// 	MyClass()
/// 	{
/// 		KSettings k_settins();
/// 		myParam = k_settins.myParam();
/// 	}
/// 	~MyClass()
/// 	{
/// 		KSettings k_settins();
/// 		k_settins.saveMyParam(myParam);
/// 	}
/// private:
/// 	int myParam;
/// };
/// \endcode
class KSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString jid READ jid WRITE setJid NOTIFY jidChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(bool thereIsASavedAccount READ thereIsASavedAccount NOTIFY savedAccountPresenceChanged)
    Q_PROPERTY(bool autologin READ autologin WRITE setAutologin NOTIFY autologinChanged)

public:
    KSettings(QObject *parent = nullptr);
    QString jid();
    QString password();
    bool thereIsASavedAccount();
    bool autologin();
    void setJid(QString);
    void setPassword(QString);
    void setAutologin(bool);
    Q_INVOKABLE void saveAccount(QString jid, QString password);

signals:
    void jidChanged();
    void passwordChanged();
    void savedAccountPresenceChanged();
    void autologinChanged();

private:
    QSettings q_settings;
    void saveJid(QString);
    void savePassword(QString);
    void saveAutologin(bool);
    inline static const QString login_path = "account/jid";
    inline static const QString password_path = "account/password";
    inline static const QString autologin_path = "autologin";
};

#endif // KSETTINGS_H
