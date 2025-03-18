#include "parachuteview.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

ParachuteView::ParachuteView(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(400, 400);
}

void ParachuteView::setMessage(Message* message) {
    m_message = message;
    update();
}

void ParachuteView::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Centrer le parachute
    painter.translate(width() / 2, height() / 2);
    
    if (m_message) {
        drawParachute(painter);
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