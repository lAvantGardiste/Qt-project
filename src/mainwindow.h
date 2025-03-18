#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include "model/message.h"
#include "view/parachuteview.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void updateMessage();
    void updateRings(int value);
    void updateSectors(int value);
    void toggleBinaryView(bool enabled);

private:
    void setupUI();
    void createMenus();
    
    Message* m_message;
    ParachuteView* m_parachuteView;
    QLineEdit* m_messageEdit;
    QSpinBox* m_ringsSpinBox;
    QSlider* m_sectorsSlider;
};

#endif // MAINWINDOW_H 