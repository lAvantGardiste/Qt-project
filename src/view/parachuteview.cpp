#include "parachuteview.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <QFontMetrics>

ParachuteView::ParachuteView(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(400, 400);
    // Fond blanc par défaut
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

void ParachuteView::setMessage(Message* message) {
    m_message = message;
    update();
}

void ParachuteView::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    if (!m_message) return;

    if (m_binaryViewMode) {
        drawBinaryView(painter);
    } else {
        // Centrer le parachute
        painter.translate(width() / 2, height() / 2);
        drawParachute(painter);
    }
}

void ParachuteView::drawBinaryView(QPainter& painter) {
    if (!m_message) return;

    QString binary = m_message->getBinaryString();
    QFontMetrics fm(font());
    int charWidth = fm.horizontalAdvance('0');
    int charHeight = fm.height();
    int charsPerLine = width() / (charWidth + 2);
    
    int x = 10;
    int y = charHeight + 10;  // Ajout d'une marge en haut
    
    painter.setPen(Qt::black);  // Assurer que le texte est noir
    
    // Afficher le message original
    painter.drawText(10, y - 5, "Message: " + m_message->getText());
    y += charHeight + 10;  // Espace après le message
    
    // Afficher les bits en groupes de 7
    int bitCount = 0;
    for (int i = 0; i < binary.length(); ++i) {
        if (bitCount == 7) {
            bitCount = 0;
            x = 10;
            y += charHeight + 5;
        }
        painter.drawText(x, y, QString(binary[i]));
        x += charWidth + 2;
        bitCount++;
    }
}

void ParachuteView::drawParachute(QPainter& painter) {
    const int rings = m_message->getRings();
    const int sectors = m_message->getSectors();
    
    // Dessiner chaque secteur
    for (int ring = 0; ring < rings; ++ring) {
        for (int sector = 0; sector < sectors; ++sector) {
            bool filled = m_message->getBitAt(ring, sector);
            drawSector(painter, ring, sector, filled);
        }
    }
}

void ParachuteView::drawSector(QPainter& painter, int ring, int sector, bool filled) {
    // Calculer les points du trapèze
    QPointF p1 = getTrapezePoint(ring, sector, false, true);
    QPointF p2 = getTrapezePoint(ring, sector, true, true);
    QPointF p3 = getTrapezePoint(ring, sector, true, false);
    QPointF p4 = getTrapezePoint(ring, sector, false, false);
    
    // Créer le chemin du trapèze
    QPainterPath path;
    path.moveTo(p1);
    path.lineTo(p2);
    path.lineTo(p3);
    path.lineTo(p4);
    path.closeSubpath();
    
    // Dessiner le trapèze
    if (filled) {
        painter.fillPath(path, Qt::red);
    }
    painter.strokePath(path, QPen(Qt::black, 1));
}

QPointF ParachuteView::getTrapezePoint(int ring, int sector, bool isOuter, bool isStart) const {
    const int sectors = m_message->getSectors();
    const double anglePerSector = 2 * M_PI / sectors;
    
    // Calculer le rayon
    double radius = OUTER_RADIUS - (ring * RING_THICKNESS);
    if (isOuter) {
        radius -= RING_THICKNESS;
    }
    
    // Calculer l'angle
    double angle = sector * anglePerSector;
    if (!isStart) {
        angle += anglePerSector;
    }
    
    // Convertir en coordonnées cartésiennes
    return QPointF(radius * qCos(angle), radius * qSin(angle));
} 