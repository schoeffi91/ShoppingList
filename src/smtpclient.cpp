#include "smtpclient.h"
#include <QSettings>

SmtpClient::SmtpClient(int port, int timeout, QObject* parent)
    : QObject(parent)
{
    socket = new QSslSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
        SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this,
        SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    this->port = port;
    this->timeout = timeout;
}

void SmtpClient::saveLogin(const QString& username,
    const QString& password,
    const QString& server,
    const QString& destination)
{
    QSettings settings("MyApp", "MyShoppingList");
    settings.setValue("ShoppingListUser", username);

    //Find a better way to store password!
    settings.setValue("ShoppingListPassword", password); // <-- !! Not secure!
    settings.setValue("ShoppingListServer", server);
    settings.setValue("ShoppingListDestination", destination);
}

void SmtpClient::sendMail(const QString& subject,
    const QString& body,
    QStringList files)
{
    QSettings settings("MyApp", "MyShoppingList");
    this->user = settings.value("ShoppingListUser").toString();
    this->pass = settings.value("ShoppingListPassword").toString();

    this->host = settings.value("ShoppingListServer").toString();
    this->destination = settings.value("ShoppingListDestination").toString();

    if (user.isEmpty() || pass.isEmpty() || host.isEmpty() || destination.isEmpty()) {
        emit loginRequired();
        return;
    }

    message = "To: " + destination + "\n";
    message.append("From: " + user + "\n");
    message.append("Subject: " + subject + "\n");

    // Let's intitiate multipart MIME with cutting boundary "frontier"
    message.append("MIME-Version: 1.0\n");
    message.append("Content-Type: multipart/mixed; boundary=frontier\n\n");

    message.append("--frontier\n");
    // message.append( "Content-Type: text/html\n\n" );  //Uncomment this for HTML
    // formating, coment the line below
    message.append("Content-Type: text/plain\n\n");
    message.append(body);
    message.append("\n\n");

    if (!files.isEmpty()) {
        qDebug() << "Files to be sent: " << files.size();
        foreach (QString filePath, files) {
            QFile file(filePath);
            if (file.exists()) {
                if (!file.open(QIODevice::ReadOnly)) {
                    qDebug("Couldn't open the file");
                    qWarning() << tr("Qt Simple SMTP client")
                               << tr("Couldn't open the file\n\n");
                    return;
                }
                QByteArray bytes = file.readAll();
                message.append("--frontier\n");
                message.append(
                    "Content-Type: application/octet-stream\nContent-Disposition: "
                    "attachment; filename="
                    + QFileInfo(file.fileName()).fileName() + ";\nContent-Transfer-Encoding: base64\n\n");
                message.append(bytes.toBase64());
                message.append("\n");
            }
        }
    } else
        qDebug() << "No attachments found";

    message.append("--frontier--\n");

    message.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
    message.replace(QString::fromLatin1("\r\n.\r\n"),
        QString::fromLatin1("\r\n..\r\n"));

    rcpt = destination;
    state = Init;

    socket->connectToHostEncrypted(
        host, port); //"smtp.gmail.com" and 465 for gmail TLS
    if (!socket->waitForConnected(timeout)) {
        qDebug() << socket->errorString();
    }
}

void SmtpClient::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "stateChanged " << socketState;
}

void SmtpClient::errorReceived(QAbstractSocket::SocketError socketError)
{
    qDebug() << "error " << socketError;
}

void SmtpClient::disconnected()
{
    qDebug() << "disconneted";
    qDebug() << "error " << socket->errorString();
}

void SmtpClient::connected()
{
    qDebug() << "Connected ";
}

void SmtpClient::readyRead()
{
    qDebug() << "readyRead";
    // SMTP is line-oriented

    QTextStream t(socket);

    QString responseLine;
    do {
        responseLine = socket->readLine();
        response += responseLine;
    } while (socket->canReadLine() && responseLine[3] != ' ');

    responseLine.truncate(3);

    qDebug() << "Server response code:" << responseLine;
    qDebug() << "Server response: " << response;

    if (state == Init && responseLine == "220") {
        // banner was okay, let's go on
        t << "EHLO localhost"
          << "\r\n";
        t.flush();

        state = HandShake;
    }
    // No need, because I'm using socket->startClienEncryption() which makes the
    // SSL handshake for you
    /*else if (state == Tls && responseLine == "250")
  {
      // Trying AUTH
      qDebug() << "STarting Tls";
      t << "STARTTLS" << "\r\n";
      t.flush();
      state = HandShake;
  }*/
    else if (state == HandShake && responseLine == "250") {
        socket->startClientEncryption();
        if (!socket->waitForEncrypted(timeout)) {
            qDebug() << socket->errorString();
            state = Close;
        }

        // Send EHLO once again but now encrypted

        t << "EHLO localhost"
          << "\r\n";
        t.flush();
        state = Auth;
    } else if (state == Auth && responseLine == "250") {
        // Trying AUTH
        qDebug() << "Auth";
        t << "AUTH LOGIN"
          << "\r\n";
        t.flush();
        state = User;
    } else if (state == User && responseLine == "334") {
        // Trying User
        qDebug() << "Username";
        // GMAIL is using XOAUTH2 protocol, which basically means that password and
        // username has to be sent in base64 coding
        // https://developers.google.com/gmail/xoauth2_protocol
        t << QByteArray().append(user).toBase64() << "\r\n";
        t.flush();

        state = Pass;
    } else if (state == Pass && responseLine == "334") {
        // Trying pass
        qDebug() << "Pass";
        t << QByteArray().append(pass).toBase64() << "\r\n";
        t.flush();

        state = Mail;
    } else if (state == Mail && responseLine == "235") {
        // HELO response was okay (well, it has to be)

        // Apperantly for Google it is mandatory to have MAIL FROM and RCPT email
        // formated the following way -> <email@gmail.com>
        qDebug() << "MAIL FROM:<" << user << ">";
        t << "MAIL FROM:<" << user << ">\r\n";
        t.flush();
        state = Rcpt;
    } else if (state == Rcpt && responseLine == "250") {
        // Apperantly for Google it is mandatory to have MAIL FROM and RCPT email
        // formated the following way -> <email@gmail.com>
        t << "RCPT TO:<" << rcpt << ">\r\n"; // r
        t.flush();
        state = Data;
    } else if (state == Data && responseLine == "250") {
        t << "DATA\r\n";
        t.flush();
        state = Body;
    } else if (state == Body && responseLine == "354") {
        t << message << "\r\n.\r\n";
        t.flush();
        state = Quit;
    } else if (state == Quit && responseLine == "250") {
        t << "QUIT\r\n";
        t.flush();
        // here, we just close.
        state = Close;
        emit status(tr("Message sent"));
    } else if (state == Close) {
        deleteLater();
        return;
    } else {
        // something broke.
        qWarning() << tr("Qt Simple SMTP client")
                   << tr("Unexpected reply from SMTP server:\n\n");
        state = Close;
        emit status(tr("Failed to send message"));
    }
    response = "";
}
