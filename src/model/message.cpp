#include "message.h"

Message::Message() : m_text("ENSICAEN_RULES"), m_rings(5), m_sectors(21) {}

Message::Message(const QString& text) : m_text(text), m_rings(5), m_sectors(21) {}

void Message::setText(const QString& text) {
    m_text = text;
    // Assurons-nous que le texte est en majuscules
    m_text = m_text.toUpper();
}

void Message::setRings(int rings) {
    if (rings > 0) {
        m_rings = rings;
    }
}

void Message::setSectors(int sectors) {
    if (sectors > 0) {
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

QString Message::getBinaryString() const {
    QString result;
    auto pattern = getBinaryPattern();
    for (bool bit : pattern) {
        result.append(bit ? '1' : '0');
    }
    return result;
}

bool Message::getBitAt(int ring, int sector) const {
    if (ring < 0 || ring >= m_rings || sector < 0 || sector >= m_sectors) {
        return false;
    }
    
    auto pattern = getBinaryPattern();
    
    // Calcul de l'index en partant du centre
    int index = 0;
    // On inverse le calcul pour partir du centre (ring 0) vers l'extérieur
    ring = m_rings - 1 - ring;  // Inverser l'ordre des anneaux
    
    for (int r = 0; r < ring; ++r) {
        index += m_sectors;
    }
    index += sector;
    
    return index < pattern.size() ? pattern[index] : false;
}

QVector<bool> Message::encodeCharacter(QChar c) const {
    QVector<bool> bits;
    int value = getBinaryValue(c);
    
    // Convertir en binaire sur 7 bits
    for (int i = 6; i >= 0; --i) {
        bool bit = (value >> i) & 1;
        bits.append(bit);
    }
    
    return bits;
}

int Message::getAsciiOffset(QChar c) {
    if (c >= 'A' && c <= 'Z') {
        return 64;  // Pour les majuscules (A=1, B=2, etc.)
    }
    if (c == '_') {
        return 0;   // Underscore a une valeur spéciale
    }
    return 0;
}

int Message::getBinaryValue(QChar c) {
    if (c >= 'A' && c <= 'Z') {
        return c.unicode() - 64;  // A=1, B=2, etc.
    }
    if (c == '_') {
        return 27;  // Valeur spéciale pour l'underscore
    }
    return 0;
} 