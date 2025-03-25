#ifndef PARACHUTEVIEW_H
#define PARACHUTEVIEW_H

#include <QWidget>
#include <QPainter>
#include "../model/message.h"

class ParachuteView : public QWidget {
    Q_OBJECT

public:
    explicit ParachuteView(QWidget* parent = nullptr);
    
    void setMessage(Message* message);
    void setBinaryViewMode(bool enabled) { m_binaryViewMode = enabled; update(); }
    bool isBinaryViewMode() const { return m_binaryViewMode; }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Message* m_message{nullptr};
    bool m_binaryViewMode{false};
    
    // Méthodes de dessin
    void drawBinaryView(QPainter& painter);
    void drawParachute(QPainter& painter);
    void drawSector(QPainter& painter, int ring, int sector, bool filled);
    QPointF getTrapezePoint(int ring, int sector, bool isOuter, bool isStart) const;
    
    // Constantes de dessin
    static constexpr double OUTER_RADIUS = 180.0;
    static constexpr double RING_THICKNESS = 30.0;
};

#endif // PARACHUTEVIEW_H 