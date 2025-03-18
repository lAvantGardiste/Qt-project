#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QVector>

class Message {
public:
    Message();
    explicit Message(const QString& text);

    // Getters et setters
    QString getText() const { return m_text; }
    void setText(const QString& text);
    
    // Paramètres du parachute
    void setRings(int rings);
    void setSectors(int sectors);
    int getRings() const { return m_rings; }
    int getSectors() const { return m_sectors; }

    // Encodage
    QVector<bool> getBinaryPattern() const;
    bool getBitAt(int ring, int sector) const;

private:
    QString m_text;
    int m_rings{5};      // Nombre de pistes (anneaux concentriques)
    int m_sectors{20};   // Nombre de secteurs
    
    // Méthodes privées pour l'encodage
    QVector<bool> encodeCharacter(QChar c) const;
    static int getAsciiOffset(QChar c);
};

#endif // MESSAGE_H 