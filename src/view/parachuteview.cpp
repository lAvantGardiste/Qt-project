#include "parachuteview.h"
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QtMath>

ParachuteView::ParachuteView(Message* message, QWidget* parent)
    : QWidget(parent)
    , m_message(message)
    , m_binaryViewMode(false)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(200, 200);
}

void ParachuteView::setMessage(Message* message)
{
    m_message = message;
    update();
}

void ParachuteView::setBinaryViewMode(bool enabled)
{
    m_binaryViewMode = enabled;
    update();
}

void ParachuteView::paintEvent(QPaintEvent* /*event*/)
{
    if (!m_message) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_binaryViewMode) {
        drawBinaryView(painter);
    } else {
        drawParachuteView(painter);
    }
}

void ParachuteView::drawBinaryView(QPainter& painter)
{
    QString text = m_message->getText();
    QString binary = m_message->getBinaryString();
    int charCount = text.length();
    
    // Calculate dimensions for the grid of dots
    int viewWidth = width();
    int viewHeight = height();
    int totalWidth = charCount * (DOT_SIZE + DOT_SPACING);
    int totalHeight = BITS_PER_CHAR * (DOT_SIZE + DOT_SPACING);
    
    // Center the grid in the view
    int startX = (viewWidth - totalWidth) / 2;
    int startY = (viewHeight - totalHeight) / 2;
    
    // For each character in the message
    for (int charIndex = 0; charIndex < charCount; ++charIndex) {
        // For each bit in the character (7 bits for ASCII)
        for (int bit = 0; bit < BITS_PER_CHAR; ++bit) {
            // Calculate position
            int x = startX + charIndex * (DOT_SIZE + DOT_SPACING);
            int y = startY + bit * (DOT_SIZE + DOT_SPACING);
            
            // Get the bit value from the binary string
            int binaryIndex = charIndex * BITS_PER_CHAR + bit;
            bool isBitSet = binary[binaryIndex] == '1';
            
            // Draw the dot
            painter.setPen(Qt::darkGray);
            painter.setBrush(isBitSet ? Qt::red : Qt::white);
            painter.drawEllipse(x, y, DOT_SIZE, DOT_SIZE);
        }
    }
}

void ParachuteView::drawParachuteView(QPainter& painter)
{
    // Get the dimensions of the widget
    int size = qMin(width(), height());
    painter.translate(width() / 2, height() / 2);
    
    // Scale to fit
    double scale = size / 220.0;  // 220 is a good base size
    painter.scale(scale, scale);
    
    // Draw the parachute pattern
    int rings = m_message->getRings();
    int sectors = m_message->getSectors();
    
    // Calculate ring and sector sizes
    double ringSize = 100.0 / rings;  // 100 is the radius
    double sectorAngle = 360.0 / sectors;
    
    // Draw each ring and sector
    for (int ring = 0; ring < rings; ++ring) {
        for (int sector = 0; sector < sectors; ++sector) {
            bool filled = m_message->getBitAt(ring, sector);
            drawSector(painter, ring, sector, filled);
        }
    }
}

void ParachuteView::drawSector(QPainter& painter, int ring, int sector, bool filled)
{
    int rings = m_message->getRings();
    int sectors = m_message->getSectors();
    
    double ringSize = 100.0 / rings;
    double sectorAngle = 360.0 / sectors;
    
    double innerRadius = (rings - ring - 1) * ringSize;
    double outerRadius = (rings - ring) * ringSize;
    double startAngle = sector * sectorAngle;
    
    QPainterPath path;
    path.moveTo(QPointF(innerRadius * qCos(qDegreesToRadians(startAngle)),
                       innerRadius * qSin(qDegreesToRadians(startAngle))));
    path.arcTo(-outerRadius, -outerRadius, 2*outerRadius, 2*outerRadius,
               -startAngle, -sectorAngle);
    path.arcTo(-innerRadius, -innerRadius, 2*innerRadius, 2*innerRadius,
               -(startAngle + sectorAngle), sectorAngle);
    path.closeSubpath();
    
    painter.setPen(Qt::black);
    painter.setBrush(filled ? Qt::red : Qt::white);
    painter.drawPath(path);
} 