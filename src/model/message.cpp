#include "message.h"

Message::Message() : m_text("ENSICAEN") {}

Message::Message(const QString& text) : m_text(text) {}

void Message::setText(const QString& text) {
    m_text = text;
}

void Message::setRings(int rings) {
    if (rings > 0) {
        m_rings = rings;
    }
}

void Message::setSectors(int sectors) {
    if (sectors > 0 && sectors % 4 == 0) {  // Nombre de secteurs multiple de 4
        m_sectors = sectors;
    }
}

QVector<bool> Message::getBinaryPattern() const {
    QVector<bool> pattern;
    for (const QChar& c : m_text) {
        auto charPattern = encodeCharacter(c);
        pattern.append(charPattern);
    }
    return pattern;
}

bool Message::getBitAt(int ring, int sector) const {
    if (ring < 0 || ring >= m_rings || sector < 0 || sector >= m_sectors) {
        return false;
    }
    
    auto pattern = getBinaryPattern();
    int index = ring * m_sectors + sector;
    return index < pattern.size() ? pattern[index] : false;
}

QVector<bool> Message::encodeCharacter(QChar c) const {
    QVector<bool> bits;
    int ascii = c.unicode();
    int offset = getAsciiOffset(c);
    int encoded = ascii - offset;
    
    // Convertir en binaire sur 7 bits
    for (int i = 6; i >= 0; --i) {
        bits.append((encoded >> i) & 1);
    }
    
    return bits;
}

int Message::getAsciiOffset(QChar c) {
    // Table de décalage selon le sujet
    if (c >= 'A' && c <= 'Z') {
        return 64;  // Pour les majuscules
    }
    // Ajouter d'autres cas si nécessaire
    return 0;
} 