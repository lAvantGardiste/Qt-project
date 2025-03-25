#ifndef PARACHUTEVIEW_H
#define PARACHUTEVIEW_H

#include <QWidget>
#include <QPainter>
#include "../model/message.h"

class ParachuteView : public QWidget {
    Q_OBJECT

public:
    explicit ParachuteView(Message* message = nullptr, QWidget* parent = nullptr);
    
    void setMessage(Message* message);
    bool isBinaryViewMode() const { return m_binaryViewMode; }
    void setBinaryViewMode(bool enabled);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawParachuteView(QPainter& painter);
    void drawBinaryView(QPainter& painter);
    
    Message* m_message;
    bool m_binaryViewMode;
    
    // Constants for binary view
    static constexpr int DOT_SIZE = 10;
    static constexpr int DOT_SPACING = 2;
    static constexpr int BITS_PER_CHAR = 7;
    
    // Méthodes de dessin
    void drawParachute(QPainter& painter);
    void drawSector(QPainter& painter, int ring, int sector, bool filled);
    QPointF getTrapezePoint(int ring, int sector, bool isOuter, bool isStart) const;
    
    // Constantes de dessin
    static constexpr double OUTER_RADIUS = 180.0;
    static constexpr double RING_THICKNESS = 30.0;
};

#endif // PARACHUTEVIEW_H 