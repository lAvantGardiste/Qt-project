#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QActionGroup>
#include <QTranslator>
#include "model/message.h"
#include "view/parachuteview.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void updateMessage();
    void updateRings(int value);
    void updateSectors(int value);
    void toggleBinaryView(bool enabled);
    void changeLanguage(QAction* action);
    void showAbout();
    void openFile();
    void saveFile();

private:
    void setupUI();
    void createMenus();
    void loadTranslations();
    
    Message* m_message;
    ParachuteView* m_parachuteView;
    QLineEdit* m_messageEdit;
    QSpinBox* m_ringsSpinBox;
    QSpinBox* m_sectorsSpinBox;
    QSlider* m_ringsSlider;
    QSlider* m_sectorsSlider;
    QTranslator m_translator;
    QAction* m_frenchAction;
    QAction* m_englishAction;
};

#endif // MAINWINDOW_H